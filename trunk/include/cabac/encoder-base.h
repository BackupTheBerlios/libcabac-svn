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

#ifndef _OHTU7AY3EI_CABAC_ENCODER_IMPL_H
#define _OHTU7AY3EI_CABAC_ENCODER_IMPL_H 1

#include <cabac/common-base.h>

namespace cabac {
namespace impl {

/**
 * @internal CABAC encoder base class.
 */
class encoder_base : public base {

  protected:

  encoder_base( const state_vector &states ) :
    base( states ) {
  }

  encoder_base( const encoder_base &other ) :
    base( other ) {
  }

  encoder_base& operator=( const encoder_base &other ) {
    base::operator=( other );
    return *this;
  }

  public:

  /**
   * Simulate a single binary decision without update.
   *
   * This method returns the approximate self information that encoding the given binary
   * decision would take given the current state vector.
   *
   * This method does not alter the CABAC engine state. If you need to simulate more than
   * one decision, you can branch an encoder< void > from the current object.
   *
   * @param idx the index of the CABAC context
   * @param bin_val the value of the bin
   * @return self information of the encoded decision in bits * 256
   */
  unsigned int encode_test( const state_vector::size_type idx, const bool bin_val ) const {
    assert( 0 <= idx );
    assert( idx < _states.size() );
    const unsigned int bits = bits_tab[ _states[ idx ] ^ bin_val ];
  #ifndef NDEBUG
    const unsigned int state_idx = _states[ idx ] >> 1;
    const bool val_mps = _states[ idx ] & 1;
    if ( bin_val != val_mps ) {
      assert( bits == FIX8( BITS( PLPS( state_idx ) ) ) );
    } else {
      assert( bits == FIX8( BITS( 1 - PLPS( state_idx ) ) ) );
    }
  #endif
    return bits;
  }

};

}
}

#endif
