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

#ifndef _OHTU7AY3EI_CABAC_DECODER_H
#define _OHTU7AY3EI_CABAC_DECODER_H 1

#include <cabac/common-base.h>

namespace cabac {

/**
 * CABAC %decoder.
 *
 * Reads the bitstream from an arbitrary STL-compatible iterator of unsigned char.
 *
 * Typically, the declaration of the decoder would be done this way:
 *
 * @code
 * typedef std::vector< unsigned char > bitstream;
 * typedef bitstream::const_iterator iter_type;
 *
 * bitstream bs = ... // get bitstream from somewhere
 * cabac::decoder< iter_type > dec( bs.begin(), initial_states );
 *
 * dec.decode( ... ); // decode from here
 * @endcode
 *
 * Note that you can use *any* STL-compatible iterator to read the bitstream from and that a pointer
 * to unsigned char is an iterator, too! (Hence, raw memory access is possible though not recommended.)
 *
 * As another example, to read the bitstream directly from a file, you could use a standard istream_iterator:
 *
 * @code
 * typedef std::istream_iterator< unsigned char > iter_type;
 *
 * std::istream is( "some.file" );
 * cabac::decoder< iter_type >( iter_type( is ), initial_states );
 *
 * dec.decode( ... ); // decode from here
 * @endcode
 */
template< typename I >
class decoder : public impl::decoder_base {

  I _data;

  unsigned int _range;
  unsigned int _offset;
  unsigned int _mask;

  // prohibit duplication of object
  decoder( const decoder &other );
  decoder& operator=( const decoder &other );

  /**
   * read_bits according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   */
  bool read_bit() {
    const bool b = *_data & _mask;
    _mask >>= 1;
    if ( !_mask ) {
      _data++;
      _mask = 128;
    }
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << "READING BIT " << b << ::std::endl;
#endif
    return b;
  }

  /**
   * RenormD according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   */
  void renorm() {
    while ( _range < 0x100 ) {
      _range <<= 1;
      _offset <<= 1;
      _offset |= read_bit();
    }
  }

  public:

  /**
   * Constructor.
   *
   * @param input an STL-compatible input iterator on a container of unsigned char, used to read the bitstream
   * @param states the initial state vector
   */
  decoder( const I &input, const state_vector &states ) :
    impl::decoder_base( states ),
    _data( input ),
    _range( 0x1fe ),
    _offset( ( *_data++ << 1 ) | ( ( *_data & 128 ) >> 7 ) ),
    _mask( 64 ) {
  }

  /**
   * Decode a binary decision.
   *
   * @see encoder::encode
   *
   * Corresponds to DecodeDecision according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   *
   * @param idx the index of the CABAC context
   * @return the value of the decoded bin
   */
  bool decode( const state_vector::size_type idx ) {
    assert( 0 <= idx );
    assert( idx < _states.size() );
    const unsigned int state_idx = _states[ idx ] >> 1;
    bool val_mps = _states[ idx ] & 1;
#ifdef CABAC_DEBUG_OUTPUT
    ::std::ostringstream s;
    s << ::std::setfill( '0' ) << ::std::right
      <<  "CTX " << ::std::setw( 3 ) << idx
      << " RNG " << ::std::bitset< 16 >( _range ).to_string()
      << " OFS " << ::std::bitset< 16 >( _offset ).to_string()
      << " MPS " << val_mps
      << " IDX " << ::std::setw( 2 ) << state_idx
      << " DEC ";
#endif
    const unsigned int range_idx = ( _range >> 6 ) & 3;
    const unsigned int range_lps = range_tab_lps[ state_idx ][ range_idx ];
    _range -= range_lps;
    bool bin_val;
    if ( _offset >= _range ) {
      bin_val = !val_mps;
      _offset -= _range;
      _range = range_lps;
      if ( state_idx == 0 )
        val_mps = !val_mps;
      _states[ idx ] = ( trans_idx_lps[ state_idx ] << 1 ) | val_mps;
    } else {
      bin_val = val_mps;
      _states[ idx ] = ( trans_idx_mps[ state_idx ] << 1 ) | val_mps;
    }
    renorm();
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << s.str() << bin_val << ::std::endl;
#endif
    return bin_val;
  }

  /**
   * Decode a binary decision using the bypass engine.
   *
   * @see encoder::encode_bypass
   *
   * Corresponds to DecodeBypass according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   *
   * @return the value of the decoded bin
   */
  bool decode_bypass() {
#ifdef CABAC_DEBUG_OUTPUT
    ::std::ostringstream s;
    s << ::std::setfill( '0' ) << ::std::right
      <<  "BYPASS "
      << " RNG " << ::std::bitset< 16 >( _range ).to_string()
      << " OFS " << ::std::bitset< 16 >( _offset ).to_string()
      << " MPS -"
      << " IDX --"
      << " DEC ";
#endif
    _offset <<= 1;
    _offset |= read_bit();
    const bool bin_val = ( _offset >= _range );
    if ( bin_val )
      _offset -= _range;
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << s.str() << bin_val << ::std::endl;
#endif
    return bin_val;
  }

  /**
   * Decode a terminal bit.
   *
   * @see encoder::encode_terminal
   *
   * Corresponds to DecodeTerminate according to ISO/IEC 14496-10 / ITU-T Rec. H.264.
   *
   * @return the value of the decoded bin
   */
  bool decode_terminal() {
#ifdef CABAC_DEBUG_OUTPUT
    ::std::ostringstream s;
    s << ::std::setfill( '0' ) << ::std::right
      <<  "TERM   "
      << " RNG " << ::std::bitset< 16 >( _range ).to_string()
      << " OFS " << ::std::bitset< 16 >( _offset ).to_string()
      << " MPS -"
      << " IDX --"
      << " DEC ";
#endif
    _range -= 2;
    if ( _offset >= _range ) {
#ifdef CABAC_DEBUG_OUTPUT
      ::std::cout << s.str() << '1' << ::std::endl;
#endif
      return 1;
    }
    renorm();
#ifdef CABAC_DEBUG_OUTPUT
    ::std::cout << s.str() << '0' << ::std::endl;
#endif
    return 0;
  }

};

}

#endif
