#include "../include/View.h"

using namespace std;

const float toRadians = 3.14159265f / 180.0f;
int dir = 1;

void View::CreateScene(){
  projection = glm::ortho(-1,1,-1,1,-1,1);
  // int bufferWidth = 800;
  // int bufferHeight = 600;
  // projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

  texture_images.push_back("data/marble.jpg");
  texture_images.push_back("data/checkerboard.jpeg");
  texture_images.push_back("data/map.jpeg");
  texture_images.push_back("data/lena.png");

  Texture wood;
  wood.image_fname = "data/wood.jpg";
  floor.curr_texture = wood;
  floor.type = 2;
  floor.CompileShaders();
  floor.fName = "data/floor.ply";
  glm::vec3 col = glm::vec3(1,1,1);
	floor.color = col;
  floor.CreateMesh();
  floor.z_center = 0;
  floor.scale *= 2.3;

  Texture tex;
  tex.image_fname = texture_images[texture_image_ind];

  scene[0].curr_texture = tex;
  scene[0].CompileShaders();
  scene[0].fName = "data/cube.ply";
  glm::vec3 col0 = glm::vec3(1,1,1);
	scene[0].color = col0;
  scene[0].CreateMesh();
  scene[0].x_center = 0;
  scene[0].y_center = -0.75;
  scene[0].z_center = 0;
  scene[0].scale *= 0.3;

  scene[1].curr_texture = tex;
  scene[1].CompileShaders();
  scene[1].fName = "data/canstick.ply";
  glm::vec3 col1 = glm::vec3(1,1,1);
	scene[1].color = col1;
  scene[1].CreateMesh();
  scene[1].x_center = 0.35;
  scene[1].y_center = -0.75;
  scene[1].z_center = 0;
  scene[1].scale *= 0.3;

  scene[2].curr_texture = tex;
  scene[2].CompileShaders();
  scene[2].fName = "data/sphere.ply";
  glm::vec3 col2 = glm::vec3(1,1,1);
	scene[2].color = col2;
  scene[2].CreateMesh();
  scene[2].x_center = 0.75;
  scene[2].y_center = -0.75;
  scene[2].z_center = 0;
  scene[2].scale *= 0.3;

  scene[3].curr_texture = tex;
  scene[3].CompileShaders();
  scene[3].fName = "data/big_atc.ply";
  glm::vec3 col4 = glm::vec3(1.0,1.0,1.0);
  scene[3].color = col4;
  scene[3].CreateMesh();
  scene[3].x_center = 0.75;
  scene[3].y_center = -0.5;
  scene[3].z_center = 0;
  scene[3].scale *= 0.3;

  glm::vec3 white = glm::vec3(1.0,1.0,1.0);

  state[0] = 1;
  light[0].curr_texture = tex;
  light[0].type = 1;
  light[0].CompileShaders();
  light[0].fName = "data/circle.ply";
  light[0].color = glm::vec3(state[0],state[0],state[0]);
  light[0].CreateMesh();
  light[0].x_center = 0;
  light[0].y_center = 0.8;
  light[0].z_center = 0;
  light[0].scale *= 0.1;

  state[1] = 1;
  light[1].curr_texture = tex;
  light[1].type = 1;
  light[1].CompileShaders();
  light[1].fName = "data/circle.ply";
  light[1].color = glm::vec3(state[1],state[1],state[1]);
  light[1].CreateMesh();
  light[1].x_center = 0.25;
  light[1].y_center = 0.8;
  light[1].z_center = 0;
  light[1].scale *= 0.1;

  state[2] = 1;
  light[2].curr_texture = tex;
  light[2].type = 1;
  light[2].CompileShaders();
  light[2].fName = "data/circle.ply";
  light[2].color = glm::vec3(state[2],state[2],state[2]);
  light[2].CreateMesh();
  light[2].x_center = 0.75;
  light[2].y_center = 0.8;
  light[2].z_center = 0;
  light[2].scale *= 0.1;

  state[3] = 1;
  light[3].curr_texture = tex;
  light[3].type = 1;
  light[3].CompileShaders();
  light[3].fName = "data/circle.ply";
  light[3].color = glm::vec3(state[3],state[3],state[3]);
  light[3].CreateMesh();
  light[3].x_center = 0.75;
  light[3].y_center = 0.9;
  light[3].z_center = 0;
  light[3].scale *= 0.1;

  CreateSceneGraph();
}

void View::CreateSceneGraph(){
  floor.children.push_back(&scene[0]);
  scene[0].children.push_back(&light[0]);
  scene[0].children.push_back(&scene[1]);
  scene[1].children.push_back(&light[1]);
  scene[1].children.push_back(&scene[2]);
  scene[2].children.push_back(&light[2]);
  scene[2].children.push_back(&scene[3]);
  scene[3].children.push_back(&light[3]);
}

int wobble_time = 0;
float wobble = 0.03;

void View::animate(){
  float centerX,centerZ;
  float pointX,pointZ;
  centerX = scene[0].x_center;
  centerZ = scene[0].z_center;
  pointX = scene[1].x_center;
  pointZ = scene[1].z_center;
  float radius = sqrt(((centerX-pointX)*(centerX-pointX))+((centerZ-pointZ)*(centerZ-pointZ)));
  float angle = atan2(pointZ-centerZ,pointX-centerX);
  angle += speed/radius;

  if(wobble_time==1){
    wobble_time = 0;
    wobble = -1*wobble;
  }
  wobble_time++;

  scene[1].x_center = centerX+radius*cos(angle)+wobble;
  scene[1].z_center = centerZ+radius*sin(angle);
  scene[1].y_center = scene[0].y_center;
  light[1].x_center = scene[1].x_center;
  light[1].z_center = scene[1].z_center;

  centerX = scene[1].x_center;
  centerZ = scene[1].z_center;
  pointX = scene[2].x_center;
  pointZ = scene[2].z_center;
  angle = atan2(pointZ-centerZ,pointX-centerX);
  int dirx,dirz;
  if(centerX-pointX<0)
    dirx = -1;
  else
    dirx = 1;
  if(centerZ-pointZ<0)
    dirz = -1;
  else
    dirz = 1;
  scene[2].x_center = pointX + dirx*abs(cos(angle))*speed/2;
  scene[2].z_center = pointZ + dirz*abs(sin(angle))*speed/2;
  scene[2].y_center = scene[1].y_center;
  light[2].x_center = scene[2].x_center;
  light[2].z_center = scene[2].z_center;

  scene[3].x_center = scene[2].x_center;
  scene[3].z_center = scene[2].z_center;
  // scene[3].y_center = scene[2].y_center+;
  light[3].x_center = scene[3].x_center;
  light[3].z_center = scene[3].z_center;
  //y_center = -0.5 to 0.5
  float pointY = scene[3].y_center+dir*speed;
  if(pointY>scene[2].y_center+0.55)
    dir = -1;
  if(pointY<scene[2].y_center+0.25)
    dir = 1;
  scene[3].y_center += dir*speed;
  light[3].y_center = scene[3].y_center+1.0;

  for(int i=0;i<numObjs;i++){
    if(scene[i].is_rotating){
      scene[i].curAngle += 15;
      if (scene[i].curAngle >= 360)
      {
        scene[i].curAngle -= 360;
      }
    }
  }
}

void View::DrawScene(){
  Model* curr_model;
  std::queue<Model*> remaining;
  remaining.push(root_scene_graph);

  while (!remaining.empty()) {
    curr_model = remaining.front();

    for(int i=0;i<(curr_model->children).size();i++)
      remaining.push((curr_model->children)[i]);

    remaining.pop();

    glUseProgram(curr_model->shader);

    glUniformMatrix4fv(curr_model->uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(curr_model->x_center,curr_model->y_center,curr_model->z_center));
    // model *= curr_model->rotate_model;
    model = glm::rotate(model, curr_model->curAngle * toRadians, glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(curr_model->scale, curr_model->scale, curr_model->scale));
    glUniformMatrix4fv(curr_model->uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glm::vec3 lights[4] = {glm::vec3(light[0].x_center,light[0].y_center,light[0].z_center+1),glm::vec3(light[1].x_center,light[1].y_center,light[1].z_center+1)
                          ,glm::vec3(light[2].x_center,light[2].y_center,light[2].z_center+1),glm::vec3(light[3].x_center,light[3].y_center,light[3].z_center+1)};
    glUniform3fv(curr_model->lightPos,4,glm::value_ptr(lights[0]));
    glUniform1iv(curr_model->lightSwitch,4,&(state[0]));

    if(curr_model->type == 0){
      Texture tex;
      tex.image_fname = texture_images[texture_image_ind];
      curr_model->curr_texture = tex;

      (*curr_model).DrawMesh(mapping);
    }
    else
      (*curr_model).DrawMesh(0);

    glUseProgram(0);
  }
  animate();
}
