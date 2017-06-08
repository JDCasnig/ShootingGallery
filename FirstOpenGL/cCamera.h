
// cGameObject.h
#ifndef _cCamera_HG_
#define _cCamera_HG_

#include "cGameObject.h"
//#include "GlobalStuff.h"


class cCamera : public cGameObject
{
public:
	cCamera();

	// TODO: Get rid of this pre-rotation...
	//	float pre_Rot_X, pre_Rot_Y, pre_Rot_Z;		// Euler angles 
	//	float x, y, z;			// Position ("translation")
	glm::vec3 position;
	glm::vec3 lastPosition;
	glm::vec3 cameraTarget;
	//cGameObject* objectToFollow;
	float followDistance;
	//	float post_Rot_X, post_Rot_Y, post_Rot_Z;	// Euler angles 
	glm::quat qOrientaion;
	float moveSpeed;
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
	cGameObject* objectToFollow;
	bool mHasObjectToFollow;
public:

	//float Vx, Vy, Vz;
	//float Ax, Ay, Az;
	glm::vec3 velocity;
	glm::vec3 accel;
	//std::vector< AABB* > collisionBoxes;
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
	bool bUsesHAABB;
	//
	std::vector< cGameObject* > vec_pChildren;
	void AttachToGameObject(cGameObject* gameObject);
	void Dettach();
	void Update();

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




