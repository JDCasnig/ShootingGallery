#ifndef _cBulletRigidBody_HG_
#define _cBulletRigidBody_HG_

#include <iRigidBody.h>
#include <iShape.h>
#include "cBulletShape.h"
#include "src\btBulletCollisionCommon.h"
#include "src\btBulletDynamicsCommon.h"
class cBulletRigidBody : public nPhysics::iRigidBody
{
public:
	cBulletRigidBody(nPhysics::cRigidBodyDef bodyDef, cBulletShape bodyShape);
	~cBulletRigidBody();

	nPhysics::cRigidBodyDef* GetRigidBodyDef();
	nPhysics::iShape* GetShape();
	btRigidBody* GetPhysicsBody();
private:
	nPhysics::cRigidBodyDef mBodyDef;
	cBulletShape mBodyShape;
	btRigidBody* mBulletRigidBody;
};
#endif // !_cBulletRigidBody_HG_
