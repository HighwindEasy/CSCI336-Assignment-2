#include "Asssignment2.h"

// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set the color the color buffer should be cleared to
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// compile and link a vertex and fragment shader pair
	gShader.compileAndLink("lighting.vert", "lighting_cubemap.frag");

	// initialise view matrix
	gViewMatrix["Right"] = glm::lookAt(glm::vec3(0.0f, 2.0f, 4.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	gViewMatrix["Top"] = glm::lookAt(glm::vec3(0.0f, 15.0f, 0.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// initialise projection matrix
	gProjectionMatrix["Left"] = glm::ortho(-2.f, 2.f, -2.f, 2.f, -.2f, 10.f);
	gProjectionMatrix["Right"] = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 10.0f);
	gProjectionMatrix["Top"] = glm::ortho(-5.f, 5.f, -5.f, 5.f, .1f, 50.f);

	gProjectionMatrix["Main"] = glm::ortho(0.0f, static_cast<float>(gWindowWidth), 0.0f, static_cast<float>(gWindowHeight), 0.1f, 10.0f);

	// initialise directional light properties
	gLight.dir = glm::vec3(0.3f, -0.7f, -0.5f);
	gLight.La = glm::vec3(0.3f);
	gLight.Ld = glm::vec3(1.0f);
	gLight.Ls = glm::vec3(1.0f);

	// initialise material properties
	gMaterial.Ka = glm::vec3(0.2f);
	gMaterial.Kd = glm::vec3(0.2f, 0.7f, 1.0f);
	gMaterial.Ks = glm::vec3(0.2f, 0.7f, 1.0f);
	gMaterial.shininess = 40.0f;

	// initialise model matrices
	gModelMatrix = glm::mat4(1.0f);

	// load cube environment map texture
	gCubeEnvMap.generate(
		"./images/cm_front.bmp", "./images/cm_back.bmp",
		"./images/cm_left.bmp", "./images/cm_right.bmp",
		"./images/cm_top.bmp", "./images/cm_bottom.bmp");

	// load model
	gModel.loadModel("./models/torus.obj");
	
}

// function used to update the scene
static void update_scene(GLFWwindow* window)
{
	if (gRotation)
	{
		gRotateX += 1;
	}

	gViewMatrix["Left"] = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	gViewMatrix["Right"] = glm::lookAt(glm::vec3(0.0f, 0.0f, gDistance), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	gViewMatrix["Top"] = glm::lookAt(glm::vec3(-5.0f, 40.0f, 0.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//gModelMatrix = gModelMatrix * glm::rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
	gModelMatrix = glm::rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::radians(gRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
}

// function to render the scene
static void render_scene()
{
	// clear colour buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the shaders associated with the shader program
	gShader.use();

	// set light properties
	gShader.setUniform("uLight.dir", gLight.dir);
	gShader.setUniform("uLight.La", gLight.La);
	gShader.setUniform("uLight.Ld", gLight.Ld);
	gShader.setUniform("uLight.Ls", gLight.Ls);

	// set material properties
	gShader.setUniform("uMaterial.Ka", gMaterial.Ka);
	gShader.setUniform("uMaterial.Kd", gMaterial.Kd);
	gShader.setUniform("uMaterial.Ks", gMaterial.Ks);
	gShader.setUniform("uMaterial.shininess", gMaterial.shininess);

	// set viewing position
	glm::mat4 MVP;

	glViewport(0, 0, 400, 400);
	// calculate matrices
	
	MVP = gProjectionMatrix["Left"] * gViewMatrix["Left"] * gModelMatrix;
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(gModelMatrix)));

	// set uniform variables
	gShader.setUniform("uModelViewProjectionMatrix", MVP);
	gShader.setUniform("uModelMatrix", gModelMatrix);
	gShader.setUniform("uNormalMatrix", normalMatrix);

	// set cube environment map
	gShader.setUniform("uEnvironmentMap", 0);

	glActiveTexture(GL_TEXTURE0);
	gCubeEnvMap.bind();

	// render model
	gModel.drawModel();









	glViewport(400, 0, 400, 400);
	// calculate matrices
	MVP = gProjectionMatrix["Right"] * gViewMatrix["Right"] * gModelMatrix;
	normalMatrix = glm::mat3(glm::transpose(glm::inverse(gModelMatrix)));

	// set uniform variables
	gShader.setUniform("uModelViewProjectionMatrix", MVP);
	gShader.setUniform("uModelMatrix", gModelMatrix);
	gShader.setUniform("uNormalMatrix", normalMatrix);

	// set cube environment map
	gShader.setUniform("uEnvironmentMap", 0);

	glActiveTexture(GL_TEXTURE0);
	gCubeEnvMap.bind();

	// render model
	gModel.drawModel();


	glViewport(400, 400, 400, 400);
	// calculate matrices
	MVP = gProjectionMatrix["Top"] * gViewMatrix["Top"] * gModelMatrix;
	normalMatrix = glm::mat3(glm::transpose(glm::inverse(gModelMatrix)));

	// set uniform variables
	gShader.setUniform("uModelViewProjectionMatrix", MVP);
	gShader.setUniform("uModelMatrix", gModelMatrix);
	gShader.setUniform("uNormalMatrix", normalMatrix);

	// set cube environment map
	gShader.setUniform("uEnvironmentMap", 0);

	glActiveTexture(GL_TEXTURE0);
	gCubeEnvMap.bind();

	// render model
	gModel.drawModel();

	// flush the graphics pipeline
	glFlush();
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// close the window when the ESCAPE key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

// mouse movement callback function
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// pass cursor position to tweak bar
	TwEventMousePosGLFW(static_cast<int>(xpos), static_cast<int>(ypos));
}

// mouse button callback function
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// pass mouse button status to tweak bar
	TwEventMouseButtonGLFW(button, action);
}

// error callback function
static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;	// output error description
}

// create and populate tweak bar elements
TwBar* create_UI(const std::string name)
{
	// create a tweak bar
	TwBar* twBar = TwNewBar(name.c_str());

	// give tweak bar the size of graphics window
	TwWindowSize(gWindowWidth, gWindowHeight);
	TwDefine(" TW_HELP visible=false ");	// disable help menu
	TwDefine(" GLOBAL fontsize=3 ");		// set large font size

	TwDefine(" Main label='User Interface' refresh=0.02 text=light size='250 450' position='10 10' ");

	// create frame stat entries
	TwAddVarRO(twBar, "Frame Rate", TW_TYPE_FLOAT, &gFrameRate, " group='Frame Stats' precision=2 ");
	TwAddVarRO(twBar, "Frame Time", TW_TYPE_FLOAT, &gFrameTime, " group='Frame Stats' ");

	// scene controls
	TwAddVarRW(twBar, "Wireframe", TW_TYPE_BOOLCPP, &gWireframe, " group='Controls' ");

	// object rotation
	TwAddVarRW(twBar, "Rotation", TW_TYPE_BOOLCPP, &gRotation, " group='Controls' ");

	// light control
	TwAddVarRW(twBar, "Direction", TW_TYPE_DIR3F, &gLight.dir, " group='Light' opened=true ");

	// material controls
	TwAddVarRW(twBar, "Ka", TW_TYPE_COLOR3F, &gMaterial.Ka, " group='Material' ");
	TwAddVarRW(twBar, "Kd", TW_TYPE_COLOR3F, &gMaterial.Kd, " group='Material' ");
	TwAddVarRW(twBar, "Ks", TW_TYPE_COLOR3F, &gMaterial.Ks, " group='Material' ");
	TwAddVarRW(twBar, "Shininess", TW_TYPE_FLOAT, &gMaterial.shininess, " group='Material' min=1 max=255 step=1 ");


	TwAddVarRW(twBar, "Camera Distance", TW_TYPE_FLOAT, &gDistance, " group='Controls' min=1.0 max=10.0 step=0.1 ");


	return twBar;
}

int main(void)
{
	GLFWwindow* window = nullptr;	// GLFW window handle

	glfwSetErrorCallback(error_callback);	// set GLFW error callback function

	// initialise GLFW
	if (!glfwInit())
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Lab", nullptr, nullptr);

	// check if window created successfully
	if (window == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialise GLEW
		std::cerr << "GLEW initialisation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// set GLFW callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// initialise scene and render settings
	init(window);

	// initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, nullptr);
	TwBar* tweakBar = create_UI("Main");		// create and populate tweak bar elements

	// timing data
	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time since last update
	int frameCount = 0;						// number of frames since last update

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window);	// update the scene

		// if wireframe set polygon render mode to wireframe
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		render_scene();			// render the scene

		// set polygon render mode to fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TwDraw();				// draw tweak bar

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events

		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime;	// time since last update

		// if elapsed time since last update > 1 second
		if (elapsedTime > 1.0)
		{
			gFrameTime = elapsedTime / frameCount;	// average time per frame
			gFrameRate = 1 / gFrameTime;			// frames per second
			lastUpdateTime = glfwGetTime();			// set last update time to current time
			frameCount = 0;							// reset frame counter
		}
	}

	// clean up
	// uninitialise tweak bar
	TwDeleteBar(tweakBar);
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}