#include "cJRigidBody.h"
#include <iostream>

cJRigidBody::cJRigidBody(nPhysics::cRigidBodyDef bodyDef, cJShape bodyShape)
{
	mBodyDef = bodyDef;

	if (bodyShape.GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		nPhysics::sPlane temp;
		bodyShape.GetPlane(temp);
		mBodyShape = temp;
	}
	else if (bodyShape.GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		nPhysics::sSphere temp;
		bodyShape.GetSphere(temp);
		mBodyShape = temp;
	}
	
	//mBodyShape = bodyShape;
	//std::cout << mBodyShape.GetShapeType() << std::endl;
}

cJRigidBody::~cJRigidBody()
{
}

nPhysics::cRigidBodyDef * cJRigidBody::GetRigidBodyDef()
{
	return &mBodyDef;
}

nPhysics::iShape * cJRigidBody::GetShape()
{
	return &mBodyShape;
}
