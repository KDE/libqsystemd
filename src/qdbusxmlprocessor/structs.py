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

class DInterface(object):
    """ This class represents a DBus interface. """
    def __init__(self, name):
        self.name = name
        self.methods = []
        self.signals = []
        self.properties = []

class DMethod(object):
    """ This class represents a method in a DBus interface. """
    def __init__(self, name):
        self.name = name
        self.inArgs = []
        self.outArgs = []

class DSignal(object):
    """ This class represents a signal in a DBus interface. """
    def __init__(self, name):
        self.name = name
        self.args = []

class DArgument(object):
    """ This class represents an argument in a DBus method or signal. """
    def __init__(self, name, typeSig, direction):
        self.name = name
        self.typeSig = typeSig
        self.direction = direction

class DProperty(object):
    """ This class represents a property in a DBus interface. """
    def __init__(self, name, typeSig, access):
        self.name = name
        self.typeSig = typeSig
        self.access = access
