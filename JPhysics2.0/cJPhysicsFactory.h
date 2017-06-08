#ifndef _cJPhysicsFactory_HG_
#define _cJPhysicsFactory_HG_
#include <iPhysicsFactory.h>
#include <iRigidBody.h>
class cJPhysicsFactory : public nPhysics::iPhysicsFactory
{
public:
	cJPhysicsFactory();
	~cJPhysicsFactory();
	nPhysics::iPhysicsWorld* CreatePhysicsWorld();
	nPhysics::iRigidBody* CreateRigidBody(nPhysics::cRigidBodyDef& rbDef,nPhysics::iShape* shape);
	nPhysics::iSoftBody* CreateSoftBody(nPhysics::cSoftBodyDef* sbDef);
	nPhysics::iShape* CreateShape(const nPhysics::sSphere& sphere);
	nPhysics::iShape* CreateShape(const nPhysics::sPlane& plane);


};



#endif // !_cJPhysicsFactory_HG_

