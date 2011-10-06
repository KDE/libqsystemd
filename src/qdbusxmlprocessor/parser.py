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

import sys, xml.dom.minidom as xmldom

from .structs import DInterface, DMethod, DSignal, DArgument, DProperty

class XmlParseException(Exception):
    pass
def _childElementsOf(element):
    return [n for n in element.childNodes if element.ELEMENT_NODE == n.nodeType]

class _XmlParser(object):
    def __init__(self, xmlfile):
        self.interfaces = self.__parseRoot(xmldom.parse(xmlfile))

    def __parseRoot(self, doc):
        nodes = _childElementsOf(doc)
        if len(nodes) != 1 or nodes[0].localName != "node":
            raise XmlParseException, "<node> element not found."
        interfaceNodes = _childElementsOf(nodes[0])
        if len(interfaceNodes) == 0:
            raise XmlParseException, "<interface> element not found."
        interfaces = []
        for interfaceNode in interfaceNodes:
            if interfaceNode.localName != "interface":
                msg = "Unexpected <%s> element." % interfaceNode.localName
                raise XmlParseException, msg
            interfaces.append(self.__parseInterface(interfaceNode))
        return interfaces

    def __parseInterface(self, interfaceNode):
        interface = DInterface(interfaceNode.getAttribute("name"))
        for node in _childElementsOf(interfaceNode):
            if node.localName == "method":
                interface.methods.append(self.__parseMethod(node))
            elif node.localName == "signal":
                interface.signals.append(self.__parseSignal(node))
            elif node.localName == "property":
                interface.properties.append(self.__parseProperty(node))
            else:
                raise XmlParseException, "Unexpected node: " + node.localName
        return interface

    def __parseMethod(self, methodNode):
        method = DMethod(methodNode.getAttribute("name"))
        if method.name == "":
            raise XmlParseException, "No method name set."
        for index, node in enumerate(_childElementsOf(methodNode)):
            if node.localName == "arg":
                arg = self.__parseArg(node, index)
                if arg.direction == "in":
                    method.inArgs.append(arg)
                elif arg.direction == "out":
                    method.outArgs.append(arg)
                else:
                    raise XmlParseException, "Unknown direction: "+arg.direction
            elif node.localName == "annotation":
                print "WARNING: Dropping unknown annotation", node.getAttribute("name")
            else:
                raise XmlParseException, "Unexpected node: " + node.localName
        return method

    def __parseSignal(self, signalNode):
        signal = DSignal(signalNode.getAttribute("name"))
        if signal.name == "":
            raise XmlParseException, "No signal name set."
        for index, node in enumerate(_childElementsOf(signalNode)):
            if node.localName == "arg":
                signal.args.append(self.__parseArg(node, index))
            elif node.localName == "annotation":
                print "WARNING: Dropping unknown annotation", node.getAttribute("name")
            else:
                raise XmlParseException, "Unexpected node: " + node.localName
        return signal

    def __parseArg(self, argNode, argIndex):
        name = argNode.getAttribute("name")
        if name == "":
            name = "argument" + str(argIndex)
        typeSig = argNode.getAttribute("type")
        if typeSig == "":
            raise XmlParseException, "No argument type set."
        return DArgument(name, typeSig, argNode.getAttribute("direction"))

    def __parseProperty(self, propNode):
        name = propNode.getAttribute("name")
        if name == "":
            raise XmlParseException, "No property name set."
        typeSig = propNode.getAttribute("type")
        if typeSig == "":
            raise XmlParseException, "No property type set."
        access = propNode.getAttribute("access")
        if not access in ["read", "readwrite"]:
            raise XmlParseException, "No property access set."
        return DProperty(name, typeSig, access)

def parseXml(xmlfile):
    """ This class processes a DBus interface definition file (an XML document).
    """
    return _XmlParser(xmlfile).interfaces
