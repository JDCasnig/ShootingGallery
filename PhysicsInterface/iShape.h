#ifndef _iShape_HG_
#define _iShape_HG_

#include "GLMIncludes.h"

namespace nPhysics
{
	// base class/interface for all shapes

	enum eShapeType
	{
		GHOST_SHAPE		= 0, // yet to be implemented
		SPHERE_SHAPE = 1,
		PLANE_SHAPE = 2,
		//BOX_SHAPE		= 3  // yet to be implemented
	};

		/*maybe these should be in a glm extension folder?*/

		struct sSphere
		{
			sSphere() : Radius(1.f) {}
			sSphere(float radius) : Radius(radius) {}
			float Radius;
		};
		struct sPlane
		{
			sPlane() : Normal(0.f, 1.f, 0.f) {}
			sPlane(const glm::vec3& normal) : Normal(normal) {}
			sPlane(float nX, float nY, float nZ) : Normal(nX, nY, nZ) {}
			//glm::vec3 Position; // the body will have the position
			glm::vec3 Normal;
			//float DotProduct;  // maybe...
		};

	class iShape
	{
	public:
		virtual ~iShape() {}
		// Use GetShapeType()
		// if this isn't the right shape, it will do nothing
		// and return false
		virtual bool GetSphere(sSphere& sphereOut) = 0;
		virtual bool GetPlane(sPlane& planeOut) = 0;
		// eShapeType
		// what shape am I?
		virtual	eShapeType GetShapeType() = 0;


	/*private:
		eShapeType mShapeType;*/
	};
}

#endif