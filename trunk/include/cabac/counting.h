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

#ifndef _OHTU7AY3EI_CABAC_COUNTING_H
#define _OHTU7AY3EI_CABAC_COUNTING_H 1

#include <cabac/common.h>

namespace cabac {

/**
 * CABAC frequency vector.
 *
 * Each pair in this container represents the number of zeroes (pair.first)
 * and ones (pair.second) which were encoded / decoded.
 */
typedef ::std::vector< ::std::pair< unsigned int, unsigned int > > frequency_vector;

namespace impl {

/**
 * @internal Base class implementing counting behaviour.
 */
class counting {

  frequency_vector _frequencies;

  // prohibit duplication of object
  counting( const counting &other );
  counting& operator=( const counting &other );

  protected:

  counting( const frequency_vector::size_type num ) :
    _frequencies( num ) {
  }

  inline void count( const frequency_vector::size_type idx, const bool bin_val ) {
    if ( bin_val )
      _frequencies[ idx ].second++;
    else
      _frequencies[ idx ].first++;
  }

  public:

  /**
   * Get current frequency vector.
   *
   * @return a reference to a frequency_vector containing the current count of encoded / decoded decisions.
   */
  inline const frequency_vector& frequencies() const {
    return _frequencies;
  }

};

}

template< typename I >
class encoder;

/**
 * CABAC %encoder which counts relative frequencies.
 *
 * This class behaves in the same way as the encoder class, only that it counts the number of zeroes and ones
 * encoded using each context. These frequencies can be acquired through frequencies().
 */
template< typename I >
class counting_encoder : public encoder< I >, public impl::counting {

  public:

  /**
   * Constructor.
   *
   * @param output an STL-compatible output iterator on a container of uint8_t, used to write the bitstream
   * @param states the initial state vector
   */
  counting_encoder( const I &output, const state_vector &states ) :
    encoder< I >( output, states ),
    counting( states.size() ) {
  }

  void encode( const state_vector::size_type idx, const bool bin_val ) {
    encoder< I >::encode( idx, bin_val );
    counting::count( idx, bin_val );
  }

};

/**
 * CABAC %encoder which counts relative frequencies and discards bitstream.
 *
 * This class behaves in the same way as the encoder< void > class, only that it counts the number of zeroes and ones
 * encoded using each context. These frequencies can be acquired through frequencies().
 *
 * In contrast to encoder< void >, this class cannot be constructed from or assigned from another encoder object, since
 * no use case exists for this (or does it?).
 */
template<>
class counting_encoder< void > : public encoder< void >, public impl::counting {

  public:

  /**
   * Constructor.
   *
   * @param states initial states.
   */
  counting_encoder( const state_vector &states ) :
    encoder< void >( states ),
    counting( states.size() ) {
  }

  void encode( const state_vector::size_type idx, const bool bin_val ) {
    encoder< void >::encode( idx, bin_val );
    counting::count( idx, bin_val );
  }

};

template< typename I >
class decoder;

/**
 * CABAC %decoder which counts relative frequencies.
 *
 * This class behaves in the same way as the decoder class, only that it counts the number of zeroes and ones
 * decoded using each context. These frequencies can be acquired through frequencies().
 */
template< typename I >
class counting_decoder : public decoder< I >, public impl::counting {

  public:

  /**
   * Constructor.
   *
   * @param input an STL-compatible input iterator on a container of uint8_t, used to read the bitstream
   * @param states the initial state vector
   */
  counting_decoder( const I &input, const state_vector &states ) :
    decoder< I >( input, states ),
    counting( states.size() ) {
  }

  bool decode( const state_vector::size_type idx ) {
    const bool bin_val = decoder< I >::decode( idx );
    counting::count( idx, bin_val );
    return bin_val;
  }

};

/**
 * Compute optimal state initialization vector from given frequency vector.
 *
 * @param f the measured frequency vector
 * @return a state_vector which approximates the measured frequencies
 */
state_vector initialization_vector( const frequency_vector& f ) {
  const frequency_vector::size_type size = f.size();
  state_vector s;
  s.reserve( size );
  for ( frequency_vector::size_type i = 0; i < size; ++i ) {
    const unsigned int sum = f[ i ].first + f[ i ].second;
    if ( !sum ) {
      s.push_back( 0 );
      continue;
    }
    const float expect = f[ i ].second / static_cast< float >( sum );
    unsigned int best_idx;
    float best = 2;
    for ( unsigned int idx = 0; idx < 126; ++idx ) {
      const float diff = ::std::abs( expect - expect_tab[ idx ] );
      if ( diff < best ) {
        best = diff;
        best_idx = idx;
      }
    }
    s.push_back( best_idx );
  }
  return s;
}

/**
 * Accumulate frequencies from different vectors.
 *
 * @param lhs the output vector
 * @param rhs the vector to accumulate
 */
void operator+=( frequency_vector& lhs, const frequency_vector& rhs ) {
  const frequency_vector::size_type size = lhs.size();
  assert( size == rhs.size() );
  for ( frequency_vector::size_type i = 0; i < size; ++i ) {
    lhs[ i ].first += rhs[ i ].first;
    lhs[ i ].second += rhs[ i ].second;
  }
}

}

#endif
