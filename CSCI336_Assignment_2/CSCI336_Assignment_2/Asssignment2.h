#ifndef ASSIGNMENT2_H_
#define ASSIGNMENT2_H_


#include "utilities.h"
#include "SimpleModel.h"
#include "Texture.h"

// global variables
// settings
unsigned int gWindowWidth = 800;
unsigned int gWindowHeight = 800;

// frame stats
float gFrameRate = 60.0f;
float gFrameTime = 1 / gFrameRate;

// scene content
ShaderProgram gShader;	// shader program object

std::map<std::string, glm::mat4> gViewMatrix;			// view matrix
std::map<std::string, glm::mat4> gProjectionMatrix;	// projection matrix
glm::mat4 gModelMatrix = glm::mat4(1.f);			// object matrix

Light gLight;					// light properties
Material gMaterial;				// material properties
SimpleModel gModel;				// object model
Texture gCubeEnvMap;			// cube environment map

// controls
bool gWireframe = false;	// wireframe control
bool gRotation = false; //Rotation Control
float gDistance = 3.f;
float gRotateX = 0.0f;



#endif