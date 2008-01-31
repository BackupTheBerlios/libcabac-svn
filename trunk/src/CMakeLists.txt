#
# Author: Johannes Ballé <balle@ient.rwth-aachen.de>, (C) 2008
#
# Copyright: See COPYING file that comes with this distribution
#

include_directories( ${CMAKE_SOURCE_DIR}/include )

add_library( cabac cabac.cpp )

add_executable( test-cabac test-cabac.cpp )
target_link_libraries( test-cabac cabac )
