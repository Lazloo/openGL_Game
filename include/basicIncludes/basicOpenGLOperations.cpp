#pragma once
#define GLM_FORCE_RADIANS
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
	// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GL\gl.h>
	// Include GLFW
#include <GLFW\glfw3.h>
	// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
//// Include OpenAL
//#include "Framework.h"
#include "basicIncludes\basicOpenGLOperations.h"

// ----------------------------------------------------
basicOpenGLOperations::basicOpenGLOperations(void)
{
	
}

// ----------------------------------------------------
void basicOpenGLOperations::doInit(void){
	glOrtho(-1,1,-1,1,0.00,100000);
	camPos = glm::vec3(0,0,10);
	//ModelMatrix =  glm::mat4(1.0f);
	setViewMatrix(	camPos, // Camera Position
					glm::vec3(0,3,0), // Camera look at the origin
					glm::vec3(0,1,0) // No inverted camare view
				);
	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ViewField = 45.0f;
	ViewRatio = 4.0f/3.0f;
	ProjectionMatrix = glm::perspective(ViewField,ViewRatio,-1.0f,1.0f);
	
	// Set no inversion of screen (=(0,1,0))
	InversionVec = glm::vec3(0,1,0);

	// ###################### Initialize GLFW #############################//
	if( !glfwInit() ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		std::cout<<"glfwInit() went failed. Output was "<<glfwInit()<<"but expected was 0"<<std::endl;
		system("pause");
	}	

	// Create Window
	WindowMember = createWindowGLFW();
	glfwMakeContextCurrent(WindowMember);
	// Get its properties
	determineWindowProperties(WindowMember);
	
	// ###################### Initialize GLEW #############################//
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		std::cout<<"glewInit() went failed. Output was "<<glewInit()<<"but expected was "<<GLEW_OK<<std::endl;
		system("pause");
	}

	initializeOpenAL();	

}


void basicOpenGLOperations::checkWindowSize(void){

	glfwGetWindowSize(WindowMember,&WindowWidth,&WindowHeight);
	ViewRatio = static_cast<GLfloat>(WindowWidth)/static_cast<GLfloat>(WindowHeight);
	// Get mouse position
	ProjectionMatrix = glm::perspective(ViewField,ViewRatio,0.1f,100.0f);
	//ProjectionMatrix = glm::perspective(45.0f,ViewRatio,0.1f,100.0f);

	////glRasterPos2d rect = view.bounds;     
	//glOrtho(-1.0,                                          // Left
 //             1.0,                                          // Right
 //            -1.0 / (WindowWidth / WindowHeight),   // Bottom
 //             1.0 / (WindowWidth / WindowHeight),   // Top
 //             0.01,                                         // Near
 //             10000.0);                                     // Far
 //   glViewport(0, 0, WindowWidth, WindowHeight);

	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0.0, WindowWidth, 0.0, WindowHeight);
}
// ----------------------------------------------------
void basicOpenGLOperations::doShaderStuff(GLuint programID,GLuint MatrixID,GLuint TextureID,glm::mat4 MVP,GLuint Texture){
		// Use the created Shader
	glUseProgram(programID);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);
}

// ----------------------------------------------------
void basicOpenGLOperations::defineModelAndTexture(GLuint vertexBuffer,GLuint UVBuffer){
	glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		
	// Define second attributes (Texture)
	glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER,UVBuffer);
		glVertexAttribPointer(
			1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
}

glm::mat4 basicOpenGLOperations::createMVP(){
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(ViewField,ViewRatio,0.1f,100.0f);
	//glm::mat4 Projection = glm::perspective(90.0f,16.0f/9.0f,0.1f,1000.0f);
		// Camera matrix
	setViewMatrix(	camPos, // Camera Position
					glm::vec3(0,3,0), // Camera look at the origin
					glm::vec3(0,1,0) // No inverted camare view
		  		 );

		// Model matrix : an identity matrix (model will be at the origin)
	ModelMatrix = glm::mat4(1.0f);
	//ModelTriangle = glm::translate(Model,glm::vec3(1.0f,1.0f,0.0f));

		// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP   = Projection*ViewMatrix*ModelMatrix;
	return MVP;
}

void basicOpenGLOperations::determineWindowProperties(GLFWwindow* window){

	// Get Window information and Move Mouse the desired location
	glfwGetWindowSize(window, &WindowWidth, &WindowHeight);
	glfwSetCursorPos(window, WindowWidth / 2, WindowHeight / 2);
}

GLFWwindow* basicOpenGLOperations::createWindowGLFW(){
	GLFWwindow* window;
	//GLFW_FSAA_SAMPLES
	// ############### Open a window and create its OpenGL context ############################## //
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_VERSION_MINOR, 3); // We want OpenGL 3.3

	//if (glfwGetPrimaryMonitor()==NULL){
		window = glfwCreateWindow( 800, 600, "Level One", NULL, NULL);
	//}
	//else{
	//	window = glfwCreateWindow( 1024, 768, "Level One", glfwGetPrimaryMonitor(), NULL);
	//}
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
	}
	//  ####### Ensure we can capture the escape key being pressed below ###### //
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwMakeContextCurrent(window);
	return(window);
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
GLuint basicOpenGLOperations::loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	// fp = fopen(imagepath, "rb"); 
	fopen_s(&fp,imagepath,"rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); 
		return 0;
	}
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	case FOURCC_DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case FOURCC_DXT3: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case FOURCC_DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		free(buffer); 
		return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 

	free(buffer); 

	return textureID;
}


GLuint basicOpenGLOperations::LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void basicOpenGLOperations::initText2D(const char * texturePath){
	// Initialize texture
	Text2DTextureID = loadDDS(texturePath);

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	// Initialize Shader
	Text2DShaderID = LoadShaders( "TextVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation( Text2DShaderID, "myTextureSampler" );
};

std::size_t basicOpenGLOperations::printText2D(char * text, int x, int y, int size){

	std::size_t length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	float widthFactor = 2.0f;
	//float widthFactor = 2.0f;

	// Generate a single box 
	glm::vec2 vertex_up_left_box = glm::vec2(0,y+size);
	glm::vec2 vertex_down_left_box = glm::vec2(0,y);
	glm::vec2 vertex_up_right_box = glm::vec2(1000,y+size);
	glm::vec2 vertex_down_right_box = glm::vec2(1000,y);

	vertices.push_back(vertex_up_left_box   );
	vertices.push_back(vertex_down_left_box );
	vertices.push_back(vertex_up_right_box  );

	vertices.push_back(vertex_down_right_box);
	vertices.push_back(vertex_up_right_box);
	vertices.push_back(vertex_down_left_box);

	glm::vec2 uv_up_left_box    = glm::vec2(0.0f,0.0f);
	glm::vec2 uv_up_right_box   = glm::vec2(0.0f,0.0f);
	glm::vec2 uv_down_right_box = glm::vec2(0.0f,0.0f);
	glm::vec2 uv_down_left_box  = glm::vec2(0.0f,0.0f);
	UVs.push_back(uv_up_left_box   );
	UVs.push_back(uv_down_left_box );
	UVs.push_back(uv_up_right_box  );

	UVs.push_back(uv_down_right_box);
	UVs.push_back(uv_up_right_box);
	UVs.push_back(uv_down_left_box);

	for ( unsigned int i=0 ; i<length ; i++ ){
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size/widthFactor     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size/widthFactor+size/widthFactor, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size/widthFactor+size/widthFactor, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size/widthFactor     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		//float width = 32.0f;
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16-2.0f)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f/widthFactor, uv_y);
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f/widthFactor, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	return vertices.size();
};

void basicOpenGLOperations::drawText(std::size_t nVertices){
		// Bind shader
	glUseProgram(Text2DShaderID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(nVertices));

}

void basicOpenGLOperations::initializeOpenAL(void){
	// Initialize Framework
	ALFWInit();

	//ALFWprintf("PlayStatic Test Application\n");
	//alcOpenDevice(NULL);
	if (!ALFWInitOpenALUserDefined(0))
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		system("Pause");
	}

	// Generate an AL Buffer
	alGenBuffers( 1, &uiBuffer );
	alGenSources( 1, &uiSource );
};

void basicOpenGLOperations::loadAndPlayWaveFile(const char * path){
	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(path), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(path));
		system("Pause");
	}

	// Generate a Source to playback the Buffer
    /*alGenSources( 1, &uiSource );*/
	//alDeleteBuffers(
	// Attach Source to Buffer
	alSourcei( uiSource, AL_BUFFER, uiBuffer );
	
	ALfloat sourcePos[] = {0,0,0};
	ALfloat sourceOri[] = {0,0,1};
	alSourcefv (uiSource, AL_POSITION, sourcePos);
	alSourcefv (uiSource, AL_VELOCITY, 0);
	alSourcefv (uiSource, AL_DIRECTION, sourceOri);

	// Play Source
    alSourcePlay( uiSource );
};

void basicOpenGLOperations::freeOpenALBuffer(void){
	alSourceStop(uiSource);
	alDeleteSources(1,&uiSource);
	alDeleteBuffers(1,&uiBuffer);
	alGenBuffers( 1, &uiBuffer );
	alGenSources(1,&uiSource);
};