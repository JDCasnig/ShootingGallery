#ifndef _cMeshTypeManager_HG_
#define _cMeshTypeManager_HG_

#include <string>
#include <vector>
#include <map>

// For cMesh
#include <glad\glad.h>		// For GLuint
#include <glm/glm.hpp>
#include "vert_XYZ_RGB_Nxyz_STU.h"

//struct cPlyVertexXYZ
//{
//	cPlyVertexXYZ(): x(0.0f), y(0.0f), z(0.0f), nX(0.0f), nY(0.0f), nZ(0.0f) {}
//	float x;
//	float y;
//	float z;
//	float nX;
//	float nY;
//	float nZ;
//};

struct cPlyVertexXYZ_N_STU
{
	cPlyVertexXYZ_N_STU(): x(0.0f), y(0.0f), z(0.0f), nX(0.0f), nY(0.0f), nZ(0.0f), s(0.0f), t(0.0f), u(0.0f) {}
	float x;
	float y;
	float z;
	float nX;
	float nY;
	float nZ;
	float s;
	float t;
	float u;
};

struct cPlyTriFace
{
	cPlyTriFace(): v1(-1), v2(-1), v3(-1) {}	// can never be -1 in an actual model
	int v1;
	int v2;
	int v3;
};




class cMeshTypeManager 
{
public:
	cMeshTypeManager();
	~cMeshTypeManager();

	void ScaleEverythingToBoundingBoxOnLoad( bool bScale, float BoxWidth );


	// Loads file from disk, AND
	// Copies inforamtion into VAO (Vertex Array Object)
	bool LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath,  
								  bool bMakeFabulousRainbows = false );
	bool LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath, 
								  float vertRed, float vertGreen, float vertBlue );
	bool LoadPlyFileIntoGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath, 
								  float vertRed, float vertGreen, float vertBlue, 
								  bool bMakeFabulousRainbows = false );


	// basepath is "" by default, and persists between calls to LoadPlyFileIntoGLBuffer
	void SetBaseFilePath( std::string basepath );

	bool LookUpMeshInfo( std::string meshName, 
						 unsigned int &VAO_ID,	    // GLuint by ref
						 int &numberOfIndices,
						 float &unitScale );		// by ref

	// *********************************************************************************
	// Added: Dynamic mesh manipulation
	bool LoadPlyFileIntoDynamicGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath, 
										 std::string meshNameToUse );
	// Another method to get at the mesh vertex info...
	//cMeshTypeManager::cMesh* FindDynamicMeshByName( std::string meshName );

	bool UpdateDynamicMesh( std::string meshName );
	// *********************************************************************************
	bool CreateDebugLineBuffer( unsigned int maxNumberOfVertices, 
							    GLuint shaderProgramID );
	void AddDebugLine( glm::vec3 start, glm::vec3 end, glm::vec3 colour );
	// "Clears" the debug lines
	void ClearBuffer(void);
	// Called by render
	bool GetDebugLineBufferForRender( unsigned int &VAO_ID,	    // GLuint by ref
						              unsigned int &numberOfVerticesToDraw );
private: 
	vert_XYZ_RGB_Nxyz_STU* m_p_vertArrayLines;  
	unsigned int m_sizeOfDebugLineArrayBufferInBytes;
	unsigned int m_nextLineIndexStart;
	GLuint m_line_VAO_ID;
	GLuint m_line_vert_buffer_ID;




	bool m_loadPlyModel( std::string filename, 
				   std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts, 
				   std::vector< cPlyTriFace > &vecPlyIndices );

	std::string m_basepath;

	bool m_bScaleEverythingToBoundingBoxOnLoad;	// = false by default
	float m_BoundingBoxScalingSize;	// = 1.0f by default
	
	void m_Normalize( std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts );

	void m_CalculateExtents( const std::vector< cPlyVertexXYZ_N_STU > &vecPlyVerts, 
                             float &minX, float &minY, float &minZ, 
                             float &maxX, float &maxY, float &maxZ, 
                             float &extentX, float &extentY, float &extentZ, 
                             float &maxExtent );


	struct VAOInfo
	{	
		VAOInfo() : VAO_ID(0), numberOfIndices(0), unitScaleValue(1.0f) {}
		std::string meshName;
		unsigned int VAO_ID;
		unsigned int numberOfIndices;
		float unitScaleValue;		// Multiply by this scale and object 1.0f
	};
	// Map... aka "dictionay" "look up table"
	std::map< std::string, VAOInfo > m_MapMeshNameToVAOInfo;

	// *********************************************************************************
	// For Dynamic mesh info
	// This holds a mesh, for use as a "dynamic" mesh
	// Note that this assumes we aren't changing the NUMBER of vertices...
	//	...which we +++CAN'T+++ do, anyway; we can draw ranges of them, but 
	//  we can't CHANGE the size of either the vertex or index buffer
	// The only way to do this would be to delete one, then re-create.
public:			// So that we can update the mesh 
	class cMesh
	{
	public:
		cMesh();
		~cMesh();		// Calls DeleteArrays() for cleanup
		VAOInfo meshVAOInfo;	// VAO_ID, Num_Indices, ModelFileName

		GLuint vertex_buffer_ID;
		unsigned int numberOfVertices;	// this isn't in VAOInfo
		unsigned int vertexBufferSizeInBytes;
		vert_XYZ_RGB_Nxyz_STU* pVertArray;

		GLuint index_buffer_ID;
		// unsigned int numberOfIndices;  is already in VAOInfo
		unsigned int indexBufferSizeInBytes;
		int* pIndexArray;

		void DeleteArrays(void);
	};
	// Another method to get at the mesh vertex info...
	cMeshTypeManager::cMesh* FindDynamicMeshByName( std::string meshName );

private:
	std::map< std::string /*meshName*/, cMesh* /*pMesh*/ > m_mapNameToDynamicMesh;
	// *********************************************************************************
};

#endif
  
// vecNames["bunny.ply"] = ( "bunny.ply", 1, 1928 );
// vecNames["555-3432"] = "Robin"
// vecNames["555-3432"] = "Jacob"
// vecNames[3] = "Fraser"