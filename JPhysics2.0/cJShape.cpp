#include "cJShape.h"


cJShape::cJShape()
{
	mShapeType = nPhysics::GHOST_SHAPE;
}

cJShape::cJShape(nPhysics::sSphere sphereIn)
{
	mSphere = sphereIn;
	mShapeType = nPhysics::SPHERE_SHAPE;
}

cJShape::cJShape(nPhysics::sPlane planeIn)
{
	mPlane = planeIn;
	mShapeType = nPhysics::PLANE_SHAPE;
}

cJShape::~cJShape()
{

}
// Use GetShapeType()
// if this isn't the right shape, it will do nothing
// and return false
bool cJShape::GetSphere(nPhysics::sSphere& sphereOut)
{

	if (this->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
	//if (mShapeType == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		sphereOut = mSphere;
		return true;
	}
	
	return false;
}
bool cJShape::GetPlane(nPhysics::sPlane& planeOut)
{
	//if (mShapeType == nPhysics::eShapeType::SPHERE_SHAPE)

	if (this->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		planeOut = mPlane;
		return true;
	}

	return false;
}

nPhysics::eShapeType cJShape::GetShapeType()
{
	return mShapeType;
}