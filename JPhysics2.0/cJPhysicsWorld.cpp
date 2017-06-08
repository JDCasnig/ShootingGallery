#include "cJPhysicsWorld.h"
#include <iostream>
#include "cJRigidBody.h"
#include <cSoftBodyDef.h>
cJPhysicsWorld::cJPhysicsWorld()
{
	mGravity = glm::vec3(0.f, -10.f, 0.f);
}

cJPhysicsWorld::~cJPhysicsWorld()
{

}

bool cJPhysicsWorld::AddBody(nPhysics::iRigidBody * body)
{
	mWorldBodies.push_back(body);
	return true;
}

bool cJPhysicsWorld::AddBody(nPhysics::iSoftBody * body)
{
	mWorldSoftBodies.push_back(body);
	return true;
}

void cJPhysicsWorld::SetGravity(glm::vec3 gravity)
{
	mGravity = gravity;
}

void cJPhysicsWorld::GetGravity(glm::vec3 & gravity)
{
	gravity = mGravity;
}

void cJPhysicsWorld::TimeStep(float deltaTime)
{
	PhysicsStep(deltaTime);
//	CollisionStep(deltaTime);
}

void cJPhysicsWorld::ApplyImpulse(nPhysics::iRigidBody * body, glm::vec3 Impulse)
{
	//cJRigidBody* cJTempBody = (cJRigidBody)body;
	//cJTempBody->GetRigidBodyDef();
	nPhysics::cRigidBodyDef* tempBody =  body->GetRigidBodyDef();
	tempBody->force += Impulse;
}

void cJPhysicsWorld::PhysicsStep(float deltaTime)
{
	


	for (unsigned int index = 0; index < mWorldBodies.size(); index++)
	{
		nPhysics::cRigidBodyDef* bodyDef = mWorldBodies[index]->GetRigidBodyDef();
	//	std::cout << "Internal JPhysics:: Original Position: " << bodyDef->position.y << std::endl;
		if (!bodyDef->staticBody)
		{
			//bodyDef->position -= mGravity;
			Solve(bodyDef, deltaTime,1.f);
			bodyDef->velocity = bodyDef->velocity*.98f;
		//	SolveSoftBody(bodyDef, deltaTime, 1.f);
		}
	//	std::cout << "Internal JPhysics:: Updated Position: " << bodyDef->position.y << std::endl;
	}

	for (unsigned int index = 0; index < mWorldSoftBodies.size(); index++)
	{
		nPhysics::cSoftBodyDef* bodyDef = mWorldSoftBodies[index]->GetSoftBodyDef();
		//	std::cout << "Internal JPhysics:: Original Position: " << bodyDef->position.y << std::endl;

		SolveSoftBody(bodyDef, deltaTime, 1.f);
		for (unsigned int i = 0; i < bodyDef->nodes.size(); i++)
		{
			bodyDef->nodes[i]->velocity = bodyDef->nodes[i]->velocity*.98f;
		}
		//for (unsigned int internalBodyIndex = 0; internalBodyIndex < bodyDef->nodes.size(); internalBodyIndex++)
		//{
		//	if (!bodyDef->nodes[internalBodyIndex]->staticBody)
		//	{
		//		//bodyDef->position -= mGravity;
		//		Solve(bodyDef->nodes[internalBodyIndex], deltaTime, 0.25f);

		//	}
		//}

		UpdateNodes(bodyDef);

		//	std::cout << "Internal JPhysics:: Updated Position: " << bodyDef->position.y << std::endl;
	}

}

void cJPhysicsWorld::CollisionStep(float deltaTime)
{
	for (unsigned int index = 0; index < mWorldBodies.size(); index++)
	{
		nPhysics::iShape* shape1 = mWorldBodies[index]->GetShape();
	//	std::cout << "Shape 1 shape:" << shape1->GetShapeType() << std::endl;
	//	std::cout << "Actual Sphere Shape:" << nPhysics::eShapeType::SPHERE_SHAPE << std::endl;
		if (shape1->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
		{
			nPhysics::sSphere sphere1;
			shape1->GetSphere(sphere1);
			for (unsigned int innerIndex = index+1; innerIndex < mWorldBodies.size(); innerIndex++)
			{
				nPhysics::iShape* shape2 = mWorldBodies[innerIndex]->GetShape();
				if (shape2->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)
				
				{
					//nPhysics::sSphere sphere1;
					//shape1->GetSphere(sphere1);
					nPhysics::sSphere sphere2;
					shape2->GetSphere(sphere2);
					//mWorldBodies[index]->GetRigidBodyDef();
					if (testSphereSphereCollision(mWorldBodies[index]->GetRigidBodyDef(), mWorldBodies[innerIndex]->GetRigidBodyDef(), sphere1.Radius, sphere2.Radius))
					{
			//			std::cout << "Collision!" << std::endl;
						glm::vec3 tempVel = mWorldBodies[index]->GetRigidBodyDef()->velocity;
						mWorldBodies[index]->GetRigidBodyDef()->velocity = (mWorldBodies[innerIndex]->GetRigidBodyDef()->velocity/mWorldBodies[index]->GetRigidBodyDef()->mass)*.95f;
						mWorldBodies[innerIndex]->GetRigidBodyDef()->velocity = (tempVel/mWorldBodies[innerIndex]->GetRigidBodyDef()->mass)*.95f;
						
					}
					
 				}
				
				else if (shape2->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
				{
					nPhysics::sPlane plane1;
					shape2->GetPlane(plane1);
					if (TestSpherePlane(mWorldBodies[index]->GetRigidBodyDef(), mWorldBodies[innerIndex]->GetRigidBodyDef(), sphere1.Radius, plane1.Normal))
					{
				//		std::cout << "Collision!" << std::endl;
						mWorldBodies[index]->GetRigidBodyDef()->velocity = (glm::reflect(mWorldBodies[index]->GetRigidBodyDef()->velocity, plane1.Normal))*.95f;
						cJRigidBody* temp = (cJRigidBody*)mWorldBodies[index];
						temp->rotSpeed += mWorldBodies[index]->GetRigidBodyDef()->velocity;
					}
				}
			}


		}
	}
}

void cJPhysicsWorld::Solve(nPhysics::cRigidBodyDef* body, float deltaTime, float radius)
{
//	std::cout << "I'm in the solve routine" << std::endl;
	body->force += mGravity;
	State initialState;
	initialState.position = glm::vec3(0.f);// body.position;
	initialState.velocity = glm::vec3(0.f);// body.velocity;
	State rK1 = EvaluateDerivative(0.f,body, initialState);
	State rK2 = EvaluateDerivative(deltaTime*0.5f,body, rK1);
	State rK3 = EvaluateDerivative(deltaTime*0.5f,body, rK2);
	State rK4 = EvaluateDerivative(deltaTime,body, rK3);

	//std::cout << "rK1 pos: " << rK1.position.x << " " << rK1.position.y << " " << rK1.position.z << std::endl;
	//std::cout << "rK2 pos: " << rK2.position.x << " " << rK2.position.y << " " << rK2.position.z << std::endl;
	//std::cout << "rK3 pos: " << rK3.position.x << " " << rK3.position.y << " " << rK3.position.z << std::endl;
	//std::cout << "rK4 pos: " << rK4.position.x << " " << rK4.position.y << " " << rK4.position.z << std::endl;

	glm::vec3 difference = (rK1.position + 2.f*(rK2.position + rK3.position) + rK4.position)*deltaTime / 6.f;

//	std::cout << "difference pos: " << difference.x << " " << difference.y << " " << difference.z << std::endl;

//	std::cout << "body before pos: " << body->position.x << " " << body->position.y << " " << body->position.z << std::endl;
	
	
	//body->position  += (rK1.position + 2.f*(rK2.position + rK3.position) + rK4.position)*deltaTime / 6.f;

	glm::vec3 tempPosition = body->position;

	body->position += (rK1.position)*deltaTime / 6.f;
	if (!TestSphereCollisionStep(body, radius))
	{
		tempPosition = body->position;
		body->position += 2.f*(rK2.position)*deltaTime / 6.f;
		if (!TestSphereCollisionStep(body, radius))
		{
			tempPosition = body->position;
			body->position += 2.f*(rK3.position)*deltaTime / 6.f;
			if (!TestSphereCollisionStep(body, radius))
			{
				body->position += (rK4.position)*deltaTime / 6.f;
			}
			else
			{
				body->position = tempPosition;
				rK3.velocity = body->velocity;
				rK3.position = tempPosition;
				State rK4 = EvaluateDerivative(deltaTime, body, rK3);
				body->position += (rK4.position)*deltaTime / 6.f;
				body->velocity += (rK4.velocity)*deltaTime / 6.f;
				
			}
		}
		else
		{
			body->position = tempPosition;
			rK2.position = tempPosition;
			rK2.velocity = body->velocity;
			State rK3 = EvaluateDerivative(deltaTime*0.5f, body, rK2);
			State rK4 = EvaluateDerivative(deltaTime, body, rK3);
			body->position += 2.f*(rK3.position)*deltaTime / 6.f;
			body->position += (rK4.position)*deltaTime / 6.f;
			body->velocity += 2.f*(rK3.velocity)*deltaTime / 6.f;
			body->velocity += (rK4.velocity)*deltaTime / 6.f;
		}
	}
	else
	{
		body->position = tempPosition;
		rK1.position = tempPosition;
		rK1.velocity = body->velocity;
		State rK2 = EvaluateDerivative(deltaTime*0.5f, body, rK1);
		State rK3 = EvaluateDerivative(deltaTime*0.5f, body, rK2);
		State rK4 = EvaluateDerivative(deltaTime, body, rK3);

		body->position += 2.f*(rK2.position)*deltaTime / 6.f;
		body->position += 2.f*(rK3.position)*deltaTime / 6.f;
		body->position += (rK4.position)*deltaTime / 6.f;
		body->velocity += 2.f*(rK2.velocity)*deltaTime / 6.f;
		body->velocity += 2.f*(rK3.velocity)*deltaTime / 6.f;
		body->velocity += (rK4.velocity)*deltaTime / 6.f;
	}




	//TestSphereCollisionStep(body, 1.f);
	//body->position += 2.f*(rK3.position)*deltaTime / 6.f;
	//TestSphereCollisionStep(body, 1.f);
	//body->position += (rK4.position)*deltaTime / 6.f;
	//TestSphereCollisionStep(body, 1.f);
	


//	std::cout << "body after pos: " << body->position.x << " " << body->position.y << " " << body->position.z << std::endl;
	body->velocity += (rK1.velocity + 2.f*(rK2.velocity + rK3.velocity) + rK4.velocity)*deltaTime / 6.f;
	
	body->force = glm::vec3(0.f);
}

void cJPhysicsWorld::SolveSoftBody(nPhysics::cSoftBodyDef* softBody, float deltaTime, float radius)
{

	for (unsigned int i = 0; i < softBody->nodes.size(); i++)
	{

		nPhysics::cRigidBodyDef* body = softBody->nodes[i];
		if (!body->staticBody)
		{


			//	std::cout << "I'm in the solve routine" << std::endl;
			body->force += mGravity;
			State initialState;
			initialState.position = glm::vec3(0.f);// body.position;
			initialState.velocity = glm::vec3(0.f);// body.velocity;
			State rK1 = EvaluateDerivative(0.f, body, initialState);
			State rK2 = EvaluateDerivative(deltaTime*0.5f, body, rK1);
			State rK3 = EvaluateDerivative(deltaTime*0.5f, body, rK2);
			State rK4 = EvaluateDerivative(deltaTime, body, rK3);

			//std::cout << "rK1 pos: " << rK1.position.x << " " << rK1.position.y << " " << rK1.position.z << std::endl;
			//std::cout << "rK2 pos: " << rK2.position.x << " " << rK2.position.y << " " << rK2.position.z << std::endl;
			//std::cout << "rK3 pos: " << rK3.position.x << " " << rK3.position.y << " " << rK3.position.z << std::endl;
			//std::cout << "rK4 pos: " << rK4.position.x << " " << rK4.position.y << " " << rK4.position.z << std::endl;

			glm::vec3 difference = (rK1.position + 2.f*(rK2.position + rK3.position) + rK4.position)*deltaTime / 6.f;

			//	std::cout << "difference pos: " << difference.x << " " << difference.y << " " << difference.z << std::endl;

			//	std::cout << "body before pos: " << body->position.x << " " << body->position.y << " " << body->position.z << std::endl;


			//body->position  += (rK1.position + 2.f*(rK2.position + rK3.position) + rK4.position)*deltaTime / 6.f;

			glm::vec3 tempPosition = body->position;

			body->position += (rK1.position)*deltaTime / 6.f;
			if (!TestSphereCollisionStepSoftBody(body, radius, i))
			{
				tempPosition = body->position;
				body->position += 2.f*(rK2.position)*deltaTime / 6.f;
				if (!TestSphereCollisionStepSoftBody(body, radius, i))
				{
					tempPosition = body->position;
					body->position += 2.f*(rK3.position)*deltaTime / 6.f;
					if (!TestSphereCollisionStepSoftBody(body, radius, i))
					{
						body->position += (rK4.position)*deltaTime / 6.f;
					}
					else
					{
						body->position = tempPosition;
						rK3.velocity = body->velocity;
						rK3.position = tempPosition;
						State rK4 = EvaluateDerivative(deltaTime, body, rK3);
						body->position += (rK4.position)*deltaTime / 6.f;
						body->velocity += (rK4.velocity)*deltaTime / 6.f;

					}
				}
				else
				{
					body->position = tempPosition;
					rK2.position = tempPosition;
					rK2.velocity = body->velocity;
					State rK3 = EvaluateDerivative(deltaTime*0.5f, body, rK2);
					State rK4 = EvaluateDerivative(deltaTime, body, rK3);
					body->position += 2.f*(rK3.position)*deltaTime / 6.f;
					body->position += (rK4.position)*deltaTime / 6.f;
					body->velocity += 2.f*(rK3.velocity)*deltaTime / 6.f;
					body->velocity += (rK4.velocity)*deltaTime / 6.f;
				}
			}
			else
			{
				body->position = tempPosition;
				rK1.position = tempPosition;
				rK1.velocity = body->velocity;
				State rK2 = EvaluateDerivative(deltaTime*0.5f, body, rK1);
				State rK3 = EvaluateDerivative(deltaTime*0.5f, body, rK2);
				State rK4 = EvaluateDerivative(deltaTime, body, rK3);

				body->position += 2.f*(rK2.position)*deltaTime / 6.f;
				body->position += 2.f*(rK3.position)*deltaTime / 6.f;
				body->position += (rK4.position)*deltaTime / 6.f;
				body->velocity += 2.f*(rK2.velocity)*deltaTime / 6.f;
				body->velocity += 2.f*(rK3.velocity)*deltaTime / 6.f;
				body->velocity += (rK4.velocity)*deltaTime / 6.f;
			}




			//TestSphereCollisionStep(body, 1.f);
			//body->position += 2.f*(rK3.position)*deltaTime / 6.f;
			//TestSphereCollisionStep(body, 1.f);
			//body->position += (rK4.position)*deltaTime / 6.f;
			//TestSphereCollisionStep(body, 1.f);



			//	std::cout << "body after pos: " << body->position.x << " " << body->position.y << " " << body->position.z << std::endl;
			body->velocity += (rK1.velocity + 2.f*(rK2.velocity + rK3.velocity) + rK4.velocity)*deltaTime / 6.f;

			body->force = glm::vec3(0.f);
		}
	}
}






cJPhysicsWorld::State cJPhysicsWorld::EvaluateDerivative(float deltaTime, nPhysics::cRigidBodyDef* body, const State & currentState)
{
	State temp;
	temp.position = body->position + currentState.position*deltaTime;
	temp.velocity = body->velocity + currentState.velocity*deltaTime;
	temp.position = temp.velocity;
	temp.velocity = (body->force / body->mass);
	return temp;
	
}


float cJPhysicsWorld::distanceBetweenSpheres(nPhysics::cRigidBodyDef* pSphere1, nPhysics::cRigidBodyDef* pSphere2)
{
	float deltaX = pSphere1->position.x - pSphere2->position.x;	//	pBallA->x - pBallB->x;
	float deltaY = pSphere1->position.y - pSphere2->position.y;	//	pBallA->y - pBallB->y;
	float deltaZ = pSphere1->position.z - pSphere2->position.z;	//	pBallA->z - pBallB->z;

	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

}

bool cJPhysicsWorld::testSphereSphereCollision(nPhysics::cRigidBodyDef* pSphere1, nPhysics::cRigidBodyDef* pSphere2, float radius1, float radius2)
{
	float totalRadius = radius1 + radius2;
	if (distanceBetweenSpheres(pSphere1, pSphere2) <= totalRadius)
	{
 	return true;
	}
	return false;
}


bool cJPhysicsWorld::TestSpherePlane(nPhysics::cRigidBodyDef* pSphere1, nPhysics::cRigidBodyDef* pPlane1, float radius, glm::vec3 normal)
{
	// For a normalized plane (|p.n| = 1), evaluating the plane equation
	// for a point gives the signed distance of the point to the plane
	float dist = glm::dot(pSphere1->position,normal) - (glm::dot(pPlane1->position, normal));
	// If sphere center within +/-radius from plane, plane intersects sphere
	return (abs(dist) <=radius);
}

bool cJPhysicsWorld::TestSphereCollisionStep(nPhysics::cRigidBodyDef* pSphere, float radius)
{

	//Test against Soft Bodies
	for (unsigned int index = 0; index < mWorldSoftBodies.size(); index++)
	{
		nPhysics::cSoftBodyDef* bodyDef = mWorldSoftBodies[index]->GetSoftBodyDef();
		
		for (unsigned int nodeIndex=0; nodeIndex < bodyDef->nodes.size(); nodeIndex++)
		{






			
			//mWorldBodies[index]->GetRigidBodyDef();
			if (pSphere!= bodyDef->nodes[nodeIndex])
			if (testSphereSphereCollision(pSphere, bodyDef->nodes[nodeIndex], radius, .25f))
			{
					std::cout << "Collision!" << std::endl;



					//[pmath]vec{ x } = s1.pos – s2.pos[/ pmath]
					//	[pmath]vec{ x }.normalize()[/ pmath]

					//	Then we calculate the x - direction velocity vector and the perpendicular y - vector.
					//	[pmath]vec{ v1 } = s1.vel[/ pmath]
					//	[pmath]x1 = x.dot(vec{ v1 })[/ pmath]
					//	[pmath]vec{ v1x } = vec{ x } *x1[/ pmath]
					//	[pmath]vec{ v1y } = vec{ v1 } – vec{ v1x }[/ pmath]
					//	[pmath]m1 = s1.mass[/ pmath]

					//	Same procedure for the other sphere.
					//	[pmath]vec{ x } = vec{ x }*-1[/ pmath]
					//	[pmath]vec{ v2 } = s2.vel[/ pmath]
					//	[pmath]x2 = vec{ x }.dot(vec{ v2 })[/ pmath]
					//	[pmath]vec{ v2x } = vec{ x } *x2[/ pmath]
					//	[pmath]vec{ v2y } = vec{ v2 } – vec{ v2x }[/ pmath]
					//	[pmath]m2 = s2.mass[/ pmath]

					//	Then we mix and play around with some of Newton’s laws to obtain a formula for the speed(in vector format) after the collision.
					//	[pmath]s1.vel = vec{ v1x }{(m1 - m2) / (m1 + m2)} +vec{ v2x }{(2 * m2) / (m1 + m2)} +vec{ v1y }[/ pmath]
					//	[pmath]s2.vel = vec{ v1x }{(2 * m1) / (m1 + m2)} +vec{ v2x }{(m2 - m1) / (m1 + m2)} +vec{ v2y }[/ pmath]


					glm::vec3 x = pSphere->position - bodyDef->nodes[nodeIndex]->position;
					x = glm::normalize(x);

					glm::vec3 vel1 = pSphere->velocity;
					float x1 = glm::dot(x, vel1);

					glm::vec3 v1x = x*x1;
					glm::vec3 v1y = vel1 - v1x;

					float mass1 = pSphere->mass;

					x = x*-1.f;
					glm::vec3 vel2 = bodyDef->nodes[nodeIndex]->velocity;
					float x2 = glm::dot(x, vel2);
					glm::vec3 v2x = x*x2;
					glm::vec3 v2y = vel2 - v2x;
					float mass2 = bodyDef->nodes[nodeIndex]->mass;

				//	[pmath]s1.vel = vec{ v1x }{(m1 - m2) / (m1 + m2)} +vec{ v2x }{(2 * m2) / (m1 + m2)} +vec{ v1y }[/ pmath]
				//	[pmath]s2.vel = vec{ v1x }{(2 * m1) / (m1 + m2)} +vec{ v2x }{(m2 - m1) / (m1 + m2)} +vec{ v2y }[/ pmath]
					pSphere->velocity = (v1x*((mass1 - mass2) / mass1 + mass2) + v2x*((2 * mass2) / mass1 + mass2) + v1y)*0.8f;
					bodyDef->nodes[nodeIndex]->velocity = (v1x*((2 * mass1) / mass1 + mass2) + v2x*((mass2 - mass1) / mass1 + mass2) + v2y)*0.8f;
				//glm::vec3 tempVel = pSphere->velocity;
			//	pSphere->velocity = (mWorldBodies[index]->GetRigidBodyDef()->velocity / pSphere->mass)*.95f;
			//	mWorldBodies[index]->GetRigidBodyDef()->velocity = (tempVel / mWorldBodies[index]->GetRigidBodyDef()->mass)*.95f;
				return true;
			}
		}
		
	}

	//Test against Rigid Bodies
	for (unsigned int index=0; index < mWorldBodies.size(); index++)
	{
		nPhysics::cRigidBodyDef* body2;
		body2 = mWorldBodies[index]->GetRigidBodyDef();
		if (body2 != pSphere)
		{

			nPhysics::iShape* shape2 = mWorldBodies[index]->GetShape();
			if (shape2->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)

			{
				//nPhysics::sSphere sphere1;
				//shape1->GetSphere(sphere1);
				nPhysics::sSphere sphere2;
				shape2->GetSphere(sphere2);
				//mWorldBodies[index]->GetRigidBodyDef();
				if (testSphereSphereCollision(pSphere, mWorldBodies[index]->GetRigidBodyDef(), radius, sphere2.Radius))
				{
				//	std::cout << "Collision!" << std::endl;



					glm::vec3 x = pSphere->position - mWorldBodies[index]->GetRigidBodyDef()->position;
					x = glm::normalize(x);

					glm::vec3 vel1 = pSphere->velocity;
					float x1 = glm::dot(x, vel1);

					glm::vec3 v1x = x*x1;
					glm::vec3 v1y = vel1 - v1x;

					float mass1 = pSphere->mass;

					x = x*-1.f;
					glm::vec3 vel2 = mWorldBodies[index]->GetRigidBodyDef()->velocity;
					float x2 = glm::dot(x, vel2);
					glm::vec3 v2x = x*x2;
					glm::vec3 v2y = vel2 - v2x;
					float mass2 = mWorldBodies[index]->GetRigidBodyDef()->mass;

					//	[pmath]s1.vel = vec{ v1x }{(m1 - m2) / (m1 + m2)} +vec{ v2x }{(2 * m2) / (m1 + m2)} +vec{ v1y }[/ pmath]
					//	[pmath]s2.vel = vec{ v1x }{(2 * m1) / (m1 + m2)} +vec{ v2x }{(m2 - m1) / (m1 + m2)} +vec{ v2y }[/ pmath]
					pSphere->velocity = (v1x*((mass1 - mass2) / mass1 + mass2) + v2x*((2 * mass2) / mass1 + mass2) + v1y)*.8f;
					mWorldBodies[index]->GetRigidBodyDef()->velocity = (v1x*((2 * mass1) / mass1 + mass2) + v2x*((mass2 - mass1) / mass1 + mass2) + v2y)*.8f;







					//glm::vec3 tempVel = pSphere->velocity;
					//pSphere->velocity = (mWorldBodies[index]->GetRigidBodyDef()->velocity / pSphere->mass)*.95f;
				//	mWorldBodies[index]->GetRigidBodyDef()->velocity = (tempVel / mWorldBodies[index]->GetRigidBodyDef()->mass)*.95f;
					return true;
				}

			}

			else if (shape2->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
			{
				nPhysics::sPlane plane1;
				shape2->GetPlane(plane1);
				if (TestSpherePlane(pSphere, mWorldBodies[index]->GetRigidBodyDef(), radius, plane1.Normal))
				{
					//std::cout << "Collision!" << std::endl;
					pSphere->velocity = glm::reflect(pSphere->velocity, plane1.Normal)*.8f;
					return true;
				}
			}
		}


	}
	return false;
}



bool cJPhysicsWorld::TestSphereCollisionStepSoftBody(nPhysics::cRigidBodyDef* pSphere, float radius, int nodeNumber)
{

	//Test against Soft Bodies
	for (unsigned int index = 0; index < mWorldSoftBodies.size(); index++)
	{
		nPhysics::cSoftBodyDef* bodyDef = mWorldSoftBodies[index]->GetSoftBodyDef();

		


		for (unsigned int nodeIndex = 0; nodeIndex < bodyDef->nodes.size(); nodeIndex++)
		{

			


			bool skip = false;

			if (nodeNumber == 0)
			{
				if (nodeIndex == 1 || nodeIndex == 3)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 1)
			{
				if (nodeIndex == 0 || nodeIndex == 4 || nodeIndex == 2)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 2)
			{
				if (nodeIndex == 1 || nodeIndex == 5)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 3)
			{
				if (nodeIndex == 0 || nodeIndex == 4 || nodeIndex == 6)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 4)
			{
				if (nodeIndex == 1 || nodeIndex == 3 || nodeIndex == 5 || nodeIndex == 7)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 5)
			{
				if (nodeIndex == 1 || nodeIndex == 3 || nodeIndex == 5 || nodeIndex == 7)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 6)
			{
				if (nodeIndex == 3 || nodeIndex == 7)// || nodeIndex == 5 || nodeIndex == 7)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 7) //LOOK AT THIS AND UPDATE NODE INDEXES, INDICES, JACKASS
			{
				if (nodeIndex == 6 || nodeIndex == 4 || nodeIndex == 8)// || nodeIndex == 7)
				{
					skip = false;
				}
				else skip = true;
			}
			else if (nodeNumber == 8)
			{
				if (nodeIndex == 5 || nodeIndex == 7)
				{
					skip = false;
				}
				else skip = true;
			}

			//mWorldBodies[index]->GetRigidBodyDef();
			if (pSphere != bodyDef->nodes[nodeIndex] && skip)
				if (testSphereSphereCollision(pSphere, bodyDef->nodes[nodeIndex], radius, .25f))
				{
					std::cout << "Collision!" << std::endl;



					//[pmath]vec{ x } = s1.pos – s2.pos[/ pmath]
					//	[pmath]vec{ x }.normalize()[/ pmath]

					//	Then we calculate the x - direction velocity vector and the perpendicular y - vector.
					//	[pmath]vec{ v1 } = s1.vel[/ pmath]
					//	[pmath]x1 = x.dot(vec{ v1 })[/ pmath]
					//	[pmath]vec{ v1x } = vec{ x } *x1[/ pmath]
					//	[pmath]vec{ v1y } = vec{ v1 } – vec{ v1x }[/ pmath]
					//	[pmath]m1 = s1.mass[/ pmath]

					//	Same procedure for the other sphere.
					//	[pmath]vec{ x } = vec{ x }*-1[/ pmath]
					//	[pmath]vec{ v2 } = s2.vel[/ pmath]
					//	[pmath]x2 = vec{ x }.dot(vec{ v2 })[/ pmath]
					//	[pmath]vec{ v2x } = vec{ x } *x2[/ pmath]
					//	[pmath]vec{ v2y } = vec{ v2 } – vec{ v2x }[/ pmath]
					//	[pmath]m2 = s2.mass[/ pmath]

					//	Then we mix and play around with some of Newton’s laws to obtain a formula for the speed(in vector format) after the collision.
					//	[pmath]s1.vel = vec{ v1x }{(m1 - m2) / (m1 + m2)} +vec{ v2x }{(2 * m2) / (m1 + m2)} +vec{ v1y }[/ pmath]
					//	[pmath]s2.vel = vec{ v1x }{(2 * m1) / (m1 + m2)} +vec{ v2x }{(m2 - m1) / (m1 + m2)} +vec{ v2y }[/ pmath]


					glm::vec3 x = pSphere->position - bodyDef->nodes[nodeIndex]->position;
					x = glm::normalize(x);

					glm::vec3 vel1 = pSphere->velocity;
					float x1 = glm::dot(x, vel1);

					glm::vec3 v1x = x*x1;
					glm::vec3 v1y = vel1 - v1x;

					float mass1 = pSphere->mass;

					x = x*-1.f;
					glm::vec3 vel2 = bodyDef->nodes[nodeIndex]->velocity;
					float x2 = glm::dot(x, vel2);
					glm::vec3 v2x = x*x2;
					glm::vec3 v2y = vel2 - v2x;
					float mass2 = bodyDef->nodes[nodeIndex]->mass;

					//	[pmath]s1.vel = vec{ v1x }{(m1 - m2) / (m1 + m2)} +vec{ v2x }{(2 * m2) / (m1 + m2)} +vec{ v1y }[/ pmath]
					//	[pmath]s2.vel = vec{ v1x }{(2 * m1) / (m1 + m2)} +vec{ v2x }{(m2 - m1) / (m1 + m2)} +vec{ v2y }[/ pmath]
					pSphere->force = (v1x*((mass1 - mass2) / mass1 + mass2) + v2x*((2 * mass2) / mass1 + mass2) + v1y)*.8f;// *.000000000001f;
					bodyDef->nodes[nodeIndex]->force = (v1x*((2 * mass1) / mass1 + mass2) + v2x*((mass2 - mass1) / mass1 + mass2) + v2y)*.8f;// *.000000000001f;
					//glm::vec3 tempVel = pSphere->velocity;
					//	pSphere->velocity = (mWorldBodies[index]->GetRigidBodyDef()->velocity / pSphere->mass)*.95f;
					//	mWorldBodies[index]->GetRigidBodyDef()->velocity = (tempVel / mWorldBodies[index]->GetRigidBodyDef()->mass)*.95f;
					return true;
				}
		}

	}

	//Test against Rigid Bodies
	for (unsigned int index = 0; index < mWorldBodies.size(); index++)
	{
		nPhysics::cRigidBodyDef* body2;
		body2 = mWorldBodies[index]->GetRigidBodyDef();
		if (body2 != pSphere)
		{

			nPhysics::iShape* shape2 = mWorldBodies[index]->GetShape();
			if (shape2->GetShapeType() == nPhysics::eShapeType::SPHERE_SHAPE)

			{
				//nPhysics::sSphere sphere1;
				//shape1->GetSphere(sphere1);
				nPhysics::sSphere sphere2;
				shape2->GetSphere(sphere2);
				//mWorldBodies[index]->GetRigidBodyDef();
				if (testSphereSphereCollision(pSphere, mWorldBodies[index]->GetRigidBodyDef(), radius, sphere2.Radius))
				{
					//	std::cout << "Collision!" << std::endl;



					glm::vec3 x = pSphere->position - mWorldBodies[index]->GetRigidBodyDef()->position;
					x = glm::normalize(x);

					glm::vec3 vel1 = pSphere->velocity;
					float x1 = glm::dot(x, vel1);

					glm::vec3 v1x = x*x1;
					glm::vec3 v1y = vel1 - v1x;

					float mass1 = pSphere->mass;

					x = x*-1.f;
					glm::vec3 vel2 = mWorldBodies[index]->GetRigidBodyDef()->velocity;
					float x2 = glm::dot(x, vel2);
					glm::vec3 v2x = x*x2;
					glm::vec3 v2y = vel2 - v2x;
					float mass2 = mWorldBodies[index]->GetRigidBodyDef()->mass;

					//	[pmath]s1.vel = vec{ v1x }{(m1 - m2) / (m1 + m2)} +vec{ v2x }{(2 * m2) / (m1 + m2)} +vec{ v1y }[/ pmath]
					//	[pmath]s2.vel = vec{ v1x }{(2 * m1) / (m1 + m2)} +vec{ v2x }{(m2 - m1) / (m1 + m2)} +vec{ v2y }[/ pmath]
					pSphere->force = (v1x*((mass1 - mass2) / mass1 + mass2) + v2x*((2 * mass2) / mass1 + mass2) + v1y)*.8f;// *.00000001f;
					mWorldBodies[index]->GetRigidBodyDef()->force = (v1x*((2 * mass1) / mass1 + mass2) + v2x*((mass2 - mass1) / mass1 + mass2) + v2y)*.8f;// *.00000001f;







					//glm::vec3 tempVel = pSphere->velocity;
					//pSphere->velocity = (mWorldBodies[index]->GetRigidBodyDef()->velocity / pSphere->mass)*.95f;
					//	mWorldBodies[index]->GetRigidBodyDef()->velocity = (tempVel / mWorldBodies[index]->GetRigidBodyDef()->mass)*.95f;
					return true;
				}

			}

			else if (shape2->GetShapeType() == nPhysics::eShapeType::PLANE_SHAPE)
			{
				nPhysics::sPlane plane1;
				shape2->GetPlane(plane1);
				if (TestSpherePlane(pSphere, mWorldBodies[index]->GetRigidBodyDef(), radius, plane1.Normal))
				{
					//std::cout << "Collision!" << std::endl;
					pSphere->velocity = glm::reflect(pSphere->velocity, plane1.Normal)*.8f;
					return true;
				}
			}
		}


	}
	return false;
}


void cJPhysicsWorld::UpdateNodes(nPhysics::cSoftBodyDef* bodyDef)
{

	for (unsigned int i = 0; i < bodyDef->nodeConstraints.size(); i++)
	{
		ApplyConstraint(bodyDef->nodeConstraints[i]);
	}


}

void cJPhysicsWorld::ApplyElasticForce(nPhysics::cRigidBodyDef * body1, nPhysics::cRigidBodyDef * body2, float relaxedDistance, float seperatingDistance, float elasticStrength, float seperatingStrength)
{

	

	float distance = glm::distance(body1->position, body2->position);
	std::cout << "distance Frame 1: " <<  distance << std::endl;
	if (abs(distance) < seperatingDistance)
	{

	}
	if (abs(distance) > relaxedDistance)
	{
		
		if (distance > 2 * relaxedDistance)
		{
			body1->force -= (body1->position - body2->position)*(body1->position - body2->position)*elasticStrength*2.f;
		}
		else
		{
			body1->force -= (body1->position - body2->position)*(body1->position - body2->position)*elasticStrength;
		}
		//body1->force -= (body1->position - body2->position)*(body1->position-body2->position)*elasticStrength;
		//body2->force += (body2->position - body1->position)*(body2->position - body1->position)*elasticStrength;
		distance = glm::distance(body1->position, body2->position);
		std::cout << "distance Frame 2: " << distance << std::endl;
	}
	//else if (abs(distance) < seperatingDistance)
	//{
	//	body1->force -= (body1->position - body2->position)*(body1->position - body2->position)*seperatingStrength;
	//	body2->force -= (body1->position - body2->position)*(body2->position - body1->position)*seperatingStrength;
	//	distance = glm::distance(body1->position, body2->position);
	//	std::cout << "distance Frame 3: " << distance << std::endl;
	//}
	// distance = glm::distance(body1->position, body2->position);
	//std::cout << "distance Frame 1: " << distance << std::endl;
	/*if (abs(distance) > relaxedDistance*1.5f)
	{
		glm::vec3 tempValue = (body1->position - body2->position);
		tempValue = glm::normalize(tempValue);
		body2->position = body1->position+ tempValue*seperatingDistance*1.5f;
	}*/

}

void cJPhysicsWorld::ApplyElasticForce(nPhysics::cRigidBodyDef * body1, nPhysics::cRigidBodyDef * body2, glm::vec3 constraintVector, float relaxedDistance, float seperatingDistance, float elasticStrength, float seperatingStrength)
{
	float distance = 0;
	if (constraintVector == glm::vec3(0, 1, 0))
	{
		glm::vec3 posA = glm::vec3(0.f, body1->position.y, 0.f);
		glm::vec3 posB = glm::vec3(0.f, body2->position.y, 0.f);
		distance = glm::distance(posA, posB);
		if (distance > relaxedDistance)
		{
			body2->force += constraintVector*elasticStrength;
		}
		if (distance <  -relaxedDistance)
		{
			body2->force -= constraintVector*elasticStrength;
		}
	} 
	else if (constraintVector == glm::vec3(0, -1, 0))
	{
		glm::vec3 posA = glm::vec3(0.f, body1->position.x, 0.f);
		glm::vec3 posB = glm::vec3(0.f, body2->position.x, 0.f);
		distance = glm::distance(posA, posB);
		if (distance < relaxedDistance)
		{
			body2->force += constraintVector*elasticStrength;
		}
	}
	else if (constraintVector == glm::vec3(1, 0, 0))
	{
		glm::vec3 posA = glm::vec3(0.f, body1->position.x, 0.f);
		glm::vec3 posB = glm::vec3(0.f, body2->position.x, 0.f);
		distance = glm::distance(posA, posB);
		if (distance > relaxedDistance)
		{
			body2->force += constraintVector*elasticStrength;
		}
	}
	else if (constraintVector == glm::vec3(-1, 0, 0))
	{
		glm::vec3 posA = glm::vec3(0.f, body1->position.x, 0.f);
		glm::vec3 posB = glm::vec3(0.f, body2->position.x, 0.f);
		distance = glm::distance(posA, posB);
		if (distance > relaxedDistance)
		{
			body2->force += constraintVector*elasticStrength;
		}
	}


	if (body1->position.z > -10.f)
	{
		body1->force += glm::distance(body1->position, (glm::vec3(body1->position.x, body1->position.x, -10.f)))*.5f;
	}
	else if (body1->position.z < -10.f)
	{
		body1->force += glm::distance(body1->position, (glm::vec3(body1->position.x, body1->position.x, -10.f)))*.5f;
	}


	//float distance = glm::distance(body1->position, body2->position);
	//std::cout << "distance Frame 1: " << distance << std::endl;
	//if (abs(distance) > relaxedDistance)
	//{

	//	body1->force -= (body1->position - body2->position)*(body1->position - body2->position)*elasticStrength;
	//	//body2->force += (body2->position - body1->position)*(body2->position - body1->position)*elasticStrength;
	//	distance = glm::distance(body1->position, body2->position);
	//	std::cout << "distance Frame 2: " << distance << std::endl;
	//}
	////else if (abs(distance) < seperatingDistance)
	////{
	////	body1->force -= (body1->position - body2->position)*(body1->position - body2->position)*seperatingStrength;
	////	body2->force -= (body1->position - body2->position)*(body2->position - body1->position)*seperatingStrength;
	////	distance = glm::distance(body1->position, body2->position);
	////	std::cout << "distance Frame 3: " << distance << std::endl;
	////}
	//// distance = glm::distance(body1->position, body2->position);
	////std::cout << "distance Frame 1: " << distance << std::endl;
	///*if (abs(distance) > relaxedDistance*1.5f)
	//{
	//glm::vec3 tempValue = (body1->position - body2->position);
	//tempValue = glm::normalize(tempValue);
	//body2->position = body1->position+ tempValue*seperatingDistance*1.5f;
	//}*/

}

void cJPhysicsWorld::ApplyConstraint(nPhysics::sNodeConstraint constraint)
{
	float distanceFromPoints = glm::distance(constraint.nodeA->position, constraint.nodeB->position);

	//if (distanceFromPoints < .5f)
	//{
	//	glm::vec3 direction = (constraint.nodeA->position - constraint.nodeB->position);

	//	direction = glm::normalize(direction);

	//	

	//	glm::vec3 force = (direction)*.9f;
	//	constraint.nodeB->force += force;
	//	return;
	//}

	if (distanceFromPoints > constraint.engagedDistance)
	{
		
		glm::vec3 direction = (constraint.nodeA->position - constraint.nodeB->position);

		direction = glm::normalize(direction);

		float engagedDistance = distanceFromPoints - (constraint.engagedDistance / 2.f);

		glm::vec3 force = (direction*engagedDistance*constraint.tensorStrength);
		constraint.nodeB->velocity *= .9f;
		constraint.nodeB->force += force;
	}
}