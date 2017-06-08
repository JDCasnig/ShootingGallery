#ifndef _GlobalStuff_HG_
#define _GlobalStuff_HG_

#include <glad/glad.h>		
#include <GLFW/glfw3.h>	

#include <vector>
#include "cGameObject.h"
#include "cMeshTypeManager.h"	
#include "cShaderManager.h"
#include "cContact.h"	// Object collision object
#include "cParticleEmitter.h"
#include "Texture/cBasicTextureManager.h"	// ADDED
#include "cCamera.h"
#include <iPhysicsFactory.h>
#include <cJPhysicsFactory.h>
#include <cBulletPhysicsWorld.h>
#include <cBulletFactory.h>

extern void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// We should place this into a class...
extern float g_CameraX;
extern float g_CameraY;
extern float g_CameraZ;	
extern float g_CameraMovementSpeed;

extern float g_TargetX;
extern float g_TargetY;
extern float g_TargetZ;

extern cCamera* camera1;
extern cCamera* camera2;
extern cCamera* camera3;




extern glm::vec3 lightPos;		
extern float lightConstAtten;
extern float lightLinearAtten;
extern float lightQuadAtten;
extern bool bDrawLightDebugSpheres;
extern bool bDrawLightDebugLocation;
extern bool bNightVisionMode;
extern GLint UniformLoc_ID_bIsNightVision;
extern GLint UniformLoc_ID_bIsTextureScope;
extern GLint locID_matModel;		// 
extern GLint locID_matView;
extern GLint locID_matProj;

extern GLint UniformLoc_ID_objectColour;
extern GLint UniformLoc_ID_isWireframe;
extern GLint UniformLoc_ID_bUseDebugDiffuseNoLighting;

extern GLint UniformLoc_ID_bIsLineDebugDrawingPass;

extern GLint UniformLoc_alpha;

extern GLint UniformLoc_texSamp2D_00;
extern GLint UniformLoc_texSamp2D_01;
extern GLint UniformLoc_texSamp2D_02;
extern GLint UniformLoc_texSamp2D_03;
extern GLint UniformLoc_bUseTextureAsDiffuse;
extern GLint UniformLoc_bUseLighting;
// These two are for the 2nd pass rendering to the quad...
extern GLint UniformLoc_tex2ndPass;				// Actual texture sampler
extern GLint UniformLoc_bIsSecondRenderPass;	// Flag to inticate it's the 2nd pass in the shader

extern GLuint g_FBO;				// = 0;
extern GLuint g_FBO_colourTexture;	// = 0;
extern GLuint g_FBO_depthTexture;	// = 0;
extern GLint g_FBO_SizeInPixes;		// = 512;

extern GLuint g_FBO2;
extern GLuint g_FBO_colourTexture2;
extern GLuint g_FBO_depthTexture2;
extern GLint g_FBO_SizeInPixes2;



extern int ScopeSelection;

extern GLint UniformLoc_bIsImposterBlackIsAlpha;

// TODO: REMOVE THIS!!!!
extern cGameObject* g_pObjectToRotate;


extern std::vector< cGameObject* > g_vec_pGOs;
extern std::vector< cContact > g_vec_Contacts;

extern cMeshTypeManager* g_pTheMeshTypeManager;

extern cShaderManager* g_pTheShaderManager;

extern cParticleEmitter* g_pParticles;

extern cBasicTextureManager* g_pTextureManager;

void DrawDebugLightSpheres(void);
void DrawDebugBall( glm::vec3 position, 
				    glm::vec4 colour, float scale );
void DrawQuad( bool bTwoSided, glm::vec3 position, glm::vec3 rotation, std::string texture, float scale,
			   glm::vec4 debugColour, bool bUseDebugColour, 
			   bool bIsWireFrame );

void DrawObject( cGameObject* pCurGO, 
				 glm::mat4x4 matModel,		// NOT passing by reference 
				 bool bUseLighting = true );

cGameObject* findObjectByID( unsigned int ID );


extern unsigned int g_NumberOfIndicesRendered;

extern std::string g_coolSoundEffect;

extern nPhysics::iPhysicsFactory* physicsFactory;
extern nPhysics::iPhysicsWorld* physicsWorld;
extern nPhysics::iSoftBody* curtain;
extern int fileSelector;
#endif
