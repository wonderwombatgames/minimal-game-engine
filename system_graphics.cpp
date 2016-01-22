#include "system_graphics.hpp"
#include "component_transform.hpp"

#include <iostream>

namespace W2E
{

namespace System
{

GraphicComponentBinder::GraphicComponentBinder(SystemsInterface* system,
                                               Component::GraphicInterface* resource,
                                               ComponentsHashMap* components)
    : system_(system)
    , resource_(resource)
    , components_(components)
{
}

ErrorCode GraphicComponentBinder::toEntity(EntityRegistrar* sp)
{
  if(system_ != nullptr)
  {
    EntityID id = this->registerIntoSystem_(system_, sp);

    auto it = this->components_->find(id);
    if(it != this->components_->end())
    {
      it->second.resource = resource_;
      return NO_ERROR;
    }
  }
  return UNKNOWN_ERROR;
}

void Graphics::insert(Component::EntityPod& entity)
{
  if(entity.transform)
  {
    Component::GraphicPod pod;
    this->components_.emplace(entity.entityId, pod);
    this->components_[entity.entityId].transformData = entity.transform;
  }
}

void Graphics::remove(const Component::EntityPod& entity)
{
  auto it = this->components_.find(entity.entityId);
  if(it != this->components_.end())
  {
    this->components_.erase(it);
  }
}

ErrorCode Graphics::setCameraTransform(Component::TransformPod& transformData)
{
  this->camera_ = transformData;
  return NO_ERROR;
}

void Graphics::tick(TimeDim delta)
{
  for(auto comp : this->components_)
  {
    if(comp.second.isVisible && comp.second.resource)
    {
      comp.second.resource->paint(comp.second, this->camera_);
    }
  }
}

ComponentBinderPtr Graphics::getComponentBinder(ResourceID resourceId)
{
  ComponentBinderPtr retVal;
  auto it = resources_.find(resourceId);
  if(it != resources_.end())
  {
    retVal.reset(new GraphicComponentBinder(this, it->second.get(), &components_));
  }
  return retVal;
}

} // end namespace System

} // end namespace W2E
