#pragma once
#define GLM_FORCE_RADIANS
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
	// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GL\gl.h>
	// Include GLFW
#include <GLFW\glfw3.h>
	// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
//#ifndef MODELDEF
//#define MODELDEF
//	//#include "Model.h"
//#endif
#include "monster1.h"


monster1::monster1(void)
{
	// Stuff which has always to be done regards less what happens after
	monster1::doInit();
	GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,1.0f,	-1.0f,1.0f,1.0f,	0.6f,1.0f,1.0f, // Front = 1
		-1.0f,-1.0f,1.0f,	0.6f,-1.0f,1.0f,	0.6f,1.0f,1.0f,
	};

	GLfloat g_uv_buffer_data[] = {
		0.0f, 1.0f-0.0f,		0.0f, 1.0f-1.0f,		1.0f, 1.0-1.0f, // 1
		0.0f, 1.0f-0.0f,		1.0f, 1.0f-0.0f,		1.0f, 1.0-1.0f,
	};


	std::vector<std::size_t> indicesTextureVec(4,0);
	indicesTextureVec[0] = 0;
	indicesTextureVec[1] = 3;
	indicesTextureVec[2] = 6;
	indicesTextureVec[3] = 9;
	setIndicesTextureVec(indicesTextureVec);

	// number of existing images
	std::vector<unsigned short int> nTextureVecNew(4,0);
	nTextureVecNew[0] = 3;
	nTextureVecNew[1] = 3;
	nTextureVecNew[2] = 3;
	nTextureVecNew[3] = 3;
	setnTextureVec(nTextureVecNew);

	setG_vertex_buffer_data(g_vertex_buffer_data);
	setG_uv_buffer_data(g_uv_buffer_data);
	// CharacterSpeed
	setMovementSpeed(2.0f); // 3 units / second

	// Create Shader and save information
	std::vector<GLuint> outputVector;
	outputVector = createShader();
	setProgramID(outputVector[0]);
	setMatrixID(outputVector[1]);
	setTextureID(outputVector[2]);

	setVertexData(getG_vertex_buffer_data());
	setUVData(getG_uv_buffer_data());

	// Save vertices of the object
	setVertexBuffer(createVertexBufferModelAndSetSize());

	// Save the texture information
	std::vector<GLuint> tempTextureVec = getTextureVec();
	std::vector<GLuint> tempUVBufferVec = getUVBufferVec();
	tempTextureVec.resize(12);
	tempUVBufferVec.resize(12);
	

	//// Right
	outputVector = createBufferTexture("Images/monster1/Rechts/Rechts_Stehen.DDS");
	tempTextureVec[0] = outputVector[0];
	tempUVBufferVec[0] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Rechts/Rechts_Laufen_1.DDS");
	tempTextureVec[1] = outputVector[0];
	tempUVBufferVec[1] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Rechts/Rechts_Laufen_2.DDS");
	tempTextureVec[2] = outputVector[0];
	tempUVBufferVec[2] = outputVector[1];

	//// Left
	outputVector = createBufferTexture("Images/monster1/Links/Links_Stehen.DDS");
	tempTextureVec[3] = outputVector[0];
	tempUVBufferVec[3] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Links/Links_Laufen_1.DDS");
	tempTextureVec[4] = outputVector[0];
	tempUVBufferVec[4] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Links/Links_Laufen_2.DDS");
	tempTextureVec[5] = outputVector[0];
	tempUVBufferVec[5] = outputVector[1];

	//// Up
	outputVector = createBufferTexture("Images/monster1/Hoch/Hoch_Stehen.DDS");
	tempTextureVec[6] = outputVector[0];
	tempUVBufferVec[6] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Hoch/Hoch_Laufen_1.DDS");
	tempTextureVec[7] = outputVector[0];
	tempUVBufferVec[7] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Hoch/Hoch_Laufen_2.DDS");
	tempTextureVec[8] = outputVector[0];
	tempUVBufferVec[8] = outputVector[1];

	//// Down
	outputVector = createBufferTexture("Images/monster1/Runter/Runter_Stehen.DDS");
	tempTextureVec[9] = outputVector[0];
	tempUVBufferVec[9] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Runter/Runter_Laufen_1.DDS");
	tempTextureVec[10] = outputVector[0];
	tempUVBufferVec[10] = outputVector[1];
	outputVector = createBufferTexture("Images/monster1/Runter/Runter_Laufen_2.DDS");
	tempTextureVec[11] = outputVector[0];
	tempUVBufferVec[11] = outputVector[1];

	setTextureVec(tempTextureVec);
	setUVBufferVec(tempUVBufferVec);

	
	//_CrtDumpMemoryLeaks();
	//std::cout<<"Initialization Complete"<<std::endl;
}


monster1::~monster1(void)
{
}

