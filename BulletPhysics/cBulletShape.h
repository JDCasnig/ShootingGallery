#ifndef _cBulletShape_HG_
#define _cBulletShape_HG_

#include <iShape.h>


class cBulletShape : public nPhysics::iShape
{
public:
	cBulletShape();
	cBulletShape(nPhysics::sSphere sphereIn);
	cBulletShape(nPhysics::sPlane planeIn);
	~cBulletShape();
	// Use GetShapeType()
	// if this isn't the right shape, it will do nothing
	// and return false
	bool GetSphere(nPhysics::sSphere& sphereOut);
	bool GetPlane(nPhysics::sPlane& planeOut);
	// eShapeType
	// what shape am I?
	nPhysics::eShapeType GetShapeType();// { return mShapeType; }


private:
	nPhysics::eShapeType mShapeType;
	nPhysics::sSphere mSphere;
	nPhysics::sPlane mPlane;
};

#endif // !_cBulletShape_HG_