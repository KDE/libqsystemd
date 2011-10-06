#!/usr/bin/env python2
# Copyright 2011 Stefan Majewsky <majewsky@gmx.net>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

from .cppgenerator import CppGenerator
from .structs import DInterface, DMethod, DSignal, DArgument, DProperty

class UnknownDBusSignatureError(Exception):
    pass
class MultipleOutputArgsError(Exception):
    pass

_qtdbus_primitiveTypes = {
    # type character: (value type, const-reference type)
    r"y": ("uchar", "uchar"),
    r"b": ("bool", "bool"),
    r"n": ("short", "short"),
    r"q": ("ushort", "ushort"),
    r"i": ("int", "int"),
    r"u": ("uint", "uint"),
    r"x": ("qlonglong", "qlonglong"),
    r"t": ("qulonglong", "qulonglong"),
    r"d": ("double", "double"),
    r"s": ("QString", "const QString&"),
    r"o": ("QDBusObjectPath", "const QDBusObjectPath&"),
    r"g": ("QDBusSignature", "const QDBusSignature&"),
    r"v": ("QDBusVariant", "const QDBusVariant&")
}
_qtdbus_knownComplexTypes = {
    r"ay": ("QByteArray", "const QByteArray&"),
    r"as": ("QString", "const QString&"),
    r"ao": ("QList<QDBusObjectPath>", "const QList<QDBusObjectPath>&"),
    r"ag": ("QList<QDBusSignature>", "const QList<QDBusSignature>&"),
    r"av": ("QVariant", "const QVariant&"),
}

class InterfaceGenerator(CppGenerator):
    """ This class generates a QDBusAbstractInterface for a DBus interface. """
    def __init__(self, className, interfaceName):
        CppGenerator.__init__(self, className)
        self.__interfaceName = interfaceName

        decl = "static inline const char* staticInterfaceName() { return \"%s\"; }"
        self.addDeclaration(decl % interfaceName)

    def _convertType(self, signature):
        """ Return for a DBus type signature the corresponding Qt type
            (actually a pair of type and const-reference type).
        """
        if signature == "":
            raise UnknownDBusSignatureError, "Signature is " + signature
        # handle known types
        elif signature in _qtdbus_primitiveTypes:
            return _qtdbus_primitiveTypes[signature]
        elif signature in _qtdbus_knownComplexTypes:
            return _qtdbus_knownComplexTypes[signature]
        # cannot handle this signature
        raise UnknownDBusSignatureError, "Signature is " + signature

    def addDBusProperties(self, properties):
        for prop in properties:
            self.addDBusProperty(prop)
    def addDBusProperty(self, prop, propertyName = None):
        """ If propertyName is given, it will be used instead of prop.name
            in the C++ API.
        """
        name = propertyName or prop.name
        getterName = name[0].lower() + name[1:]
        setterName = "set" + name[0].upper() + name[1:]
        type, refType = self._convertType(prop.typeSig)
        # empty line in .h file
        self.addDeclaration("", access="public")
        # generate getter method
        decl = "%s %s() const;" % (type, getterName)
        impl = "return qvariant_cast<%s >(property(\"%s\"));" % (type, prop.name)
        self.addMethod(decl, impl)
        # generate setter method
        if prop.access == "readwrite":
            decl = "void %s(%s value);" % (setterName, refType)
            impl = "setProperty(\"%s\", qVariantFromValue(value));" % prop.name
            self.addMethod(decl, impl)
        # generate Q_PROPERTY macro
            decl = "Q_PROPERTY(%s %s READ %s WRITE %s)" % (type, name, getterName, setterName)
            self.addDeclaration(decl, access="")
        else:
            decl = "Q_PROPERTY(%s %s READ %s)" % (type, name, getterName)
            self.addDeclaration(decl, access="")

    def addDBusMethods(self, methods):
        for method in methods:
            self.addDBusMethod(method)
    def addDBusMethod(self, method, methodName=None):
        """ If methodName is given, it will be used instead of method.name
            in the C++ API.
        """
        name = methodName or method.name
        # find type information for return value
        if len(method.outArgs) == 0:
            retType = "QDBusPendingReply<>"
        elif len(method.outArgs) == 1:
            retType = self._convertType(method.outArgs[0].typeSig)[0]
            retType = "QDBusPendingReply<%s>" % retType
        else:
            raise MultipleOutputArgsError, "Not supported by this generator."
        # find type information etc. for parameters
        arginfo = []
        for inArg in method.inArgs:
            type, refType = self._convertType(inArg.typeSig)
            arginfo.append((inArg.name, type, refType))
        # declaration
        declArgs = []
        for name, type, refType in arginfo:
            declArgs.append("%s %s" % (refType, name))
        decl = "%s ::%s(%s);" % (retType, method.name, ", ".join(declArgs))
        # implementation
        impl = "QList<QVariant> args;\n"
        for name, type, refType in arginfo:
            impl += "args << qVariantFromValue(%s);\n" % name
        code = "return asyncCallWithArgumentList(QLatin1String(\"%s\"), args);"
        impl += code % method.name # not the name variable here!
        # empty line in .h file
        self.addDeclaration("", access="public")
        # generate method
        self.addMethod(decl, impl)

    def addDBusSignals(self, signals):
        for signal in signals:
            self.addDBusSignal(signal)
    def addDBusSignal(self, signal, signalName=None):
        """ If signalName is given, it will be used instead of signal.name
            in the C++ API.
        """
        name = signalName or signal.name
        raise NotImplementedError #TODO
