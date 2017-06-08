#ifndef _cBulletFactory_HG_
#define _cBulletFactory_HG_
#include <iPhysicsFactory.h>
#include <iRigidBody.h>
#include "cBulletPhysicsWorld.h"
class cBulletFactory : public nPhysics::iPhysicsFactory
{
public:
	cBulletFactory();
	~cBulletFactory();
	nPhysics::iPhysicsWorld* CreatePhysicsWorld();
	nPhysics::iRigidBody* CreateRigidBody(nPhysics::cRigidBodyDef& rbDef, nPhysics::iShape* shape);
	nPhysics::iSoftBody* CreateSoftBody(nPhysics::cSoftBodyDef& sbDef);
	nPhysics::iShape* CreateShape(const nPhysics::sSphere& sphere);
	nPhysics::iShape* CreateShape(const nPhysics::sPlane& plane);


};



#endif // !_cJPhysicsFactory_HG_

