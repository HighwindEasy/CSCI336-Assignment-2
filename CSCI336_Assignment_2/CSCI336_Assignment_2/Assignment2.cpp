#include "Asssignment2.h"

// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set the color the color buffer should be cleared to
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// compile and link a vertex and fragment shader pair
	gShader.compileAndLink("lighting.vert", "reflection.frag");
	Cube_envm_Map_Program.compileAndLink("lighting.vert", "lighting_cubemap.frag");
	Floor_ShaderProgram.compileAndLink("lightingAndTexture.vert", "pointLightTexture.frag");

	// initialise view matrix
	gCamera.setViewMatrix(glm::vec3(0.0f, 2.0f, 4.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	// initialise projection matrix
	gCamera.setProjMatrix(glm::perspective(glm::radians(45.0f),
		static_cast<float>(gWindowWidth) / gWindowHeight, 0.1f, 10.0f));

	// initialise directional light properties
	gLight.dir = glm::vec3(0.3f, -0.7f, -0.5f);
	gLight.La = glm::vec3(0.8f);
	gLight.Ld = glm::vec3(1.0f);
	gLight.Ls = glm::vec3(1.0f);
	gLight.att = glm::vec3(1.0f, 0.f, 0.f);

	// initialise material properties
	gMaterial["Floor"].Ka = glm::vec3(0.2f);
	gMaterial["Floor"].Kd = glm::vec3(0.2f, 0.7f, 1.0f);
	gMaterial["Floor"].Ks = glm::vec3(0.2f, 0.7f, 1.0f);
	gMaterial["Floor"].shininess = 40.0f;

	gMaterial["Cube"].Ka = glm::vec3(0.2f);
	gMaterial["Cube"].Kd = glm::vec3(1.0f, 0.7f, 0.2f);
	gMaterial["Cube"].Ks = glm::vec3(1.0f, 0.7f, 0.2f);
	gMaterial["Cube"].shininess = 10.0f;

	// initialise model matrices
	gModelMatrix["Floor"] = glm::mat4(1.0f);
	gModelMatrix["Cube"] = glm::translate(glm::vec3(-1.0f, 0.5f, 0.0f)) * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));

	// load model
	gModel.loadModel("./models/cube.obj");

	//Generate unused VBO and VAO identifiers
	glGenBuffers(index_for_VBO_VAO, gVBO);
	glGenVertexArrays(index_for_VBO_VAO, gVAO);


	/* Cube environment mapping stuff here */

	Cube_Material.Ka = glm::vec3(0.2f);
	Cube_Material.Kd = glm::vec3(0.2f, 0.7f, 1.0f);
	Cube_Material.Ks = glm::vec3(0.2f, 0.7f, 1.0f);
	Cube_Material.shininess = 40.0f;

	//Initialising model matrices
	Cube_ModelMatrix = glm::mat4(1.f);
	//Load cube envm map texture
	Cube_envm_Map.generate(
		"./images/cm_front.bmp", "./images/cm_back.bmp",
		"./images/cm_left.bmp", "./images/cm_right.bmp",
		"./images/cm_top.bmp", "./images/cm_bottom.bmp");
	Cube_Model.loadModel("./models/torus.obj");

	/* Cube Environment map ends here */

	/* Floor stuff Starts here */

	Floor_ModelMatrix = glm::mat4(1.f);

	Floor_Material.Ka = glm::vec3(0.25f, 0.21f, 0.21f);
	Floor_Material.Kd = glm::vec3(1.0f, 0.83f, 0.83f);
	Floor_Material.Ks = glm::vec3(0.3f, 0.3f, 0.3f);
	Floor_Material.shininess = 11.3f;

	Floor_Texture.generate("./images/check.bmp");

	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Floor_Vertices.size(), &Floor_Vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(gVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, position)));	// specify format of position data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, normal)));		// specify format of colour data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormTex),
		reinterpret_cast<void*>(offsetof(VertexNormTex, texCoord)));	// specify format of texture coordinate data

	glEnableVertexAttribArray(0);	// enable vertex attributes
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	/* Floor Stuff ends here */

}

// function used to update the scene
static void update_scene(GLFWwindow* window)
{
	// stores camera forward/back, up/down and left/right movements
	float moveForward = 0.0f;
	float moveRight = 0.0f;

	// update movement variables based on keyboard input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveForward += gCamMoveSensitivity * gFrameTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveForward -= gCamMoveSensitivity * gFrameTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveRight -= gCamMoveSensitivity * gFrameTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveRight += gCamMoveSensitivity * gFrameTime;

	// update camera position and direction
	gCamera.update(moveForward, moveRight);
}

void Draw_Cube_Environment_Map()
{
	Cube_envm_Map_Program.use();

	// set light properties
	Cube_envm_Map_Program.setUniform("uLight.dir", gLight.dir);
	Cube_envm_Map_Program.setUniform("uLight.La", gLight.La);
	Cube_envm_Map_Program.setUniform("uLight.Ld", gLight.Ld);
	Cube_envm_Map_Program.setUniform("uLight.Ls", gLight.Ls);

	// set material properties
	Cube_envm_Map_Program.setUniform("uMaterial.Ka", Cube_Material.Ka);
	Cube_envm_Map_Program.setUniform("uMaterial.Kd", Cube_Material.Kd);
	Cube_envm_Map_Program.setUniform("uMaterial.Ks", Cube_Material.Ks);
	Cube_envm_Map_Program.setUniform("uMaterial.shininess", Cube_Material.shininess);

	// set viewing position
	Cube_envm_Map_Program.setUniform("uViewpoint", gCamera.getPosition());

	glm::mat4 MVP = gCamera.getProjMatrix() * gCamera.getViewMatrix() * Cube_ModelMatrix;
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(Cube_ModelMatrix)));

	// set uniform variables
	Cube_envm_Map_Program.setUniform("uModelViewProjectionMatrix", MVP);
	Cube_envm_Map_Program.setUniform("uModelMatrix", Cube_ModelMatrix);
	Cube_envm_Map_Program.setUniform("uNormalMatrix", normalMatrix);

	// set cube environment map
	Cube_envm_Map_Program.setUniform("uEnvironmentMap", 0);

	glActiveTexture(GL_TEXTURE0);
	Cube_envm_Map.bind();

	// render model
	Cube_Model.drawModel();

}

void Draw_Floor()
{
	Floor_ShaderProgram.use();

	// set light properties
	Floor_ShaderProgram.setUniform("uLight.pos", gLight.pos);
	Floor_ShaderProgram.setUniform("uLight.La", gLight.La);
	Floor_ShaderProgram.setUniform("uLight.Ld", gLight.Ld);
	Floor_ShaderProgram.setUniform("uLight.Ls", gLight.Ls);
	Floor_ShaderProgram.setUniform("uLight.att", gLight.att);

	// set material properties
	Floor_ShaderProgram.setUniform("uMaterial.Ka", Floor_Material.Ka);
	Floor_ShaderProgram.setUniform("uMaterial.Kd", Floor_Material.Kd);
	Floor_ShaderProgram.setUniform("uMaterial.Ks", Floor_Material.Ks);
	Floor_ShaderProgram.setUniform("uMaterial.shininess", Floor_Material.shininess);

	// set viewing position
	Floor_ShaderProgram.setUniform("uViewpoint", glm::vec3(0.0f, 2.0f, 4.0f));

	// calculate matrices
	glm::mat4 MVP = gCamera.getProjMatrix() * gCamera.getViewMatrix() * Floor_ModelMatrix;
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(Floor_ModelMatrix)));

	// set uniform variables
	Floor_ShaderProgram.setUniform("uModelViewProjectionMatrix", MVP);
	Floor_ShaderProgram.setUniform("uModelMatrix", Floor_ModelMatrix);
	Floor_ShaderProgram.setUniform("uNormalMatrix", normalMatrix);

	// set texture
	gShader.setUniform("uTextureSampler", 0);

	glActiveTexture(GL_TEXTURE0);
	Floor_Texture.bind();

	glBindVertexArray(gVAO[0]);				// make VAO active
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	// render the vertices
}


// function to render the scene
static void render_scene()
{
	/************************************************************************************
	 * Clear colour buffer, depth buffer and stencil buffer
	 ************************************************************************************/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(400, 0, 400, 400);

	/************************************************************************************
	 * Draw the scene
	 ************************************************************************************/
	

	Draw_Cube_Environment_Map();
	Draw_Floor();

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

	// previous cursor coordinates
	static glm::vec2 previousPos = glm::vec2(xpos, ypos);
	static int counter = 0;

	// allow camera rotation when right mouse button held
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// stablise cursor coordinates for 5 updates
		if (counter < 5)
		{
			// set previous cursor coordinates
			previousPos = glm::vec2(xpos, ypos);
			counter++;
		}

		// change based on previous cursor coordinates
		float deltaYaw = (previousPos.x - xpos) * gCamRotateSensitivity * gFrameTime;
		float deltaPitch = (previousPos.y - ypos) * gCamRotateSensitivity * gFrameTime;

		// update camera's yaw and pitch
		gCamera.updateRotation(deltaYaw, deltaPitch);

		// set previous cursor coordinates
		previousPos = glm::vec2(xpos, ypos);
	}
	else
	{
		counter = 0;
	}
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

	TwDefine(" Main label='User Interface' refresh=0.02 text=light size='250 250' position='10 10' ");

	// create frame stat entries
	TwAddVarRO(twBar, "Frame Rate", TW_TYPE_FLOAT, &gFrameRate, " group='Frame Stats' precision=2 ");
	TwAddVarRO(twBar, "Frame Time", TW_TYPE_FLOAT, &gFrameTime, " group='Frame Stats' ");

	// scene controls
	TwAddVarRW(twBar, "Wireframe", TW_TYPE_BOOLCPP, &gWireframe, " group='Controls' ");

	// light control
	TwAddVarRW(twBar, "Position X", TW_TYPE_FLOAT, &gLight.pos.x, " group='Light' min=-3 max=3 step=0.01 ");
	TwAddVarRW(twBar, "Position Y", TW_TYPE_FLOAT, &gLight.pos.y, " group='Light' min=-3 max=3 step=0.01 ");
	TwAddVarRW(twBar, "Position Z", TW_TYPE_FLOAT, &gLight.pos.z, " group='Light' min=-3 max=3 step=0.01 ");

	// reflective amount
	TwAddVarRW(twBar, "Blend", TW_TYPE_FLOAT, &gAlpha, " group='Reflection' min=0.2 max=1 step=0.01 ");

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
	glDeleteBuffers(1, gVBO);
	glDeleteVertexArrays(1, gVAO);

	// uninitialise tweak bar
	TwDeleteBar(tweakBar);
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}