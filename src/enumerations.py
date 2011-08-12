#!/usr/bin/env python2
# This script autogenerates declarations and implementations for C++
# enumerations.
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
json_def = au.parseJSONFile(infilename)["objects"]

def readEnumDef(enum):
    # mandatory values: namespace, typename, values
    ns, tn, vals = enum["namespace"], enum["enumeration"], enum["values"]
    # optional values
    strs = enum.get("strings", [])
    inv = enum.get("hasInvalidValue", False)
    # only use strings if their number matches the values
    if len(strs) != len(vals):
        strs = None
    return ns, tn, vals, strs, inv

################################################################################
# write header file

hf = open(outbasename + ".h", "w")
au.autogenWarning(hf, infilename)
au.beginIncludeGuard(hf, outbasename + ".h")

# includes
hf.write("#include <QtCore/QMetaType>\n")
hf.write("#include <QtCore/QString>\n\n")

# foreach enumeration...
for enum in json_def:
    ns, tn, vals, strs, inv = readEnumDef(enum)
    # start namespace
    hf.write("namespace %s {\n" % ns)
    # declare enumerations
    hf.write("\tenum %s {\n" % tn)
    for val in vals:
        hf.write("\t\t%s,\n" % val)
    if inv:
        hf.write("\t\tInvalid = -1\n")
    hf.write("\t};\n")
    # declare string conversions
    if strs is not None:
        hf.write("\t%s::%s fromString(const QString&);\n" % (ns, tn))
        hf.write("\tQString toString(%s::%s);\n" % (ns, tn))
    # end namespace
    hf.write("}\n\n")

# declare metatype for usage with QVariant (see e.g. unit.{h,cpp}.in)
for enum in json_def:
    ns, tn, _, _, _ = readEnumDef(enum)
    hf.write("Q_DECLARE_METATYPE(%s::%s)\n" % (ns, tn))

# close file
hf.write("#endif\n")
hf.close()

################################################################################
# write source file

sf = open(outbasename + ".cpp", "w")
au.autogenWarning(sf, infilename)

# includes
sf.write("#include \"%s.h\"\n" % outbasename)
sf.write("#include <QtCore/QVector>\n\n")

# foreach enumeration...
for enum in json_def:
    ns, tn, _, strs, _ = readEnumDef(enum)
    nstn = "%s::%s" % (ns, tn)
    if strs is None:
        continue
    # define conversion from string
    sf.write("%s %s::fromString(const QString& str) {\n" % (nstn, ns))
    sf.write("\tstatic const QVector<QString> strs = QVector<QString>()")
    for s in strs:
        sf.write("\n\t\t<< QLatin1String(\"%s\")" % s)
    sf.write(";\n\treturn static_cast<%s>(strs.indexOf(str));\n" % nstn)
    sf.write("}\n\n")
    # define conversion to string (TODO: avoid duplicate string vector)
    sf.write("QString %s::toString(%s val) {\n" % (ns, nstn))
    sf.write("\tstatic const QVector<QString> strs = QVector<QString>()")
    for s in strs:
        sf.write("\n\t\t<< QLatin1String(\"%s\")" % s)
    sf.write(";\n\treturn strs.value(val);\n")
    sf.write("}\n\n")

# close file
sf.close()
