#include "../include/Control.h"

using namespace std;

glm::vec3 Control::mouse(GLFWwindow* window, double x, double y){
	GLint viewport[4];
	float winX,winY,winZ;
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	winX = (2*winX / width) - 1;
	winY = (2*winY / height) - 1 ;
	winZ = winZ * 2  - 1;
	return glm::vec3(winX,winY,winZ);
}

Model* Control::set_obj(float wx,float wy,float wz){
	float minX,minY,minZ,maxX,maxY,maxZ;
	for(int i=0;i<no_objs;i++){
		Model* obj = &(view->scene[i]);
		float dist = obj->scale*obj->max_diff/2;
		minX = obj->x_center-dist;
		maxX = obj->x_center+dist;
		minY = obj->y_center-dist;
		maxY = obj->y_center+dist;
		minZ = obj->z_center-dist;
		maxZ = obj->z_center+dist;
		if(wx>=minX && wx<=maxX && wy>=minY && wy<=maxY){
			control_obj = obj;
			return obj;
		}
	}
	return NULL;
}

void Control::rotate(float curr_x,float curr_y,float x, float y){
	// printf("%f %f %f %f\n",curr_x,curr_y,x,y );
    glm::vec3 v1 = glm::vec3(glm::normalize(getTrackBallCoordinates(x, y)));
    glm::vec3 v2 = glm::vec3(glm::normalize(getTrackBallCoordinates(curr_x, curr_y)));
    if(v1 != v2){
        glm::vec3 axis = glm::normalize(glm::cross(v1, v2));
        float angle = glm::orientedAngle(v1, v2, -axis);
        glm::vec3 prev_rot = glm::axis(control_obj->rot_prev);
        float prev_ang = glm::angle(control_obj->rot_prev);
        glm::quat rot_now = glm::angleAxis(angle, axis);
        auto rot_quat = glm::rotate(rot_now, prev_ang, prev_rot);
				control_obj->rot_prev = rot_now;
        control_obj->rotate_model *= glm::toMat4(rot_quat);
   }
}

glm::vec3 Control::getTrackBallCoordinates(float x, float y){
  double d = sqrt((x - control_obj->x_center) * (x - control_obj->x_center) + (y - control_obj->y_center) * (y - control_obj->y_center));
  double r = 0.7;
  double zpos;
  if (d < (r * 0.70710678118654752440)){
    zpos = sqrt(r * r - d * d);
  }
  else{
    double t = (r / 1.41421356237309504880);
    zpos = (t * t) / d;
  }
  return glm::vec3(x - control_obj->x_center, y - control_obj->y_center, zpos);
}

void Control::handleKeys(GLFWwindow* window, int key, int code, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS){
		if(control_obj!=NULL)
			control_obj->scale *= 1.1;
	}
	if(control_obj!=NULL){
		if (key == GLFW_KEY_MINUS && action == GLFW_PRESS){
			if(control_obj!=NULL)
				control_obj->scale /= 1.1;
		}
		// if (key == GLFW_KEY_UP && action == GLFW_PRESS){
		// 	if(control_obj!=NULL)
		// 		control_obj->y_center += 0.01;
		// }
		// if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		// 	if(control_obj!=NULL)
		// 		control_obj->y_center -= 0.01;
		// }
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
			if(control_obj!=NULL)
				control_obj->x_center -= 0.01;
		}
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
			if(control_obj!=NULL)
				control_obj->x_center += 0.01;
		}
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS){
		view->speed*=1.3;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		view->speed/=1.3;
	}
	if(key == GLFW_KEY_A && action == GLFW_PRESS){
		control_obj = &(view->scene[0]);
		control_obj->is_rotating = !(control_obj->is_rotating);
	}
	if(key == GLFW_KEY_B && action == GLFW_PRESS){
		control_obj = &(view->scene[1]);
		control_obj->is_rotating = !(control_obj->is_rotating);
	}
	if(key == GLFW_KEY_C && action == GLFW_PRESS){
		control_obj = &(view->scene[2]);
		control_obj->is_rotating = !(control_obj->is_rotating);
	}
	if(key == GLFW_KEY_D && action == GLFW_PRESS){
		control_obj = &(view->scene[3]);
		control_obj->is_rotating = !(control_obj->is_rotating);
	}
	if(key == GLFW_KEY_1 && action == GLFW_PRESS){
		(view->state[0]) = 1-(view->state[0]);
	}
	if(key == GLFW_KEY_2 && action == GLFW_PRESS){
		(view->state[1]) = 1-(view->state[1]);
	}
	if(key == GLFW_KEY_3 && action == GLFW_PRESS){
		(view->state[2]) = 1-(view->state[2]);
	}
	if(key == GLFW_KEY_4 && action == GLFW_PRESS){
		(view->state[3]) = 1-(view->state[3]);
	}
	if(key == GLFW_KEY_T && action == GLFW_PRESS){
		view->texture_image_ind = ((view->texture_image_ind+1)%(view->texture_images.size()));
	}
	if(key == GLFW_KEY_M && action == GLFW_PRESS){
		view->mapping = ((view->mapping+1)%3);
	}
}

bool pressed;

void Control::handleMouse(GLFWwindow* window, int button, int action, int mods){
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec3 world = mouse(window,xpos,ypos);
		set_obj(world.x,world.y,world.z);
	}
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec3 world = mouse(window,xpos,ypos);
		if(control_obj!=NULL){
			control_obj->x_center = world.x;
			control_obj->y_center = world.y;
			control_obj->z_center = world.z;
		}
	}
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec3 world = mouse(window,xpos,ypos);
		Model* obj = set_obj(world.x,world.y,world.z);
		if(obj!=NULL)
			obj->is_rotating = !(obj->is_rotating);
	}
	// if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
	// 	pressed = true;
	// }
	// if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
	// 	pressed = false;
	// }
}


double rot_xpos, rot_ypos;

void Control::handleScroll(GLFWwindow* window,double x,double y){
	if(pressed){
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec3 win = mouse(window,xpos,ypos);
		xpos = win.x;
		ypos = win.y;
		rotate(rot_xpos,rot_ypos,xpos,ypos);
		rot_xpos = xpos;
		rot_ypos = ypos;
	}
}
