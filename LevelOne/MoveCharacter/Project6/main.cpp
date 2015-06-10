// Include standard headers
#define GLM_FORCE_RADIANS
////Memory Leak
#include <stdlib.h>
#include "vld.h"
//#define _DEBUG


#include <fstream>
#include <iostream>

#include <vector>
#include <stdio.h>

//#include "Framework.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// Include GLFW
#include <GLFW\glfw3.h>

#include "LevelOne.h"

std::ofstream feedbackTextFile;
//std::vector<bool> destinationReached(0);
//class LevelOne levelObject;


int main(void){
	//_CrtSetBreakAlloc(601);
	//// Save the data in a text file
	//levelVec.push_back(LevelOne())

	feedbackTextFile.open ("feedback.txt",std::ios::trunc);
	feedbackTextFile.close();
	
	std::size_t indexLevel = 0;
	std::size_t indexLevelOld = indexLevel+1;
	bool doIni = true;
	
	std::vector<LevelOne> levelVec(2,LevelOne());
	//levelObject.doInit();
	//levelVec[0].doInit();
	
	//levelObject.addTrees();
	//levelObject.addMovingModels();
	//levelObject.addModelFromFile("home.txt");
	//levelObject.addModelFromFile("home.txt");
	//glfwSetTime(0.0);
	//initText2D("Images/ExportedFont.dds");
	//levelObject.OpenGLObject.loadAndPlayWaveFile("music/testWave.wav");
	
	do{
		
		// Level is already initialized
		if(indexLevel==indexLevelOld)
		{
			if(!(levelVec[indexLevel].getPrintTextFlag())){
				levelVec[indexLevel].moveModels();
				levelVec[indexLevel].moveCameraWithMainCharacter();
				levelVec[indexLevel].pressedButtons();
			}
			else{
				//std::cout<<"YES"<<std::endl;
				levelVec[indexLevel].pressedButtonsTextIsActive();
			}
			// Create Callback for model position
			glm::vec2 position = levelVec[indexLevel].getListOfObjectsReferences()[levelVec[indexLevel].getIndexMainCharacter()].getModelPosition();
			levelVec[indexLevel].pressedButtonsTextIsActive();

			//std::cout << "Position H: " << position[0] << "\tV: " << position[1] << std::endl;

			double xpos, ypos;
			glfwGetCursorPos(levelVec[indexLevel].OpenGLObject.getWindowMember(),&xpos,&ypos);
			levelVec[indexLevel].OpenGLObject.checkWindowSize();
			levelVec[indexLevel].drawLevel();

			// Close old level window and create new level window if either ESC was pressed or if Window was closed
			// Additionally change index of levels (indexLevel) such that new level can be created
			if(glfwGetKey(levelVec[indexLevel].getOpenGLObject().getWindowMember(),GLFW_KEY_ESCAPE) == GLFW_PRESS){
				indexLevel=1;
				glfwTerminate();
			}
		}
		// New Level has to be initialized
		else
		{
			std::cout<<"New Level"<<std::endl;
			levelVec[indexLevel].doInit();
			switch (indexLevel)
			{
				case 0:{
					levelVec[indexLevel].addModelFromFile("home1.txt");
						}break;
				case 1:{
					levelVec[indexLevel].addModelFromFile("testLevel2.txt");
						}break;
				default:
					break;
			}			
			//doIni = false;
			glfwSetTime(0.0);
			//std::cout << levelVec[indexLevel].getLevelWidth() << "\t" << levelVec[indexLevel].getLevelHeight();
			//doIni = true;
			indexLevelOld = indexLevel;
		}
		
	} // Check if the ESC key was pressed or the window was closed
	//while (	glfwGetKey(levelVec[indexLevel].getOpenGLObject().getWindowMember(),GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	//		glfwWindowShouldClose(levelVec[indexLevel].getOpenGLObject().getWindowMember()) == 0);
	while (glfwWindowShouldClose(levelVec[indexLevel].getOpenGLObject().getWindowMember()) == 0);

	feedbackTextFile.close();

	for (int i = 0; i < levelVec.size(); i++){
		levelVec[i].~LevelOne();
		//levelVec[i].era;
	}
	//levelVec.erase(levelVec.begin(),levelVec.begin()+1);
	levelVec.~levelVec();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
