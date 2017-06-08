#include "cJPhysicsFactory.h"
#include "cJPhysicsWorld.h"
#include "cJShape.h"
#include "cJRigidBody.h"
#include "cJSoftBody.h"
#include <cSoftBodyDef.h>
cJPhysicsFactory::cJPhysicsFactory()
{

}

cJPhysicsFactory::~cJPhysicsFactory()
{

}

nPhysics::iPhysicsWorld* cJPhysicsFactory::CreatePhysicsWorld()
{
	nPhysics::iPhysicsWorld* world = 0;
	world = new cJPhysicsWorld();
	return world;
	//return new cJPhysicsFactory();
}

nPhysics::iRigidBody * cJPhysicsFactory::CreateRigidBody(nPhysics::cRigidBodyDef & rbDef, nPhysics::iShape* shape)
{
	cJShape* tempShape;
	if (shape->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		nPhysics::sSphere temp;
		shape->GetSphere(temp);
		tempShape = new cJShape(temp);
	}
	else if (shape->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		nPhysics::sPlane temp;
		shape->GetPlane(temp);
		tempShape = new cJShape(temp);
	}
	else
	{
		cJShape* tempShape = new cJShape(shape->GetShapeType());
	}
	//cJShape* tempShape = new cJShape(shape->GetShapeType());
	nPhysics::iRigidBody* tempBody = new cJRigidBody(rbDef, *tempShape);
	return tempBody;
}

nPhysics::iSoftBody* cJPhysicsFactory::CreateSoftBody(nPhysics::cSoftBodyDef* sbDef)
{
	return new cJSoftBody(sbDef);
}

nPhysics::iShape* cJPhysicsFactory::CreateShape(const nPhysics::sSphere& sphere)
{
	
	nPhysics::iShape* tempShape = new cJShape(sphere);
	unsigned int temp = tempShape->GetShapeType();
	return tempShape;
	//nPhysics::iShape* newShape = new 
}
nPhysics::iShape* cJPhysicsFactory::CreateShape(const nPhysics::sPlane& plane)
{
	nPhysics::iShape* tempShape = new cJShape(plane);
	return tempShape;
}



