/**
  * transform data component
  */

#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "utils_types.hpp"

namespace Engine
{
using namespace Utils;

namespace Component
{

  struct TransformPod
  {

    // position
    // absolute in pixels (float values)
    // can contain z-order for 2d spaces
    Vector3 position;//{ .x = 0.0f, .y = 0.0f, .zorder = 0.0f};

    // rotation
    // values between 0.0 - 1.0  (= 0 - 360)
    Rotation3 rotation;//{ .roll = 0.0f, .pitch = 0.0f, .yaw = 0.0f};

    // scales
    // <1.0 = smaller | > 1.0 = larger
    // <0.0 = mirror
    // this attribure is shared by physics and
    // graphics, and might have application
    // to the entity behaviour as well
    Vector3 scale;//{ .x = 1.0f, .y = 1.0f, .z = 1.0f};

    // kind of space 2D/3D
    eSpace kind;

  };


} // namespace Component

} // end namespace Engine

#endif // TRANSFORM_COMPONENT_HPP
