#ifndef CONTROL_H
#define CONTROL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Model.h"
#include "View.h"
#include <glm/gtx/vector_angle.hpp>
// #include <glm/gtx/quaternion.hpp>

using namespace std;

class Control{

  public:
  View *view;
  Model *control_obj;
  GLint width,height,no_objs;
  public:
  void handleMouse(GLFWwindow* window, int button, int action, int mods);
  void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
  void handleScroll(GLFWwindow* window,double x,double y);
  glm::vec3 mouse(GLFWwindow* window, double x, double y);
  Model* set_obj(float wx,float wy,float wz);
  void rotate(float curr_x,float curr_y,float x, float y);
  glm::vec3 getTrackBallCoordinates(float x, float y);
};

#endif
