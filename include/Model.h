#ifndef MODEL_H
#define MODEL_H

#include <bits/stdc++.h>
#include <cmath>

#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glm/glm.hpp>

#include "Texture.h"

using namespace std;

class Model{

	public:
	string fName;
	GLuint VAO,VBO[3],IBO;
	GLfloat *vertices;
	GLfloat *vertex_nrml;
	GLfloat *tex_coords;
	glm::vec3 color;
	unsigned int *indices;
	GLfloat *face_nrml;
	int no_vertices,no_faces;
	float min_x,min_y,min_z,max_x,max_y,max_z;
	float x_center,y_center,z_center,proj_dist;
	float x_center_orgi,y_center_orgi,z_center_orgi;
	float max_diff; // for normalising into unit cube
	float scale;
	float curAngle = 0;
	bool is_rotating = false;
	GLuint shader, uniformModel, uniformProjection, lightPos,Col,lightSwitch;
	GLuint uniformTexture;
	Texture curr_texture;

	vector<Model*> children;
	int type = 0;//0 for objects, 1 for lights and 2 for floor

	glm::quat rot_prev = glm::quat(0,0,0,1);
	glm::mat4 rotate_model = glm::mat4(1.0f);

	public:
  string readFile(const char* fileLocation);
  void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
  void CompileShaders();
  void readPLY();
  void CreateMesh();
  void DrawMesh(int map);
  void DrawWiredMesh();
	void DrawVertices();
	void ComputeNormals();
	void ComputeTextureCoods(int n);
	void TextureMapping();
	glm::vec2 spherical_coord(glm::vec3 position);
	glm::vec2 cylinder_coord(glm::vec3 position);
};

#endif
