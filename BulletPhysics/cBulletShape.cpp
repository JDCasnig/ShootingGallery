#include "cBulletShape.h"


cBulletShape::cBulletShape()
{
	mShapeType = nPhysics::GHOST_SHAPE;
}

cBulletShape::cBulletShape(nPhysics::sSphere sphereIn)
{
	mSphere = sphereIn;
	mShapeType = nPhysics::SPHERE_SHAPE;
}

cBulletShape::cBulletShape(nPhysics::sPlane planeIn)
{
	mPlane = planeIn;
	mShapeType = nPhysics::PLANE_SHAPE;
}

cBulletShape::~cBulletShape()
{

}
// Use GetShapeType()
// if this isn't the right shape, it will do nothing
// and return false
bool cBulletShape::GetSphere(nPhysics::sSphere& sphereOut)
{

	if (this->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
		//if (mShapeType == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		sphereOut = mSphere;
		return true;
	}

	return false;
}
bool cBulletShape::GetPlane(nPhysics::sPlane& planeOut)
{
	//if (mShapeType == nPhysics::eShapeType::SPHERE_SHAPE)

	if (this->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		planeOut = mPlane;
		return true;
	}

	return false;
}

nPhysics::eShapeType cBulletShape::GetShapeType()
{
	return mShapeType;
}