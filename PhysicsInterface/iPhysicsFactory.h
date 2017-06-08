#ifndef _iPhysicsFactory_HG_
#define _iPhysicsFactory_HG_
//#include "iRigidBody.h"
//#include "cRigidBodyDef.h"
//#include "shapes.h"
#include "iShape.h"
#include "iPhysicsWorld.h"
#include "cRigidBodyDef.h"
#include "iRigidBody.h"
namespace nPhysics
{
	class iPhysicsFactory
	{
	public:
		
		virtual ~iPhysicsFactory() {}
		virtual iPhysicsWorld* CreatePhysicsWorld() = 0;
		virtual iRigidBody* CreateRigidBody(cRigidBodyDef& rbDef, iShape* shape) = 0;
		virtual iSoftBody* CreateSoftBody(cSoftBodyDef* sbDef) = 0;
		virtual iShape* CreateShape(const sSphere& sphere) = 0;
		virtual iShape* CreateShape(const sPlane& plane) = 0;
	};
}
#endif