
#include "cCamera.h"

int cCamera::getID(void)
{
	return this->m_uniqueID;
	
}

//static 
int cCamera::m_nextID = 1;

cCamera::cCamera()
{
	this->m_uniqueID = cCamera::m_nextID;
	cCamera::m_nextID++;
	bUsesHAABB = false;
	moveSpeed = 1.f;
	this->position = glm::vec3(0.f, 3.f, 25.f);
	this->cameraTarget = glm::vec3(0.f);
	// C'tor to init all values to 0.0f;
	//	this->x = this->y = this->z = 0.0f;
	//	this->Vx = this->Vy = this->Vz = 0.0f;
	//	this->Ax = this->Ay = this->Az = 0.0f;

	//this->pre_Rot_X = this->pre_Rot_Y = this->pre_Rot_Z = 0.0f;
	//this->post_Rot_X = this->post_Rot_Y = this->post_Rot_Z = 0.0f;
	this->scale = 1.0f;

	this->bIsWireframe = false;	// Solid 
	this->debugRGBA = glm::vec4(1.0f);	// Solid white

	this->bIsVisible = true;
	this->bUseDebugColours = false;

	this->alpha = 1.0f;

	this->radius = 0.0f;

	this->meshID = 0;

	this->bIsImposter = false;

	// Assume they are updated by the physics "sub-system"
	this->bIsUpdatedByPhysics = false;
	this->mHasObjectToFollow = false;
	return;
}


void cCamera::AttachToGameObject(cGameObject* gameObject)
{
	objectToFollow = gameObject;
	mHasObjectToFollow = true;

}


void cCamera::Dettach()
{
	objectToFollow = NULL;
	mHasObjectToFollow = false;
}

void cCamera::Update()
{
	if (mHasObjectToFollow)
	{
		cameraTarget = objectToFollow->position;
	}
}
