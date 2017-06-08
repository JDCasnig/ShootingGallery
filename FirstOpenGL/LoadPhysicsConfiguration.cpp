#include "cGameObject.h"
#include "Utilities.h"	// For getRandFloat
#include <vector>
#include "GlobalStuff.h"
#include <pugixml\pugixml.hpp>
#include <iostream>


bool LoadPhysicsConfiguration(std::string configFile)
{
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(configFile.c_str());

	std::cout << "Load result: " << result.description() << ", Physics Engine: " << doc.child("PhysicsEngine").attribute("name").value() << std::endl;
	
	if (doc.child("PhysicsEngine").attribute("name").value() == "JPhysics")
	{
		physicsFactory = new cJPhysicsFactory();
	}
	else if (doc.child("PhysicsEngine").attribute("name").value() == "Bullet")
	{
		physicsFactory = new cJPhysicsFactory(); //cBulletFactory();
	}
	else
	{
		physicsFactory = new cJPhysicsFactory();
	}
	
	physicsWorld = physicsFactory->CreatePhysicsWorld();
	
	if (!doc.child("PhysicsEngine").child("Gravity").empty())
	{
		physicsWorld->SetGravity(glm::vec3(doc.child("PhysicsEngine").child("Gravity").attribute("x").as_double(), doc.child("PhysicsEngine").child("Gravity").attribute("y").as_double(), doc.child("PhysicsEngine").child("Gravity").attribute("z").as_double()));
	}

	return true;
}