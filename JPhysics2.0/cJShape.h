#ifndef _cJShape_HG_
#define _cJShape_HG_

#include <iShape.h>


class cJShape : public nPhysics::iShape
{
public:
	cJShape();
	cJShape(nPhysics::sSphere sphereIn);
	cJShape(nPhysics::sPlane planeIn);
	~cJShape();
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

#endif // !_cJShape_HG_