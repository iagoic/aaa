#ifndef CONTROL_H
#define CONTROL_H
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    void computeMatricesFromInputs(GLFWwindow* window);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

#endif // CONTROL_H
