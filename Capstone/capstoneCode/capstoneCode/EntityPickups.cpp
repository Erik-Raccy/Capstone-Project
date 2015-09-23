#include "EntityPickups.h"
#include "C4Engine.h"
using namespace C4;

//Constructors
EntityPickups::EntityPickups() : EntityUndamageable()
{

}

EntityPickups::EntityPickups(EntityType _type, Point3D _location, char* _model) :EntityUndamageable(_type, _location, _model)
{
}


EntityPickups::~EntityPickups()
{
}

void EntityPickups::OnPickup()
{

}
