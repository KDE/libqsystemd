#!/usr/bin/env python2
# This script autogenerates declarations and implementations for C++ data
# structure types which are used to decode DBus arguments.
#
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

import autogenutils as au
infilename, outbasename = au.parseJSONCommandLine()
json_def = au.parseJSONFile(infilename)

################################################################################
# generate header file
hf = open(outbasename + ".h", "w")
au.autogenWarning(hf, infilename)
au.beginIncludeGuard(hf, outbasename + ".h")

# includes, forward declarations
hf.write("#include <QtCore/QMetaType>\n")
hf.write("class QDBusArgument;\n")
hf.write("#include <QtDBus/QDBusObjectPath>\n")

# begin namespace
hf.write("namespace QsdPrivate {\n")

# struct declarations
for structName, members in json_def.iteritems():
    hf.write("struct " + structName + " {\n")
    for member in members:
        # skip commands to the generator in this step
        if not member.startswith("_"):
            hf.write("\t" + member + ";\n")
    hf.write("};\n")

# registerMetaTypes method
hf.write("void registerMetaTypes();\n")

# end namespace
hf.write("} // namespace QsdPrivate\n")

# QDBusArgument operators
opOut = "QDBusArgument& operator<<(QDBusArgument& arg, const QsdPrivate::%s& val)"
opIn = "const QDBusArgument& operator>>(const QDBusArgument& arg, QsdPrivate::%s& val)"
for structName in json_def.iterkeys():
    hf.write(opOut % structName + ";\n")
    hf.write(opIn % structName + ";\n")

# Q_DECLARE_METATYPE macros
for structName, members in json_def.iteritems():
    hf.write("Q_DECLARE_METATYPE(QsdPrivate::" + structName + ")\n")
    if "_REGISTER_QLIST_METATYPE" in members:
        hf.write("Q_DECLARE_METATYPE(QList<QsdPrivate::" + structName + ">)\n")

# end file
hf.write("#endif\n")
hf.close()

################################################################################
# generate source file
sf = open(outbasename + ".cpp", "w")
au.autogenWarning(sf, infilename)

# includes
sf.write("#include \"" + outbasename + ".h\"\n")
sf.write("#include <QtDBus/QDBusArgument>\n")
sf.write("#include <QtDBus/QDBusMetaType>\n")

# output operators
for structName, members in json_def.iteritems():
    sf.write(opOut % structName + "\n{\n")
    sf.write("\targ.beginStructure();\n")
    for member in members:
        # skip commands to the generator in this step
        if not member.startswith("_"):
            memberType, memberName = member.split(" ")
            sf.write("\targ << val." + memberName + ";\n")
    sf.write("\targ.endStructure();\n")
    sf.write("\treturn arg;\n")
    sf.write("}\n")

# input operators
for structName, members in json_def.iteritems():
    sf.write(opIn % structName + "\n{\n")
    sf.write("\targ.beginStructure();\n")
    for member in members:
        # skip commands to the generator in this step
        if not member.startswith("_"):
            memberType, memberName = member.split(" ")
            sf.write("\targ >> val." + memberName + ";\n")
    sf.write("\targ.endStructure();\n")
    sf.write("\treturn arg;\n")
    sf.write("}\n")

# registerMetaTypes method
def writeRegisterCalls(type):
    # extra space before ">" needed for nested templates
    sf.write("qRegisterMetaType<" + type + " >(\"" + type + "\");\n")
    sf.write("qDBusRegisterMetaType<" + type + " >();\n")

sf.write("void QsdPrivate::registerMetaTypes()\n{\n")
for structName, members in json_def.iteritems():
    type = "QsdPrivate::" + structName
    writeRegisterCalls(type)
    if "_REGISTER_QLIST_METATYPE" in members:
        writeRegisterCalls("QList<" + type + ">")
sf.write("}\n")

# end file
sf.close()
