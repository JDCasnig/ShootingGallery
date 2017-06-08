#include "cBulletFactory.h"
#include "cBulletShape.h"
#include "cBulletRigidBody.h"
cBulletFactory::cBulletFactory()
{

}

cBulletFactory::~cBulletFactory()
{

}

nPhysics::iPhysicsWorld* cBulletFactory::CreatePhysicsWorld()
{
	nPhysics::iPhysicsWorld* world = 0;
	world = new cBulletPhysicsWorld();
	return world;
	//return new cJPhysicsFactory();
}

nPhysics::iRigidBody * cBulletFactory::CreateRigidBody(nPhysics::cRigidBodyDef & rbDef, nPhysics::iShape* shape)
{





	cBulletShape* tempShape;
	if (shape->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		nPhysics::sSphere temp;
		shape->GetSphere(temp);
		tempShape = new cBulletShape(temp);
	}
	else if (shape->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		nPhysics::sPlane temp;
		shape->GetPlane(temp);
		tempShape = new cBulletShape(temp);





	}
	else
	{
		cBulletShape* tempShape = new cBulletShape(shape->GetShapeType());
	}
	//cJShape* tempShape = new cJShape(shape->GetShapeType());
	nPhysics::iRigidBody* tempBody = new cBulletRigidBody(rbDef, *tempShape);
	return tempBody;
//	return NULL;
}

nPhysics::iShape* cBulletFactory::CreateShape(const nPhysics::sSphere& sphere)
{

	nPhysics::iShape* tempShape = new cBulletShape(sphere);
	unsigned int temp = tempShape->GetShapeType();
	return tempShape;
	//nPhysics::iShape* newShape = new 
	//return NULL;
}
nPhysics::iShape* cBulletFactory::CreateShape(const nPhysics::sPlane& plane)
{
	nPhysics::iShape* tempShape = new cBulletShape(plane);
	return tempShape;
//	return NULL;
}

