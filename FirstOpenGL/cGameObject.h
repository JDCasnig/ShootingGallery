// cGameObject.h
#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>
#include <iRigidBody.h>
#include <cRigidBodyDef.h>
#include <iSoftBody.h>
#include <cSoftBodyDef.h>
class cGameObject
{
public:
	cGameObject();

	// TODO: Get rid of this pre-rotation...
//	float pre_Rot_X, pre_Rot_Y, pre_Rot_Z;		// Euler angles 
//	float x, y, z;			// Position ("translation")
	glm::vec3 position;
	glm::vec3 lastPosition;

	nPhysics::iRigidBody* rigidBody;
	nPhysics::cRigidBodyDef* bodyDef;
	bool isSoftBody;
	nPhysics::iSoftBody* softBody;
	nPhysics::cSoftBodyDef* softBodyDef;
//	float post_Rot_X, post_Rot_Y, post_Rot_Z;	// Euler angles 
	glm::quat qOrientaion;

	// Speed of rotation around each axis.
	glm::vec3 rotSpeed;		//(1.0f, 0.0f, 0.0f);

	float scale;		// Relative to a unit scale


	float alpha;

	// Used in Physics and in graphics
	float radius;
	//float lastX, lastY, lastZ;		

	int getID(void);					
private:
	int m_uniqueID;
	static int m_nextID;

public:

	//float Vx, Vy, Vz;
	//float Ax, Ay, Az;
	glm::vec3 velocity;
	glm::vec3 accel;

	// Velocity that Starbuck really wants 
	//  in relation to her.... 
	// This never changes orientation
	//  Z = forward (+ve) and backwards (-ve)
	//  Y = up and down
	//  Z = left and right
	glm::vec3 velDirected; 

	int meshID;	// Which model we are drawing
	std::string meshName;		// "bunny.ply" or whatever

	bool bIsWireframe;
//	float solid_R, solid_G, solid_B;		// Solid RGB
	glm::vec4 debugRGBA;
	bool bUseDebugColours;
	bool bIsVisible;

	std::string texture00Name;
	std::string texture01Name;

	bool bIsImposter;

	bool bIsUpdatedByPhysics;

	//
	std::vector< cGameObject* > vec_pChildren;

};

	////  Spherical bounding object
	//float radius;

	//// AABB 
	//float deltaX, deltaY, deltaZ;
	//float minX, minY, minZ;
	//float maxX, maxY, maxZ;

	//// AABB cube
	//float distanceFromCentre;

#endif 
