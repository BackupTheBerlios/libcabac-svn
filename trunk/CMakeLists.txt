#
# This file is part of libcabac.
#
# Copyright 2008 Johannes Ballé <balle@ient.rwth-aachen.de>
#
# libcabac is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libcabac is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libcabac.  If not, see <http:#www.gnu.org/licenses/>.
#

project( libcabac )

string( REGEX MATCH "[a-zA-Z]+$" CMAKE_BUILD_TYPE "${CMAKE_BINARY_DIR}" )

#if you don't want the full compiler output, remove the following line
set( CMAKE_VERBOSE_MAKEFILE ON )

add_subdirectory( src )
