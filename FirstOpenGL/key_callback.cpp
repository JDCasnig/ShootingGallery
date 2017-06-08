#include "GlobalStuff.h"

#include <sstream>
#include "Utilities.h"
#include <pugixml\pugixml.hpp>

extern unsigned int g_1SidedQuadID;
extern unsigned int g_2SidedQuadID;


bool LoadObjectsIntoScene2(std::vector< cGameObject* > &vec_pGOs, std::string sceneFile)
{

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(sceneFile.c_str());

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

		std::string name = it->name();
		if (name == "Target")
		{
			////////////////////////////////////////

			nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(it->child("Scale").attribute("Size").as_double()));
			nPhysics::cRigidBodyDef bodyDef;
			bodyDef.position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);
			bodyDef.staticBody = it->child("PhysicsEnabled").attribute("staticbody").as_bool();
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
		else if (name == "Shot")
		{
			nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere(it->child("Scale").attribute("Size").as_double()));
			nPhysics::cRigidBodyDef bodyDef;
			bodyDef.position = glm::vec3(it->child("position").attribute("x").as_double(), it->child("position").attribute("y").as_double(), it->child("position").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f);
			bodyDef.staticBody = it->child("PhysicsEnabled").attribute("staticbody").as_bool();
			bodyDef.mass = 1.0f;
			bodyDef.velocity = glm::vec3(it->child("velocity").attribute("x").as_double(), it->child("velocity").attribute("y").as_double(), it->child("velocity").attribute("z").as_double());//camera1->position;//glm::vec3(5.f, 5.f, 0.f); //glm::vec3(0.f);//glm::vec3(0.f, 0.f, -10.f) + (camera1->cameraTarget-camera1->position);
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
		p1SidedQuad->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);
		p1SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
		p1SidedQuad->meshName = "1x1_2Tri_Quad_1_Sided_XY_Plane.ply";
		p1SidedQuad->texture00Name = "smoketex.bmp";
		p1SidedQuad->bIsVisible = false;
		p1SidedQuad->bIsImposter = true;
		::g_1SidedQuadID = p1SidedQuad->getID();
		vec_pGOs.push_back(p1SidedQuad);
	}
	{	// Add a 2 sided quad
		cGameObject* p2SidedQuad = new cGameObject();
		p2SidedQuad->bIsUpdatedByPhysics = false;	// 
		p2SidedQuad->bIsWireframe = true;
		//p2SidedQuad->x = 0.0f;	p2SidedQuad->y = 0.0f;	p2SidedQuad->z = 0.0f;
		p2SidedQuad->bodyDef->position = glm::vec3(0.0f, 0.0f, 0.0f);
		p2SidedQuad->scale = 1.0f;	// Unit sphere (0.5 units raduis)
		p2SidedQuad->meshName = "1x1_2Tri_Quad_2_Sided_XY_Plane.ply";
		p2SidedQuad->bIsVisible = false;
		p2SidedQuad->bIsImposter = true;
		p2SidedQuad->texture00Name = "smoketex.bmp";
		::g_2SidedQuadID = p2SidedQuad->getID();
		vec_pGOs.push_back(p2SidedQuad);
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

void SaveObjectsInScene(std::string xmlTarget)
{

	pugi::xml_document doc;

	doc.append_child("GameObjects");


	for (unsigned int i = 0; i < g_vec_pGOs.size(); i++)
	{
		if (g_vec_pGOs[i]->meshName == "Isoshphere_xyz_InvertedNormals.ply")
		{
			if (g_vec_pGOs[i]->bodyDef->staticBody)
			{
				pugi::xml_node targetNode = doc.child("GameObjects").append_child("Target");
				pugi::xml_node positionNode = targetNode.append_child("position");
				positionNode.append_attribute("x") = g_vec_pGOs[i]->bodyDef->position.x;		
				positionNode.append_attribute("y") = g_vec_pGOs[i]->bodyDef->position.y;
				positionNode.append_attribute("z") = g_vec_pGOs[i]->bodyDef->position.z;


				pugi::xml_node meshNode = targetNode.append_child("mesh");
				meshNode.append_attribute("name") = g_vec_pGOs[i]->meshName.c_str();
				pugi::xml_node scaleNode = targetNode.append_child("Scale");;
				scaleNode.append_attribute("Size") = g_vec_pGOs[i]->scale;
				pugi::xml_node  physicsNode = targetNode.append_child("mesh");
				physicsNode.append_attribute("staticbody") = true;
				
			}
			else if (!g_vec_pGOs[i]->bodyDef->staticBody)
			{
				pugi::xml_node targetNode = doc.child("GameObjects").append_child("Shot");
				pugi::xml_node positionNode = targetNode.append_child("position");
				positionNode.append_attribute("x") = g_vec_pGOs[i]->bodyDef->position.x;
				positionNode.append_attribute("y") = g_vec_pGOs[i]->bodyDef->position.y;
				positionNode.append_attribute("z") = g_vec_pGOs[i]->bodyDef->position.z;

				pugi::xml_node velocityNode = targetNode.append_child("velocity");
				velocityNode.append_attribute("x") = g_vec_pGOs[i]->bodyDef->velocity.x;
				velocityNode.append_attribute("y") = g_vec_pGOs[i]->bodyDef->velocity.y;
				velocityNode.append_attribute("z") = g_vec_pGOs[i]->bodyDef->velocity.z;



				pugi::xml_node meshNode = targetNode.append_child("mesh");
				meshNode.append_attribute("name") = g_vec_pGOs[i]->meshName.c_str();
				pugi::xml_node scaleNode = targetNode.append_child("Scale");;
				scaleNode.append_attribute("Size") = g_vec_pGOs[i]->scale;
				pugi::xml_node  physicsNode = targetNode.append_child("mesh");
				physicsNode.append_attribute("staticbody") = false;

			}
		}
	}

	doc.save_file(xmlTarget.c_str());

}

bool isShiftDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
	{
		return true;
	}
	return false;
}

bool isCrtlDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL )
	{
		return true;
	}
	return false;
}

bool isAltDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT )
	{
		return true;
	}
	return false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	//std::string coolSoundEffect = "";

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

	switch (key)
	{
	
	case GLFW_KEY_UP:
		{
			// FORWARD!!!
			//::g_pObjectToRotate->velocity.z += 0.1f;
			::g_pObjectToRotate->velDirected.z = 0.05f;

			// A point "in front" of the viper 
			// (as the model is oriented in "model" space).
			glm::vec3 vecFrontOfViper = glm::vec3( 0.0f, 0.0f, 1.0f );
			// From our vertex shader:
			// vertWorld = matModel3x3 * vPosition.xyz;
	//
	// Taken from DrawObject()
	//	glm::mat4 matRotation( pCurGO->qOrientaion );

			glm::mat3 matOrientation(::g_pObjectToRotate->qOrientaion); 

			glm::vec3 vecFrontFinal;
			vecFrontFinal = matOrientation * vecFrontOfViper;

			// update the velocity based on this orientation
			// Calcualte the final (actual) velocities, aligned to the axes
			//  based on this rotated "point" that is at the front of the viper
			glm::vec3 velChange = matOrientation * ::g_pObjectToRotate->velDirected;
	//		glm::vec3 velChange = vecFrontFinal * ::g_pObjectToRotate->velDirected;

			::g_pObjectToRotate->velocity += velChange;
			::g_pObjectToRotate->velDirected.z = 0.0f;

		}
		break;
	case GLFW_KEY_DOWN:
		{
			// BRAKE!!!!
			//::g_pObjectToRotate->velocity.z -= 0.1f;
			::g_pObjectToRotate->velDirected.z = -0.01f;
			glm::mat3 matOrientation(::g_pObjectToRotate->qOrientaion); 
			glm::vec3 velChange = matOrientation * ::g_pObjectToRotate->velDirected;
			::g_pObjectToRotate->velocity += velChange;
			::g_pObjectToRotate->velDirected.z = 0.0f;
		}
		break;
	// Light WSAD, QE, but for the light...
	case GLFW_KEY_A:		
		
		 //camera1->position.x -= camera1->moveSpeed;
		camera1->cameraTarget.x -= camera1->moveSpeed;
		break;
	case GLFW_KEY_D:
		 
		 /*camera1-9>position.x += camera1->moveSpeed;*/
		camera1->cameraTarget.x += camera1->moveSpeed;
		break;
	case GLFW_KEY_W:
		
		 camera1->cameraTarget.y += camera1->moveSpeed;
		break;
	case GLFW_KEY_S:
	
		 camera1->cameraTarget.y -= camera1->moveSpeed;
		break;
	case GLFW_KEY_1:
		ScopeSelection = 0;
		break;
	case GLFW_KEY_2:
		ScopeSelection = 1;
		break;
	case GLFW_KEY_3:
		ScopeSelection = 2;
		break;
	case GLFW_KEY_7:
		::g_vec_pGOs.clear();
		physicsWorld = physicsFactory->CreatePhysicsWorld();
		LoadObjectsIntoScene2(::g_vec_pGOs, "ShootingGalleryEasy.xml");
		fileSelector = 0;
		break;
	case GLFW_KEY_8:
		::g_vec_pGOs.clear();
		physicsWorld = physicsFactory->CreatePhysicsWorld();
		LoadObjectsIntoScene2(::g_vec_pGOs, "ShootingGalleryMedium.xml");
		fileSelector = 1;
		break;
	case GLFW_KEY_9:
		::g_vec_pGOs.clear();
		physicsWorld = physicsFactory->CreatePhysicsWorld();
		LoadObjectsIntoScene2(::g_vec_pGOs, "ShootingGalleryHard.xml");
		fileSelector = 2;
		break;
	case GLFW_KEY_0:
		if (fileSelector == 0)
		{
			SaveObjectsInScene("ShootingGalleryEasy.xml");
		}
		else 	if (fileSelector == 1)
		{
			SaveObjectsInScene("ShootingGalleryMedium.xml");
		}
		else 	if (fileSelector == 2)
		{
			SaveObjectsInScene("ShootingGalleryHard.xml");
		}
		//else 
		//{
		//	SaveObjectsInScene("ShootingGalleryEasy.xml");
		//}
		break;
	case GLFW_KEY_SPACE:
		if (action==1)
		::bNightVisionMode = !::bNightVisionMode;
			break;
	case GLFW_KEY_F:
		if (action == 1)
		{
			nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());
			nPhysics::cRigidBodyDef bodyDef;
			bodyDef.position = camera1->position;//glm::vec3(5.f, 5.f, 0.f);
			bodyDef.staticBody = false;
			bodyDef.mass = 1.f;
			bodyDef.velocity = (camera1->cameraTarget - camera1->position)*2.f;// glm::vec3(0.f, 0.f, -10.f) + (camera1->cameraTarget-camera1->position);
			nPhysics::iRigidBody* rigidBody = physicsFactory->CreateRigidBody(bodyDef, sphereShape);
			nPhysics::cRigidBodyDef* bodyDefCheck;
			bodyDefCheck = rigidBody->GetRigidBodyDef();
			cGameObject* pSphere = new cGameObject();
			pSphere->bIsUpdatedByPhysics = false;
			pSphere->bodyDef = bodyDefCheck;
			pSphere->meshName = "Isoshphere_xyz_InvertedNormals.ply";
			pSphere->scale = 1.0f;			// Ground is 50.0x50.0 

			pSphere->bIsWireframe = true;
			pSphere->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pSphere->bUseDebugColours = true;
			physicsWorld->AddBody(rigidBody);
			::g_vec_pGOs.push_back(pSphere);
		}
		break;
	case GLFW_KEY_KP_ADD:
		if (camera3->cameraTarget.z > -40.f)
		{
			camera3->cameraTarget.z -= camera3->moveSpeed;
			camera3->position.z -= camera3->moveSpeed;
		}
		break;
	case GLFW_KEY_KP_SUBTRACT:
		if (camera3->cameraTarget.z < 0.f)
		{
			camera3->cameraTarget.z += camera3->moveSpeed;
			camera3->position.z += camera3->moveSpeed;
		}
		break;
	}//switch (key)




	// Update the camera position in the title...
//	glfwSetWindowTitle( window, "Sexy" );

	std::stringstream ssTitle;
	//ssTitle << "Camera: " 
	//	<< ::g_CameraX << ", "
	//	<< ::g_CameraY << ", "
	//	<< ::g_CameraZ
	//	<< "  Light: " 
	//	<< ::lightPos.x << ", " 
	//	<< ::lightPos.y << ", " 
	//	<< ::lightPos.z
	//	<< " Lin: " << ::lightLinearAtten
	//	<< " Quad: " << ::lightQuadAtten
	//	<< " Number of triangles per frame: " << g_NumberOfIndicesRendered / 3;
	ssTitle << g_coolSoundEffect;
	glfwSetWindowTitle( window, ssTitle.str().c_str() );


	return;
}
