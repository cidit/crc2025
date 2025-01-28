#pragma once
#include <math/angles.hpp>


// TODO: x and y dont need to be constrained to 0, its all just a ratio anyways

/**
 * this function treats y=1 as y=0 because of how the controller works. probably soething that needs changing eventually, but not very consequential for now.
 */
Angle x_y_to_angle(float x, float y)
{
  if (y == 1 && x == 0)
  {
    return Angle::zero();
  }
  if (x == 0 && y > 0)
  {
    return Angle::from_rad(PI / 2);
  }
  if (x == 0 && y < 1)
  {
    return Angle::from_rad(3 * PI / 2);
  }
  if (x > 0 && y == 1)
  {
    return Angle::zero();
  }
  if (x < 0 && y == 1)
  {
    return Angle::from_rad(PI);
  }
  if ((x != 0) && y < 1)
  {
    return Angle::from_rad(atan2(y, x) + (2 * PI));
  }
  return Angle::from_rad(atan2(y, x));
}
