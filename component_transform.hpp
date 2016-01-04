/**
  * transform data component
  */

#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "utils_types.hpp"

namespace Engine
{
using namespace Utils;

  struct TransformComponent
  {
    // kind of space 2D/3D
    eSpace kind;

    // position
    // absolute in pixels (float values)
    // can contain z-order for 2d spaces
    Vector3 position;

    // rotation
    // values between 0.0 - 1.0  (= 0 - 360)
    Rotation3 rotation;

    // scales
    // <1.0 : smaller | > 1.0 : larger
    // <0.0 : mirror
    // this attribure is shared by physics and
    // graphics, and might have application
    // to the entity behaviour as well
    Vector3 scale;
  };

} // end namespace Engine

#endif // TRANSFORM_COMPONENT_HPP
