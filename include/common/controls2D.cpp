// Include GLFW
//#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls2D.hpp"
int windowWidth, windowHeight;

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 10 ); 
glm::vec3 cameraPosition = glm::vec3( 0, 0, 10 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
//float horizontalAngle = 0.0f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(glm::mat4 &ModelMatrix){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwGetWindowSize(window,&windowWidth,&windowHeight);
	//glfwSetCursorPos(window, windowWidth/2, windowHeight/2);

	// Compute new orientation
	horizontalAngle += 0* float(windowWidth/2 - xpos );
	verticalAngle   += 0* float( windowHeight/2 - ypos );

	// Initialize the translation matrix
	glm::mat4 translateMatrix;

	// Move upward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(0.0f,deltaTime * speed,0.0f));
	}
	// Move downward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(0.0f,-deltaTime * speed,0.0f));
	}

	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(deltaTime * speed,0.0f,0.0f));
		//position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		translateMatrix = glm::translate(glm::vec3(-deltaTime * speed,0.0f,0.0f));
		//position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	// ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	//ViewMatrix       = glm::lookAt(
	//							cameraPosition,           // Camera is here
	//							position+direction, // and looks here : at the same position, plus "direction"
	//							up                  // Head is up (set to 0,-1,0 to look upside-down)
	//					   );
	ModelMatrix = translateMatrix*ModelMatrix;

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}