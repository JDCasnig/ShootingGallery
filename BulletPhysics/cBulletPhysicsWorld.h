#ifndef  _cBulletPhysicsWorld_HG_
#define  _cBulletPhysicsWorld_HG_

#include <iPhysicsWorld.h>

#include <GLMIncludes.h>
#include "src\btBulletCollisionCommon.h"
#include "src\btBulletDynamicsCommon.h"
#include "cBulletRigidBody.h"
#include <vector>

class cBulletPhysicsWorld : public nPhysics::iPhysicsWorld
{
public:
	cBulletPhysicsWorld();
	~cBulletPhysicsWorld();

	bool AddBody(nPhysics::iRigidBody* body);
	bool AddBody(nPhysics::iSoftBody* body);
	//	virtual bool RemoveBody(iRigidBody* body) = 0;
	void SetGravity(glm::vec3 gravity);
	void GetGravity(glm::vec3& gravity);
	void TimeStep(float dt);
	void ApplyImpulse(nPhysics::iRigidBody* body, glm::vec3 Impulse) ;
private:



	void cBulletPhysicsWorld::createEmptyDynamicsWorld();
	btDefaultCollisionConfiguration*	m_collisionConfiguration;
	btCollisionDispatcher*	m_dispatcher; 
	btDbvtBroadphase*	m_broadphase;
	btSequentialImpulseConstraintSolver* m_solver; 
	btDiscreteDynamicsWorld*	m_dynamicsWorld;

	std::vector<nPhysics::iRigidBody*> mBodies;
};

#endif