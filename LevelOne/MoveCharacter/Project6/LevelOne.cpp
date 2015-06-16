#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <vector>
#include <stdio.h>
	// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GL\gl.h>
	// Include GLFW
#include <GLFW\glfw3.h>
	// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
	// Include GLM
//#include <common/Shader.cpp>
//#include "common\text2D.hpp"
//#include <common/texture.cpp>
//#include <common/Shader.cpp>
//#include "basicIncludes\basicModelOperations.h"
//#include "basicIncludes\basicOpenGLOperations.h"
#include "LevelOne.h"
//#include <common/texture.cpp>

#define LEVEL_TEST 1
#define LEVEL_TEST_TWO 2

LevelOne::LevelOne(void)
{
	//ListOfObjectsReferences.resize(0);
	//OpenGLObject.createMVP();

	//// Set Options
	////// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);
	////// Enable blending
	//glEnable(GL_BLEND);
	////// Transparency
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	////// Background Color
	//glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
}


LevelOne::~LevelOne(void)
{
	for (int i = 0; i < ListOfObjectsReferences.size(); i++){
		ListOfObjectsReferences[i].~basicModelOperations();
		//levelVec[i].era;
	}
	ListOfObjectsReferences.~ListOfObjectsReferences();
	OpenGLObject.~basicOpenGLOperations();

	OpenGLObject.freeOpenALBuffer();
}

void LevelOne::doInit(void){
	OpenGLObject.doInit();
	ListOfObjectsReferences.resize(0);
	PropertyVector.resize(0);
	OpenGLObject.createMVP();

	// Set Options
	//// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//// Enable blending
	glEnable(GL_BLEND);
	//// Transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//// Background Color
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	
	PrintTextFlag = false;
	ResetPosition[0]=0;
	ResetPosition[1]=-4;
	TextLine1 = "Test";
	TextLine2 = "Test Test";
	stateKeyEnter = GLFW_RELEASE;
	OpenGLObject.initText2D("Images/ExportedFont.dds");

	NextLevel = LEVEL_TEST;
	LevelWidth = 0.0;
	LevelHeight = 0.0;
}
void LevelOne::addTrees(void){

	int nTreesHorizontal = 20;
	int nTreesVertical = 11;
	for(int iTrees=0;iTrees<nTreesHorizontal+2*nTreesVertical;iTrees++){
		ListOfObjectsReferences.push_back(tree1());
		ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setCurrentWindow(OpenGLObject.getWindowMember());
		PropertyVector.push_back(std::vector<double>(5,0));
		// Is neutral
		PropertyVector[ListOfObjectsReferences.size()-1][3]=0;
		PropertyVector[ListOfObjectsReferences.size()-1][4]=0;
	}

	for(int iTrees=0;iTrees<nTreesHorizontal;iTrees++){
		ListOfObjectsReferences[iTrees].setModelPositionAndReferencePoint(glm::vec2(-5.25+iTrees,-5));
	}

	for(int iTrees=nTreesHorizontal;iTrees<nTreesHorizontal+nTreesVertical;iTrees++){
		ListOfObjectsReferences[iTrees].setModelPositionAndReferencePoint(glm::vec2(-6.25,-5+(iTrees-nTreesHorizontal)*1.5));
	}

	for(int iTrees=nTreesHorizontal+nTreesVertical;iTrees<nTreesHorizontal+2*nTreesVertical;iTrees++){
		ListOfObjectsReferences[iTrees].setModelPositionAndReferencePoint(glm::vec2(6.25,-5+(iTrees-nTreesHorizontal-nTreesVertical)*1.5));
		std::cout<<"Position: "<<ListOfObjectsReferences[iTrees].getModelPosition()[0]<<"-"<<ListOfObjectsReferences[iTrees].getModelPosition()[1]<<std::endl;
	}

	// Add line 
	for(int iTrees=0;iTrees<3;iTrees++){
		ListOfObjectsReferences.push_back(tree1());
		ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(
			glm::vec2(5-iTrees,-0.5));
		PropertyVector.push_back(std::vector<double>(5,0));
		// Is neutral
		PropertyVector[ListOfObjectsReferences.size()-1][3]=0;
		PropertyVector[ListOfObjectsReferences.size()-1][4]=0;
	}

	// Add line 
	for(int iTrees=0;iTrees<3;iTrees++){
		ListOfObjectsReferences.push_back(tree1());
		ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(
			glm::vec2(-2-iTrees,1.75));
		PropertyVector.push_back(std::vector<double>(5,0));
		// Is neutral
		PropertyVector[ListOfObjectsReferences.size()-1][3]=0;
		PropertyVector[ListOfObjectsReferences.size()-1][4]=0;
	}

};

void LevelOne::addMovingModels(void){
	ListOfObjectsReferences.push_back(mainCharacter());
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(glm::vec2(3,10));
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setCurrentWindow(OpenGLObject.getWindowMember());
	IndexMainCharacter = ListOfObjectsReferences.size()-1;
	PropertyVector.push_back(std::vector<double>(5,0));
	// Allow jumpNrun
	PropertyVector[ListOfObjectsReferences.size()-1][0]=2;
	// Use Gravity
	PropertyVector[ListOfObjectsReferences.size()-1][2]=1;
	// is friend
	PropertyVector[ListOfObjectsReferences.size()-1][3]=2;
	// No Event
	PropertyVector[ListOfObjectsReferences.size()-1][4]=0;
	// Position if hit by the enemy
	ResetPosition[0]=3;
	ResetPosition[1]=-4;

	// Add Monster
	ListOfObjectsReferences.push_back(monster1());
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(glm::vec2(-3,10));
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setCurrentWindow(OpenGLObject.getWindowMember());
	PropertyVector.push_back(std::vector<double>(5,1));
	// Distance of movement
	PropertyVector[ListOfObjectsReferences.size()-1][1]=1;
	// is enemy
	PropertyVector[ListOfObjectsReferences.size()-1][3]=1;
	// No Event
	PropertyVector[ListOfObjectsReferences.size()-1][4]=0;

	// Add Cat girl
	ListOfObjectsReferences.push_back(catGirl());
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(glm::vec2(0,10));
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setCurrentWindow(OpenGLObject.getWindowMember());
	PropertyVector.push_back(std::vector<double>(5,1));
	// Distance of movement
	PropertyVector[ListOfObjectsReferences.size()-1][1]=1;
	// is friend
	PropertyVector[ListOfObjectsReferences.size()-1][3]=2;
	// Write Text
	PropertyVector[ListOfObjectsReferences.size()-1][4]=1;
};


void LevelOne::addModelFromFile(std::string fileName){

	std::ifstream myFile(fileName);
	std::string line;
	std::size_t modelType;
	std::vector<double> modelPos(2,0);
	std::size_t  movementType=0;
	double  movementDetails=0;
	std::size_t  gravity=0;
	std::size_t  relationToMainChara=0;
	std::size_t  eventIndex=0;

	// First Line contains the size of the level
	std::getline(myFile, line);
	std::istringstream issFirst(line);
	issFirst >> LevelWidth;
	issFirst >> LevelHeight;
	//std::cout << "LevelWidth: " << LevelWidth << "\tLevelHeight: " << LevelHeight << std::endl;

	while (std::getline(myFile, line))
    {
        std::istringstream iss(line);
		bool testBool=false;
		testBool = testBool||!(iss >> modelType);
		testBool = testBool||!(iss >> modelPos[0]);
		testBool = testBool||!(iss >> modelPos[1]);
		testBool = testBool||!(iss >> movementType);
		testBool = testBool||!(iss >> movementDetails);
		testBool = testBool||!(iss >> gravity);
		testBool = testBool||!(iss >> relationToMainChara);
		testBool = testBool||!(iss >> eventIndex);
		//std::cout<<"modelType: "<<modelType<<"\tmovementType: "<<movementType<<std::endl;
		//std::cout<<"movementDetails: "<<movementDetails<<"\tgravity: "<<gravity<<std::endl;
		//std::cout<<"relationToMainChara: "<<relationToMainChara<<"\teventIndex: "<<eventIndex<<std::endl;
		//std::cout<<"x: "<<modelPos[0]<<"\ty: "<<modelPos[1]<<std::endl;
        //if (!(iss >> modelType >> modelPos[0]>>modelPos[1]>>movementType>>movementDetails>>gravity>>relationToMainChara>>eventIndex)) {
		if(testBool){
			std::cout<<line<<std::endl;
            std::cout<<"break"<<std::endl;
            break;
        } // error
		

		switch (modelType)
		{
		case 0:
			ListOfObjectsReferences.push_back(mainCharacter());
			break;
		case 1:
			ListOfObjectsReferences.push_back(catGirl());
			break;
		case 2:
			ListOfObjectsReferences.push_back(monster1());
			break;
		case 3:
			ListOfObjectsReferences.push_back(tree1());
			break;
		default:
			break;
		}

		ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(glm::vec2(modelPos[0]/55,-modelPos[1]/55));
		ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setCurrentWindow(OpenGLObject.getWindowMember());
		PropertyVector.push_back(std::vector<double>(6,0));
		// Allow jumpNrun
		PropertyVector[ListOfObjectsReferences.size()-1][0]=static_cast<double>(movementType);
		// Allow jumpNrun
		PropertyVector[ListOfObjectsReferences.size()-1][1]=movementDetails;
		// Use Gravity
		PropertyVector[ListOfObjectsReferences.size()-1][2]=static_cast<double>(gravity);
		// is friend
		PropertyVector[ListOfObjectsReferences.size()-1][3]=static_cast<double>(relationToMainChara);
		// Event if Enter is pressed
		PropertyVector[ListOfObjectsReferences.size()-1][4]=static_cast<double>(eventIndex);
		// Event as soon as collision happens
		PropertyVector[ListOfObjectsReferences.size() - 1][5] = static_cast<double>(eventIndex);
    }


	// Put in the Main Character
	ListOfObjectsReferences.push_back(mainCharacter());
	//ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(glm::vec2(3,10));
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setModelPositionAndReferencePoint(glm::vec2(486.5/55,-2678/55));
	ListOfObjectsReferences[ListOfObjectsReferences.size()-1].setCurrentWindow(OpenGLObject.getWindowMember());
	IndexMainCharacter = ListOfObjectsReferences.size()-1;
	PropertyVector.push_back(std::vector<double>(5,0));
	// Allow jumpNrun
	PropertyVector[ListOfObjectsReferences.size()-1][0]=2;
	// Use Gravity
	PropertyVector[ListOfObjectsReferences.size()-1][2]=1;
	// is friend
	PropertyVector[ListOfObjectsReferences.size()-1][3]=2;
	// No Event
	PropertyVector[ListOfObjectsReferences.size()-1][4]=0;

	IndexMainCharacter = ListOfObjectsReferences.size()-1;
	// Position if hit by the enemy
	ResetPosition[0]=3;
	ResetPosition[1]=-2678/55;
};

void LevelOne::pressedButtonsTextIsActive(void){
	if (glfwGetKey(OpenGLObject.getWindowMember(), GLFW_KEY_ENTER) == GLFW_PRESS
		&&stateKeyEnter==GLFW_RELEASE){
			doEvent(static_cast<std::size_t>(PropertyVector[saveIndexTextEvent][4]),saveIndexTextEvent);
			stateKeyEnter = GLFW_PRESS;
	}
	else if(glfwGetKey(OpenGLObject.getWindowMember(), GLFW_KEY_ENTER) == GLFW_RELEASE){
		stateKeyEnter = GLFW_RELEASE;
	}
	
	// Reset Time of last movement (avoid bug such as infinity jump)
	MoveModel.resetLastTime();
	MoveModel.setStartTimeJump(glfwGetTime());
}

void LevelOne::pressedButtons(void){
	//static int stateKeyEnter;

	if (glfwGetKey(OpenGLObject.getWindowMember(), GLFW_KEY_R) == GLFW_PRESS){
		OpenGLObject.freeOpenALBuffer();
		//OpenGLObject.loadAndPlayWaveFile("music/testWave.wav");
		ListOfObjectsReferences[IndexMainCharacter].setModelPositionAndReferencePoint(ResetPosition);
	}

	if (glfwGetKey(OpenGLObject.getWindowMember(), GLFW_KEY_ENTER) == GLFW_PRESS
		&&stateKeyEnter==GLFW_RELEASE){
			for(std::size_t iModel=0;iModel<ListOfObjectsReferences.size();iModel++){
				if(ListOfObjectsReferences[IndexMainCharacter].checkModelInNearVicinityHorizontal(ListOfObjectsReferences[iModel])){
					doEvent(static_cast<std::size_t>(PropertyVector[iModel][4]),iModel);
				}
			}
		//PrintTextFlag = !(PrintTextFlag);
		stateKeyEnter = GLFW_PRESS;
	}
	else if(glfwGetKey(OpenGLObject.getWindowMember(), GLFW_KEY_ENTER) == GLFW_RELEASE){
		stateKeyEnter = GLFW_RELEASE;
	}
	//std::cout<<"Enter: "<<glfwGetKey(OpenGLObject.getWindowMember(), GLFW_KEY_ENTER) == GLFW_PRESS<<"\t"
}


void LevelOne::doEvent(std::size_t indexEvent,std::size_t modelIndex){
	switch (indexEvent)
	{
		case 0:
				break;
		case 1:
			TextLine1 = "Hallo";
			TextLine2 = "Hallo Hallo";
			saveIndexTextEvent = modelIndex;
			PrintTextFlag = !PrintTextFlag;
		case 2:
			ListOfObjectsReferences[IndexMainCharacter].setModelPositionAndReferencePoint(ResetPosition);
	default:
		break;
	}
};

void LevelOne::checkLevelIndex(void){
	
	
	//LevelIndex;
};

void LevelOne::moveModels(void){
	// Movement
	for(int iModel=0;iModel<PropertyVector.size();iModel++){
		switch (static_cast<std::size_t>(PropertyVector[iModel][0])){
			case 0:
				break;
			case 1:
				MoveModel.moveModelHorizontal(ListOfObjectsReferences,iModel,static_cast<double>(PropertyVector[iModel][1]));
				break;
			case 2:
				//MoveModel.moveModelByKeyboardJumpNRun(ListOfObjectsReferences,iModel);
				break;
			default:
				break;
		}

		// Main Character run into an APC
		if(iModel==IndexMainCharacter){
			// Standard Jump and run movement
			//MoveModel.resetLastTime();
			MoveModel.moveModelByKeyboardJumpNRun(ListOfObjectsReferences,iModel);
			// Check for Collision
			for(int iModel2=0;iModel2<PropertyVector.size();iModel2++){
				// If Model collide against each other
				if ((MoveModel.getCollisionVectorTmp()[iModel2] == true)){
					doEvent(static_cast<std::size_t>(PropertyVector[iModel2][5]), iModel2);
				}
			}
		}

		// APC runs into main character
		else if(PropertyVector[iModel][3]==1){
			// Check Collision
			if ((MoveModel.getCollisionVectorTmp()[IndexMainCharacter] == true)){
				doEvent(static_cast<std::size_t>(PropertyVector[iModel][5]), iModel);
			}
		}
	}

	MoveModel.moveModelByKeyboardJumpNRun(ListOfObjectsReferences,IndexMainCharacter);
	for(int iModel2=0;iModel2<PropertyVector.size();iModel2++){
		//if ((MoveModel.getCollisionVectorTmp()[iModel2] == true) && PropertyVector[iModel2][3] == 1){
		//	ListOfObjectsReferences[IndexMainCharacter].setModelPositionAndReferencePoint(ResetPosition);
		//	//MoveModel.moveModelByKeyboardJumpNRun(ListOfObjectsReferences,iModel);
		//	//std::cout<<"iModel2: "<<iModel2<<"\tPropertyVector[iModel2][3]: "<<PropertyVector[iModel2][3]<<std::endl;
		//}
		if ((MoveModel.getCollisionVectorTmp()[iModel2] == true) && PropertyVector[iModel2][3] == 1){
			doEvent(static_cast<std::size_t>(PropertyVector[iModel2][5]), iModel2);
		}
	}
	// Gravitation
	for(int iModel=0;iModel<PropertyVector.size();iModel++){
		if(PropertyVector[iModel][2]==1){
			MoveModel.moveByGravitation(ListOfObjectsReferences,iModel);
		}
	}
	//for(int iGravitation=0;iGravitation<IndicesGraviationModels.size();iGravitation++){
	//	MoveModel.moveByGravitation(ListOfObjectsReferences,IndicesGraviationModels[iGravitation]);
	//};

	//MoveModel.moveModelHorizontal(ListOfObjectsReferences,IndexMainCharacter+1,1);
};

void LevelOne::moveCameraWithMainCharacter(void){
	glm::vec3 cameraPosition = glm::vec3(ListOfObjectsReferences[IndexMainCharacter].getModelMatrix()[3].x,
								ListOfObjectsReferences[IndexMainCharacter].getModelMatrix()[3].y,
								10);

	glm::vec3 cameraLookAt = glm::vec3(ListOfObjectsReferences[IndexMainCharacter].getModelMatrix()[3].x,
								ListOfObjectsReferences[IndexMainCharacter].getModelMatrix()[3].y,
								0);
	
	//std::cout<<"x: "<<ListOfObjectsReferences[IndexMainCharacter].getModelMatrix()[3].x<<"\ty: "<<ListOfObjectsReferences[IndexMainCharacter].getModelMatrix()[3].y<<std::endl;
	//std::cout<<"x: "<<
	//glm::vec3 inversionVec = glm::vec3(0,1,0);
	OpenGLObject.setViewMatrix(cameraPosition,cameraLookAt,OpenGLObject.getInversionVec());
};

void LevelOne::drawLevel(void){
	// Clear the screen 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw Models
	//std::cout<<"Test: "<<ListOfObjectsPointers.at(0)->getTextureVec().size()<<std::endl;
	for(int iModel=0;iModel<ListOfObjectsReferences.size();iModel++){
		ListOfObjectsReferences[iModel].setCurrentWindow(OpenGLObject.getWindowMember());
		MVP = OpenGLObject.getProjectionMatrix() * OpenGLObject.getViewMatrix() * ListOfObjectsReferences[iModel].getModelMatrix();
		OpenGLObject.doShaderStuff(ListOfObjectsReferences[iModel].getProgramID(),
									ListOfObjectsReferences[iModel].getMatrixID(), 
									ListOfObjectsReferences[iModel].getTextureID(), 
									MVP, 
									ListOfObjectsReferences[iModel].getTextureVec()[ListOfObjectsReferences[iModel].getImageStatus()]);
		OpenGLObject.defineModelAndTexture(ListOfObjectsReferences[iModel].getVertexBuffer(),
											ListOfObjectsReferences[iModel].getUVBufferVec()[ListOfObjectsReferences[iModel].getImageStatus()]);
		glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
	}

	//initText2D("Images/ExportedFont.dds");
	if(PrintTextFlag){
		std::size_t nVertices = OpenGLObject.printText2D(TextLine1,0,50,50);
		OpenGLObject.drawText(nVertices);
		std::size_t nVertices2 = OpenGLObject.printText2D(TextLine2,0,0,50);
		OpenGLObject.drawText(nVertices2);
	}
		// Close all attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
		// Swap buffers
	//glfwSwapBuffers(OpenGLObject.getWindowMember());
	glfwSwapBuffers(OpenGLObject.getWindowMember());
	glfwPollEvents();

	// Reset Current Time
	//MoveModel.resetLastTime();
};

