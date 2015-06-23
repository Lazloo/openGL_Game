#include "basicIncludes\basicModelOperations.h"
#include "basicIncludes\basicOpenGLOperations.h"
#include "basicIncludes\moveModel.h"
#include "models\Tree1.h"
#include "models\mainCharacter.h"
#include "models\monster1.h"
#include "models\catGirl.h"
#include "models\invisibleBlock.h"
//#include <common/texture.cpp>
#pragma once
class LevelOne
{
public:
	explicit LevelOne(void);
	~LevelOne(void);
	
	void doInit(void);

	void addTrees(void);

	void addMovingModels(void);

	void addModelFromFile(std::string fileName);

	void moveModels(void);

	void moveCameraWithMainCharacter(void);

	void drawLevel(void);

	void pressedButtons(void);

	void pressedButtonsTextIsActive(void);

	void doEvent(std::size_t indexEvent,std::size_t modelIndex);

	void checkLevelIndex(void);

	void devideMapInParts(void);

// ########## Get ########## 

	std::vector<basicModelOperations> getListOfObjectsReferences(void){
		return ListOfObjectsReferences;
	}

	std::size_t getLevelIndex(){
	
	};

	basicOpenGLOperations getOpenGLObject(void){
		return OpenGLObject;
	}

	glm::mat4 getMVP(void){
		return MVP;
	}

	glm::mat4 getModelMatrix(void){
		return ModelMatrix;
	}

	std::vector<std::vector<double>> getPropertyVector(void){
		return PropertyVector;
	}

	glm::vec2 getResetPosition(void){
		return ResetPosition;
	}

	std::size_t getIndexMainCharacter(void){
		return IndexMainCharacter;
	}

	bool getPrintTextFlag(void){
		return PrintTextFlag;
	}

	char * getTextLine1(void){
		return TextLine1;
	}

	char * getTextLine2(void){
		return TextLine2;
	}

	std::size_t getNextLevel(void){
		return NextLevel;
	};

	double getLevelWidth(void){
		return LevelWidth;
	};

	double getLevelHeight(void){
		return LevelHeight;
	};

// ########## Set ########## 
	 void setListOfObjectsReferences(std::vector<basicModelOperations> listOfObjectsReferences){
		// Resize
		ListOfObjectsReferences.resize(listOfObjectsReferences.size());
		
		// Copy
		for(int iList=0;iList<listOfObjectsReferences.size();iList++){
			ListOfObjectsReferences[iList] = listOfObjectsReferences[iList];
		}
	}

	void setOpenGLObject(basicOpenGLOperations openGLObject){
		OpenGLObject = openGLObject;
	}

	void setMVP(glm::mat4 mvp){
		MVP = mvp;
	}

	void setModelMatrix(glm::mat4 modelMatrix){
		ModelMatrix = modelMatrix;
	}

	void setPropertyVector(std::vector<std::vector<double>> propVec){
		PropertyVector.resize(propVec.size(),std::vector<double>(propVec[0].size()));
		for(int iModel=0;iModel<propVec.size();iModel++){
			for(int iProp=0;iProp<propVec.size();iProp++){
				PropertyVector[iModel][iProp] = propVec[iModel][iProp];
			}
		}
	}
	
	void setResetPosition(glm::vec2 resetPos){
		for(int iPos=0;iPos<2;iPos++){
			ResetPosition[iPos]=resetPos[iPos];
		}
	}

	void setIndexMainCharacter(std::size_t indexMainCharacter){
		IndexMainCharacter = indexMainCharacter;
	}


	void setPrintTextFlag(bool printTextFlag){
		PrintTextFlag = printTextFlag;
	}

	void setTextLine1(char * textLine1){
		TextLine1 = textLine1;
	}

	void setTextLine2(char * textLine2){
		TextLine2 = textLine2;
	}

	void setNextLevel(std::size_t nextLevel){
		NextLevel = nextLevel;
	};

	// Object of the basicOpenGLOperations class. Contains all necessary method for starting the program
	basicOpenGLOperations OpenGLObject;
private:
	// Contains all object in the level
	std::vector<basicModelOperations> ListOfObjectsReferences;

	

	// A 4x4 matrix which contains all necessary information about the camera position
	glm::mat4 MVP;

	// A 4x4 matrix which contains all necessary information about the model position
	glm::mat4 ModelMatrix;

	// Object of the moveModel class. Contains all necessary method for moving the models
	moveModel MoveModel;

	// Index of the Main character model in ListOfObjectsReferences
	std::size_t IndexMainCharacter;

	// Contains important information about the models
	// Row represents the model index
	// First column: Movement type: 
	//		- 0 = no movement
	//		- 1 = horizontal movement
	//		- 2 = User controlled jumpNRun movement
	// Second column: additional information about the chosen movement
	//		- case 1st col is 0: entry is ignored
	//		- case 1st col is 1: entry represents the distance of the movement
	//		- case 1st col is 2: entry is ignored
	// Third column: save if gravation effect the model or not
	// Fourth column: neutral or enemy or friend
	//		- 0 ... neutral
	//		- 1 ... enemy
	//		- 2 ... friend
	// Fifth column: Index of event that happens if enter is pressed if main character 
	//			     and model are near to each other
	//		- 0 ... nothing happens
	// Sixth column: Index of event that happens if collision with main character 
	//			     and another model happens
	//		- 0 ... nothing happens
	// Seventh column: Is physically avaiable? (Can models do not run through?)
	//		- 0 No (Models can run through)
	//		- 1 Yes (Models cannot run through)
	std::vector<std::vector<double>> PropertyVector;
	
	glm::vec2 ResetPosition;
	
	// if PrintTextFlag==1 than text in TextLine1 and TextLine2 is displayed
	bool PrintTextFlag;
	char * TextLine1;
	char * TextLine2;

	// Saves if the enter key is pressed or not
	int stateKeyEnter;
	// Save the index with who the main character is talking to
	std::size_t saveIndexTextEvent;

	// This member variable defines the index of the level which shall be displayed
	std::size_t NextLevel;

	
	//
	double LevelWidth;
	double LevelHeight;
};

