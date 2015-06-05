#pragma once
#include "basicIncludes\basicModelOperations.h"
class moveModel
{
public:
	// Constructor
	explicit moveModel(void);
	// Destructor
	~moveModel(void);

// ################ Class Methods ###############

	// This function moves the model rightward by "distance"-units and sets its "referencePosition" at the new point if the final destination is reached
	// It return true if destination is reached
	bool moveModelRightWard(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance);

	// This function moves the model rightward by "distance"-units and sets its "referencePosition" at the new point if the final destination is reached
	// It return true if destination is reached
	bool moveModelLeftWard(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance);

	// This function moves the model downward by "distance"-units and sets its "referencePosition" at the new point if the final destination is reached
	// It return true if destination is reached
	bool moveModelDownward(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance);

	// Does a constant Movement downward without animation
	void moveByGravitation(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex);

	// Check if a collision happens after moving. If yes and this collision is new -> reset model to previous position
	//void checkForCollisionAfterwardsAndReset(std::vector<basicModelOperations*> listOfModels,std::size_t modelIndex,bool collisionBefore);

	// This function moves the model upward by "distance"-units and sets its "referencePosition" at the new point if the final destination is reached
	// It return true if destination is reached
	bool moveModelUpward(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double distance);

	// This Function allowes a model to be controlled by the keyboard. You can move the model up/down/right/leftward
	void moveModelByKeyboard(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex);

	// This Function allowes a model to be controlled by the keyboard
	void moveModelByKeyboardJumpNRun(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex);

	// This function does a automatically movement of the chosen model
	void moveModelHorizontal(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double maxDistance);

	// This function does a automatically movement of the chosen model
	void moveModelVertical(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,double maxDistance);
	
	void changeImage(basicModelOperations &model,std::size_t indexMovement);

	// Check if collision with other models exists: Output is  true if collision happened
	bool doPreliminaryCheck(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex){
		bool collisionBefore = false;
		for(int iModel=0;iModel<listOfModels.size();iModel++){
			if(iModel!=modelIndex){
				collisionBefore = collisionBefore||listOfModels[modelIndex].checkCollisionWithSprite(listOfModels[iModel]);
			}
		}
		return collisionBefore;
	};

	// Check if collision with other models exists: Output is a boolean vector of size 2 
	// first entry is true if the x coordiantes overlap
	// second entry is true if the y coordiantes overlap
	// The two models collide if both entry are true
	std::vector<bool> moveModel::doPreliminaryCheckDetailed(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex);

	// Check if after movement a collison with other models happen and if yes reset to old position
	void doAfterCheckAndResetting(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,bool collisionBefore){
		// Check for collision
		bool collisionAfterwards = false;
		for(int iModel=0;iModel<listOfModels.size();iModel++){
			if(iModel!=modelIndex){
				//collisionAfterwards = collisionAfterwards||listOfModels[modelIndex].checkCollisionWithSprite(listOfModels[iModel]);
				CollisionVectorTmp[iModel]=listOfModels[modelIndex].checkCollisionWithSprite(listOfModels[iModel]);;
				collisionAfterwards = collisionAfterwards||CollisionVectorTmp[iModel];
			}
		}

		if(collisionAfterwards==true&&collisionBefore==false){
			listOfModels[modelIndex].setModelMatrix(listOfModels[modelIndex].getModelMatrixBackup());
		};
	};

	// Used for gravity if collision with a second model happens than the actual model is set back upwards
	// to a location with minimal distance to the second one without colliding with it
	void doAfterCheckAndResettingGravity(double deltaTime,std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,std::vector<bool> collisionVectorBefore);

	double doGeneralInitMovements(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex){
		// Update list if list of models changes
		if(lastTime.size()<listOfModels.size()){
			lastTime.resize(listOfModels.size(),0.0);
		}
		if(CollisionVectorTmp.size()<listOfModels.size()){
			CollisionVectorTmp.resize(listOfModels.size(),false);
		}
	
		// Save current time
		double RightNow1 = glfwGetTime();
		RightNow = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		double deltaTime = (currentTime - lastTime[modelIndex] );
		//lastTime[modelIndex] = glfwGetTime();
		//double deltaTime = (currentTime - RightNow1 );

		//if(deltaTime>0.1){
		//	//std::cout<<"currentTime: "<<currentTime <<"lastTime: "<<lastTime[modelIndex]<<"\tdeltaTime: "<<deltaTime<<std::endl;	
		//	deltaTime=0.1;
		//}

		//Exit
		return deltaTime;
	};

	// Set all lastTime to current time
	void resetLastTime(void);

	// Used for jump
	void translateUpwards(std::vector<basicModelOperations> &listOfModels,std::size_t modelIndex,bool collisionBefore,double deltaTime);

	void doInit(void);
// ################ Get Functions ################ 
	double getRightNow(void){
		return RightNow;
	}

	std::vector<bool> getCollisionVectorTmp(void){
		return CollisionVectorTmp;
	}

	double getStartTimeJump(void){
		return startTimeJump;
	};
// ################ Set Functions ################ 
	void setRightNow(double rightNow){
		RightNow = rightNow;
	}

	void setCollisionVectorTmp(std::vector<bool> colVec){
		CollisionVectorTmp.resize(colVec.size(),false);
		for(std::size_t iVec=0;iVec<colVec.size();iVec++){
			CollisionVectorTmp[iVec] = colVec[iVec];
		}
	};

	void setStartTimeJump(double startimeJump){
		startTimeJump = startimeJump;
	};
private:
// ################ Members ################ 
	//int ImageStatus;
	//double ImageTime;
	double RightNow;

	// Save the last time the character moved
	std::vector<double> lastTime;

	// Save the last time the character moved
	std::vector<double> lastTimeGravitation;


	// Saves if the model moves rightwards or leftward
	std::vector<bool> moveRight;

	// Saves if the model moves upwardwards or downward
	std::vector<bool> moveUp;

	// Saves temporary if a collision whith was detected with another model
	// Index in the vectroe is associated with the model index
	std::vector<bool> CollisionVectorTmp;
	
	// Time when jump started
	double startTimeJump;
};

