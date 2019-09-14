#include "Model.cpp"
#include "Control.cpp"
#include "View.cpp"
#include "Texture.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;

Control *cnt;
void keyHandler(GLFWwindow* window, int key, int code, int action, int mode){
  return cnt->handleKeys(window, key, code, action, mode);
}
void mouseHandler(GLFWwindow* window, int button, int action, int mods){
	return cnt->handleMouse(window,button,action,mods);
}
void scrollHandler(GLFWwindow* window, double x,double y){
	return cnt->handleScroll(window,x,y);
}

int main(int argc, char const *argv[]){
	printf("Controlling Object setting:\n");
  printf("Press A to activate Cube\n");
  printf("Press B to activate Canstick\n");
  printf("Press C to activate Sphere\n");
  printf("Press D to activate Bike\n\n");
  printf("Commands to control selected object:\n");
  printf("Right click to translate center of object to the mouse position\n");
  printf("Click + to zoom the object\n");
  printf("Click - to zoom out the object\n\n");
  printf("Press 1,2,3,4 to toggle 1st,2nd,3rd,4th lights respectively\n\n");
  printf("Press T to change textures\n");
  printf("Press M to change the mapping of textures\n\n");
  printf("Up arrow to increase speed of objects\n");
  printf("Down arrow to decrease speed of objects\n\n");
  printf("Click on object to make object spin around itself, and picking same object again will stop\n");

  	if (!glfwInit()){
  		printf("GLFW initialisation failed!");
  		glfwTerminate();
  		return 1;
  	}
  	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
  	if (!mainWindow)
  	{
  		printf("GLFW window creation failed!");
  		glfwTerminate();
  		return 1;
  	}

  	int bufferWidth, bufferHeight;
  	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  	glfwMakeContextCurrent(mainWindow);

  	glewExperimental = GL_TRUE;

  	if (glewInit() != GLEW_OK)
  	{
  		printf("GLEW initialisation failed!");
  		glfwDestroyWindow(mainWindow);
  		glfwTerminate();
  		return 1;
  	}

  	glEnable(GL_DEPTH_TEST);

  	glViewport(0, 0, bufferWidth, bufferHeight);

    View v1;
    v1.bufferWidth = 800;
    v1.bufferHeight = 600;
    v1.CreateScene();

  	cnt = (Control*) malloc(sizeof(Control));
    cnt->width = WIDTH;
    cnt->height = HEIGHT;
    cnt->control_obj = &(v1.scene[0]);
    cnt->no_objs = v1.numObjs;
    cnt->view = &v1;


  	while (!glfwWindowShouldClose(mainWindow))
  	{
  		// glfwWaitEvents();
      glfwPollEvents();
  		glfwSetKeyCallback(mainWindow,keyHandler);
  		glfwSetMouseButtonCallback(mainWindow,mouseHandler);
      glfwSetCursorPosCallback(mainWindow, scrollHandler);

  		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      v1.DrawScene();
  		glfwSwapBuffers(mainWindow);
  	}
  return 0;
}
