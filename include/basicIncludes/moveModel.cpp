#pragma once
#define GLM_FORCE_RADIANS
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
	// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GL\gl.h>
	// Include GLFW
#include <GLFW\glfw3.h>
	// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <fstream>
#include "basicIncludes\moveModel.h"

moveModel::moveModel(void)
{
	doInit();
	
}

moveModel::~moveModel(void)
{
}

void moveModel::doInit(void){
	RightNow = 0.0;
	CollisionVectorTmp.resize(0,false);
};

void moveModel::moveModelByKeyboard(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex){

	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);

	// Move upward
	if (glfwGetKey(listOfModels[modelIndex].getCurrentWindow(), GLFW_KEY_UP) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(0.0f, deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f));
		changeImage(listOfModels[modelIndex],2);
	}
	// Move downward
	else if (glfwGetKey(listOfModels[modelIndex].getCurrentWindow(), GLFW_KEY_DOWN) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(0.0f, -deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f));
		changeImage(listOfModels[modelIndex],3);
	}

	// Strafe right
	else if (glfwGetKey(listOfModels[modelIndex].getCurrentWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		changeImage(listOfModels[modelIndex],0);
	}
	// Strafe left
	else if (glfwGetKey(listOfModels[modelIndex].getCurrentWindow(), GLFW_KEY_LEFT) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(-deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		changeImage(listOfModels[modelIndex],1);
	}

	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());
	// Check if after movement a collison with other models happen and if yes reset to old position
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];
}

void moveModel::moveModelHorizontal(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double maxDistance){
	
	// Update list if list of models changes
	if(moveRight.size()<listOfModels.size()){
		moveRight.resize(listOfModels.size());
	}

	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);
	
	// Make Moement decisions
	if( ((listOfModels[modelIndex].getReferencePosition())[0]
		    -(listOfModels[modelIndex].getModelMatrix())[3].x)>abs(maxDistance)){
		moveRight[modelIndex] = true;
	}
	else if (((listOfModels[modelIndex].getReferencePosition())[0]
		    -(listOfModels[modelIndex].getModelMatrix())[3].x)<-abs(maxDistance)){
		moveRight[modelIndex] = false;
	}
	if(moveRight[modelIndex]){
		translateMatrix = glm::translate(glm::vec3(deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		changeImage(listOfModels[modelIndex],0);
	}
	else if(!moveRight[modelIndex]){
		translateMatrix = glm::translate(glm::vec3(-deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		changeImage(listOfModels[modelIndex],1);
	}

	// Make Backup for the case that collision happens after movement
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Move Model
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

	// Check if after movement a collison with other models happen and if yes reset to old position
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];
};

void moveModel::moveModelVertical(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double maxDistance){
	
	// Update list if ListofModels has changed
	if(moveUp.size()<listOfModels.size()){
		moveUp.resize(listOfModels.size());
	}	
	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);

	// Move rightward
	if( ((listOfModels[modelIndex].getReferencePosition())[1]
		    -(listOfModels[modelIndex].getModelMatrix())[3].y)>abs(maxDistance)){
		moveUp[modelIndex] = true;
	}
	else if (((listOfModels[modelIndex].getReferencePosition())[1]
		    -(listOfModels[modelIndex].getModelMatrix())[3].y)<-abs(maxDistance)){
		moveUp[modelIndex] = false;
	}
	if(moveUp[modelIndex]){
		translateMatrix = glm::translate(glm::vec3(0.0f, deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f));
		changeImage(listOfModels[modelIndex],2);
	}
	else if(!moveUp[modelIndex]){
		translateMatrix = glm::translate(glm::vec3(0.0f, -deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f));
		changeImage(listOfModels[modelIndex],3);
	}

	// Make Backup for the case that collision happens after movement
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Move Model
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

	// Check if after movement a collison with other models happen and if yes reset to old position
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];
};

bool moveModel::moveModelUpward(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance){

	if(moveUp.size()<listOfModels.size()){
		moveUp.resize(listOfModels.size());
	}
		
	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);

	// Check if determination is reached
	if( abs((listOfModels[modelIndex].getReferencePosition())[1]
		    -(listOfModels[modelIndex].getModelMatrix())[3].y)<=abs(distance)){
		moveUp[modelIndex] = true;
	}
	else{
		// Set new Refrence point
		listOfModels[modelIndex].setReferencePosition(glm::vec2(
			(listOfModels[modelIndex].getModelMatrix())[3].x, 
			(listOfModels[modelIndex].getModelMatrix())[3].y ) );

		moveUp[modelIndex] = false;
	}

	if(moveUp[modelIndex]){
		//feedbackTextFile<<"y old: "<<(listOfModels[modelIndex].getModelMatrix())[3].y<<"\t deltay: "<< deltaTime * listOfModels[modelIndex].getMovementSpeed()<<std::endl;
		translateMatrix = glm::translate(glm::vec3(0.0f, deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f));
		changeImage(listOfModels[modelIndex],2);
	}

	// Make Backup for the case that collision happens after movement
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Move Model
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

	// Check if after movement a collison with other models happen and if yes reset to old position
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];
	
	// Return if destination is reached
	return !moveUp[modelIndex];
};

bool moveModel::moveModelDownward(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance){

	if(moveUp.size()<listOfModels.size()){
		moveUp.resize(listOfModels.size());
	}
		
	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);

	// Check if determination is reached
	if( abs((listOfModels[modelIndex].getReferencePosition())[1]
		    -(listOfModels[modelIndex].getModelMatrix())[3].y)<abs(distance)){
		moveUp[modelIndex] = false;
		//std::cout<<"Inside"<<std::endl;
	}
	else{
		listOfModels[modelIndex].setReferencePosition(glm::vec2(
			(listOfModels[modelIndex].getModelMatrix())[3].x, 
			(listOfModels[modelIndex].getModelMatrix())[3].y ) );
		moveUp[modelIndex] = true;
		//std::cout<<"Inside2"<<std::endl;
	}
	if(!moveUp[modelIndex]){
		translateMatrix = glm::translate(glm::vec3(0.0f, -deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f));
		changeImage(listOfModels[modelIndex],3);
	}

	// Make Backup for the case that collision happens after movement
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Move Model
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

	// Check if after movement a collison with other models happen and if yes reset to old position
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];
	
	// Return if destination is reached
	return (moveUp[modelIndex]);
};

bool moveModel::moveModelRightWard(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance){
	
	if(moveRight.size()<listOfModels.size()){
		moveRight.resize(listOfModels.size());
	}
		
	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);
	
	// Move rightward
	if( abs((listOfModels[modelIndex].getReferencePosition())[0]
		-(listOfModels[modelIndex].getModelMatrix())[3].x)<abs(distance)){
		moveRight[modelIndex] = true;
	}
	else{
		listOfModels[modelIndex].setReferencePosition(glm::vec2(
			(listOfModels[modelIndex].getModelMatrix())[3].x, 
			(listOfModels[modelIndex].getModelMatrix())[3].y ) );
		moveRight[modelIndex] = false;
	}
	if(moveRight[modelIndex]){
		translateMatrix = glm::translate(glm::vec3(deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		changeImage(listOfModels[modelIndex],0);
	}

	// Make Backup for the case that collision happens after movement
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Move Model
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

	// Check if after movement a collison with other models happen and if yes reset to old position
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];

	// Return if final destination is reached
	return !moveRight[modelIndex];
};

bool moveModel::moveModelLeftWard(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance){
	

	if(moveRight.size()<listOfModels.size()){
		moveRight.resize(listOfModels.size());
	}
		
	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);
	
	// Check if final destination is reached
	if( abs((listOfModels[modelIndex].getReferencePosition())[0]
		-(listOfModels[modelIndex].getModelMatrix())[3].x)<abs(distance)){
		moveRight[modelIndex] = false;
	}
	else{
		listOfModels[modelIndex].setReferencePosition(glm::vec2(
			(listOfModels[modelIndex].getModelMatrix())[3].x, 
			(listOfModels[modelIndex].getModelMatrix())[3].y ) );
		moveRight[modelIndex] = true;
	}
	if(!moveRight[modelIndex]){
		translateMatrix = glm::translate(glm::vec3(-deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		changeImage(listOfModels[modelIndex],1);
	}

	// Make Backup for the case that collision happens after movement
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Move Model
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

	// Check if after movement a collison with other models happen and if yes reset to old position
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];

	// Return if final destination is reached
	//std::cout<<"Collision"<<std::endl;
	return moveRight[modelIndex];
};

void moveModel::moveByGravitation(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex){

	// glfwGetTime is called only once, the first time this function is called
	if(lastTimeGravitation.size()<listOfModels.size()){
		lastTimeGravitation.resize(listOfModels.size());
	}

	// Initialize all necessary Information
	//// deltaTime = currentTime - lastTime since lastTime is used in all the other movement function 
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);
	deltaTime = deltaTime + lastTime[modelIndex]  - lastTimeGravitation[modelIndex];

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	//bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);
	std::vector<bool> collisionVectorBefore = doPreliminaryCheckDetailed(listOfModels,modelIndex);

	translateMatrix = glm::translate(glm::vec3(0.0f, -deltaTime * 4.0, 0.0f));

	// Make Backup for the case that collision happens after movement
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Move Model
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

	// Check if after movement a collison with other models happen and if yes reset to old position
	//doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	doAfterCheckAndResettingGravity(deltaTime,listOfModels,modelIndex,collisionVectorBefore);

	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTimeGravitation[modelIndex] = deltaTime + lastTimeGravitation[modelIndex];
	
};

void checkForCollisionAfterwardsAndReset(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,bool collisionBefore){
	bool collisionAfterwards = false;
	for(int iModel=0;iModel<listOfModels.size();iModel++){
		if(iModel!=modelIndex){
				collisionAfterwards = collisionAfterwards||listOfModels[modelIndex].checkCollisionWithSprite(listOfModels[iModel]);
		}
	}

	if(collisionAfterwards==true&&collisionBefore==false){
		listOfModels[modelIndex].setModelMatrix(listOfModels[modelIndex].getModelMatrixBackup());
		//listOfModels[modelIndex].setModelPosition(glm::vec2(listOfModels[modelIndex].getModelPosition()));
	}
	//listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

};

void moveModel::translateUpwards(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,bool collisionBefore,double deltaTime){
	if(deltaTime>0.01){deltaTime=0.01;};

	glm::mat4 translateMatrix = glm::translate(glm::vec3(0.0f, deltaTime * (4.0+10.0), 0.0f));
		
	listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());
	//doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);

	std::size_t indexRightwards = listOfModels[modelIndex].getIndicesTextureVec()[0];
	std::size_t indexLeftwards = listOfModels[modelIndex].getIndicesTextureVec()[1];

	if (listOfModels[modelIndex].getImageStatus()<indexLeftwards){
		listOfModels[modelIndex].setImageStatus(indexRightwards+1);
	}
	else{
		listOfModels[modelIndex].setImageStatus(indexLeftwards+1);
	}
	listOfModels[modelIndex].setImageTime(glfwGetTime());
}

void moveModel::moveModelByKeyboardJumpNRun(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex){

	// glfwGetTime is called only once, the first time this function is called
	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());
	static double startTimeJump = glfwGetTime();
	RightNow = glfwGetTime();

	// Initialize all necessary Information
	double deltaTime = doGeneralInitMovements(listOfModels,modelIndex);

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Check if collision with other models exists
	bool collisionBefore = doPreliminaryCheck(listOfModels,modelIndex);

	// Move upward
	bool condition1 = false;
	for(int iModel=0;iModel<listOfModels.size();iModel++){
		if(iModel!=modelIndex){
			condition1 = condition1||listOfModels[modelIndex].checkCollisionWithSpriteBelow(listOfModels[iModel]);
		}
	}
	// Up-Key is pressed
	bool condition2 = (glfwGetKey(listOfModels[modelIndex].getCurrentWindow(), GLFW_KEY_UP) == GLFW_PRESS);
	bool condition3 = (RightNow-startTimeJump)>0.4;
	
	if (condition1&&condition2&&condition3){
		startTimeJump=glfwGetTime();
		translateUpwards(listOfModels,modelIndex,collisionBefore,deltaTime);
	}
	else if(condition2&&!condition3){
		//std::cout<<"condition1: "<<condition1<<"\tcond3: "<<condition3<<std::endl;
		translateUpwards(listOfModels,modelIndex,collisionBefore,deltaTime);
	}


	bool condition4 = false;
	for(int iModel=0;iModel<listOfModels.size();iModel++){
		if(iModel!=modelIndex){
			condition4 = condition4||listOfModels[modelIndex].checkCollisionWithSpriteTop(listOfModels[iModel]);
		}
	}

	if (condition4){
		listOfModels[modelIndex].setModelMatrix(listOfModels[modelIndex].getModelMatrixBackup());
	}

	listOfModels[modelIndex].setModelMatrixBackup(listOfModels[modelIndex].getModelMatrix());

	// Strafe right
	if (glfwGetKey(listOfModels[modelIndex].getCurrentWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS){
		// Translate Right
		translateMatrix = glm::translate(glm::vec3(deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());

		// Set Image
		changeImage(listOfModels[modelIndex],0);

	}
	// Strafe left
	else if (glfwGetKey(listOfModels[modelIndex].getCurrentWindow(), GLFW_KEY_LEFT) == GLFW_PRESS){
		// Translate Right
		translateMatrix = glm::translate(glm::vec3(-deltaTime * listOfModels[modelIndex].getMovementSpeed(), 0.0f, 0.0f));
		listOfModels[modelIndex].setModelMatrix(translateMatrix * listOfModels[modelIndex].getModelMatrix());
		
		//std::cout<<"deltaTime * speed"<<deltaTime * listOfModels[modelIndex].getMovementSpeed()<<"\tdeltaTime: "<<deltaTime<<"\tspeed: "<<listOfModels[modelIndex].getMovementSpeed()<<std::endl;
		// Set Image
		changeImage(listOfModels[modelIndex],1);

	}
	doAfterCheckAndResetting(listOfModels,modelIndex,collisionBefore);
	
	// For the next frame, the "last time" will be "now" deltaTime = currentTime - lastTime
	lastTime[modelIndex] = deltaTime + lastTime[modelIndex];
}

void moveModel::resetLastTime(){
	if (lastTime.size()>0){
		//std::cout<<"1 glfwGetTime(): "<<(glfwGetTime())<<"\tlastTime: "<<lastTime[0]<<std::endl;
		for(std::size_t iModel = 0;iModel<lastTime.size();iModel++){
			lastTime[iModel]=glfwGetTime();
			lastTimeGravitation[iModel]=glfwGetTime();
		}
		//std::cout<<"2 glfwGetTime(): "<<glfwGetTime()<<"\tlastTime: "<<lastTime[0]<<std::endl;
	}
}

// Check if after movement a collison with other models happen and if yes reset to old position
void moveModel::doAfterCheckAndResettingGravity(double deltaTime,std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,std::vector<bool> collisionVectorBefore){

	// Check for collision
	bool collisionAfterwards = false;
	for(int iModel=0;iModel<listOfModels.size();iModel++){
		if(iModel!=modelIndex){
			//collisionAfterwards = collisionAfterwards||listOfModels[modelIndex].checkCollisionWithSprite(listOfModels[iModel]);
			std::vector<bool>CollisionVector = listOfModels[modelIndex].checkCollisionWithSpriteTest(listOfModels[iModel]);;
			//collisionAfterwards = collisionAfterwards||CollisionVectorTmp[iModel];

			//std::cout<<"c0: "<<CollisionVector[0]<<"\tc1: "<<CollisionVector[1]<<"\tc0b: "<<collisionVectorBefore[0]<<"\tc1b: "<<collisionVectorBefore[1]<<std::endl;
			//if(CollisionVector[0]&&CollisionVector[1]&&!(collisionVectorBefore[0]&&collisionVectorBefore[1])){
			if(CollisionVector[0]&&CollisionVector[1]){
				double correctionDistance = 0*(deltaTime * 4.0)+(listOfModels[iModel].getModelHeight()/2+listOfModels[modelIndex].getModelHeight()/2);

					listOfModels[modelIndex].setModelPosition(
						glm::vec2(listOfModels[modelIndex].getModelPosition()[0],
						listOfModels[iModel].getModelPosition()[1]+
						correctionDistance ));
				break;
			}
		}
	}

	//if(collisionAfterwards==true&&collisionBefore==false){
	//	listOfModels[modelIndex].setModelMatrix(listOfModels[modelIndex].getModelMatrixBackup());
	//}
};

// Check if collision with other models exists
std::vector<bool> moveModel::doPreliminaryCheckDetailed(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex){

	std::vector<bool> collisionVectorBefore(2,false);
	//std::vector<bool> collisionVectorProto(2,false);
	for(int iModel=0;iModel<listOfModels.size();iModel++){
		if(iModel!=modelIndex){
			collisionVectorBefore = listOfModels[modelIndex].checkCollisionWithSpriteTest(listOfModels[iModel]);
			if(collisionVectorBefore[0]&&collisionVectorBefore[1]){
				break;
			}
		}
	}
	return collisionVectorBefore;
};


void moveModel::changeImage(basicModelOperations &model,std::size_t indexMovement){
	if (RightNow - model.getImageTime() > 0.1){
		// What as the last image
		bool cond11 = (model.getImageStatus() == (model.getIndicesTextureVec()[indexMovement]));
		bool cond12 = (model.getImageStatus() == (model.getIndicesTextureVec()[indexMovement]+1));
		bool cond13 = (model.getImageStatus() == (model.getIndicesTextureVec()[indexMovement]+2));
		bool cond14 = (model.getImageStatus() == (model.getIndicesTextureVec()[indexMovement]+3));
		bool cond15 = (model.getImageStatus() == (model.getIndicesTextureVec()[indexMovement]+4));
		bool cond16 = (model.getImageStatus() == (model.getIndicesTextureVec()[indexMovement]+5));
		// How many images exist in total
		bool cond21 = ((model.getnTextureVec()[indexMovement])>=3);
		bool cond22 = ((model.getnTextureVec()[indexMovement])>=4);
		bool cond23 = ((model.getnTextureVec()[indexMovement])>=5);
		bool cond24 = ((model.getnTextureVec()[indexMovement])>=6);
		// Set new image texture
		model.setImageStatus(model.getIndicesTextureVec()[indexMovement] 
							+ cond11
							+ 2*cond12*cond21
							+ 3*cond13*cond22
							+ 4*cond14*cond23
							+ 5*cond15*cond24);
		// Reset time
		model.setImageTime(RightNow);
	}
};