#ifndef VIEW_H
#define VIEW_H

#include "Model.h"
#include<bits/stdc++.h>


using namespace std;

class View{
  public:
    Model scene[4];
    Model light[4];
    Model floor;
    int state[4];
    int mapping = 0;
    std::vector<const char*> texture_images;
    int texture_image_ind = 0;
    glm::mat4 projection;
    int bufferWidth;
    int bufferHeight;
    int numObjs = 4;
    Model* root_scene_graph = &floor;

    float speed = 0.03;

  public:
    void CreateScene();
    void DrawScene();
    void CreateSceneGraph();
    void animate();
};

#endif
