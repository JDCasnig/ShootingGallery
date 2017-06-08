#include "cMeshTypeManager.h"

#include <glad/glad.h>			// OpenGL defines...
#include <glm/glm.hpp>

#include "vert_XYZ_RGB_Nxyz_STU.h"		// Now with texture coordinates!!


bool cMeshTypeManager::CreateDebugLineBuffer( unsigned int maxNumberOfVertices, 
											  GLuint shaderProgramID )
{
	// Create a VAO of this particular size
	// Fill it with zeros...

	this->m_p_vertArrayLines = new vert_XYZ_RGB_Nxyz_STU[maxNumberOfVertices];

	this->m_sizeOfDebugLineArrayBufferInBytes = sizeof(vert_XYZ_RGB_Nxyz_STU) * maxNumberOfVertices;

	// Or you could do a loop, or some sexy C++ library thingy...
	memset(this->m_p_vertArrayLines, 0, this->m_sizeOfDebugLineArrayBufferInBytes); 

	this->m_nextLineIndexStart = 0;

	glGenVertexArrays(1, &m_line_VAO_ID );	
	glBindVertexArray( m_line_VAO_ID );

    glGenBuffers(1, &m_line_vert_buffer_ID );  
    glBindBuffer(GL_ARRAY_BUFFER, m_line_vert_buffer_ID);

	glBufferData(GL_ARRAY_BUFFER, 
				 this->m_sizeOfDebugLineArrayBufferInBytes,	
				 this->m_p_vertArrayLines,				
				 GL_DYNAMIC_DRAW );				


	// Bind the vertex attributes to the shader
	int sizeOfVertInBytes = sizeof( vert_XYZ_RGB_Nxyz_STU );

	// Telling the shader where the vertex info is...
	// It's "layout"
	// Note that we need the shader program ID to store the connection 
	//	between the vertex buffers and the shader program attributes
    GLuint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	// 12 byes
    GLuint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");	// 12 bytes, starts at byte 12
    GLuint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");	// 12 bytes, starts at byte 24
    GLuint vstu_location = glGetAttribLocation(shaderProgramID, "vTexSTU");	// 12 bytes, starts at byte 36

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


	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


	return true;
}


void cMeshTypeManager::AddDebugLine( glm::vec3 start, glm::vec3 end, 
									 glm::vec3 colour )
{
	// Add two vertices at the locations I've asked for

	// V1 = this->m_nextLineIndexStart 
	this->m_p_vertArrayLines[this->m_nextLineIndexStart].x = start.x;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart].y = start.y;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart].z = start.z;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart].r = colour.r;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart].g = colour.g;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart].b = colour.b;

	// V2 = this->m_nextLineIndexStart + 1 
	this->m_p_vertArrayLines[this->m_nextLineIndexStart + 1].x = end.x;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart + 1].y = end.y;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart + 1].z = end.z;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart + 1].r = colour.r;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart + 1].g = colour.g;
	this->m_p_vertArrayLines[this->m_nextLineIndexStart + 1].b = colour.b;

	// Set "next" index to +2
	this->m_nextLineIndexStart += 2;

	return;
}

// "Clears" the debug lines
void cMeshTypeManager::ClearBuffer(void)
{
	// Don't actually DELETE the buffer, just point to the start again
	this->m_nextLineIndexStart = 0;
	return;
}

// Called by render
bool cMeshTypeManager::GetDebugLineBufferForRender( unsigned int &VAO_ID,	    // GLuint by ref
						                            unsigned int &numberOfVertices )
{
	// Copy the latest CPU side buffer to the GPU
	if ( this->m_nextLineIndexStart > 0 )
	{	// update buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_line_vert_buffer_ID);

		glBufferData(GL_ARRAY_BUFFER, 
					 this->m_sizeOfDebugLineArrayBufferInBytes,	
					 this->m_p_vertArrayLines,				
					 GL_DYNAMIC_DRAW );		
	}


	VAO_ID = this->m_line_VAO_ID;
	numberOfVertices = this->m_nextLineIndexStart;

	return true;
}
