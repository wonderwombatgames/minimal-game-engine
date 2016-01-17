/**
  *
  */

#ifndef SYSTEM_TRANSFORM_HPP
#define SYSTEM_TRANSFORM_HPP

#include "system_interface.hpp"
#include "component_transform.hpp"

namespace Engine
{
using namespace std;

namespace System
{

  class Transform : public SystemsInterface
  {
  public:
    Transform();
    Transform(const char * name);
    Transform(Transform & other) = delete;
    virtual ~Transform();

  protected:
    virtual void insert(Component::EntityPod & entity) override;
    virtual void remove(const Component::EntityPod & entity) override;
    virtual void tick(TimeDim delta) override;

    HashMap< EntityID, Component::TransformPod > _components;
  };

} // end namespace System


} // end namespace Engine

#endif // SYSTEM_TRANSFORM_HPP