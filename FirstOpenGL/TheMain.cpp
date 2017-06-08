#include <glad/glad.h>		
#include <GLFW/glfw3.h>		
#include <iostream>

#include "GlobalStuff.h"

//#include "linmath.h"
//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "GLMIncludes.h"
#include <stdlib.h>
#include <stdio.h>
#include <sstream>		// "String Stream", used for setting the windows title

#include <vector>

#include "vert_XYZ_RGB.h"


#include "Utilities.h"

#include <pugixml\pugixml.hpp>
//#include <pugixml\pugiconfig.hpp>



bool LoadObjectsIntoScene( std::vector< cGameObject* > &vec_pGOs, std::string sceneFile );

bool LoadPhysicsConfiguration(std::string configFile);

void LoadFloor();
void LoadLeftWall();
void LoadRightWall();

void DrawFloor();
void DrawLeftWall();
void DrawRightWall();




std::vector<cGameObject*> floorVec;
std::vector<cGameObject*> leftWallVec;
std::vector<cGameObject*> rightWallVec;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}



void SetUpFBO(void)
{
	//glCreateFramebuffers( 1, &g_FBO );		// 4.5 only
	glGenFramebuffers( 1, &g_FBO );			// every other GL
	glBindFramebuffer( GL_FRAMEBUFFER, g_FBO );

	glGenTextures( 1, &g_FBO_colourTexture );
	glBindTexture( GL_TEXTURE_2D, g_FBO_colourTexture );
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGB8, g_FBO_SizeInPixes, g_FBO_SizeInPixes);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glGenTextures( 1, &g_FBO_depthTexture );
	glBindTexture( GL_TEXTURE_2D, g_FBO_depthTexture );
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, g_FBO_SizeInPixes, g_FBO_SizeInPixes);

	glFramebufferTexture( GL_FRAMEBUFFER, 
				   GL_COLOR_ATTACHMENT0, 
				g_FBO_colourTexture, 0 );
	glFramebufferTexture( GL_FRAMEBUFFER, 
					GL_DEPTH_ATTACHMENT, 
				 g_FBO_depthTexture, 0 );

	static const GLenum draw_bufers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers( 1, draw_bufers );

	// Point back to default frame buffer
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return;
}

void SetUpFBO2(void)
{
	//glCreateFramebuffers( 1, &g_FBO );		// 4.5 only
	glGenFramebuffers(2, &g_FBO2);			// every other GL
	glBindFramebuffer(GL_FRAMEBUFFER, g_FBO2);

	glGenTextures(1, &g_FBO_colourTexture2);
	glBindTexture(GL_TEXTURE_2D, g_FBO_colourTexture2);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, g_FBO_SizeInPixes2, g_FBO_SizeInPixes2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &g_FBO_depthTexture2);
	glBindTexture(GL_TEXTURE_2D, g_FBO_depthTexture2);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, g_FBO_SizeInPixes2, g_FBO_SizeInPixes2);

	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		g_FBO_colourTexture2, 0);
	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		g_FBO_depthTexture2, 0);

	static const GLenum draw_bufers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufers);

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return;
}





std::vector< cParticleEmitter::cParticle > g_vecParticlesToRender;

int main(void)
{

	bool physicsLoaded = LoadPhysicsConfiguration("PhysicsConfig.xml");
	
	if (!physicsLoaded)
	{
		std::cout << "Physics Configuration did not load." << std::endl;
		return 0;
	}

    GLFWwindow* window;
	// Added an "index_buffer" variable here
    //GLuint vertex_buffer, index_buffer, vertex_shader, fragment_shader, program;
	//GLuint vertex_shader, fragment_shader; //, program;
//    GLint mvp_location;//, vpos_location, vcol_location;

	//GLint locID_matModel = -1;		// 
	//GLint locID_matView = -1;
	//GLint locID_matProj = -1;

	GLint locID_lightPosition = -1;
	GLint locID_lightDiffuse = -1;
	GLint locID_lightAttenuation = -1;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(1200, 800, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);


	std::cout << glGetString(GL_VENDOR) << " " 
		<< glGetString(GL_RENDERER) << ", " 
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

//	std::cout << glGetString(GL_EXTENSIONS) << std::endl;

	::g_pTheShaderManager = new cShaderManager();

	cShaderManager::cShader verShader;	
	cShaderManager::cShader fragShader;

	//verShader.fileName = "simpleVert.glsl";
	//fragShader.fileName = "simpleFrag.glsl";
	verShader.fileName = "phongVert.glsl";
	fragShader.fileName = "phongFrag.glsl";
	::g_pTheShaderManager->setBasePath( "assets//shaders//" );
	if ( ! ::g_pTheShaderManager->createProgramFromFile( "simple", verShader, fragShader ) )
	{	// Oh no!
		std::cout << ::g_pTheShaderManager->getLastError() << std::endl;
		std::cout.flush();
		// TODO: Shut it all down... 
		return -1;
	}
	


	// Note: this returns a bool, which you might want to check...
	// (We won't right now, though)
	GLuint shadProgID = ::g_pTheShaderManager->getIDFromFriendlyName("simple");

	

	// glGetActiveUniforms... 
	locID_matModel = glGetUniformLocation(shadProgID, "matModel");
	locID_matView = glGetUniformLocation(shadProgID, "matView");
	locID_matProj = glGetUniformLocation(shadProgID, "matProj");


	locID_lightPosition = glGetUniformLocation(shadProgID, "theLights[0].position");
	locID_lightDiffuse = glGetUniformLocation(shadProgID, "theLights[0].diffuse");
	locID_lightAttenuation = glGetUniformLocation(shadProgID, "theLights[0].attenuation");// = 0.0f;		// Can set to 1.0 to "turn off" a light

	//GLuint UniformLoc_ID_objectColour = 0;
	//GLuint UniformLoc_ID_isWireframe = 0;

	UniformLoc_ID_objectColour = glGetUniformLocation( shadProgID, "objectColour" );
	UniformLoc_ID_isWireframe = glGetUniformLocation( shadProgID, "isWireframe" );
	UniformLoc_ID_bUseDebugDiffuseNoLighting = glGetUniformLocation( shadProgID, "bUseDebugDiffuseNoLighting" );

	UniformLoc_ID_bIsLineDebugDrawingPass = glGetUniformLocation( shadProgID, "bIsLineDebugDrawingPass" );

	UniformLoc_alpha = glGetUniformLocation( shadProgID, "alpha" );

	UniformLoc_texSamp2D_00 = glGetUniformLocation( shadProgID, "texSamp2D_00" );
	UniformLoc_texSamp2D_01 = glGetUniformLocation( shadProgID, "texSamp2D_01" );
	UniformLoc_texSamp2D_02 = glGetUniformLocation( shadProgID, "texSamp2D_02" );
	UniformLoc_texSamp2D_03 = glGetUniformLocation( shadProgID, "texSamp2D_03" );

	UniformLoc_bUseTextureAsDiffuse = glGetUniformLocation( shadProgID, "bUseTextureAsDiffuse" );
	UniformLoc_bUseLighting = glGetUniformLocation( shadProgID, "bUseLighting" );

	UniformLoc_bIsImposterBlackIsAlpha  = glGetUniformLocation( shadProgID, "bIsImposterBlackIsAlpha" );
	
	UniformLoc_tex2ndPass = glGetUniformLocation( shadProgID, "tex2ndPass" );
	UniformLoc_bIsSecondRenderPass = glGetUniformLocation( shadProgID, "bIsSecondRenderPass" );
	UniformLoc_ID_bIsNightVision = glGetUniformLocation(shadProgID, "bIsNightVision");
	UniformLoc_ID_bIsTextureScope = glGetUniformLocation(shadProgID, "bIsTextureScope");

	// is now in the cMeshTypeManager
	::g_pTheMeshTypeManager = new cMeshTypeManager();


	::g_pTheMeshTypeManager->SetBaseFilePath( "assets//models//" );

	::g_pTheMeshTypeManager->ScaleEverythingToBoundingBoxOnLoad( true, 10.0f );

		// Objects we can use for imposters
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "1x1_2Tri_Quad_1_Sided_XY_Plane.ply" );
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "1x1_2Tri_Quad_2_Sided_XY_Plane.ply" );
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "1x1_2Tri_Quad_2_Sided_XZ_Plane.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "1x1_2Tri_Quad_2_Sided_YZ_Plane.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "1x1_2Tri_Quad_2_Sided_YZ_PlaneWall.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer(shadProgID, "pleasework.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "ArrowWithHead_xyz_nxyz_ASCII_Pos_Z_1_unit_long.ply" );
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "LowerResArrow_Pos_Z_1_unit_long.ply" );
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Long_bar_x_aligned_x10.ply" );
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Long_bar_y_aligned_x10.ply" );
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Long_bar_z_aligned_x10.ply" );

	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "ViperMKVII_Z_aligned.ply" );


	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_res3_XYZ_N.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "su47_XYZ_N.ply");					// "su47_XYZ.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "dolphin_XYZ_N.ply");				// "dolphin.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Sphere_N.ply");					// "Sphere.ply");
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Ground_XYZ_N_Rot_Y_is_up.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Sphere_InvertedNormals.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"

	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Isoshphere.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Isoshphere_xyz_InvertedNormals.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "cow_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "tie_withNormals.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_res2_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "bun_zipper_res4_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Cube_1x1x1_N.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Cube2.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"
	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Utah_Teapot.ply");	// "Ground_XYZ_Rot_Y_is_up.ply"

	//::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "Seafloor2_N.ply" );

	// Load the seafloor "dynamically"
	::g_pTheMeshTypeManager->LoadPlyFileIntoDynamicGLBuffer( shadProgID, "Seafloor2_N.ply", 
															 "seafloor" );

//	::g_pTheMeshTypeManager->LoadPlyFileIntoGLBuffer( shadProgID, "dragon_vrip_res2_N.ply" );

	// Create the Debug line drawing thing
	const unsigned int NUMBEROFLINESINBUFFER = 1'000; 
	::g_pTheMeshTypeManager->CreateDebugLineBuffer( NUMBEROFLINESINBUFFER, 
												    shadProgID );

	cGameObject* stencilObject = new cGameObject();
	stencilObject->meshName = "Isoshphere_xyz_InvertedNormals.ply";
	stencilObject->scale = 10.f;
	stencilObject->position = glm::vec3(0.f, 0.f, 10.f);

	camera1->position = glm::vec3(6.9f, 6.5f, 28.7f);
	camera3->position = glm::vec3(20.f, 3.f, 30.f);
	//float g_CameraX = 6.9f;
	//float g_CameraY = 6.5f;
	//float g_CameraZ = 28.7f;
//	struct vertXYRGB
//{
//    float x, y (z);				// 4 + 4 
//    float r, g, b;			// 4 + 4 + 4  = 20 bytes
//};

	// Create a particle emitter
	//::g_pParticles = new cParticleEmitter();

	//cParticleEmitter::sParticleDesc emitDescript;
	//// Change stuff in the description, perhaps?
	//emitDescript.lifetimeMin = 2.0f;
	//emitDescript.lifetimeMax = 4.0f;
	//emitDescript.particlesCreatedPerUpdate = 20;
	//// You would change this "force" to be in the direction
	//// you wanted to travel (or whatever)
	//emitDescript.ongoingForce = glm::vec3( 0.0f, 0.5f, 0.0f );

	//emitDescript.initVelMin = glm::vec3( -0.2f,  0.0f,  -0.2f);
	//emitDescript.initVelMax = glm::vec3(  0.2f,   1.0f,  0.2f);


	//::g_pParticles->allocateParticles( 2000, emitDescript /*, true*/);


	LoadFloor();
	LoadLeftWall();
	LoadRightWall();

	::g_pTextureManager = new cBasicTextureManager();

	::g_pTextureManager->SetBasePath("assets/textures");
	if ( ! ::g_pTextureManager->Create2DTextureFromBMPFile( "brickTexture_square_1024pixels.bmp", true ) )
	{
		std::cout << "WARNING: Didn't load texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	if ( ! ::g_pTextureManager->Create2DTextureFromBMPFile( "scope.bmp", true ) )
	{
		std::cout << "WARNING: Didn't load smoketex.bmp texture" << std::endl;
	}
	else
	{
		std::cout << "Texture loaded OK" << std::endl;
	}

	::g_pTextureManager->Create2DTextureFromBMPFile( "FenceActual.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "FenceAlphaMask.bmp", true );

	if ( ::g_pTextureManager->Create2DTextureFromBMPFile( "obama_smile.bmp", true ) )
	{
		std::cout << "Loaded Obama texture OK" << std::endl;
	}
	else
	{
		std::cout << "ERROR: OBAMA wasn't loaded!!!" << std::endl;
	}


	// Shader loading and building was here...

//    mvp_location = glGetUniformLocation(shadProgID, "MVP");


//SOFT BODY MADNESS
	std::vector<nPhysics::cRigidBodyDef*> nodes;
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{

			nPhysics::iShape* sphereShape = physicsFactory->CreateShape(nPhysics::sSphere());
			nPhysics::cRigidBodyDef bodyDef;
			bodyDef.position = glm::vec3(i*2.f, 7.f + j*2.f, -10.f);//glm::vec3(5.f, 5.f, 0.f);
			if (j == 2)
			{
				bodyDef.staticBody = true;
			}
			else
			{
				bodyDef.staticBody = false;
			}
			bodyDef.mass = 1.0f;
			bodyDef.velocity = glm::vec3(0.f, 0.f, 0.f);
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

			::g_vec_pGOs.push_back(pSphere);
			nodes.push_back(bodyDefCheck);

		}
	}

	

	std::vector<nPhysics::sNodeConstraint> nodeConstraints;


	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[8], nodes[7], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[8], nodes[7], 7.5f, 2.f, 30.f));
	/*nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[7], nodes[8], 7.5f, 2.f, 30.f));*/

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[6], nodes[7], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[7], nodes[6], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[5], nodes[4], 7.5f,  2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[5], nodes[4], 7.5f, 2.f, 30.f));
	//nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[4], nodes[5], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[3], nodes[4], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[4], nodes[3], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[2], nodes[1], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[2], nodes[1], 7.5f, 2.f, 30.f));
	//nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[1], nodes[2], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[0], nodes[1], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[1], nodes[0], 7.5f, 2.f, 30.f));
	/////////////////////////////////////////////////////
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[0], nodes[3], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[3], nodes[0], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[1], nodes[4], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[1], nodes[4], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[2], nodes[5], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[5], nodes[2], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[3], nodes[6], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[6], nodes[3], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[4], nodes[7], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[7], nodes[4], 7.5f, 2.f, 30.f));

	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[5], nodes[8], 7.5f, 2.f, 30.f));
	nodeConstraints.push_back(nPhysics::sNodeConstraint(nodes[8], nodes[5], 7.5f, 2.f, 30.f));




		nPhysics::cSoftBodyDef sbDef = nPhysics::cSoftBodyDef(nodes, nodeConstraints);




	::curtain = physicsFactory->CreateSoftBody(&sbDef);
	physicsWorld->AddBody(::curtain);
	//END SOFT BODY MADNESS



	if ( ! LoadObjectsIntoScene( ::g_vec_pGOs, "ShootingGallery.xml" ))
	{
		std::cout << "WARNING: Could not load all models into the scene." << std::endl;
	}


	// Set up the Frame Buffer Object (the off-screen one)
	SetUpFBO();
	SetUpFBO2();




	// Note that GLFW would have had to create a "depth buffer" also
	// (which it does, apparently...)
	glEnable(GL_DEPTH_TEST);

	cAverageSample timerAverage(100);
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
	//	stencilObject->position = glm::vec3(::g_CameraX, ::g_CameraY, ::g_CameraZ - 5.f);
		
		

		::g_coolSoundEffect = "";

		float ratio;
		int width, height;


		// *****************************************************************
		glUniform1i(UniformLoc_bIsSecondRenderPass, FALSE);		// 
																// Point rendering to the off-screen frame buffer object
		glBindFramebuffer(GL_FRAMEBUFFER, ::g_FBO2);

		// Clear the offscreen framebuffer
		glViewport(0, 0, g_FBO_SizeInPixes2, g_FBO_SizeInPixes2);
		GLfloat	zero = 0.0f;
		GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, &zero);
		glClearBufferfv(GL_DEPTH, 0, &one);

		width = g_FBO_SizeInPixes2;
		height = g_FBO_SizeInPixes2;

		// *****************************************************************

		ratio = width / (float)height;

		glViewport(0, 0, width, height);


		//        mat4x4 m, p, mvp;
		glm::mat4x4 matProjection;		// Was just "p"

		glm::mat4x4 matView(1.0f);	// "View" (or camera) matrix

									//		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective(0.6f, ratio, 0.01f, 10000.0f);

		//v = glm::lookAt( glm::vec3( 0.0f, 0.0f, -10.0f), // Eye
		matView = glm::lookAt(
			glm::vec3(camera3->position), // Eye
			glm::vec3(camera3->cameraTarget), // At (target)
			glm::vec3(0.0f, 1.0f, 0.0f));	// Up

											//		const float TIMESTEPCONST = 0.01f;	// 10ms

		static const float MAXDELTATIME = 0.1f;	// 100 ms
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();
		if (deltaTime > MAXDELTATIME)
		{
			deltaTime = MAXDELTATIME;
		}
		timerAverage.AddSample(deltaTime);
		deltaTime = static_cast<float>(timerAverage.getAverage());

		physicsWorld->TimeStep(deltaTime);
		if (bNightVisionMode)
		{
			glUniform1i(UniformLoc_ID_bIsNightVision, TRUE);
		}
		else
		{
			glUniform1i(UniformLoc_ID_bIsNightVision, FALSE);
		}

		glUniformMatrix4fv(locID_matProj, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matProjection));

		// This is set once at the start of the "scene" draw.
		glUniformMatrix4fv(locID_matView, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matView));

		glUniform4f(locID_lightPosition, lightPos.x, lightPos.y, lightPos.z, 1.0f);
		glUniform4f(locID_lightDiffuse, 1.0, 1.0f, 1.0f, 1.0f);
		glUniform4f(locID_lightAttenuation, ::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 1.0f);


		std::vector< cGameObject* > vecGO_Sorted;
		vecGO_Sorted.reserve(::g_vec_pGOs.size());
		// Copy to vector
		for (std::vector<cGameObject*>::iterator itGO = ::g_vec_pGOs.begin(); itGO != ::g_vec_pGOs.end(); itGO++)
		{
			vecGO_Sorted.push_back(*itGO);
		}

		// Do one pass of bubble sort, becuase the objects are likely to be in 
		// almost the correct order from frame to frame		
		glm::vec3 eye = camera1->position;
		if (vecGO_Sorted.size()>2)
		for (int index = 0; index != vecGO_Sorted.size() - 1; index++)
		{
			cGameObject* p1 = vecGO_Sorted[index];
			cGameObject* p2 = vecGO_Sorted[index + 1];

			if (glm::distance(eye, p1->position) < glm::distance(eye, p2->position))
			{	// p1 is closer than p2, but 
				// we are drawing from back to front, so switch them
				vecGO_Sorted[index] = p2;
				vecGO_Sorted[index + 1] = p1;
			}

		}//for ( int index = 0

		 // Copy order back to main vectro
		for (int index = 0; index != vecGO_Sorted.size(); index++)
		{
			::g_vec_pGOs[index] = vecGO_Sorted[index];
		}

		::g_NumberOfIndicesRendered = 0;

		// Start of Draw Scene
		//		for ( int index = 0; index != ::g_vec_pGOs.size(); index++)
		DrawFloor();
		DrawLeftWall();
		for (int index = 0; index != vecGO_Sorted.size(); index++)
		{
			//cGameObject* pCurGO = ::g_vec_pGOs[index];
			cGameObject* pCurGO = vecGO_Sorted[index];

			// Set identity matrix for all "parent" or "root" objects
			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			// Draw an object.... 
			DrawObject(pCurGO, matModel);

		}
		 // *********************************
		// *********************************

		

		//SECOND FRAME DRAW

		// *****************************************************************
		glUniform1i(UniformLoc_bIsSecondRenderPass, FALSE);		// 
		// Point rendering to the off-screen frame buffer object
		glBindFramebuffer(GL_FRAMEBUFFER, ::g_FBO);

		// Clear the offscreen framebuffer
		glViewport(0, 0, g_FBO_SizeInPixes, g_FBO_SizeInPixes);
//		GLfloat	zero = 0.0f;
//		GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, &zero);
		glClearBufferfv(GL_DEPTH, 0, &one);

		width = g_FBO_SizeInPixes;
		height = g_FBO_SizeInPixes;

		// *****************************************************************

		//// Move this line to later in the code when we draw the quad
		//// on the ACTUAL framebuffer we are "presenting" to the screen
		//glfwGetFramebufferSize(window, &width, &height);

		ratio = width / (float)height;

		glViewport(0, 0, width, height);

		// 1st one clears the "colour" buffer (i.e. the colour of the pixels)
		// 2nd one clears the "depth" or "Z" buffer 
 //       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );




		// The "view" matrix is (usually) only updated once per "scene"
		// The "projection" matrix only needs to be updated when the window size 
		//    changes, but here we are updating it every "scene"
		// (a "scene" is when ALL the game objects are drawn)

//        mat4x4 m, p, mvp;
		//glm::mat4x4 matProjection2;		// Was just "p"

		//glm::mat4x4 matView2(1.0f);	// "View" (or camera) matrix

//		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective(0.6f, ratio, 0.01f, 10000.0f);

		//v = glm::lookAt( glm::vec3( 0.0f, 0.0f, -10.0f), // Eye
		matView = glm::lookAt(
			glm::vec3(camera1->position), // Eye
			glm::vec3(camera1->cameraTarget), // At (target)
			glm::vec3(0.0f, 1.0f, 0.0f));	// Up

//		const float TIMESTEPCONST = 0.01f;	// 10ms

//		static const float MAXDELTATIME = 0.1f;	// 100 ms
		//float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		//lastTime = glfwGetTime();
		//if (deltaTime > MAXDELTATIME)
		//{
		//	deltaTime = MAXDELTATIME;
		//}
		timerAverage.AddSample(deltaTime);
		deltaTime = static_cast<float>(timerAverage.getAverage());

		// Physics update
		//PhysicsStep( deltaTime );		// 10 ms
		//// Collision detection step 
		//CollisionStep(deltaTime);

		//// Place the emitter at the light #0 location...
		//::g_pParticles->position = lightPos;
		//// Inidcate where the eye is (for the imposters)
		//::g_pParticles->particleSettings.imposterEyeLocation.x = ::g_CameraX;
		//::g_pParticles->particleSettings.imposterEyeLocation.y = ::g_CameraY;
		//::g_pParticles->particleSettings.imposterEyeLocation.z = ::g_CameraZ;
		//::g_pParticles->Update( deltaTime );
		if (bNightVisionMode)
		{
			glUniform1i(UniformLoc_ID_bIsNightVision, TRUE);
		}
		else
		{
			glUniform1i(UniformLoc_ID_bIsNightVision, FALSE);
		}
		// update the bunny rotation
//		::g_vec_pGOs[0]->pre_Rot_X += 0.01f;
//		::g_vec_pGOs[0]->post_Rot_X += 0.01f;

		// Only needs to be set if window is created and-or 
		// is resized (you could put this in the window re-size
		// callback in GLFW or freeGLUT)
		glUniformMatrix4fv(locID_matProj, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matProjection));

		// This is set once at the start of the "scene" draw.
		glUniformMatrix4fv(locID_matView, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matView));

		glUniform4f(locID_lightPosition, lightPos.x, lightPos.y, lightPos.z, 1.0f);
		glUniform4f(locID_lightDiffuse, 1.0, 1.0f, 1.0f, 1.0f);
		glUniform4f(locID_lightAttenuation, ::lightConstAtten, ::lightLinearAtten, ::lightQuadAtten, 1.0f);

		// Order the object by distance from the camera
		// Left for the students to do, for the time being...

		// Sort objects by distance from camera
		std::vector< cGameObject* > vecGO_Sorted2;
		vecGO_Sorted2.reserve(::g_vec_pGOs.size());
		// Copy to vector
		for (std::vector<cGameObject*>::iterator itGO = ::g_vec_pGOs.begin(); itGO != ::g_vec_pGOs.end(); itGO++)
		{
			vecGO_Sorted2.push_back(*itGO);
		}

		// Do one pass of bubble sort, becuase the objects are likely to be in 
		// almost the correct order from frame to frame		
		glm::vec3 eye2=camera1->position;
		if (vecGO_Sorted2.size()>2)
		for (int index = 0; index != vecGO_Sorted2.size() - 1; index++)
		{
			cGameObject* p1 = vecGO_Sorted2[index];
			cGameObject* p2 = vecGO_Sorted2[index + 1];

			// Use glm distance 
			// (note, you can use a "squared distance" function
			//  that does not do the square root, as it's faster)
			//glm::vec3 p1Pos(p1->x, p1->y, p1->z);
			//glm::vec3 p2Pos(p2->x, p2->y, p2->z);
			//if ( glm::distance( eye, p1Pos ) < glm::distance( eye, p2Pos ) )

			if (glm::distance(eye, p1->position) < glm::distance(eye, p2->position))
			{	// p1 is closer than p2, but 
				// we are drawing from back to front, so switch them
				vecGO_Sorted2[index] = p2;
				vecGO_Sorted2[index + 1] = p1;
			}

		}//for ( int index = 0

		// Copy order back to main vectro
		for (int index = 0; index != vecGO_Sorted2.size(); index++)
		{
			::g_vec_pGOs[index] = vecGO_Sorted2[index];
		}

		::g_NumberOfIndicesRendered = 0;

		// Start of Draw Scene
		//		for ( int index = 0; index != ::g_vec_pGOs.size(); index++)
		DrawFloor();
		DrawLeftWall();
		DrawRightWall();
		for (int index = 0; index != vecGO_Sorted2.size(); index++)
		{
			//cGameObject* pCurGO = ::g_vec_pGOs[index];
			cGameObject* pCurGO = vecGO_Sorted2[index];

			// Set identity matrix for all "parent" or "root" objects
			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			// Draw an object.... 
			DrawObject(pCurGO, matModel);

		}// for ( int index = 0;.... (bottom of "render scene" loop)
// End of Draw Scene

//		// Draw the particles...
//		int numParticles = ::g_pParticles->getParticlesCopy( ::g_vecParticlesToRender );
//		for ( int index = 0; index != numParticles; index++ )
//		{
//			DrawQuad( true, 
//			          ::g_vecParticlesToRender[index].position, 
//					  ::g_vecParticlesToRender[index].rotation,
//					  "",					/* texture name */
//					  1.0f,					/* scale */
//					  glm::vec3( 1.0f, 1.0f, 0.0f ), false /*useDebugColours*/, 
//					  false /*isWireframe*/);
//		}


		// *********************************
		// *********************************

	//// Adjust the transparency of the objects
	//	for ( std::vector<cGameObject*>::iterator itpGO = g_vec_pGOs.begin();
	//			itpGO != g_vec_pGOs.end(); itpGO++ )
	//	{
	//		(*itpGO)->alpha += 0.01f;
	//		// To high alpha?
	//		if ( (*itpGO)->alpha >= 1.0f )
	//		{	// Yes, so make it 0.0f
	//			(*itpGO)->alpha = 0.0f;
	//		}
	//	}

		// ***********************************************
		// ***********************************************
		// ***********************************************

		// Now we switch back the the "actual" frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (ScopeSelection == 1)
		{
			glEnable(GL_STENCIL_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			glStencilFunc(GL_NEVER, 1, 0xFF);
			glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			

			glStencilMask(0xFF);
			glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
			glm::vec3 realignScope = camera1->position - camera1->cameraTarget;
			realignScope = glm::normalize(realignScope) *10.f;
			stencilObject->scale = 5.f;
			stencilObject->bodyDef->position =  camera1->position - realignScope;
			DrawObject(stencilObject, matModel);

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);
			glStencilMask(0x00);
			// draw where stencil's value is 0
			glStencilFunc(GL_EQUAL, 0, 0xFF);
			/* (nothing to draw) */
			// draw only where stencil's value is 1
			glStencilFunc(GL_EQUAL, 1, 0xFF);
		}
		// Set the uniform to the 2nd pass
		glUniform1i(UniformLoc_bIsSecondRenderPass, TRUE);		// 
		if (ScopeSelection == 0)
		{
			glUniform1i(UniformLoc_ID_bIsTextureScope, TRUE);
		}
		else
		{
			glUniform1i(UniformLoc_ID_bIsTextureScope, FALSE);
		}
		// Set the texture for the 2nd pass
		// Pick texture unit 007
		glActiveTexture(GL_TEXTURE7);		// GL_TEXTURE0 = 33984
//		// ****JUST FOR TESTING, USE THE OBAMA TEXTURE****
//		GLuint ObamaTextureID = ::g_pTextureManager->getTextureIDFromName( "obama_smile.bmp" );
//		glBindTexture( GL_TEXTURE_2D, ObamaTextureID );
//		// ***********************************************
		glBindTexture(GL_TEXTURE_2D, g_FBO_colourTexture);
		glUniform1i(UniformLoc_tex2ndPass, 7);


		matView = glm::lookAt(
			glm::vec3(0.f, 0.f, -25.f), // Eye
			glm::vec3(0.f), // At (target)
			glm::vec3(0.0f, 1.0f, 0.0f));	// Up

		glUniformMatrix4fv(locID_matView, 1, GL_FALSE,
			(const GLfloat*)glm::value_ptr(matView));

		glm::vec3 oldCamera = camera1->position;
		//glm::vec3 oldCamera(::g_CameraX, ::g_CameraY, ::g_CameraZ);

		::g_CameraX = 0.0f;
		::g_CameraY = 0.0f;
		::g_CameraZ = 1.0f;

		// This will be the 2nd pass, drawing only the quad...
		DrawQuad(true /*2 sided*/,
			//glm::vec3(oldCamera.x, oldCamera.y, oldCamera.z - 30.0f),		/*position*/
					//oldCamera,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),		/*rotation*/
			"",								/*textureName*/
			15.0f,								/*scale*/
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),	/*debug colour*/
			false,								/*use debug colour*/
			false);			/*wireframe*/
		glActiveTexture(GL_TEXTURE7);		// GL_TEXTURE0 = 33984
											//		// ****JUST FOR TESTING, USE THE OBAMA TEXTURE****
											//		GLuint ObamaTextureID = ::g_pTextureManager->getTextureIDFromName( "obama_smile.bmp" );
											//		glBindTexture( GL_TEXTURE_2D, ObamaTextureID );
											//		// ***********************************************
		glUniform1i(UniformLoc_ID_bIsNightVision, FALSE);
		glUniform1i(UniformLoc_ID_bIsTextureScope, FALSE);
		glBindTexture(GL_TEXTURE_2D, g_FBO_colourTexture2);
		glUniform1i(UniformLoc_tex2ndPass, 7);
		if (ScopeSelection != 1 && ScopeSelection != 0)
		{
			DrawQuad(true /*2 sided*/,
				//glm::vec3(oldCamera.x, oldCamera.y, oldCamera.z - 30.0f),		/*position*/
				//oldCamera,
				glm::vec3(-6.0f, -6.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),		/*rotation*/
				"",								/*textureName*/
				4.0f,								/*scale*/
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),	/*debug colour*/
				false,								/*use debug colour*/
				false);

		}
  //::g_CameraX = oldCamera.x;
  //::g_CameraY = oldCamera.y;
  //::g_CameraZ = oldCamera.z;

  // ***********************************************
  // ***********************************************
  // ***********************************************

  // Show or "present" what we drew...
		glfwSwapBuffers(window);
		::g_CameraX = oldCamera.x;
		::g_CameraY = oldCamera.y;
		::g_CameraZ = oldCamera.z;
		if (ScopeSelection == 1)
		{
			glDisable(GL_STENCIL_TEST);
		}
        glfwPollEvents();

    }

	// Bye bye...
	delete ::g_pTheMeshTypeManager;

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


// Search by ID, then returns pointer to object
cGameObject* findObjectByID( unsigned int ID )
{
	for ( int index = 0; index != g_vec_pGOs.size(); index++ )
	{
		if ( ::g_vec_pGOs[index]->getID() == ID )
		{	// Found it! 
			return ::g_vec_pGOs[index];
		}
	}
	// Didn't find it
	return 0;
}



void DrawFloor()
{
//	cGameObject* pFloor = new cGameObject();
//	pFloor->bIsUpdatedByPhysics = false;	// 
//	pFloor->bIsWireframe = false;
//	pFloor->bodyDef->position.x = 0.f;
//	pFloor->bodyDef->position.y = 0.f;
//	pFloor->bodyDef->position.z = 0.f;
//	pFloor->bUseDebugColours = true;
//	pFloor->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);		// y = green
//	pFloor->meshName = "1x1_2Tri_Quad_2_Sided_XZ_Plane.ply";
//	pFloor->scale = 10.0f;
//	pFloor->texture00Name = "brickTexture_square_1024pixels.bmp";
//	pFloor->alpha =0.f;
////	pFloor->texture01Name = "brickTexture_square_1024pixels.bmp";
//	glm::mat4x4 matModel = glm::mat4x4(1.0f);
//
//	for (int i = -1; i < 2; i++)
//	{
//		for (int j = 10; j > -50; j--)
//		{
//			pFloor->bodyDef->position.x = i*10.f;
//			pFloor->bodyDef->position.z = j*10.f;
//			DrawObject(pFloor, matModel);
//		}
//	}

	for (unsigned i = 0; i < rightWallVec.size(); i++)
	{
		glm::mat4x4 matModel = glm::mat4x4(1.0f);
		DrawObject(floorVec[i], matModel);
	}


}



void DrawLeftWall()
{

//	cGameObject* pLeftWall = new cGameObject();
//	pLeftWall->bIsUpdatedByPhysics = false;	// 
//	pLeftWall->bIsWireframe = false;
//	pLeftWall->bodyDef->position.x = -15.f;
//	pLeftWall->bodyDef->position.y = 0.f;
//	pLeftWall->bodyDef->position.z = 0.f;
//	pLeftWall->bUseDebugColours = true;
//	pLeftWall->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);		// y = green
//	pLeftWall->meshName = "pleasework.ply";
//	pLeftWall->scale = 10.0f;
//	pLeftWall->texture00Name = "brickTexture_square_1024pixels.bmp";
////	pLeftWall->texture01Name = "brickTexture_square_1024pixels.bmp";
//	glm::mat4x4 matModel = glm::mat4x4(1.0f);
//
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 10; j > -50; j--)
//		{
//			pLeftWall->bodyDef->position.y = i*10.f+5.f;
//			
//			pLeftWall->bodyDef->position.z = j*10.f;
//			DrawObject(pLeftWall, matModel);
//		}
//	}

	for (unsigned i = 0; i < rightWallVec.size(); i++)
	{
		glm::mat4x4 matModel = glm::mat4x4(1.0f);
		DrawObject(leftWallVec[i], matModel);
	}


}
void DrawRightWall()
{
//	cGameObject* pRightWall = new cGameObject();
//	pRightWall->bIsUpdatedByPhysics = false;	// 
//	pRightWall->bIsWireframe = false;
//	pRightWall->bodyDef->position.x = 15.f;
//	pRightWall->bodyDef->position.y = 0.f;
//	pRightWall->bodyDef->position.z = 0.f;
//	pRightWall->bUseDebugColours = true;
//	pRightWall->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);		// y = green
//	pRightWall->meshName = "pleasework.ply";
//	pRightWall->scale = 10.0f;
//	pRightWall->texture00Name = "brickTexture_square_1024pixels.bmp";
//	
////	pRightWall->texture01Name = "brickTexture_square_1024pixels.bmp";
//
//	glm::mat4x4 matModel = glm::mat4x4(1.0f);
//
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 10; j > -50; j--)
//		{
//			pRightWall->bodyDef->position.y = i*10.f + 5.f;
//
//			pRightWall->bodyDef->position.z = j*10.f;
//			DrawObject(pRightWall, matModel);
//		}
//	}
	for (unsigned i = 0; i < rightWallVec.size(); i++)
	{
		glm::mat4x4 matModel = glm::mat4x4(1.0f);
		DrawObject(rightWallVec[i], matModel);
	}

}


void LoadFloor()
{
	bool colour = false;
	for (int i = -1; i < 2; i++)
	{
		for (int j = 10; j > -20; j--)
		{
	cGameObject* pFloor = new cGameObject();
	pFloor->bIsUpdatedByPhysics = false;	// 
	pFloor->bIsWireframe = false;
	pFloor->bodyDef->position.x = 0.f;
	pFloor->bodyDef->position.y = 0.f;
	pFloor->bodyDef->position.z = 0.f;
	pFloor->bUseDebugColours = true;
	if (colour)
	{
		pFloor->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);		// y = green
	}
	else
	{
		pFloor->debugRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);		// y = green
	}
	colour = !colour;		// y = green
	pFloor->meshName = "1x1_2Tri_Quad_2_Sided_XZ_Plane.ply";
	pFloor->scale = 10.0f;
	pFloor->texture00Name = "brickTexture_square_1024pixels.bmp";
	pFloor->alpha = 0.f;
	//	pFloor->texture01Name = "brickTexture_square_1024pixels.bmp";
	glm::mat4x4 matModel = glm::mat4x4(1.0f);


			pFloor->bodyDef->position.x = i*10.f;
			pFloor->bodyDef->position.z = j*10.f;
			floorVec.push_back(pFloor);
		}
	}
}


void LoadLeftWall()
{
	bool colour = false;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 10; j > -20; j--)
		{
	cGameObject* pLeftWall = new cGameObject();
	pLeftWall->bIsUpdatedByPhysics = false;	// 
	pLeftWall->bIsWireframe = false;
	pLeftWall->bodyDef->position.x = -15.f;
	pLeftWall->bodyDef->position.y = 0.f;
	pLeftWall->bodyDef->position.z = 0.f;
	pLeftWall->bUseDebugColours = true;
	if (colour)
	{
		pLeftWall->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);		// y = green
	}
	else
	{
		pLeftWall->debugRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);		// y = green
	}
	colour = !colour;
	pLeftWall->meshName = "pleasework.ply";
	pLeftWall->scale = 10.0f;
	pLeftWall->texture00Name = "brickTexture_square_1024pixels.bmp";
	//	pLeftWall->texture01Name = "brickTexture_square_1024pixels.bmp";
	glm::mat4x4 matModel = glm::mat4x4(1.0f);


			pLeftWall->bodyDef->position.y = i*10.f + 5.f;

			pLeftWall->bodyDef->position.z = j*10.f;
			leftWallVec.push_back(pLeftWall);
		}
	}


}

void LoadRightWall()
{
	bool colour = false;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 10; j > -20; j--)
		{
	cGameObject* pRightWall = new cGameObject();
	pRightWall->bIsUpdatedByPhysics = false;	// 
	pRightWall->bIsWireframe = false;
	pRightWall->bodyDef->position.x = 15.f;
	pRightWall->bodyDef->position.y = 0.f;
	pRightWall->bodyDef->position.z = 0.f;
	pRightWall->bUseDebugColours = true;
	if (colour)
	{
		pRightWall->debugRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);		// y = green
	}
	else
	{
		pRightWall->debugRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);		// y = green
	}
	colour = !colour;
	pRightWall->meshName = "pleasework.ply";
	pRightWall->scale = 10.0f;
	pRightWall->texture00Name = "brickTexture_square_1024pixels.bmp";

	//	pRightWall->texture01Name = "brickTexture_square_1024pixels.bmp";

	glm::mat4x4 matModel = glm::mat4x4(1.0f);


			pRightWall->bodyDef->position.y = i*10.f + 5.f;

			pRightWall->bodyDef->position.z = j*10.f;
			rightWallVec.push_back(pRightWall);
		}
	}

}