#ifndef _cJRigidBody_HG_
#define _cJRigidBody_HG_

#include <iRigidBody.h>
#include <iShape.h>
#include "cJShape.h"
class cJRigidBody : public nPhysics::iRigidBody 
{
public:
	cJRigidBody(nPhysics::cRigidBodyDef bodyDef,cJShape bodyShape);
	~cJRigidBody();

	nPhysics::cRigidBodyDef* GetRigidBodyDef();
	nPhysics::iShape* GetShape();
	glm::vec3 rotSpeed;
	glm::quat rot;
private:
	nPhysics::cRigidBodyDef mBodyDef;
	cJShape mBodyShape;


};
#endif // !_cJRigidBody_HG_
