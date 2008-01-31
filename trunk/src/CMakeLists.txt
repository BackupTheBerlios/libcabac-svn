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

include_directories( ${CMAKE_SOURCE_DIR}/include )

add_library( cabac cabac.cpp )

add_executable( test-cabac test-cabac.cpp )
target_link_libraries( test-cabac cabac )
