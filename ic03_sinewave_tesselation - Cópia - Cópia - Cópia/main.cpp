#include <vector>
#include <iostream>
#include <string>
#include "vectormath/vectormath.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

#include "LoadShaders.hpp"
#include "controls.hpp"
#include "controls.cpp"
extern glm::vec3 position;
extern glm::vec3 direction;
extern glm::vec3 fc;
extern glm::vec3 nc;
extern glm::vec3 rightN;
extern float hFar;
extern float wFar;
extern float hNear;
extern float wNear;



#include "Plane.h"
#include "Plane.cpp"


static GLsizei IndexCount;
static const GLuint PositionSlot = 0;
static float TessLevelInner;
static float TessLevelOuter;
glm::vec3 up;

int main(int argv, char** argc){
	// Initialise GLFW
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1280, 1024, "IPSinewave_v6", NULL, NULL);
	if( window == NULL ){
    cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cout<<"Failed to initialize GLEW\n";
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1280/2, 1024/2);

	// Dark blue background
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
//	if()
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "Geodesic.Vertex",  "Geodesic.TessControl", "Geodesic.TessEval", "Geodesic.Fragment");
//	GLuint programID = LoadShaders( "Geodesic.Vertex", "Geodesic.Fragment");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID             = glGetUniformLocation(programID, "MVP");
	GLuint ModelMatrixID        = glGetUniformLocation(programID, "M");
	GLuint ViewMatrixID         = glGetUniformLocation(programID, "V");
	GLuint ProjectionMatrixID   = glGetUniformLocation(programID, "P");
    GLuint TessLevelInnerID     = glGetUniformLocation(programID, "TessLevelInner" );// Inner tessellation paramter
    GLuint TessLevelOuterID     = glGetUniformLocation(programID, "TessLevelOuter" );  // TessLevelOuter tessellation paramter
    GLuint distanceID           = glGetUniformLocation(programID, "distance");
    GLuint cameraPosIDX         = glGetUniformLocation(programID, "px");
    GLuint cameraPosIDY         = glGetUniformLocation(programID, "py");
    GLuint cameraPosIDZ         = glGetUniformLocation(programID, "pz");
    GLuint plane0ID              = glGetUniformLocation(programID, "plane0pn");
    GLuint plane1ID              = glGetUniformLocation(programID, "plane1pn");
    GLuint plane2ID              = glGetUniformLocation(programID, "plane2pn");
    GLuint plane3ID              = glGetUniformLocation(programID, "plane3pn");
    GLuint plane4ID              = glGetUniformLocation(programID, "plane4pn");
    GLuint plane5ID              = glGetUniformLocation(programID, "plane5pn");
	GLuint ampValue             = glGetUniformLocation(programID, "amp");


	up = glm::cross(rightN, direction );
    vec3 ftl = fc + ((hFar/2) * up) - ((wFar/2)*rightN);
    vec3 ftr = fc + ((hFar/2) * up) + ((wFar/2)*rightN);
	vec3 fbl = fc - ((hFar/2) * up) - ((wFar/2)*rightN);
	vec3 fbr = fc - ((hFar/2) * up) + ((wFar/2)*rightN);

	vec3 ntl = nc + ((hNear/2) * up) - ((wNear/2)*rightN);
	vec3 ntr = nc + ((hNear/2) * up) + ((wNear/2)*rightN);
	vec3 nbl = nc - ((hNear/2) * up) - ((wNear/2)*rightN);
	vec3 nbr = nc - ((hNear/2) * up) + ((wNear/2)*rightN);

    Plane plane0(ntr, ntl, ftl);
	Plane plane1(nbl, nbr, fbr);
	Plane plane2(ntl, nbl, fbl);
	Plane plane3(nbr, ntr, fbr);
	Plane plane4(ntl, ntr, nbr);
	Plane plane5(ftr, ftl, fbl);

	glm::vec2 plane0pn = plane0.getPN();
    glm::vec2 plane1pn = plane1.getPN();
    glm::vec2 plane2pn = plane2.getPN();
    glm::vec2 plane3pn = plane3.getPN();
    glm::vec2 plane4pn = plane4.getPN();
    glm::vec2 plane5pn = plane5.getPN();

    vector<unsigned short> indices;
    const GLuint index = 40.0;
    const GLfloat meshSize = 40.0;
    float tamAmostra = meshSize / (float)index;
    for (GLuint i = 0 ; i < index ; i++){
		for (GLuint j = 0 ; j < index ; j++) {
			indices.push_back( i*(index+1) 		+ j);		// V0
			indices.push_back( i*(index+1) 		+ (j+1));	// V1
			indices.push_back( (i+1)*(index+1) 	+ j);		// V2

			indices.push_back( i*(index+1) 		+ (j+1));	// V1
			indices.push_back( (i+1)*(index+1) 	+ (j+1));	// V3
			indices.push_back( (i+1)*(index+1) 	+ j);		// V2
		}
	}

    //float freq = 3.0;
    float amp = 4.0;
    vector<glm::vec3> vec_y;
    vector<GLfloat> vertices;
    float minn=100000000, maxx=-1000000000;
    for (GLfloat i = 0 ; i <= index ; i+=1.0){
		for (GLfloat j = 0 ; j <= index ; j+=1.0) {
            float y = cos((i*tamAmostra)) * sin((j*tamAmostra));
            y = y*amp;
            //cout<<"y = "<< y<<endl;
            if(y<minn) minn=y;
            if(y>maxx) maxx=y;
            glm::vec3 aux;
            aux.x = i*tamAmostra;
            aux.y = y;
            aux.z = j*tamAmostra;
            vec_y.push_back(aux);
            vertices.push_back((float)(i*tamAmostra));
            vertices.push_back((float)(j*tamAmostra));
        }
	}

    IndexCount = sizeof(indices) / sizeof(indices[0]);

    // Create the VBO for positions:
    GLuint vertexbuffer;
    GLsizei stride = 2 * sizeof(float);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Create the VBO for indices:
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);


    // For speed computation
    TessLevelInner = 1.0f;
    TessLevelOuter = 1.0f;
    float distance;
    //glm::vec3 camerapos = position;
    float minnn=100000000, maxxx=-1000000000;

    do{
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        float px = position.x; float py = position.y; float pz = position.z;
        float vecx = 20.0; float vecy = (minn+maxx)/2; float vecz = 20.0;
        float d = sqrt(pow( (float)(px - vecx) ,2) +
                       pow( (float)(py - vecy) ,2) +
                       pow( (float)(pz - vecz) ,2));
                           //cout<<"d = "<<d;
        /*if(d<minnn) minnn=d;
        if(d>maxxx) maxxx=d;*/
        if (d<=20)              TessLevelInner = 32;
        else if (d<=30 && d>20) TessLevelInner = 16;
        else if(d<=40 && d>30)  TessLevelInner = 8;
        else if(d<=50 && d>40)  TessLevelInner = 4;
        else if(d<=60 && d>50)  TessLevelInner = 2;
        else if(d>60)           TessLevelInner = 1;
        TessLevelOuter = TessLevelInner;
        //cout<<"     min = "<<minnn<<" e max = "<<maxxx<<endl;
        if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS){
           glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }
        if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        }

        /*if (glfwGetKey( window, GLFW_KEY_C ) == GLFW_PRESS){
           TessLevelInner+=1.0;
        }
        if (glfwGetKey( window, GLFW_KEY_V ) == GLFW_PRESS){
            TessLevelInner = TessLevelInner > 1 ? TessLevelInner - 1 : 1;
        }
        if (glfwGetKey( window, GLFW_KEY_B ) == GLFW_PRESS){
            TessLevelOuter+=1.0;
        }
        if (glfwGetKey( window, GLFW_KEY_N ) == GLFW_PRESS){
            TessLevelOuter = TessLevelOuter > 1 ? TessLevelOuter - 1 : 1;
        }*/

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &ProjectionMatrix[0][0]);
        glUniform1f( TessLevelInnerID, TessLevelInner );
        glUniform1f( TessLevelOuterID, TessLevelOuter );
        glUniform1f(cameraPosIDX, px);
        glUniform1f(cameraPosIDY, py);
        glUniform1f(cameraPosIDZ, pz);
        glUniform1f(ampValue,amp);





        glPatchParameteri(GL_PATCH_VERTICES, 3);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );

        // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
