#pragma once
#include "basicIncludes\basicModelOperations.h"
class tree1:public basicModelOperations
{
public:
	tree1(void);
	~tree1(void);

// General Functions
	// Check if sprites collide
	//bool checkCollisionWithSprite(glm::mat4 spriteModelMatrix, double spriteModelHeight, double spriteModelWidth);
	//bool checkCollisionWithSprite(basicModelOperations sprite);

// Get Functions
	//GLuint getProgramID(){
	//	return ProgramID;
	//}

	//GLuint getMatrixID(){
	//	return MatrixID;
	//}

	//GLuint getTextureID(){
	//	return TextureID;
	//}

	//GLuint getVertexBuffer(){
	//	return VertexBuffer;
	//}

	//std::vector<GLuint> getTextureVec(){
	//	return TextureVec;
	//}

	//std::vector<GLuint> getUVBufferVec(){
	//	return UVBufferVec;
	//}
private:
	//GLuint ProgramID;
	//GLuint MatrixID;
	//GLuint TextureID;
	//GLuint VertexBuffer;

	double ModelWidth;
	double ModelHeight;

	//std::vector<GLuint> TextureVec;
	//std::vector<GLuint> UVBufferVec;

	//// Contains the vertices of the model
	//GLfloat g_vertex_buffer_data[18];
	//// Contains the texture information of the model
	//GLfloat g_uv_buffer_data[12];

};

