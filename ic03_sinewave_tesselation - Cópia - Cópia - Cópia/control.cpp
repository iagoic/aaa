#include "control.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Control{
    float angle = 60.0;
    float farDist   = 100.0;
    float nearDist  = 0.1;
    float hFar = 1024.0;
    float wFar = 768.0;
    float initialFoV = 45.0f;
    float hNear = 2 * tan(initialFoV / 2) * nearDist;
    float  wNear = hNear * RATIO;
    glm::vec3 position = glm::vec3( -9, 25, 56 );
    float RATIO = 4.0f / 3.0f;
    glm::vec3 rightN;
    float speed = 9.0f;
    float mouseSpeed = 0.005f;
    float horizontalAngle = angle;
    float verticalAngle = 0.0;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::vec3 fc;
    glm::vec3 nc;
    public:
        void computeMatricesFromInputs(GLFWwindow* window);
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

};
//control::control(){
//    float angle;
//    float farDist;
//    float nearDist;
//    float hFar;
//    float wFar;
//    float hNear;
//    float wNear;
//     Initial Field of View
//    float initialFoV;
//    glm::vec3 position;
//    float RATIO;
//    glm::vec3 rightN;
//    float speed;
//    float mouseSpeed;
//     Initial horizontal angle : toward -Z
//    float horizontalAngle;
//     Initial vertical angle : none
//    float verticalAngle;
//    this->angle = 60.0;
//    this->farDist   = 100.0;
//    this->nearDist  = 0.1;
//    this->hFar = 1024.0;
//    this->wFar = 768.0;
//    this->initialFoV = 45.0f;
//    this->hNear = 2 * tan(initialFoV / 2) * nearDist;
//    this->RATIO = 4.0f / 3.0f;
//    this->wNear = hNear * RATIO;
//    this->position = glm::vec3( -9, 25, 56 );
//    this->RATIO = 4.0f / 3.0f;
//    this->rightN;
//    this->speed = 9.0f;
//    this->mouseSpeed = 0.005f;
//    this->horizontalAngle = angle;
//    this->verticalAngle = 0.0;
//}
//
//control::~control()
//{
//    dtor
//}

void Control::computeMatricesFromInputs(GLFWwindow* window){

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
	verticalAngle += mouseSpeed * float(wFar/2 - ypos );

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

    fc = position + direction * farDist;
   	nc = position + direction * nearDist;

	// Camera matrix
	ViewMatrix       = glm::lookAt(
								nc,           // (px, py, pz) Camera is here
								fc, // (lx = px + dx, ly = py + dy, lz = pz + dz) and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
