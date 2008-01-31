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

#ifndef _OHTU7AY3EI_CABAC_INTEGER_H
#define _OHTU7AY3EI_CABAC_INTEGER_H 1

#include <cabac/common.h>

namespace cabac {

/**
  * Encode an unsigned integer.
  *
  * Lower values are assumed to appear with greater possibility than higher values.
  * The encoding is done in two steps: First, num_ctx contexts starting with idx are
  * used to encode the possibilities of value being smaller than num_ctx adaptively.
  * If num_ctx is equal to zero (the default value), this step is skipped. Second,
  * value - num_ctx is encoded using an Exp-Golomb(k) encoding and the bypass engine.
  *
  * @param e the encoder object to be used
  * @param value the integer value to be encoded.
  * @param k parameterization value of the Exp-Golomb encoding
  * @param idx first context index to be used
  * @param num_ctx number of context indexes to be used
  */
template< class E >
inline void encode_ueg( E &e, unsigned int value, unsigned int k, state_vector::size_type idx = ~0, const unsigned int num_ctx = 0 ) {
  const state_vector::size_type max_idx = idx + num_ctx;
  while ( idx < max_idx ) {
    const bool zero = ( value == 0 );
    e.encode( idx++, zero );
    if ( zero )
      return;
    value--;
  }
  while ( value >= ( static_cast< unsigned int >( 1 ) << k ) ) {
    e.encode_bypass( 1 );
    value -= 1 << k++;
  }
  e.encode_bypass( 0 );
  while ( k-- )
    e.encode_bypass( ( value >> k ) & 1 );
}

/**
  * Encode a signed integer.
  *
  * Values close to zero are assumed to appear with greater possibility than values
  * approaching negative or positive infinity. The encoding is done by mapping signed
  * values to unsigned values and encoding the result using encode_ueg.
  *
  * @param e the encoder object to be used
  * @param value the integer value to be encoded.
  * @param k parameterization value of the Exp-Golomb encoding
  * @param idx first context index to be used
  * @param num_ctx number of context indexes to be used
  */
template< class E >
inline void encode_seg( E &e, const signed int value, const unsigned int k, state_vector::size_type idx = ~0, const unsigned int num_ctx = 0 ) {
  const unsigned int abs_value = ::std::abs( value );
  encode_ueg( e, abs_value * 2 - ( value < 0 ), k, idx, num_ctx );
}

/**
  * Decode an unsigned integer.
  *
  * @see encode_ueg
  *
  * @param d the decoder object to be used
  * @param k parameterization value of the Exp-Golomb encoding
  * @param idx first context index to be used
  * @param num_ctx number of context indexes to be used
  * @return the decoded value
  */
template< class D >
inline unsigned int decode_ueg( D &d, unsigned int k, state_vector::size_type idx = ~0, const unsigned int num_ctx = 0 ) {
  unsigned int value = 0;
  const state_vector::size_type max_idx = idx + num_ctx;
  while ( idx < max_idx ) {
    if ( d.decode( idx++ ) )
      return value;
    value++;
  }
  while ( d.decode_bypass() )
    value += 1 << k++;
  while ( k-- )
    if ( d.decode_bypass() )
      value += 1 << k;
  return value;
}

/**
  * Decode a signed integer.
  *
  * @see encode_seg
  *
  * @param d the decoder object to be used
  * @param k parameterization value of the Exp-Golomb encoding
  * @param idx first context index to be used
  * @param num_ctx number of context indexes to be used
  * @return the decoded value
  */
template< class D >
inline signed int decode_seg( D &d, const unsigned int k, const state_vector::size_type idx = ~0, const unsigned int num_ctx = 0 ) {
  const unsigned int dec_value = decode_ueg( d, k, idx, num_ctx );
  signed int value = ( dec_value + 1 ) / 2;
  if ( dec_value & 1 )
    value *= -1;
  return value;
}

}

#endif
