#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <SOIL\SOIL.h>    //用于载入图片的库文件
#include <glm\glm.hpp>     //GLSL的数学库
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"      //着色器头文件
#include "Texture.h"     //纹理头文件
#include "Camera.h"      //摄像机头文件

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


//TODO声明、定义变量
GLfloat visiableValue = 0.2f;
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


//摄像机
Camera myCamera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;   //用于判断是否第一次获取鼠标位置
GLfloat lastX = 400, lastY = 300; //鼠标初始坐标值
GLfloat yaw = 0.0f, pitch = 0.0f;

GLfloat deltaTime = 0.0f;  //当前帧遇上一帧的时间差
GLfloat lastFram = 0.0f;   //上一帧的时间
bool keys[1024];          

void CameraMove();
// The MAIN function, from here we start the application and run the game loop
int main()
{

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	//打开深度测试功能
	glEnable(GL_DEPTH_TEST);

	//建立并编译着色器
	Shader ourShader("path/to/shaders/transform.vs", "path/to/shaders/transform.frag");

	//顶点数据
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		                     
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glm::vec3 cubePosition[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.9f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.0f),
		glm::vec3(1.5f, 2.0f, -1.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};



	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




	//配置位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	////配置颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	//配置纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//解绑VAO对象
	glBindVertexArray(0);


	//载入并设置纹理
	Texture ourTeature1("path/to/pictures/container.jpg", 0);
	ourTeature1.setWrapping("REAPEAT");
	ourTeature1.setFiltering("LINER");
	ourTeature1.getTexture();
	ourTeature1.releaseTexture();

	Texture ourTexture2("path/to/pictures/awesomeface.png", 1);
	ourTexture2.setWrapping("REAPEAT");
	ourTexture2.setFiltering("NEAREST");
	ourTexture2.getTexture();
	ourTexture2.releaseTexture();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		CameraMove();


		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClearColor(1.0f, 0.5f, 0.31f,1.0f);
		//清除颜色缓冲区和深度缓冲区
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		std::cout << myCamera.Front.x << " " << myCamera.Front.y << " " << myCamera.Front.z << std::endl;

		//绑定纹理对象
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ourTeature1.texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ourTexture2.texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

		glUniform1f(glGetUniformLocation(ourShader.Program, "visiableValue"), visiableValue);

		ourShader.Use();



        //TODO摄像机
		glm::mat4 model, view, projection;
		
		view = myCamera.GetViewMatrix();
		projection = glm::perspective(myCamera.Zoom, (GLfloat)WIDTH /(GLfloat)HEIGHT, 0.1f, 1000.0f);
		

		GLuint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLuint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLuint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



		//绑定VAO对象
		glBindVertexArray(VAO);
	
		for (GLuint i = 0; i < 10; ++i){
			glm::mat4 model;
			//model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
			model = glm::translate(model, glm::vec3(cubePosition[i]));
			//model = glm::scale(model, glm::vec3(1, 3, 1));
			model = glm::rotate(model, (GLfloat)glfwGetTime()*100.0f, glm::vec3(0.5f, 1.0f, 0.0f));
			GLuint modelLoc = glGetUniformLocation(ourShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawArrays(GL_LINES, 0, 36);
		}
		//解绑VAO对象
		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	
	std::cout << key << std::endl;
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action==GLFW_RELEASE)
		keys[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//exit(0);
	if (key == GLFW_KEY_UP&&action == GLFW_PRESS){
		visiableValue += 0.1f;
		if (visiableValue > 1.0f) visiableValue = 1.0f;
	}
	if (key == GLFW_KEY_DOWN&&action == GLFW_PRESS){
		visiableValue -= 0.1f;
		if (visiableValue < 0.0f) visiableValue = 0.0f;
	}
	
}
void CameraMove(){

	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFram;
	lastFram = currentFrame;

	if (keys[GLFW_KEY_W]) myCamera.ProcessKeyboard(FOERWARD, deltaTime);
	if (keys[GLFW_KEY_S]) myCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A]) myCamera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D]) myCamera.ProcessKeyboard(RIGHNT, deltaTime);
	
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	myCamera.ProcessMouseMovement(xoffset, yoffset);

}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){

	myCamera.ProcessMouseScroll(yOffset);
}
