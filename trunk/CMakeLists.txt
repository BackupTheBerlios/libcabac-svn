#
# Author: Johannes Ballé <balle@ient.rwth-aachen.de>, (C) 2008
#
# Copyright: See COPYING file that comes with this distribution
#

project( libcabac )

string( REGEX MATCH "[a-zA-Z]+$" CMAKE_BUILD_TYPE "${CMAKE_BINARY_DIR}" )

#if you don't want the full compiler output, remove the following line
set( CMAKE_VERBOSE_MAKEFILE ON )

add_subdirectory( src )
