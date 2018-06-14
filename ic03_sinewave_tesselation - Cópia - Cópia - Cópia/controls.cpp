// Include GLFW
#include <GLFW/glfw3.h>
//extern; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
//#include "Plane.h"
#include "controls.hpp"
#define ANG2RAD 3.14159265358979323846/180.0

using namespace glm;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}

glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

float angle = 60.0;

// Initial position : on +Z
glm::vec3 position = glm::vec3( -9, 25, 56 );
glm::vec3 rightN;
// Initial horizontal angle : toward -Z
float horizontalAngle = angle;
// Initial vertical angle : none
float verticalAngle = 0.0;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 9.0f; // 3 units / second
float mouseSpeed = 0.005f;

float RATIO = 4.0f / 3.0f;
float farDist   = 100.0;
float nearDist  = 0.1;

float hFar = 1024.0;
float wFar = 768.0;
float hNear = 2 * tan(initialFoV / 2) * nearDist;
float wNear = hNear * RATIO;

void computeMatricesFromInputs(GLFWwindow* window){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(hFar/2 - xpos );
	verticalAngle   += mouseSpeed * float(wFar/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	direction = normalize(direction);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	right = normalize(right);
    rightN = right;
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

	ProjectionMatrix = glm::perspective(    glm::radians(FoV),  //FOV
                                                RATIO,    //RATIO
                                                nearDist,           //nearDist
                                                farDist);        //farDist

    vec3 fc = position + direction * farDist;
   	vec3 nc = position + direction * nearDist;

	// Camera matrix
	ViewMatrix       = glm::lookAt(
								nc,           // (px, py, pz) Camera is here
								fc, // (lx = px + dx, ly = py + dy, lz = pz + dz) and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
