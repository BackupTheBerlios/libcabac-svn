//
// Author: Johannes Ballé <balle@ient.rwth-aachen.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
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
