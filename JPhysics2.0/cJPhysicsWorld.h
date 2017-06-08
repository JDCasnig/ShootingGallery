#ifndef _cJPhysicsWorld_HG_
#define _cJPhysicsWorld_HG_

//#include "iRigidBody.h"
//#include <GLMIncludes.h>
#include <iPhysicsWorld.h>
#include <iRigidBody.h>
#include <vector>
class cJPhysicsWorld : public nPhysics::iPhysicsWorld
{
	struct State
	{
		glm::vec3 position, velocity;
	};

public:
	cJPhysicsWorld();
	~cJPhysicsWorld();

		bool AddBody(nPhysics::iRigidBody* body);
		bool AddBody(nPhysics::iSoftBody* body);
	//	virtual bool RemoveBody(iRigidBody* body) = 0;
		void SetGravity(glm::vec3 gravity);
		void GetGravity(glm::vec3& gravity);
		void TimeStep(float dt);
		void ApplyImpulse(nPhysics::iRigidBody* body, glm::vec3 Impulse);
private:
	std::vector<nPhysics::iRigidBody*> mWorldBodies;
	std::vector<nPhysics::iSoftBody*> mWorldSoftBodies;
	glm::vec3 mGravity;
	void PhysicsStep(float deltaTime);
	void CollisionStep(float deltaTime);
	void Solve(nPhysics::cRigidBodyDef* body,  float deltaTime, float radius);
	void SolveSoftBody(nPhysics::cSoftBodyDef* softBody, float deltaTime, float radius);
	State EvaluateDerivative(float deltaTime, nPhysics::cRigidBodyDef* body, const State& currentState);
	float distanceBetweenSpheres(nPhysics::cRigidBodyDef* pSphere1, nPhysics::cRigidBodyDef* pSphere2);
	bool testSphereSphereCollision(nPhysics::cRigidBodyDef* pSphere1, nPhysics::cRigidBodyDef* pSphere2, float radius1, float radius2);


	// Determine whether plane p intersects sphere s
	bool TestSpherePlane(nPhysics::cRigidBodyDef* pSphere1, nPhysics::cRigidBodyDef* pPlane1, float radius, glm::vec3 normal);
	bool cJPhysicsWorld::TestSphereCollisionStep(nPhysics::cRigidBodyDef* pSphere, float radius);
	bool cJPhysicsWorld::TestSphereCollisionStepSoftBody(nPhysics::cRigidBodyDef* pSphere, float radius, int nodeNumber);
	void UpdateNodes(nPhysics::cSoftBodyDef* bodyDef);
	void ApplyElasticForce(nPhysics::cRigidBodyDef * body1, nPhysics::cRigidBodyDef * body2, float relaxedDistance, float seperatingDistance, float elasticStrength, float seperatingStrength);
	void ApplyElasticForce(nPhysics::cRigidBodyDef * body1, nPhysics::cRigidBodyDef * body2, glm::vec3 constraintVector, float relaxedDistance, float seperatingDistance, float elasticStrength, float seperatingStrength);
	void ApplyConstraint(nPhysics::sNodeConstraint constraint);
};

#endif // !_cJPhysicsWorld_HG_