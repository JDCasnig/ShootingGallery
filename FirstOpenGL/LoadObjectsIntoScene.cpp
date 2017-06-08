#include "cGameObject.h"
#include "Utilities.h"	// For getRandFloat
#include <vector>
#include "GlobalStuff.h"
#include <pugixml\pugixml.hpp>
#include <iostream>

// variable is NOT in this file, but it's somewhere
// (i.e. the linker will "link" to it)
extern unsigned int g_DebugSphereID;
extern unsigned int g_1SidedQuadID;
extern unsigned int g_2SidedQuadID;

#include <glm/glm.hpp>

cGameObject* PlaceObject( std::string objectName, glm::vec3 location );
cGameObject* PlaceObject( std::string objectName, glm::vec3 location, glm::vec3 rotation );
cGameObject* PlaceObject( std::string objectName, glm::vec3 location, glm::vec3 rotation, float scale );

void AddGridLines(std::vector< cGameObject* > &vec_pGOs)
{
	{	// Axes
		float gridLimit = 50.0f; 
		float gridStep = 10.0f;

		// Make x axis... lines along z and y axes
		for ( float y = -gridLimit; y <= gridLimit; y += gridStep )
		{
			for ( float z = -gridLimit; z <= gridLimit; z += gridStep )
			{
				cGameObject* pBar = new cGameObject();
				pBar->bIsUpdatedByPhysics = false;	// 
				pBar->bIsWireframe = false;
				pBar->position.y = y;
				pBar->position.z = z;
				pBar->bUseDebugColours = true;
				pBar->debugRGBA = glm::vec4( 1.0f, 0.0f, 0.0f, 0.5f );		// x = red
				pBar->meshName = "Long_bar_x_aligned_x10.ply";
				pBar->scale = 200.0f;			// Ground is 50.0x50.0 
				vec_pGOs.push_back( pBar );
			}
		}// for ( float y = -gridLimit;

		// Make y axis... lines along x and z axes
		for ( float x = -gridLimit; x <= gridLimit; x += gridStep )
		{
			for ( float z = -gridLimit; z <= gridLimit; z += gridStep )
			{
				cGameObject* pBar = new cGameObject();
				pBar->bIsUpdatedByPhysics = false;	// 
				pBar->bIsWireframe = false;
				pBar->position.x = x;
				pBar->position.z = z;
				pBar->bUseDebugColours = true;
				pBar->debugRGBA = glm::vec4( 0.0f, 1.0f, 0.0f, 0.5f );		// y = green
				pBar->meshName = "Long_bar_y_aligned_x10.ply";
				pBar->scale = 200.0f;			// Ground is 50.0x50.0 
				vec_pGOs.push_back( pBar );
			}
		}// for ( float y = -gridLimit;

		// Make z axis... lines along x and y axes
		for ( float x = -gridLimit; x <= gridLimit; x += gridStep )
		{
			for ( float y = -gridLimit; y <= gridLimit; y += gridStep )
			{
				cGameObject* pBar = new cGameObject();
				pBar->bIsUpdatedByPhysics = false;	// 
				pBar->bIsWireframe = false;
				pBar->position.x = x;
				pBar->position.y = y;
				pBar->bUseDebugColours = true;
				pBar->debugRGBA = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );		// z = blue
				pBar->meshName = "Long_bar_z_aligned_x10.ply";
				pBar->scale = 200.0f;			// Ground is 50.0x50.0 
				vec_pGOs.push_back( pBar );
			}
		}// for ( float y = -gridLimit;

	}	// Axes

	return;
}

bool LoadObjectsIntoScene( std::vector< cGameObject* > &vec_pGOs, std::string sceneFile )
{

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(sceneFile.c_str());

	std::cout << "Load result: " << result.description() << ", Game Object: " << doc.child("GameObject").child("Mesh").attribute("name").value() << std::endl;
	
	pugi::xml_node_iterator it = doc.child("GameObjects").children().begin();

	for (it; it != doc.child("GameObjects").children().end(); it++)
	{
		//cGameObject* pGameObject = new cGameObject();
		//pGameObject->bIsUpdatedByPhysics = false;	// 
		//pGameObject->bIsWireframe = false;
		//pGameObject->bodyDef->position.x = it->child("position").attribute("x").as_double();
		//pGameObject->bodyDef->position.y = it->child("position").attribute("y").as_double();
		//pGameObject->bodyDef->position.z = it->child("position").attribute("z").as_double();
		//pGameObject->bUseDebugColours = true;
		//pGameObject->debugRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);		// y = green
		//pGameObject->meshName = it->child("Mesh").attribute("name").value();
		//pGameObject->scale = it->child("Scale").attribute("Size").as_double();			// Ground is 50.0x50.0 
		//vec_pGOs.push_back(pGameObject);
		

		////////////////////////////////////////
		
		nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(it->child("Scale").attribute("Size").as_double()));
		nPhysics::cRigidBodyDef bodyDef;
		bodyDef.position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);
		bodyDef.staticBody = true;
		bodyDef.mass = 1.0f;
		bodyDef.velocity = glm::vec3(0.f);//glm::vec3(0.f, 0.f, -10.f) + (camera1->cameraTarget-camera1->position);
		nPhysics::iRigidBody* rigidBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);
		nPhysics::cRigidBodyDef* bodyDefCheck;
		bodyDefCheck = rigidBody->GetRigidBodyDef();
		cGameObject* pSphere = new cGameObject();
		pSphere->bIsUpdatedByPhysics = it->child("PhysicsEnabled").attribute("staticbody").as_bool();//false;
		pSphere->bodyDef = bodyDefCheck;
		pSphere->meshName = it->child("Mesh").attribute("name").value();
		pSphere->scale = it->child("Scale").attribute("Size").as_double();				// Ground is 50.0x50.0 

		pSphere->bIsWireframe = true;
		pSphere->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pSphere->bUseDebugColours = true;
		physicsWorld->AddBody(rigidBody);
		vec_pGOs.push_back(pSphere);
	}


	//cGameObject* pGameObject = new cGameObject();
	//pGameObject->bIsUpdatedByPhysics = false;	// 
	//pGameObject->bIsWireframe = false;
	//pGameObject->bodyDef->position.x = doc.child("GameObjects").child("GameObject").child("position").attribute("x").as_double();
	//pGameObject->bodyDef->position.y = doc.child("GameObjects").child("GameObject").child("position").attribute("y").as_double();
	//pGameObject->bodyDef->position.z = doc.child("GameObjects").child("GameObject").child("position").attribute("z").as_double();
	//pGameObject->bUseDebugColours = true;
	//pGameObject->debugRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);		// y = green
	//pGameObject->meshName = doc.child("GameObjects").child("GameObject").child("Mesh").attribute("name").value();
	//pGameObject->scale = doc.child("GameObjects").child("GameObject").child("Scale").attribute("Size").as_double();			// Ground is 50.0x50.0 
	//vec_pGOs.push_back(pGameObject);
	//
	//cGameObject* pFloor = new cGameObject();
	//pFloor->bIsUpdatedByPhysics = false;	// 
	//pFloor->bIsWireframe = false;
	//pFloor->bodyDef->position.x = doc.child("GameObjects").child("Floor").child("position").attribute("x").as_double();
	//pFloor->bodyDef->position.y = doc.child("GameObjects").child("Floor").child("position").attribute("y").as_double();
	//pFloor->bodyDef->position.z = doc.child("GameObjects").child("Floor").child("position").attribute("z").as_double();
	//pFloor->bUseDebugColours = true;
	//pFloor->debugRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);		// y = green
	//pFloor->meshName = doc.child("GameObjects").child("Floor").child("Mesh").attribute("name").value();
	//pFloor->scale = 10.0f;			// Ground is 50.0x50.0 
	//vec_pGOs.push_back(pFloor);

	{	// Add a 1 sided quad
			cGameObject* p1SidedQuad = new cGameObject();
			p1SidedQuad->bIsUpdatedByPhysics = false;	// 
			p1SidedQuad->bIsWireframe = true;
			//p1SidedQuad->x = 0.0f;	p1SidedQuad->y = 0.0f;	p1SidedQuad->z = 0.0f;
			p1SidedQuad->bodyDef->position = glm::vec3(0.0f,0.0f,0.0f);
			p1SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
			p1SidedQuad->meshName = "1x1_2Tri_Quad_1_Sided_XY_Plane.ply";
			p1SidedQuad->texture00Name = "smoketex.bmp";
			p1SidedQuad->bIsVisible = false;
			p1SidedQuad->bIsImposter = true;
			::g_1SidedQuadID = p1SidedQuad->getID();
			vec_pGOs.push_back( p1SidedQuad );
		}
		{	// Add a 2 sided quad
			cGameObject* p2SidedQuad = new cGameObject();
			p2SidedQuad->bIsUpdatedByPhysics = false;	// 
			p2SidedQuad->bIsWireframe = true;
			//p2SidedQuad->x = 0.0f;	p2SidedQuad->y = 0.0f;	p2SidedQuad->z = 0.0f;
			p2SidedQuad->bodyDef->position = glm::vec3(0.0f,0.0f,0.0f);
			p2SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
			p2SidedQuad->meshName = "1x1_2Tri_Quad_2_Sided_XY_Plane.ply";
			p2SidedQuad->bIsVisible = false;
			p2SidedQuad->bIsImposter = true;
			p2SidedQuad->texture00Name = "smoketex.bmp"; 
			::g_2SidedQuadID = p2SidedQuad->getID();
			vec_pGOs.push_back( p2SidedQuad );
		}
	
		nPhysics::iShape* planeShape = physicsFactory->CreateShape(nPhysics::sPlane());
		nPhysics::cRigidBodyDef bodyDef2;
		bodyDef2.position = glm::vec3(0.f, 0.f, 0.f);
		bodyDef2.staticBody = true;
		nPhysics::iRigidBody* rigidBody2 = physicsFactory->CreateRigidBody(bodyDef2, planeShape);
		physicsWorld->AddBody(rigidBody2);

		nPhysics::iShape* planeShapeLeft = physicsFactory->CreateShape(nPhysics::sPlane(-1.f, 0.f, 0.f));
		nPhysics::cRigidBodyDef bodyDefLeft;
		bodyDefLeft.position = glm::vec3(-15.f, 0.f, 0.f);
		bodyDefLeft.staticBody = true;
		nPhysics::iRigidBody* rigidBodyLeft = physicsFactory->CreateRigidBody(bodyDefLeft, planeShapeLeft);
		physicsWorld->AddBody(rigidBodyLeft);


		nPhysics::iShape* planeShapeRight = physicsFactory->CreateShape(nPhysics::sPlane(1.f, 0.f, 0.f));
		nPhysics::cRigidBodyDef bodyDefRight;
		bodyDefRight.position = glm::vec3(15.f, 0.f, 0.f);
		bodyDefRight.staticBody = true;
		nPhysics::iRigidBody* rigidBodyRight = physicsFactory->CreateRigidBody(bodyDefRight, planeShapeRight);
		physicsWorld->AddBody(rigidBodyRight);

		////SOFT BODY MADNESS
		//std::vector<nPhysics::cRigidBodyDef*> nodes;
		//for (unsigned int i = 0; i < 3; i++)
		//{
		//	for (unsigned int j = 0; j < 3; j++)
		//	{

		//		nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());
		//		nPhysics::cRigidBodyDef bodyDef;
		//		bodyDef.position = glm::vec3(i*2.f,7.f+j*2.f, -10.f);//glm::vec3(5.f, 5.f, 0.f);
		//		bodyDef.staticBody = false;
		//		bodyDef.mass = 1.0f;
		//		bodyDef.velocity = glm::vec3(0.f, 0.f, 0.f);
		//		nPhysics::iRigidBody* rigidBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);
		//		nPhysics::cRigidBodyDef* bodyDefCheck;
		//		bodyDefCheck = rigidBody->GetRigidBodyDef();
		//		cGameObject* pSphere = new cGameObject();
		//		pSphere->bIsUpdatedByPhysics = false;
		//		pSphere->bodyDef = bodyDefCheck;
		//		pSphere->meshName = "Isoshphere_xyz_InvertedNormals.ply";
		//		pSphere->scale = 1.0f;			// Ground is 50.0x50.0 

		//		pSphere->bIsWireframe = true;
		//		pSphere->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//		pSphere->bUseDebugColours = true;
		//		
		//		::g_vec_pGOs.push_back(pSphere);
		//		nodes.push_back(bodyDefCheck);

		//	}
		//}

		//nPhysics::cSoftBodyDef sbDef = nPhysics::cSoftBodyDef(nodes);
		//::curtain = physicsFactory->CreateSoftBody(&sbDef);
		//physicsWorld->AddBody(::curtain);
		////END SOFT BODY MADNESS






	return true;
}