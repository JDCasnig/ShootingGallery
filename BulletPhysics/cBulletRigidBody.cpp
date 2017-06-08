#include "cBulletRigidBody.h"
#include <iostream>

cBulletRigidBody::cBulletRigidBody(nPhysics::cRigidBodyDef bodyDef, cBulletShape bodyShape)
{
	mBodyDef = bodyDef;

	if (bodyShape.GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
	{
		nPhysics::sPlane temp;
		bodyShape.GetPlane(temp);
		mBodyShape = temp;

		btVector3 inertia = btVector3(0, 0, 0);

		btCollisionShape* shape = new btStaticPlaneShape(btVector3(temp.Normal.x, temp.Normal.y, temp.Normal.z), 0);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(bodyDef.position.x, bodyDef.position.y, bodyDef.position.z)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			inertia    // local inertia
			);

		mBulletRigidBody = new btRigidBody(rigidBodyCI);
		mBulletRigidBody->setFriction(btScalar(0.4));
		mBulletRigidBody->setRestitution(btScalar(0.5));
		
		mBulletRigidBody->setRollingFriction(btScalar(0.5));
		mBulletRigidBody->updateInertiaTensor();
		//mBulletRigidBody->setLinearVelocity(btVector3(bodyDef.velocity.x, bodyDef.velocity.y, bodyDef.velocity.z));


	}
	else if (bodyShape.GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
	{
		nPhysics::sSphere temp;
		bodyShape.GetSphere(temp);
		mBodyShape = temp;

		btScalar mass = btScalar(bodyDef.mass);
		btCollisionShape* shape = new btSphereShape(temp.Radius);
		btVector3 inertia;
		btQuaternion quat;
		shape->calculateLocalInertia(1.0f, inertia);
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(quat, btVector3(bodyDef.position.x, bodyDef.position.y, bodyDef.position.z)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,                  // mass
			motionState,        // initial position
			shape,              // collision shape of body
			inertia    // local inertia
			);

		mBulletRigidBody = new btRigidBody(rigidBodyCI);
		mBulletRigidBody->setLinearVelocity(btVector3(bodyDef.velocity.x, bodyDef.velocity.y, bodyDef.velocity.z));
		mBulletRigidBody->setFriction(btScalar(0.4));
		mBulletRigidBody->setRestitution(btScalar(.9));
		mBulletRigidBody->setRollingFriction(btScalar(0.5));
		mBulletRigidBody->updateInertiaTensor();
	//	mBulletRigidBody->updateInertiaTensor();

	}

	//mBodyShape = bodyShape;
	//std::cout << mBodyShape.GetShapeType() << std::endl;
}

cBulletRigidBody::~cBulletRigidBody()
{
}

nPhysics::cRigidBodyDef * cBulletRigidBody::GetRigidBodyDef()
{
	return &mBodyDef;
}

nPhysics::iShape * cBulletRigidBody::GetShape()
{
	return &mBodyShape;
}

btRigidBody * cBulletRigidBody::GetPhysicsBody()
{
	return mBulletRigidBody;
}
