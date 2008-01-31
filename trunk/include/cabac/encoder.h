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

#ifndef _OHTU7AY3EI_CABAC_ENCODER_H
#define _OHTU7AY3EI_CABAC_ENCODER_H 1

#include <cabac/encoder-base.h>

namespace cabac {

/**
 * CABAC %encoder.
 *
 * Writes the bitstream to an arbitrary STL-compatible iterator of unsigned char.
 *
 * Typically, the declaration of the encoder would be done this way:
 *
 * @code
 * typedef std::vector< unsigned char > bitstream;
 * typedef std::back_insert_iterator< bitstream > iter_type;
 *
 * bitstream bs;
 * bs.reserve( 2048 ); // estimated bitstream size in bytes
 * cabac::encoder< iter_type > enc( iter_type( bs ), initial_states );
 *
 * enc.encode( ... ); // encode from here
 * @endcode
 *
 * This uses an STL vector of unsigned char to hold the bitstream. The call to reserve() ensures
 * that -- if the estimation is correct -- the vector does not have to reallocate memory too often.
 *
 * Note that you can use *any* STL-compatible output iterator to write the bitstream to and that a pointer
 * to unsigned char is an iterator, too! (Hence, raw memory access is possible though not recommended.)
 *
 * As another example, to write the bitstream directly to a file, you could use a standard ostream_iterator:
 *
 * @code
 * typedef std::ostream_iterator< unsigned char > iter_type;
 *
 * std::ostream os( "filename.bin" );
 * cabac::encoder< iter_type >( iter_type( os ), initial_states );
 *
 * enc.encode( ... ); // encode from here
 * @endcode
 */
template< typename I >
class encoder : public impl::encoder_base {

  I _data;

  unsigned int _low;
  unsigned int _range;
  unsigned int _bits_outstanding;
  unsigned int _byte;
  int _shift;

  // prohibit duplication of object
  encoder( const encoder &other );
  encoder& operator=( const encoder &other );

  /**
   * WriteBits according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   */
  void write_bit( const bool b ) {
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << "WRITING BIT " << b << ::std::endl;
#endif
    _byte |= ( b << _shift );
    --_shift;
    if ( _shift < 0 ) {
      *_data++ = _byte;
      _byte = 0;
      _shift = 7;
    }
  }

  /**
   * PutBit according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   */
  void put_bit( const bool b ) {
    write_bit( b );
    while ( _bits_outstanding ) {
      write_bit( !b );
      --_bits_outstanding;
    }
  }

  /**
   * RenormE according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   */
  void renorm() {
    while ( _range < 0x100 ) {
      if ( _low < 0x100 ) {
        put_bit( 0 );
      } else {
        if ( _low >= 0x200 ) {
          _low -= 0x200;
          put_bit( 1 );
        } else {
          _low -= 0x100;
          ++_bits_outstanding;
        }
      }
      _range <<= 1;
      _low <<= 1;
    }
  }

  /**
   * EncodeFlush according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   */
  void flush() {
    _range = 2;
    renorm();
    put_bit( ( _low >> 9 ) & 1 );
    write_bit( ( _low >> 8 ) & 1 );
    write_bit( 1 );
  }

  public:

  typedef I iterator_type;

  /**
   * Constructor.
   *
   * @param output an STL-compatible output iterator on a container of unsigned char, used to write the bitstream
   * @param states the initial state vector
   */
  encoder( const I &output, const state_vector &states ) :
    encoder_base( states ),
    _data( output ),
    _low( 0 ),
    _range( 0x1fe ),
    _bits_outstanding( 0 ),
    _byte( 0 ),
    _shift( 8 ) {
  }

  /**
   * Destructor.
   *
   * Destroying the encoder object terminates the bitstream. It is mandatory
   * to destroy the encoder object before full decoding of the bitstream is possible.
   */
  ~encoder() {
    flush();
    *_data++ = _byte;
  }

  /**
   * Encode a binary decision.
   *
   * This method encodes a binary decision to the bitstream with respect to its
   * estimated expectation value determined by the CABAC state vector. With this, it is
   * possible to encode a binary decision to far less than 1 bit (in terms of self
   * information / entropy) if the estimated expectation value differs from 0.5.
   *
   * Corresponds to EncodeDecision according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   *
   * @param idx the index of the CABAC context
   * @param bin_val the value of the bin
   */
  void encode( const state_vector::size_type idx, const bool bin_val ) {
    assert( 0 <= idx );
    assert( idx < _states.size() );
    const unsigned int state_idx = _states[ idx ] >> 1;
    bool val_mps = _states[ idx ] & 1;
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << ::std::setfill( '0' ) << ::std::right
      <<  "CTX " << ::std::setw( 3 ) << idx
      << " RNG " << ::std::bitset< 16 >( _range ).to_string()
      << " LOW " << ::std::bitset< 16 >( _low ).to_string()
      << " MPS " << val_mps
      << " IDX " << ::std::setw( 2 ) << state_idx
      << " DEC " << bin_val << ::std::endl;
#endif
    const unsigned int range_idx = ( _range >> 6 ) & 3;
    const unsigned int range_lps = range_tab_lps[ state_idx ][ range_idx ];
    _range -= range_lps;
    if ( bin_val != val_mps ) {
      _low += _range;
      _range = range_lps;
      if ( state_idx == 0 )
        val_mps = !val_mps;
      _states[ idx ] = ( trans_idx_lps[ state_idx ] << 1 ) | val_mps;
    } else {
      _states[ idx ] = ( trans_idx_mps[ state_idx ] << 1 ) | val_mps;
    }
    renorm();
  }

  /**
   * Encode a binary decision using the bypass engine.
   *
   * This method assumes that the estimated expectation value of the binary decision
   * to be encoded is approximately 0.5. If this is the case, this method is much faster
   * than encode() while providing almost the same compression efficiency. The internal
   * state of the CABAC engine is not updated by this method.
   *
   * Corresponds to EncodeBypass according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   *
   * @param bin_val the value of the bin
   */
  void encode_bypass( const bool bin_val ) {
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << ::std::setfill( '0' ) << ::std::right
      <<  "BYPASS "
      << " RNG " << ::std::bitset< 16 >( _range ).to_string()
      << " LOW " << ::std::bitset< 16 >( _low ).to_string()
      << " MPS -"
      << " IDX --"
      << " DEC " << bin_val << ::std::endl;
#endif
    _low <<= 1;
    if ( bin_val )
      _low += _range;
    if ( _low >= 0x400 ) {
      put_bit( 1 );
      _low -= 0x400;
    } else {
      if ( _low < 0x200 ) {
        put_bit( 0 );
      } else {
        _low -= 0x200;
        ++_bits_outstanding;
      }
    }
  }

  /**
   * Encode a terminal bit.
   *
   * This method assumes that the estimated expectation value of the binary decision
   * to be encoded is approximately 0. It should be used to indicate the termination of
   * the CABAC bitstream with a 1. After this has been done, no further decisions should be
   * encoded and the encoder object should be destroyed.
   *
   * Corresponds to EncodeTerminate according to ISO/IEC 14496-10 / ITU-T Rec. H.264. The only
   * difference is that the call to EncodeFlush has been moved to the destructor.
   *
   * @note This method is not mandatory to terminate a bitstream, only for signalling the
   * end to the decoder. If the decoder already knows that the bitstream is going to end at
   * a specific position, you may as well just destroy the encoder object in order to terminate
   * the bitstream.
   *
   * @param bin_val indicates 1 for termination of the bitstream, 0 for continuation.
   */
  void encode_terminal( const bool bin_val ) {
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << ::std::setfill( '0' ) << ::std::right
      <<  "TERM   "
      << " RNG " << ::std::bitset< 16 >( _range ).to_string()
      << " LOW " << ::std::bitset< 16 >( _low ).to_string()
      << " MPS -"
      << " IDX --"
      << " DEC " << bin_val << ::std::endl;
#endif
    _range -= 2;
    if ( bin_val ) {
      _low += _range;
    } else {
      renorm();
    }
  }

};



/**************************************************************************************************/



/**
 * CABAC %encoder which discards bitstream.
 *
 * This class is a special case of the encoder class. It behaves just like it, only that
 * no bitstream is written and the self information of the encoded symbols in bits are accumulated.
 *
 * This can be used to simulate encoding for rate-distortion decisions.
 */
template<>
class encoder< void > : public impl::encoder_base {

  unsigned int _bits;

  public:

  /**
   * Construct from initial state vector.
   *
   * @param states initial states.
   */
  encoder( const state_vector &states ) :
    encoder_base( states ),
    _bits( 0 ) {
  }

  /**
   * Construct from other encoder< void > object.
   *
   * Useful in order to branch a simulation. The bit count and state vector of the other
   * object is copied.
   */
  encoder( const encoder< void > &other ) :
    encoder_base( other ),
    _bits( other._bits ) {
  }

  /**
   * Construct from encoder object.
   *
   * The bit count starts with zero, the state vector of the encoder object is copied.
   */
  encoder( const encoder_base &other ) :
    encoder_base( other ),
    _bits( 0 ) {
  }

  /**
   * Assign from other encoder< void > object.
   *
   * Has the same semantics as its constructor counterpart.
   */
  encoder< void >& operator=( const encoder< void > &other ) {
    encoder_base::operator=( other );
    _bits = other._bits;
    return *this;
  }

  /**
   * Assign from encoder object.
   *
   * Has the same semantics as its constructor counterpart.
   */
  encoder< void >& operator=( const encoder_base &other ) {
    encoder_base::operator=( other );
    _bits = 0;
    return *this;
  }

  /**
   * Simulate a binary decision.
   *
   * This method simulates the given binary decision and updates the state vector
   * and self information bit count accordingly.
   *
   * @param idx the index of the CABAC context
   * @param bin_val the value of the bin
   */
  void encode( const state_vector::size_type idx, const bool bin_val ) {
    assert( 0 <= idx );
    assert( idx < _states.size() );
    const unsigned int bits = bits_tab[ _states[ idx ] ^ bin_val ];
    const unsigned int state_idx = _states[ idx ] >> 1;
    bool val_mps = _states[ idx ] & 1;
    _bits += bits;
    if ( bin_val != val_mps ) {
      assert( bits == FIX8( BITS( PLPS( state_idx ) ) ) );
      if ( state_idx == 0 )
        val_mps = !val_mps;
      _states[ idx ] = ( trans_idx_lps[ state_idx ] << 1 ) | val_mps;
    } else {
      assert( bits == FIX8( BITS( 1 - PLPS( state_idx ) ) ) );
      _states[ idx ] = ( trans_idx_mps[ state_idx ] << 1 ) | val_mps;
    }
  }

  /**
   * Simulate a binary decision using the bypass engine.
   *
   * This method just adds one to the bit count.
   *
   * @param bin_val the value of the bin
   */
  inline void encode_bypass( const bool bin_val ) {
    _bits += 1 << 8;
  }

  /**
   * Get self information bit count.
   *
   * @return self information of the encoded decisions since object creation in bits * 256
   */
  inline unsigned int bits() {
    return _bits;
  }

  /**
   * Reset self information bit count to zero.
   */
  inline void reset() {
    _bits = 0;
  };

};


}

#endif
