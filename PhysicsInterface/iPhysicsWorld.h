#ifndef _iPhysicsWorld_HG_
#define _iPhysicsWorld_HG_

#include "iRigidBody.h"
#include "iSoftBody.h"
//#include <GLMIncludes.h>
namespace nPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual bool AddBody(iRigidBody* body) = 0;
		virtual bool AddBody(iSoftBody* body) = 0;
	//	virtual bool RemoveBody(iRigidBody* body) = 0;
		virtual void SetGravity(glm::vec3 gravity)=0;
		virtual void GetGravity(glm::vec3& gravity)=0;
		virtual void TimeStep(float dt) = 0;
		virtual void ApplyImpulse(iRigidBody* body,glm::vec3 Impulse) = 0;
	};
}

#endif // !_iPhysicsWorld_HG_