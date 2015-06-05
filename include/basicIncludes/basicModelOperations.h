#ifndef basicModelOperationMacro
#define basicModelOperationMacro


// #############################################



class basicModelOperations
{
public:
	explicit basicModelOperations(void);
	virtual ~basicModelOperations(void){};

	// Do Initialization of the class members
	void doInit(void);

	// Moves model in 2D
	// Input:
	// std::vector<basicModelOperations*> listOfModels ... contains the model which are on the screen
	// int modelIndex ... index of the current mode in listOfModels
	void moveModelWithAnimation(std::vector<basicModelOperations*> listOfModels,int modelIndex);

	// 
	bool checkCollisionWithSprite(basicModelOperations sprite);

	std::vector<bool> checkCollisionWithSpriteTest(basicModelOperations sprite);

	bool checkModelInNearVicinity(basicModelOperations sprite);

	bool checkModelInNearVicinityHorizontal(basicModelOperations sprite);

	//
	bool checkCollisionWithSpriteBelow(basicModelOperations sprite);

	//
	bool checkCollisionWithSpriteTop(basicModelOperations sprite);
	
	// Saves the texture information in a buffer. "textureFileName" contains the path for the DDS-File
	std::vector<GLuint> createBufferTexture(char* textureFileName);
	
	// Saves the vertices of the model in a buffer
	GLuint createVertexBufferModelAndSetSize(void);

	// LOad the shader files and gives out programID, MatrixID, and TextureID
	std::vector<GLuint> createShader(void);

	// Check if sprites collide
	//bool checkCollisionWithSprite(mainCharacter sprite);


// ----------------- Get Functions -------------------
	glm::mat4 getModelMatrix(){
		return ModelMatrix;
	}

	//glm::mat4 getViewMatrix(){
	//	return ViewMatrix;
	//}

	//glm::mat4 getProjectionMatrix(){
	//	return ProjectionMatrix;
	//}

	float getMovementSpeed(void){
		return MovementSpeed;
	}

	double getModelWidth(void){
		return ModelWidth;
	}
	
	double getModelHeight(void){
		return ModelHeight;
	}
	
	GLFWwindow* getCurrentWindow(void){
		return Window;
	}

	glm::mat4 getModelMatrixBackup(void){
		return ModelMatrixBackup;
	}

	std::size_t getImageStatus(void){
		return ImageStatus;
	}

	double getImageTime(void){
		return ImageTime;
	}

	glm::vec2 getReferencePosition(void){
		return ReferencePosition;
	}
	
	glm::vec2 getModelPosition(){
		glm::vec2 modelPosition;
		modelPosition[0] = ModelMatrix[3].x;
		modelPosition[1] = ModelMatrix[3].y;
		return modelPosition;
	}

	GLuint getProgramID(){
		return ProgramID;
	}

	GLuint getMatrixID(){
		return MatrixID;
	}

	GLuint getTextureID(){
		return TextureID;
	}

	GLuint getVertexBuffer(){
		return VertexBuffer;
	}

	std::vector<GLuint> getTextureVec(){
		return TextureVec;
	}

	std::vector<GLuint> getUVBufferVec(){
		return UVBufferVec;
	}

	GLfloat* getG_uv_buffer_data(void){
		return G_uv_buffer_data;
	}

	GLfloat* getG_vertex_buffer_data(void){
		return G_vertex_buffer_data;
	}

	std::vector<std::size_t> getIndicesTextureVec(void){
		return IndicesTextureVec;
	}
	std::vector<unsigned short int> getnTextureVec(void){
		return nTextureVec;
	}
	
	
// ----------------- Set Functions -------------------
	void setCurrentWindow(GLFWwindow* currentWindow){
		Window = currentWindow;
	}

	void setMovementSpeed(float movementSpeed){
		MovementSpeed = movementSpeed;
	}

	void setModelWidth(double modelWidth){
		ModelWidth = modelWidth;
	}

	void setModelHeight(double modelHeight){
		ModelHeight = modelHeight;
	}

	void setVertexData(GLfloat* vertexData){
		for(int iVertex=0;iVertex<18;iVertex++){
			VertexData[iVertex]=vertexData[iVertex];
		}
	}

	void setModelMatrixBackup(glm::mat4 modelMatrixBackup){
		ModelMatrixBackup = modelMatrixBackup;
	}

	void setModelMatrix(glm::mat4 modelMatrix){
		ModelMatrix = modelMatrix;
	}

	void setUVData(GLfloat* uvData){
		std::ofstream feedbackTextFile;
		feedbackTextFile.open ("feedback.txt",std::ios::app);
		for(int iVertex=0;iVertex<12;iVertex++){
			UVData[iVertex]=uvData[iVertex];
			if((iVertex+1)%2){feedbackTextFile<<std::endl;}
			feedbackTextFile<<UVData[iVertex]<<"("<<uvData[iVertex]<<")\t";
		}
		feedbackTextFile<<std::endl<<"stop"<<std::endl;
		feedbackTextFile.close();
	}

	void setImageStatus(std::size_t imageStatus){
		ImageStatus = imageStatus;
	}

	void setImageTime(double imageTime){
		ImageTime = imageTime;
	}

	void setReferencePosition(glm::vec2 referencePosition){
		ReferencePosition = referencePosition;
	}

	// Input newModelPosition with newModelPosition[0]=x and newModelPosition[1]=y
	// Adjusts automatically ReferencePosition
	void setModelPosition(glm::vec2 newModelPosition){
		ModelMatrix[3].x = newModelPosition.x;
		ModelMatrix[3].y = newModelPosition.y;
	}
	
	// Input newModelPosition with newModelPosition[0]=x and newModelPosition[1]=y
	// Adjusts automatically ReferencePosition
	void setModelPositionAndReferencePoint(glm::vec2 newModelPosition){
		ModelMatrix[3].x = newModelPosition.x;
		ModelMatrix[3].y = newModelPosition.y;
		ReferencePosition[0] = newModelPosition[0];
		ReferencePosition[1] = newModelPosition[1];
	}

	void setProgramID(GLuint programID){
		ProgramID = programID;
	}

	void setMatrixID(GLuint matrixID){
		MatrixID = matrixID;
	}

	void setTextureID(GLuint textureID){
		TextureID = textureID;
	}

	void setVertexBuffer(GLuint vertexBuffer){
		VertexBuffer = vertexBuffer;
	}

	void setTextureVec(std::vector<GLuint> textureVec){
		TextureVec = textureVec;
	}

	void setUVBufferVec(std::vector<GLuint> uvBufferVec){
		UVBufferVec = uvBufferVec;
	}

	void setG_uv_buffer_data(GLfloat g_uv_buffer_data[]){
		for(int iVertex=0;iVertex<12;iVertex++){
			G_uv_buffer_data[iVertex] = g_uv_buffer_data[iVertex];
		}
	}

	void setG_vertex_buffer_data(GLfloat g_vertex_buffer_data[]){
		for(int iVertex=0;iVertex<18;iVertex++){
			G_vertex_buffer_data[iVertex] = g_vertex_buffer_data[iVertex];
		}
	}

	void setIndicesTextureVec(std::vector<std::size_t> indicesTextureVec){
		for(int iVec = 0;iVec<IndicesTextureVec.size();iVec++){
			IndicesTextureVec[iVec]=indicesTextureVec[iVec];
		}
	};

	void setnTextureVec(std::vector<unsigned short int> nTextureVecNew){
		for(int iVec = 0;iVec<nTextureVec.size();iVec++){
			nTextureVec[iVec]=nTextureVecNew[iVec];
		}
	};

// Members
private:
	// 
	glm::mat4 ModelMatrix;
	glm::mat4 ModelMatrixBackup;
	//glm::mat4 ViewMatrix;
	//glm::mat4 ProjectionMatrix;
	
	
	// Size of the reactangle of the model
	double ModelWidth;
	double ModelHeight;
	//double ModelPosition[3];


	//std::vector<GLfloat> VertexData;
	//std::vector<GLfloat> UVData;
	GLfloat VertexData[18];
	GLfloat UVData[12];

	// Initialize the translation matrix (Used in MoveModel)
	glm::mat4 TranslateMatrix;

	// Current considered window
	GLFWwindow* Window;

	// Save the index current used texture
	std::size_t ImageStatus;
	
	// Saves how much time spent since the last movement
	double ImageTime;
	
	// Controls the moving speed of the model 
	float MovementSpeed;

	// This function is used in all method where a reference position is needed such as in moveModelHorizontal
	// Initial Position if not change is [x,y]=[0,0]
	glm::vec2 ReferencePosition;

	GLuint ProgramID;
	GLuint MatrixID;
	GLuint TextureID;
	GLuint VertexBuffer;

	//double ModelWidth;
	//double ModelHeight;

	std::vector<GLuint> TextureVec;
	std::vector<GLuint> UVBufferVec;

	// Contains the vertices of the model
	GLfloat G_vertex_buffer_data[18];
	// Contains the texture information of the model
	GLfloat G_uv_buffer_data[12];

	// Save the indices in TextureVec where texture for the right/left/up/downwards movement starts
	std::vector<std::size_t> IndicesTextureVec;
	// Save information of how many images are needed for the right/left/up/downwards movement 
	std::vector<unsigned short int> nTextureVec;
};

// #############################################
#endif // !basicModelOperationMacro