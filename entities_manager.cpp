/**
  *
  */

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <deque>
#include "system_interface.hpp"
#include "entities_manager.hpp"

namespace Engine
{
using namespace std;
using namespace System;

//
// EntitiesManager Methods
//

EntitiesManager * EntitiesManager::instance()
{
  static EntitiesManager * s_instance = nullptr;
  if (nullptr == s_instance)
  {
    s_instance = new EntitiesManager();
  }
  return s_instance;
}

bool EntitiesManager::addComponent(EntityID entitityId, SystemsInterface & system)
{
  auto entity = this->_entities.find(entitityId);
  if(entity != this->_entities.end())
  {
    entity->second->addComponent(system);
    return true;
  }
  return false;
}

bool EntitiesManager::destroyEntity(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    entity->second->destroy(true);
    return true;
  }
  return false;
}

bool EntitiesManager::suspendEntity(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return entity->second->suspend();
  }
  return false;
}

bool EntitiesManager::resumeEntity(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return entity->second->resume();
  }
  return false;
}


bool EntitiesManager::isEntityActive(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return entity->second->isActive();
  }
  return false;
}

const string * EntitiesManager::lookUpentityName(EntityID entityId)
{

  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return &(entity->second->getName());
  }
  return nullptr;
}

EntityID EntitiesManager::lookUpEntityId(const string& name)
{
  auto entity = this->_lookUp.find(name);
  if (entity != this->_lookUp.end())
  {
    return entity->second;
  }
  return InvalidID;
}

int EntitiesManager::refreshEntities()
{
  // find all entities marked to be destroied
  deque<EntityID> toDestroy;
  for(auto entity : this->_entities)
  {
    if(entity.second->destroy())
    {
      toDestroy.push_back(entity.second->_entityData.entityId);
    }
  }
  // destroy the marked entities
  for(auto id : toDestroy)
  {
    this->_entities.erase(id);
  }
  this->_count = this->_entities.size();
  // return number of entities after clean up
  return this->_count;
}

EntityID EntitiesManager::newId()
{
  const EntityID maxEntities = EntitiesManager::MAX_ENTITIES_AMOUNT;
  // use current time as seed for random generator
  srand(time(0));
  EntityID randomId = 1 + ((rand() * (maxEntities + 1)) % maxEntities);
  int attempts = 0;
  // check that the id was not used yet
  while (this->_entities.count(randomId)>0)
  {
    randomId = (rand() * (maxEntities + 1)) % maxEntities;
    ++attempts;
    // this ensures we do not allocate more entities than the MAX
    assert( attempts < maxEntities);
  }
  return randomId;
}

} // end namespace Engine
