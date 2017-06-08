// cMeshTypeManager_DynamicMesh
#include "cMeshTypeManager.h"

#include <glad/glad.h>			// OpenGL defines...
#include <glm/glm.hpp>

#include "vert_XYZ_RGB_Nxyz_STU.h"		// Now with texture coordinates!!

#include "Ply/CPlyFile5nt.h"
#include "Ply/CStringHelper.h"


cMeshTypeManager::cMesh::cMesh()
{
	this->pVertArray = nullptr;
	this->vertex_buffer_ID = 0;
	this->numberOfVertices = 0;
	this->vertexBufferSizeInBytes = 0;

	this->pIndexArray = nullptr; 
	this->index_buffer_ID = 0;
	this->indexBufferSizeInBytes = 0;

	return;
}

cMeshTypeManager::cMesh::~cMesh()
{
	// Clean up the arrays
	this->DeleteArrays();
	return;
}

void cMeshTypeManager::cMesh::DeleteArrays(void)
{
	if ( this->pIndexArray )
	{
		delete [] this->pIndexArray;
	}
	if ( this->pVertArray )
	{
		delete [] this->pVertArray;
	}
	return;
}

// This is essentially the same as the "regular" (static) mesh loader, 
//	but here we keep the original mesh information around so that it can
//	be updated later if we want. 
// Note that this is fairly expensive operation, compared to NOT updating the mesh, 
//	and compared to updating the mesh on the GPU (which is more complicated, of course)
bool cMeshTypeManager::LoadPlyFileIntoDynamicGLBuffer( unsigned int programID, std::string plyFileNameNoBasePath, 
										std::string meshNameToUse )
{
	// ********************************************************************
	// *** START OF: Copying object data into VAO (vertex array object)

	// Add the basepath the the filename
	std::string fullFileName = this->m_basepath + plyFileNameNoBasePath;

	// We are using the good old ply loader, again
	// (you could use assimp or something else, if you'd like)
	CPlyFile5nt plyModel;
	std::wstring wError;
	if ( ! plyModel.OpenPLYFile2( CStringHelper::ASCIIToUnicodeQnD(fullFileName), wError ) )
	{
		return false;
	}

	// Normals? 
	if ( ! plyModel.bHasNormalsInFile() )	
	{
		plyModel.normalizeTheModelBaby();
	}

	plyModel.normlizeExistingNomrals();

	// If there aren't texture coordinates, generate them
	if ( ! plyModel.bHasTextureCoordinatesInFile() )
	{
		plyModel.GenTextureCoordsSpherical( CPlyFile5nt::POSITIVE_X, 
										    CPlyFile5nt::POSITIVE_Y, 
										    false /*based on normals*/,
										    1.0f  /*scale*/,
										    false );
	}

	// If we are to scale the model on load, then scale it! 
	if ( this->m_bScaleEverythingToBoundingBoxOnLoad )
	{
		// All the "extent" stuff is already built into the ply loader
		plyModel.ScaleToFit(this->m_BoundingBoxScalingSize);
	}

	// Here we use a cMesh, which contains the vertex array
	cMesh* pMesh = new cMesh();

	pMesh->numberOfVertices = (int)plyModel.GetNumberOfVerticies();

	//vert_XYZ_RGB_Nxyz_STU* p_vertArray = new vert_XYZ_RGB_Nxyz_STU[numberofVerts]; 
	pMesh->pVertArray = new vert_XYZ_RGB_Nxyz_STU[pMesh->numberOfVertices]; 


	for ( int index = 0; index != pMesh->numberOfVertices; index++ )
	{
		// The ply loader also has all these vertex attributes, so we aren't 
		//  loading them into an intermediate vector representation; instead
		//	we are placing them directly into the vertex array (on the CPU)
		pMesh->pVertArray[index].x = plyModel.getVertex_at(index).xyz.x;
		pMesh->pVertArray[index].y = plyModel.getVertex_at(index).xyz.y;
		pMesh->pVertArray[index].z = plyModel.getVertex_at(index).xyz.z;

		pMesh->pVertArray[index].r = plyModel.getVertex_at(index).red;
		pMesh->pVertArray[index].g = plyModel.getVertex_at(index).green;
		pMesh->pVertArray[index].b = plyModel.getVertex_at(index).blue;

		pMesh->pVertArray[index].nX = plyModel.getVertex_at(index).nx;
		pMesh->pVertArray[index].nY = plyModel.getVertex_at(index).ny;
		pMesh->pVertArray[index].nZ = plyModel.getVertex_at(index).nz;

		// Added: texture coordinates
		pMesh->pVertArray[index].s = plyModel.getVertex_at(index).tex0u;
		pMesh->pVertArray[index].t = plyModel.getVertex_at(index).tex0v;
		// There's a potential 3rd texture coordinate, but we aren't loading it now. 
		pMesh->pVertArray[index].u = 0.0f;	//vecPlyVerts[index].u;
	}

	// Create a vertex array object...	//  "bunny.ply" ---> VAO_ID
//	cMeshTypeManager::VAOInfo tempMeshVAOInfo;	
//	tempMeshVAOInfo.meshName = plyFileNameNoBasePath;
	pMesh->meshVAOInfo.meshName = plyFileNameNoBasePath;
	// Save the unit scale value
	//tempMeshVAOInfo.unitScaleValue = 1.0f / plyModel.getMaxExtent();  //0.01f
	pMesh->meshVAOInfo.unitScaleValue = 1.0f / plyModel.getMaxExtent();  //0.01f
	
	glGenVertexArrays(1, &(pMesh->meshVAOInfo.VAO_ID) );	
	glBindVertexArray( pMesh->meshVAOInfo.VAO_ID );

	//GLuint vertex_buffer = 0;
	//GLuint index_buffer = 0;

	// Create a vertex buffer
    //glGenBuffers(1, &vertex_buffer);  
    glGenBuffers(1, &(pMesh->vertex_buffer_ID) );  
	// Brings the particular vertex buffer into context
    //glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, pMesh->vertex_buffer_ID);

	//int sizeInBytes = sizeof( vert_XYZ_RGB_Nxyz_STU ) * numberofVerts;
	pMesh->vertexBufferSizeInBytes = sizeof( vert_XYZ_RGB_Nxyz_STU ) * pMesh->numberOfVertices;

    glBufferData(GL_ARRAY_BUFFER, 
				 pMesh->vertexBufferSizeInBytes,		// sizeInBytes, 
				 pMesh->pVertArray,				// p_vertArray,			// Pointer to that array				 
//******************************************************************************
// _____ _  _ ___   __  __  ___  _  _ _____   __  __  __   _   _  _____ ___ _ 
//|_   _| || | __| |  \/  |/ _ \| \| | __\ \ / / |  \/  | /_\ | |/ / __| _ (_)
//  | | | __ | _|  | |\/| | (_) | .` | _| \ V /  | |\/| |/ _ \| ' <| _||   /_ 
//  |_| |_||_|___| |_|  |_|\___/|_|\_|___| |_|   |_|  |_/_/ \_\_|\_\___|_|_(_)
//
// This is the 1st aspect of this, the key thing. We are telling OpenGL that the 
//  vertex buffer can (and likely will) be updated later. When the vertex 
//  buffer is "static" - or WRITE only - the driver has the option to place it 
//  in a location for faster reading. In extreme cases, we can update the 
//  mesh every frame, maybe?
				GL_DYNAMIC_DRAW );					//GL_STATIC_DRAW);
//******************************************************************************



	// Generate an "index" buffer
    //glGenBuffers(1, &index_buffer);  
	glGenBuffers(1, &(pMesh->index_buffer_ID) );
	// Brings the particular vertex buffer into context
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->index_buffer_ID );


	// Copy the index information into the index buffer
	//int numberOfTriangles = (int)vecPlyIndices.size();
//	int numberOfIndices = numberOfTriangles * 3;
	//tempMeshVAOInfo.numberOfIndices = numberOfTriangles * 3;

	pMesh->meshVAOInfo.numberOfIndices = plyModel.GetNumberOfElements() * 3;
	//int* pIndexArray = new int[tempMeshVAOInfo.numberOfIndices];
	pMesh->pIndexArray = new int[pMesh->meshVAOInfo.numberOfIndices];

	// indexBufferIndex			triAngleVecIndex
	//    0                        0  (v1)
	//    1                        0  (v2)
	//    2                        0  (v3)
	//    3                        1  (v1)
	//    4                        2  (v2)

	int indexBufferIndex = 0;
	for ( int triIndex = 0; 
		  triIndex != plyModel.GetNumberOfElements(); 
		  triIndex++, indexBufferIndex += 3 )
	{
		pMesh->pIndexArray[indexBufferIndex + 0]  = plyModel.getElement_at(triIndex).vertex_index_1;	// vecPlyIndices[triIndex].v1;		// Index 0
		pMesh->pIndexArray[indexBufferIndex + 1]  = plyModel.getElement_at(triIndex).vertex_index_2;	// vecPlyIndices[triIndex].v2;		// Index 0
		pMesh->pIndexArray[indexBufferIndex + 2]  = plyModel.getElement_at(triIndex).vertex_index_3;	// vecPlyIndices[triIndex].v3;		// Index 0
	}

	//int indexBufferArraySizeInBytes = tempMeshVAOInfo.numberOfIndices * sizeof(int);
	pMesh->indexBufferSizeInBytes = pMesh->meshVAOInfo.numberOfIndices * sizeof(int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
				 pMesh->indexBufferSizeInBytes, 
				 pMesh->pIndexArray,			// pIndexArray,			// index array // Pointer to that array
	// ******************************************************************************
	// NOTE: Even though we have stored the index information in the cMesh, 
	//	it's exceptionally unlikley that we'll be UPDATING this dynamically. 
	// Even systems, like Quake 1 and 2, would store a number of sets of 
	//	indices, each one pointing to DIFFERENT vertices, and then draw
	//	different ranges of indices, but they didn't UPDATE them. 
	// 
	// Why save them? Well, we might want to look up the triangle information or something, maybe?
	// 
	// Anyway, for "completeness", we'll store this as dynamic, too
				 GL_DYNAMIC_DRAW);			// GL_STATIC_DRAW);



	// Bind the vertex attributes to the shader
	int sizeOfVertInBytes = sizeof( vert_XYZ_RGB_Nxyz_STU );

	// Telling the shader where the vertex info is...
	// It's "layout"
	// Note that we need the shader program ID to store the connection 
	//	between the vertex buffers and the shader program attributes
    GLuint vpos_location = glGetAttribLocation(programID, "vPosition");	// 12 byes
    GLuint vcol_location = glGetAttribLocation(programID, "vColour");	// 12 bytes, starts at byte 12
    GLuint vnorm_location = glGetAttribLocation(programID, "vNormal");	// 12 bytes, starts at byte 24
    GLuint vstu_location = glGetAttribLocation(programID, "vTexSTU");	// 12 bytes, starts at byte 36

    glEnableVertexAttribArray(vpos_location);
//	int offsetInBytesToPosition = offsetof( vert_XYZ_RGB, r );
//	int offsetInBytesToPosition = offsetof( vert_XYZ_RGB_Nxyz, x );
	int offsetInBytesToPosition = offsetof( vert_XYZ_RGB_Nxyz_STU, x );
    glVertexAttribPointer( vpos_location, 3, 
						  GL_FLOAT, GL_FALSE,
                          sizeOfVertInBytes,	// sizeof(float) * 6,		// 
						  //(void*) 0);
						  reinterpret_cast<void*>(offsetInBytesToPosition) );

    glEnableVertexAttribArray(vcol_location);
//	int offsetInBytesToColour = offsetof( vert_XYZ_RGB, r );
//	int offsetInBytesToColour = offsetof( vert_XYZ_RGB_Nxyz, r );
	int offsetInBytesToColour = offsetof( vert_XYZ_RGB_Nxyz_STU, r );
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeOfVertInBytes,	// sizeof(float) * 5, 
						 // (void*) (sizeof(float) * 3));
						  reinterpret_cast<void*>(offsetInBytesToColour) );

    glEnableVertexAttribArray(vnorm_location);
//	int offsetInBytesToNormal = offsetof( vert_XYZ_RGB_Nxyz, nX );
 	int offsetInBytesToNormal = offsetof( vert_XYZ_RGB_Nxyz_STU, nX );
    glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_TRUE,
                          sizeOfVertInBytes,	// sizeof(float) * 5, 
						  //(void*) (sizeof(float) * 3));
						  reinterpret_cast<void*>(offsetInBytesToNormal) );

    glEnableVertexAttribArray(vstu_location);
//	int offsetInBytesToNormal = offsetof( vert_XYZ_RGB_Nxyz, nX );
 	int offsetInBytesToSTU = offsetof( vert_XYZ_RGB_Nxyz_STU, s );
    glVertexAttribPointer(vstu_location, 3, GL_FLOAT, GL_TRUE,
                          sizeOfVertInBytes,	// sizeof(float) * 5, 
						  //(void*) (sizeof(float) * 3));
						  reinterpret_cast<void*>(offsetInBytesToSTU) );


	// Data is copied into the vertex (and index) buffers, so delete array(s)
//******************************************************************************
// _____ _  _ ___   __  __  ___  _  _ _____   __  __  __   _   _  _____ ___ _ 
//|_   _| || | __| |  \/  |/ _ \| \| | __\ \ / / |  \/  | /_\ | |/ / __| _ (_)
//  | | | __ | _|  | |\/| | (_) | .` | _| \ V /  | |\/| |/ _ \| ' <| _||   /_ 
//  |_| |_||_|___| |_|  |_|\___/|_|\_|___| |_|   |_|  |_/_/ \_\_|\_\___|_|_(_)
//
// We DON'T delete the arrays, because we'll need them later. 
// Basically, we update the CPU side vertices, then recopy-update the vertex buffer
//******************************************************************************
	//delete [] p_vertArray;			// Note slightly strange syntax for array deletion
	//delete [] pIndexArray;

	// Save that VAO info for later
	this->m_MapMeshNameToVAOInfo[plyFileNameNoBasePath] = pMesh->meshVAOInfo;

	// Also save the "dynamic" inforamtion
	this->m_mapNameToDynamicMesh[meshNameToUse] = pMesh;


	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ********************************************************************
	// *** END OF: Copying object data into VAO (vertex array object)
	// ********************************************************************

	return true;
}

cMeshTypeManager::cMesh* cMeshTypeManager::FindDynamicMeshByName( std::string meshName )
{
	// Exists? 
	std::map< std::string /*meshName*/, cMeshTypeManager::cMesh* /*pMesh*/ >::iterator itMesh
		= this->m_mapNameToDynamicMesh.find( meshName );

	if ( itMesh == this->m_mapNameToDynamicMesh.end() )
	{	// What ARE you doing??? There's no mesh like that! Gheesh.
		return false;
	}
	// Found it!

	cMesh* pTheMesh = itMesh->second;

	return pTheMesh;
}

bool cMeshTypeManager::UpdateDynamicMesh( std::string meshName )
{
	// Exists? 
	std::map< std::string /*meshName*/, cMesh* /*pMesh*/ >::iterator itMesh
		= this->m_mapNameToDynamicMesh.find( meshName );

	if ( itMesh == this->m_mapNameToDynamicMesh.end() )
	{	// What ARE you doing??? There's no mesh like that! Gheesh.
		return false;
	}

	// Update the vertex buffer
	// Not that this is pretty much the same... 
	// PSYCH!! It's EXACTLY the same as the orginal update!!!
	// So TOTALLY got you!!
	// 
	// Note that you can also update a sub-set of the vertex buffer, if you'd like
	//	with the glBufferSubData() call. 
	// 
	// For we are updating the ENTIRE array

	cMesh* pTheMesh = itMesh->second;

	// Note we are CREATING (glGenBuffer) the buffer as it's ALREADY THERE, yo!
    glBindBuffer(GL_ARRAY_BUFFER, pTheMesh->vertex_buffer_ID);

//	pTheMesh->vertexBufferSizeInBytes = sizeof( vert_XYZ_RGB_Nxyz_STU ) * pTheMesh->numberOfVertices;

    glBufferData(GL_ARRAY_BUFFER, 
				 pTheMesh->vertexBufferSizeInBytes,		// sizeInBytes, 
				 pTheMesh->pVertArray,				// p_vertArray,			// Pointer to that array				 
				 GL_DYNAMIC_DRAW );					//GL_STATIC_DRAW);

	return true;
}
