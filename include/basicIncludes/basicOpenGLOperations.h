//#include <common/Shader.cpp>
// Include OpenAL 
#include "Framework.h"
using namespace glm;

class basicOpenGLOperations
{
public:
	explicit basicOpenGLOperations(void);
	virtual ~basicOpenGLOperations(void){ 
		ALFWShutdown();    
	 };

	// Do Initialization of the class members
	void doInit(void);

	// Does all necessary stuff in order to use the shader to display your model texture
	void doShaderStuff(GLuint programID,GLuint MatrixID,GLuint TextureID,glm::mat4 MVP,GLuint Texture);

	// Provides the vertex buffer and the UV buffer to the shader
	void defineModelAndTexture(GLuint vertexBuffer,GLuint UVBuffer);

	glm::mat4 createMVP();
	
	
	// 



	// Check if window was resized
	void checkWindowSize(void);

	// Creates a window 
	GLFWwindow* createWindowGLFW();

	// Does the initialization of GLEW. It take a window object in order to saves its properties "WindowWidth" and "WindowHeight"
	void determineWindowProperties(GLFWwindow* window);

	// Load an DDS texture, output TextureID
	GLuint loadDDS(const char * imagepath);

	GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

	void initText2D(const char * texturePath);

	std::size_t printText2D(char * text, int x, int y, int size);

	void drawText(std::size_t nVertices);

	void initializeOpenAL(void);

	void loadAndPlayWaveFile(const char * path);

	void freeOpenALBuffer(void);
// ----------------- Get Functions -------------------
	glm::mat4 getModelMatrix(){
	return ModelMatrix;
	}
	glm::mat4 getViewMatrix(){
		return ViewMatrix;
	}
	glm::mat4 getProjectionMatrix(){
		return ProjectionMatrix;
	}
	GLFWwindow* getWindowMember(){
		return WindowMember;
	}

	GLfloat getViewField(){
		return ViewField;
	}

	GLfloat getViewRatio(){
		return ViewRatio;
	}
	
	glm::vec3 getInversionVec(){
		return InversionVec;
	}
// ----------------- Set Functions -------------------
	void setViewField(GLfloat viewField){
		ViewField = viewField;
	}

	void setTest(int test){
		Test = test;
	}

	void setViewRatio(GLfloat viewRatio){
		ViewRatio = viewRatio;
	}

	// Reset ViewMatrix 
	// cameraPosition ... contains the coordinates of the camera in the space
	// cameraLookAt ... contains the point in the space where camera is looking at
	// inversionVec ... decide if the camera should be inverted: 
	//                  - (0,1,0) -> no
	//                  - (0,-1,0) -> yes
	void setViewMatrix(glm::vec3 cameraPosition,glm::vec3 cameraLookAt,glm::vec3 inversionVec){
		ViewMatrix  =	glm::lookAt(
						cameraPosition, // Camera Position
						cameraLookAt, // Camera look at the origin
						inversionVec // No inverted camare view
					);	
	};

	void setInversionVec(glm::vec3 inversionVec){
		InversionVec=inversionVec;
	}
	
private:
	// 
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 InversionVec;
	int WindowWidth,WindowHeight;
	GLFWwindow* WindowMember;
	GLfloat ViewField;
	GLfloat ViewRatio;
	int Test;

	// Save texture of the letters
	unsigned int Text2DTextureID;
	// Save vertices of the letters
	unsigned int Text2DVertexBufferID;
	// Save texture of the letters
	unsigned int Text2DUVBufferID;
	// Save shader info of the letters
	unsigned int Text2DShaderID;
	// Save texture of the letters
	unsigned int Text2DUniformID;

	// The ID of the OenAL buffer
	ALuint  uiBuffer;
	// The ID of the OpenAL source 
	ALuint  uiSource;  

	glm::vec3 camPos;
};

