#ifndef ASSIGNMENT2_H_
#define ASSIGNMENT2_H_

#include "utilities.h"
#include "Camera.h"
#include "SimpleModel.h"
#include "Texture.h"

// global variables
// settings
unsigned int gWindowWidth = 800;
unsigned int gWindowHeight = 800;
const float gCamMoveSensitivity = 1.0f;
const float gCamRotateSensitivity = 0.1f;

// frame stats
float gFrameRate = 60.0f;
float gFrameTime = 1 / gFrameRate;

// scene content
ShaderProgram gShader;	// shader program object

const int index_for_VBO_VAO = 1;
GLuint gVBO[index_for_VBO_VAO];		// vertex buffer object identifier
GLuint gVAO[index_for_VBO_VAO];		// vertex array object identifier

Camera gCamera;					// camera object
std::map<std::string, glm::mat4> gModelMatrix;	// object matrix

Light gLight;					// light properties
std::map<std::string, Material>  gMaterial;		// material properties
SimpleModel gModel;				// object model

// controls
bool gWireframe = false;	// wireframe control
float gAlpha = 0.5f;		// reflective amount


/*

 Section for Cube Environmental mapping

*/

ShaderProgram Cube_envm_Map_Program;
glm::mat4 Cube_ModelMatrix;
Material Cube_Material;
SimpleModel Cube_Model;
Texture Cube_envm_Map;
float RotateX;


void Draw_Cube_Environment_Map();

/*

 Section for the floor of the map

*/
ShaderProgram Floor_ShaderProgram;
glm::mat4 Floor_ModelMatrix;
Material Floor_Material;
Texture Floor_Texture;

std::vector<GLfloat> Floor_Vertices =
{
	-1.0f, 0.0f, 1.0f,	// vertex 0: position
	0.0f, 1.0f, 0.0f,	// vertex 0: normal
	0.0f, 0.0f,			// vertex 0: texture coordinate
	1.0f, 0.0f, 1.0f,	// vertex 1: position
	0.0f, 1.0f, 0.0f,	// vertex 1: normal
	1.0f, 0.0f,			// vertex 1: texture coordinate
	-1.0f, 0.0f, -1.0f,	// vertex 2: position
	0.0f, 1.0f, 0.0f,	// vertex 2: normal
	0.0f, 1.0f,			// vertex 2: texture coordinate
	1.0f, 0.0f, -1.0f,	// vertex 3: position
	0.0f, 1.0f, 0.0f,	// vertex 3: normal
	1.0f, 1.0f,			// vertex 3: texture coordinate
};

void Draw_Floor();

/*

 Section for the Walls, using Normal mapping

*/




#endif