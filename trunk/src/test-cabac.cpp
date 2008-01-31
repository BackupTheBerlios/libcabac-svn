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

#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cabac.h>

using namespace std;
using namespace cabac;

template< typename T >
struct rand_gen {
  typedef T result_type;
  result_type m;
  result_type s;
  result_type operator()() { return ( rand() % m ) - s; }
  rand_gen( result_type modulus, result_type shift ) :
    m( modulus ), s( shift ) {
  }
};

int main( int argc, char *argv[] ) {

  if ( argc != 3 ) {
    cout << "syntax: " << argv[ 0 ] << " #states #decisions" << endl;
    return -1;
  }

  const int num_states = atoi( argv[ 1 ] );
  const int num_decisions = atoi( argv[ 2 ] );

  srand ( time( 0 ) );

  state_vector states;
  vector< int > indexes;
  vector< bool > decisions;
  vector< int > ints;

  vector< unsigned char > buffer;

  generate_n( back_insert_iterator< state_vector >( states ), num_states, rand_gen< int >( 128, 0 ) );
  generate_n( back_insert_iterator< vector< int > >( indexes ), num_decisions, rand_gen< int >( num_states + 1, 0 ) );
  generate_n( back_insert_iterator< vector< bool > >( decisions ), num_decisions, rand_gen< int >( 2, 0 ) );
  generate_n( back_insert_iterator< vector< int > >( ints ), num_decisions, rand_gen< int >( 20000, 10000 ) );

  frequency_vector freq_enc;

  {
    counting_encoder< back_insert_iterator< vector< unsigned char > > >
      e( back_insert_iterator< vector< unsigned char > >( buffer ), states );

#ifdef CABAC_DEBUG_OUTPUT
    cout << "ENCODING:" << endl;
#else
    cout << ::std::setw( ( int ) ceil( log10( num_decisions ) ) ) << right;
#endif

    for ( int i = 0; i < num_decisions; ++i ) {
#ifndef CABAC_DEBUG_OUTPUT
      cout << "\rencoding decisions: " << i + 1 << flush;
#endif
      if ( indexes[ i ] == 0 )
        e.encode_bypass( decisions[ i ] );
      else
        e.encode( indexes[ i ] - 1, decisions[ i ] );
    }
    cout << endl;
    for ( int i = 0; i < num_decisions; ++i ) {
#ifndef CABAC_DEBUG_OUTPUT
      cout << "\rencoding integers: " << i + 1 << flush;
#endif
      encode_seg( e, ints[ i ], 2, 0, 20 );
    }
    cout << endl;

    freq_enc = e.frequencies();
  }

#ifdef CABAC_DEBUG_OUTPUT
  cout << "BITSTREAM:" << endl;
  for ( unsigned int i = 0; i < buffer.size(); ++i ) {
    cout << bitset< 8 >( buffer[ i ] ).to_string() << " ";
  }
  cout << endl << endl;
  cout << "DECODING:" << endl;
#endif

  counting_decoder< vector< unsigned char >::const_iterator >
    d( buffer.begin(), states );

  bool b;
  int x;
  unsigned int errors = 0;

  for ( int i = 0; i < num_decisions; ++i ) {
#ifndef CABAC_DEBUG_OUTPUT
    cout << "\rdecoding decisions: " << i + 1 << flush;
#endif
    if ( indexes[ i ] == 0 )
      b = d.decode_bypass();
    else
      b = d.decode( indexes[ i ] - 1 );
    if ( b != decisions[ i ] )
      ++errors;
  }
  cout << endl;
  for ( int i = 0; i < num_decisions; ++i ) {
#ifndef CABAC_DEBUG_OUTPUT
    cout << "\rdecoding integers: " << i + 1 << flush;
#endif
    x = decode_seg( d, 2, 0, 20 );
    if ( x != ints[ i ] )
      ++errors;
  }
  cout << endl;

  cout << errors << " decoder mismatch(es)." << endl;

  if ( d.frequencies() == freq_enc ) {
    cout << "Number of encoded and decoded decisions matches." << endl;
    return errors ? 1 : 0;
  } else {
    cout << "Number of encoded and decoded decisions does not match." << endl;
    return 1;
  }

}
