#include "cBulletPhysicsWorld.h"
#include <cRigidBodyDef.h>
#include <iostream>
cBulletPhysicsWorld::cBulletPhysicsWorld()
{
	createEmptyDynamicsWorld();
}

cBulletPhysicsWorld::~cBulletPhysicsWorld()
{
	createEmptyDynamicsWorld();
}

bool cBulletPhysicsWorld::AddBody(nPhysics::iRigidBody* body)
{
	cBulletRigidBody* temp = (cBulletRigidBody*) body;
	m_dynamicsWorld->addRigidBody(temp->GetPhysicsBody());
	mBodies.push_back(body);
	return true;
}

bool cBulletPhysicsWorld::AddBody(nPhysics::iSoftBody * body)
{
	return false;
}

//	virtual bool RemoveBody(iRigidBody* body) = 0;
void cBulletPhysicsWorld::SetGravity(glm::vec3 gravity)
{

}

void cBulletPhysicsWorld::GetGravity(glm::vec3& gravity)
{

}

void cBulletPhysicsWorld::TimeStep(float dt)
{
	m_dynamicsWorld->stepSimulation(dt);
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{

		cBulletRigidBody* tempBody = (cBulletRigidBody*)mBodies[i];
		btRigidBody* tempBulletBody = tempBody->GetPhysicsBody();
		tempBulletBody->updateInertiaTensor();
		nPhysics::cRigidBodyDef* temp = mBodies[i]->GetRigidBodyDef();
		btVector3 tempPos = tempBulletBody->getCenterOfMassPosition();
		//std::cout << "Position is: " << tempPos.x() << " : " << tempPos.y() << " : " << tempPos.z() << std::endl;
		temp->position = glm::vec3(tempPos.x(), tempPos.y(), tempPos.z());
		btMotionState* tempState = tempBulletBody->getMotionState();
		btQuaternion tempOrientation = tempBulletBody->getOrientation();
		btQuaternion tempOrientation2 =tempBulletBody->getOrientation();
		glm::quat  tempQuat = glm::quat(tempOrientation.w(), tempOrientation.x(), tempOrientation.y(), tempOrientation.z());
		temp->qOrientation = tempQuat;
		//temp->qOrientation.x = tempOrientation.x();
		//temp->qOrientation.y = tempOrientation.y();
		//temp->qOrientation.z = tempOrientation.z();
		//temp->qOrientation.w = tempOrientation.w();
		//std::cout << "Position is: " << temp->position.x << " : " << temp->position.y << " : " << temp->position.z << std::endl;
	//	std::cout << "tempQuat is: " << tempQuat.x << " : " << tempQuat.y << " : " << tempQuat.z << " : " << tempQuat.z << std::endl;
	//	std::cout << "tempBodyDefQuat is: " << temp->qOrientation.x << " : " << temp->qOrientation.y << " : " << temp->qOrientation.z << " : " << temp->qOrientation.z << std::endl;
		//btMotionState* tempState = tempBody->getMotionState();
		//btTransform tranState;
		//tempState->getWorldTransform(tranState);
		
	}
}

void cBulletPhysicsWorld::ApplyImpulse(nPhysics::iRigidBody * body, glm::vec3 Impulse)
{
	for (unsigned int i = 0; i < mBodies.size(); i++)
	{
		if (mBodies[i] == body)
		{

			cBulletRigidBody* tempBody = (cBulletRigidBody*)mBodies[i];
			btRigidBody* tempBulletBody = tempBody->GetPhysicsBody();
			tempBulletBody->applyCentralImpulse(btVector3(Impulse.x,Impulse.y,Impulse.z));
		}
	}
}

 void cBulletPhysicsWorld::createEmptyDynamicsWorld() {
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
	
}