#!/usr/bin/env python2
# This script adds type annotations for DBus interfaces to enable them to be used
# with qdbusxml2cpp. It's run at build time before qdbusxml2cpp.
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

import sys, xml.dom.minidom as dom

# static data
mappings = {
    # signature: (type annotation name, corresponding type)
    "a(usssoo)": ("com.trolltech.QtDBus.QtTypeName.Out0", "QList<QsdPrivate::JobListEntry>"),
    "a(ssssssouso)": ("com.trolltech.QtDBus.QtTypeName.Out0", "QList<QsdPrivate::UnitListEntry>")
}

# parse command line
try:
    infilename, outfilename = sys.argv[1:]
except ValueError: # wrong number of values to unpack
    sys.stdout.write("Error: Expected %i, got %i arguments.\n" % (len(sys.argv) - 1))
    sys.stdout.write("Syntax: %s [input file] [output file]\n" % (sys.argv[0]))

# function to walk the tree to add type annotations
def walk(document, parent):
    for node in parent.childNodes:
        if node.nodeType == node.ELEMENT_NODE:
            # descend to child nodes
            walk(document, node)
            # add type annotation if appropriate
            if node.nodeName in ["arg", "property"]:
                signature = node.getAttribute("type")
                if signature in mappings:
                    name, value = mappings[signature]
                    annot = document.createElement("annotation")
                    annot.setAttribute("name", name)
                    annot.setAttribute("value", value)
                    node.appendChild(annot)

# parse input file
document = dom.parse(infilename)
# add type annotations
walk(document, document.documentElement)
# write resulting DOM to output file
document.writexml(open(outfilename, "w"))
