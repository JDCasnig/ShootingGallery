#include "GlobalStuff.h"

// We should place this into a class...
float g_CameraX = 6.9f;
float g_CameraY = 6.5f;
float g_CameraZ = 28.7f;	
float g_CameraMovementSpeed = 0.1f;

float g_TargetX = 0.0f;
float g_TargetY = 0.0f;
float g_TargetZ = 0.0f;

cCamera* camera1 = new cCamera();
cCamera* camera2 = new cCamera();
cCamera* camera3 = new cCamera();


glm::vec3 lightPos(5.4f, 5.0f, 19.4f);		// I just picked these values after moving the light around until it looked "nice"
float lightConstAtten = 0.0f;
float lightLinearAtten = 0.0477747f;
float lightQuadAtten = 0.000683441f;
bool bDrawLightDebugSpheres = false;
bool bDrawLightDebugLocation = true;
bool bNightVisionMode = false;
GLint locID_matModel = -1;		// 
GLint locID_matView = -1;
GLint locID_matProj = -1;

GLint UniformLoc_ID_objectColour = -1;
GLint UniformLoc_ID_isWireframe = -1;
GLint UniformLoc_ID_bUseDebugDiffuseNoLighting = -1;
GLint UniformLoc_ID_bIsLineDebugDrawingPass = -1;
GLint UniformLoc_ID_bIsNightVision = -1;
GLint UniformLoc_ID_bIsTextureScope=-1;
GLint UniformLoc_alpha = -1;

GLint UniformLoc_texSamp2D_00 = -1;
GLint UniformLoc_texSamp2D_01 = -1;
GLint UniformLoc_texSamp2D_02 = -1;
GLint UniformLoc_texSamp2D_03 = -1;

GLint UniformLoc_bUseTextureAsDiffuse = -1;
GLint UniformLoc_bUseLighting = -1;

GLint UniformLoc_bIsImposterBlackIsAlpha = -1;

// For the 2nd pass rendering.
GLint UniformLoc_tex2ndPass = -1;				// Actual texture sampler
GLint UniformLoc_bIsSecondRenderPass = -1;		// Flag to inticate it's the 2nd pass in the shader

GLuint g_FBO = 0;
GLuint g_FBO_colourTexture = 0;
GLuint g_FBO_depthTexture = 0;
GLint g_FBO_SizeInPixes = 1024;


GLuint g_FBO2 = 0;
GLuint g_FBO_colourTexture2 = 0;
GLuint g_FBO_depthTexture2 = 0;
GLint g_FBO_SizeInPixes2 = 1024;


std::vector< cGameObject* > g_vec_pGOs;
std::vector< cContact > g_vec_Contacts;

cMeshTypeManager* g_pTheMeshTypeManager = 0;

cShaderManager* g_pTheShaderManager = 0;

cParticleEmitter* g_pParticles = 0;

cBasicTextureManager* g_pTextureManager = 0;


// TODO: REMOVE THIS!!!!
cGameObject* g_pObjectToRotate = 0;

std::string g_coolSoundEffect = "";

int ScopeSelection = 0;

nPhysics::iPhysicsFactory* physicsFactory = NULL;
nPhysics::iPhysicsWorld* physicsWorld = NULL;
nPhysics::iSoftBody* curtain = NULL;
int fileSelector = 0;