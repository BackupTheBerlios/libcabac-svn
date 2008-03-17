//
// This file is part of libcabac.
//
// Copyright 2008 Johannes Ballé <balle@ient.rwth-aachen.de>
//
// libcabac is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libcabac is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libcabac.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _OHTU7AY3EI_CABAC_COMMON_H
#define _OHTU7AY3EI_CABAC_COMMON_H 1

#include <stdint.h>
#include <vector>
#ifdef CABAC_DEBUG_OUTPUT
# include <iostream>
# include <sstream>
# include <iomanip>
# include <bitset>
#endif
#include <cassert>

#if defined( _OHTU7AY3EI_CABAC_CPP ) || !defined( NDEBUG )
# include <cmath>
#endif

/**
 * CABAC library.
 *
 * This namespace contains classes and functions implementing the CABAC
 * (context adaptive binary arithmetic coding) framework.
 *
 * The primary goal designing the class hierarchy was convenience. Secondary goals
 * were portability and efficiency. Basically, the given interface should allow the implementation
 * to be almost as fast as any existing implementation of CABAC, since no virtual functions
 * are used and the whole implementation can be inlined if required.
 *
 * The current implementation should be very portable. However, it is not yet fully optimized.
 * It is based almost literally on the standard. In the ffmpeg and x264 projects, optimized
 * implementations exist, which could be used to replace the implementation of this library in future versions.
 */
namespace cabac {

// tables in cabac.cpp
extern const uint8_t range_tab_lps[ 64 ][ 4 ];
extern const uint8_t trans_idx_lps[ 64 ];
extern const uint8_t trans_idx_mps[ 64 ];
extern const float expect_tab[ 128 ];
extern const uint16_t bits_tab[ 128 ];

/**
 * CABAC state vector.
 *
 * Each integer in this container contains the following information:
 *
 * bit 0: valMPS according to ISO/IEC 14496-10 / ITU-T Rec. H.264
 *
 * bits 1-6: pStateIdx according to ISO/IEC 14496-10 / ITU-T Rec. H.264
 */
typedef ::std::vector< uint8_t > state_vector;

#if defined( _OHTU7AY3EI_CABAC_CPP ) || !defined( NDEBUG )
inline unsigned int FIX8( const double f ) {
  return static_cast< unsigned int >( ( f * ( 1 << 8 ) + .5 ) );
}

inline double PLPS( unsigned int state_idx ) {
  return state_idx ? PLPS( state_idx - 1 ) * pow( 0.01875 / 0.5, 1.0 / 63.0 ) : 0.5;
}

inline double BITS( double p ) {
  return -log2( p );
}
#endif

}

#endif
