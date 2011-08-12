#!/usr/bin/env python2
# This script autogenerates QsdUnit and its subclasses based on .h.in and .cpp.in files.
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

import autogenutils as au, os, re, sys

# parse command line
if len(sys.argv) != 3:
    w = sys.stderr.write
    w("Error: Expected 2, got %i arguments.\n" % (len(sys.argv) - 1))
    w("Syntax: %s [input file] [output basename]\n" % (sys.argv[0]))
    sys.exit(1)
inbasename, outbasename = [os.path.abspath(x) for x in sys.argv[1:]]

# setup data fields
currentClass = None
interfaceClass = None
interfaceHeader = None
extraPrivateMembers = []
roProperties = []
isSubclass = False

################################################################################
# parser commands for the header file

# build a list of commands while defining them with a decorator
parserCommands = {}
def parserCommand(name):
    def wrap(f):
        global parserCommands
        parserCommands[name] = f
        return f
    return wrap

@parserCommand("QSDUNIT_SUBCLASS")
def pcSubclass(indent):
    global isSubclass
    isSubclass = True
    hf.write(indent + "%s(%sPrivate* d_ptr);\n" % ((currentClass,)*2))

@parserCommand("GENERAL_UTILITIES")
def pcGeneralUtilities(indent):
    hf.write(indent + "Q_DECLARE_PRIVATE(%s)\n" % currentClass)
    hf.write(indent + "Q_DISABLE_COPY(%s)\n" % currentClass)
    hf.write(indent + "friend class QsdManager;\n")          # may delete
    hf.write(indent + "friend class QsdPrivate::Factory;\n") # may construct
    hf.write(indent + "typedef %sPrivate Private;\n" % currentClass)

@parserCommand("INTERFACE_CLASS")
def pcInterfaceClass(indent, input):
    global interfaceClass
    interfaceClass = input

@parserCommand("INTERFACE_HEADER")
def pcInterfaceHeader(indent, input):
    global interfaceHeader
    interfaceHeader = input

@parserCommand("EXTRA_PRIVATE_MEMBER")
def pcExtraPrivateMember(indent, input):
    global extraPrivateMembers
    extraPrivateMembers.append(input)

@parserCommand("RO_PROPERTY")
def pcROProperty(indent, input):
    typeName, name = input.split(" ")
    hf.write(indent + "%s %s() const;\n" % (typeName, name))
    defaultFetch = "%%.%s()" % name
    global roProperties
    roProperties.append(dict(typeName=typeName, name=name, fetch=defaultFetch))

@parserCommand("FETCH")
def pcFetch(indent, input):
    global roProperties
    roProperties[len(roProperties) - 1]["fetch"] = input

################################################################################
# process public header file

hf = open(outbasename + ".h", "w")
au.autogenWarning(hf, inbasename + ".h.in")

commandLineRegex = re.compile("^(\\s*)#auto (\\S*)(.*)$")
def handleParserCommands(file, line):
    # directly output anything that does not look like a parser command
    match = commandLineRegex.match(line)
    if not match:
        file.write(line)
        return
    # read name of parser command (output line unchanged if unknown command)
    parseFunction = parserCommands.get(match.group(2))
    if parseFunction is None:
        file.write(line)
        return
    # call parser command with everything after the command name as argument
    indent = match.group(1)
    input = match.group(3).strip()
    if input == "":
        parseFunction(indent)
    else:
        parseFunction(indent, input)

classNameRegex = re.compile("^\\s*class\\s.*QSYSTEMD_EXPORT\\s*(\\S+)")
for line in open(inbasename + ".h.in").readlines():
    # read class name
    match = classNameRegex.match(line)
    if match:
        currentClass = match.group(1)
    handleParserCommands(hf, line)
hf.close()

################################################################################
# generate private header file

hf = open(outbasename + "_p.h", "w")
au.autogenWarning(hf, inbasename + ".h.in")
au.beginIncludeGuard(hf, outbasename + "_p.h")
hf.write("\n")

# includes
if interfaceClass is not None:
    hf.write("#include \"dbusref_p.h\"\n")
hf.write("#include \"%s.h\"\n" % outbasename)
if isSubclass:
    hf.write("#include \"unit_p.h\"\n")
hf.write("#include \"dbus-interface-types.h\"\n")
if interfaceHeader is not None:
    hf.write("#include \"%s\"\n" % interfaceHeader)
hf.write("#include <QtCore/QMap>\n")
hf.write("\n")

# start to generate private class
if isSubclass:
    hf.write("struct %sPrivate : public QsdUnitPrivate {\n" % currentClass)
else:
    hf.write("struct %sPrivate {\n" % currentClass)
hf.write("\tmutable QMap<int, QVariant> m_data;\n")

# DBus interface instance
if interfaceClass is not None:
    hf.write("\tQsdFromDBusRef<%s> m_interface;\n" % interfaceClass)

# generate ctor
hf.write("\t%sPrivate(const QsdPrivate::DBusRef& ref);\n" % currentClass)

# additional members as defined in the input file
for member in extraPrivateMembers:
    hf.write("\t%s\n" % member)

# close private class, include guard, file
hf.write("};\n\n#endif\n")
hf.close()

################################################################################
# parser commands for the source file

parserCommands = {}

@parserCommand("STORE")
def pcStore(indent, input):
    # read input (pn = property name)
    pn, expr = input.split(" ", 1)
    # find property with this name
    pi = -1
    for pi_, prop in enumerate(roProperties):
        if prop["name"] == pn:
            pi = pi_
            break
    # tn = type name
    tn = roProperties[pi]["typeName"]
    # evaluate expression
    def w(x): sf.write(indent + x)
    w("{\n")
    w("\tconst %s val = %s;\n" % (tn, expr))
    # NOTE: no Q_D macro here; the method containing the STORE command is
    #       supposed to do that
    w("\td->m_data.insert(%i, QVariant::fromValue<%s >(val));\n" % (pi, tn))
    w("}\n")

################################################################################
# generate source file

sf = open(outbasename + ".cpp", "w")
au.autogenWarning(sf, inbasename + ".h.in\n// " + inbasename + ".cpp.in")
sf.write("#include \"%s.h\"\n" % outbasename)
sf.write("#include \"%s_p.h\"\n\n" % outbasename)

# stream .cpp.in into .cpp, handle source file parser commands
if os.path.exists(inbasename + ".cpp.in"):
    for line in open(inbasename + ".cpp.in").readlines():
        handleParserCommands(sf, line)
    sf.write("\n")

# generate private ctor
sf.write("%sPrivate::%sPrivate(const QsdPrivate::DBusRef& ref)\n" % ((currentClass,)*2))
if isSubclass:
    sf.write("\t: QsdUnitPrivate(ref)\n\t, m_interface(ref)\n")
else:
    sf.write("\t: m_interface(ref)\n")
sf.write("{\n}\n\n")

# generate ctor for subclasses
if isSubclass:
    sf.write("%s::%s(%sPrivate* d_ptr)\n" % ((currentClass,)*3))
    sf.write("\t: QsdUnit(d_ptr)\n")
    sf.write("{\n}\n\n")

# generate getter functions
for pi, prop in enumerate(roProperties):
    # pi = property index, tn = typename, pn = property name, fc = fetch command
    tn, pn, fc = prop["typeName"], prop["name"], prop["fetch"]
    # begin function
    sf.write("%s %s::%s() const\n{\n" % (tn, currentClass, pn))
    sf.write("\tQ_D(const %s);\n" % currentClass)
    # try to find value in map (by iterator, to avoid double lookup through
    # a pair of contains()/value() calls)
    sf.write("\tQMap<int, QVariant>::const_iterator it = d->m_data.constFind(%i);\n" % pi)
    sf.write("\tif (it != d->%sPrivate::m_data.constEnd())\n" % currentClass)
    sf.write("\t\treturn it.value().value<%s >();\n" % tn)
    # use the fetch command to retrieve the value from the DBus interface
    # the magic character "%" is replaced by d->m_interface
    fc = fc.replace("%", "d->%sPrivate::m_interface" % (currentClass))
    sf.write("\tconst %s val = %s;\n" % (tn, fc))
    # store in data cache for next access
    sf.write("\td->%sPrivate::m_data.insert(%i, QVariant::fromValue<%s >(val));\n" % (currentClass, pi, tn))
    sf.write("\treturn val;\n")
    # end function
    sf.write("}\n\n")

# close file
sf.close()
