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
#include <common/texture.cpp>
#include <common/Shader.cpp>
#include "basicIncludes\basicModelOperations.h"
//#include "Framework.h"

basicModelOperations::basicModelOperations(void)
{
}

// --------------------------------------------------
void basicModelOperations::doInit(void){

	// Indices for the TextureVec
	IndicesTextureVec.resize(4,0);
	IndicesTextureVec[0] = 0;
	IndicesTextureVec[1] = 3;
	IndicesTextureVec[2] = 6;
	IndicesTextureVec[3] = 9;

	// number of existing images
	nTextureVec.resize(4,0);
	nTextureVec[0] = 3;
	nTextureVec[1] = 3;
	nTextureVec[2] = 2;
	nTextureVec[3] = 2;

	ModelMatrix =  glm::mat4(1.0f);
	//ViewMatrix  =  glm::lookAt(
	//						glm::vec3(0,0,10), // Camera Position
	//						glm::vec3(0,0,0), // Camera look at the origin
	//						glm::vec3(0,1,0) // No inverted camare view
	//					);
	//ProjectionMatrix = glm::perspective(45.0f,4.0f/3.0f,0.1f,100.0f);
	ImageStatus = 0;
	ImageTime = 0;

	// Initial Position if not change is [x,y]=[0,0]
	ReferencePosition[0]=0;
	ReferencePosition[1]=0;

}
bool basicModelOperations::checkCollisionWithSprite(basicModelOperations sprite){
	bool xCoordinate = abs(getModelMatrix()[3].x - sprite.getModelMatrix()[3].x)<(getModelWidth()/2+sprite.getModelWidth()/2);
	bool yCoordinate = abs(getModelMatrix()[3].y - sprite.getModelMatrix()[3].y)<(getModelHeight()/2 + sprite.getModelHeight()/2);

	return xCoordinate&&yCoordinate;
	//MatrixID
}

std::vector<bool> basicModelOperations::checkCollisionWithSpriteTest(basicModelOperations sprite){
	bool xCoordinate = abs(getModelMatrix()[3].x - sprite.getModelMatrix()[3].x)<(getModelWidth()/2+sprite.getModelWidth()/2);
	bool yCoordinate = abs(getModelMatrix()[3].y - sprite.getModelMatrix()[3].y)<(getModelHeight()/2 + sprite.getModelHeight()/2);
	vector<bool>checkcollisionVec(2,false);
	checkcollisionVec[0] = xCoordinate;
	checkcollisionVec[1] = yCoordinate;

	return checkcollisionVec;
	//MatrixID
}

bool basicModelOperations::checkModelInNearVicinity(basicModelOperations sprite){
	bool xCoordinate = abs(getModelMatrix()[3].x - sprite.getModelMatrix()[3].x)<(getModelWidth()/2+sprite.getModelWidth()/2)*1.1;
	bool yCoordinate = abs(getModelMatrix()[3].y - sprite.getModelMatrix()[3].y)<(getModelHeight()/2 + sprite.getModelHeight()/2)*1.1;

	return xCoordinate&&yCoordinate;
	//MatrixID
}

bool basicModelOperations::checkModelInNearVicinityHorizontal(basicModelOperations sprite){
	bool xCoordinate = abs(getModelMatrix()[3].x - sprite.getModelMatrix()[3].x)<(getModelWidth()/2+sprite.getModelWidth()/2)*1.1;
	//bool xCoordinate = abs(getModelMatrix()[3].x - sprite.getModelMatrix()[3].x)<(getModelWidth()/2+sprite.getModelWidth()/2)*1.1;
	bool yCoordinate = abs(getModelMatrix()[3].y - sprite.getModelMatrix()[3].y)<(getModelHeight()/2 + sprite.getModelHeight()/2);

	return xCoordinate&&yCoordinate;
	//MatrixID
}

bool basicModelOperations::checkCollisionWithSpriteBelow(basicModelOperations sprite){
	bool xCoordinate = abs(getModelMatrix()[3].x - sprite.getModelMatrix()[3].x)<(getModelWidth()/2+sprite.getModelWidth()/2);
	bool yCoordinate1 = (getModelMatrix()[3].y-sprite.getModelMatrix()[3].y)<(getModelHeight()/2 + sprite.getModelHeight()/2)*1.1;
	bool yCoordinate2 = (getModelMatrix()[3].y-sprite.getModelMatrix()[3].y)>(getModelHeight()/2 + sprite.getModelHeight()/2)*0.8;

	return xCoordinate&&yCoordinate1&&yCoordinate2;
	//MatrixID
}

bool basicModelOperations::checkCollisionWithSpriteTop(basicModelOperations sprite){
	bool xCoordinate = abs(getModelMatrix()[3].x - sprite.getModelMatrix()[3].x)<(getModelWidth()/2+sprite.getModelWidth()/2);
	bool yCoordinate = abs(sprite.getModelMatrix()[3].y - getModelMatrix()[3].y)<(getModelHeight()/2 + sprite.getModelHeight()/2);
	//std::cout<<"(sprite.getModelMatrix()[3].y - getModelMatrix()[3].y): "<<(sprite.getModelMatrix()[3].y - getModelMatrix()[3].y)<<std::endl;
	return xCoordinate&&yCoordinate;
	//MatrixID
}

std::vector<GLuint> basicModelOperations::createBufferTexture(char* textureFileName)
{
		// Load the texture using any two methods
	GLuint Texture = loadDDS(textureFileName);
	//GLuint Texture = ilLoadImage("Runter_Stehen_Bogen.png"); /* Loading of image "image.jpg" */
		// Create UV Buffer
	GLuint UVBuffer;
	glGenBuffers(1,&UVBuffer);
		// Make color buffer the current considered on
	glBindBuffer(GL_ARRAY_BUFFER,UVBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(UVData),UVData,GL_STATIC_DRAW);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	//std::cout<<"Number of Triangles: "<<nTriangles<<std::endl;
	std::vector<GLuint> outputVec(2);
	outputVec[0] = Texture;
	outputVec[1] = UVBuffer;
	return outputVec;
}

GLuint basicModelOperations::createVertexBufferModelAndSetSize(){
	// Model is actual defined in "Model.h"
		 //This will identify our vertexBuffer
	GLuint vertexBuffer;
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1,&vertexBuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
		// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexData),VertexData,GL_STATIC_DRAW);

	ModelWidth = 0;
	for (int iNodes = 0; iNodes < 17; iNodes = iNodes + 3){
		for (int iNodes2 = 0; iNodes2 < 17; iNodes2 = iNodes2 + 3){
			if (abs(VertexData[iNodes] - VertexData[iNodes2])>ModelWidth){
				ModelWidth = abs(VertexData[iNodes] - VertexData[iNodes2]);
			}
		}
	}
	//std::cout<<"sizeof(VertexData)"<<sizeof(VertexData)/4<<std::endl;

	ModelHeight = 0;
	for (int iNodes = 1; iNodes < 17; iNodes = iNodes + 3){
		for (int iNodes2 = 1; iNodes2 < 17 / 4; iNodes2 = iNodes2 + 3){
			if (abs(VertexData[iNodes] - VertexData[iNodes2])>ModelHeight){
				ModelHeight = abs(VertexData[iNodes] - VertexData[iNodes2]);
			}
		}
	}
	//std::ofstream feedbackTextFile;
	//feedbackTextFile.open ("feedback.txt",std::ios::app);
	//for (int iNodes = 0; iNodes < sizeof(VertexData); iNodes = iNodes++){
	//	//feedbackTextFile <<VertexData[iNodes]<<"\t";
	//	std::cout<<VertexData[iNodes]<<"\t";
	//	//if((iNodes+1)%3==0){feedbackTextFile << std::endl;}
	//}
	//feedbackTextFile << std::endl;
	//std::cout<< std::endl;
	//feedbackTextFile.close();

	//std::cout<<"After ModelHeight"<<ModelHeight<<"-Nodea: "<<std::endl;
	//std::cout<<"After ModelWidth"<<ModelWidth<<"-Nodea: "<<std::endl;
		// Output
	return vertexBuffer;
}

std::vector<GLuint> basicModelOperations::createShader(){
		// Create and compile our GLSL program from the 
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
		// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	GLuint programID2 = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );
		// Get a handle for our "MVP" uniform
	GLuint MatrixID2 = glGetUniformLocation(programID, "MVP");
		// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID2  = glGetUniformLocation(programID, "myTextureSampler");
		// Save the IDs
	std::vector<GLuint> output(4);
	output[0]=programID;
	output[1]=MatrixID;
	output[2]=TextureID;
	return output;
}

