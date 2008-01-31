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

#ifndef _OHTU7AY3EI_CABAC_COMMON_IMPL_H
#define _OHTU7AY3EI_CABAC_COMMON_IMPL_H 1

#include <cabac/common.h>

namespace cabac {
namespace impl {

/**
 * @internal CABAC state engine base class.
 */
class base {

  protected:

  state_vector _states;

  base( const state_vector &states ) :
    _states( states ) {
  }

  base( const base &other ) :
    _states( other._states ) {
  }

  base& operator=( const base &other ) {
    _states = other._states;
    return *this;
  }

  public:

  /**
   * Get current state vector.
   *
   * @return a reference to a state_vector containing the current states of the CABAC engine.
   */
  inline const state_vector& states() const {
    return _states;
  }

};

/**
 * @internal CABAC decoder base class.
 */
typedef base decoder_base;

}
}

#endif
