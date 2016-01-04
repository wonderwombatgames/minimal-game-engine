/**
  * base entities data component
  */

#ifndef ENTITY_COMPONENT_HPP
#define ENTITY_COMPONENT_HPP

#include "utils_types.hpp"

namespace Engine
{
using namespace Utils;

  struct EntityComponent
  {
    // entity unique identifier
    EntityID entityId;

    // define to what class entity belongs
    TypeID typeId;

    // whether or not this entity is active
    bool isActive;

  };

} // end namespace Engine

#endif // ENTITY_COMPONENT_HPP