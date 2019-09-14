#include "../include/Model.h"

using namespace std;

float PI = 3.14159;

string Model::readFile(const char* fileLocation){
	string content;
	ifstream fileStream(fileLocation, ios::in);
		if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Model::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType){
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void Model::CompileShaders(){
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Failed to create shader\n");
		return;
	}

	string vShaderStr = readFile("source/shader.vert");
	static const char* vShader = vShaderStr.c_str();

	string fShaderStr = readFile("source/shader.frag");
	static const char* fShader = fShaderStr.c_str();

	// cout<<glIsShader(shader)<<endl;
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
	Col = glGetUniformLocation(shader,"clr");
	lightPos = glGetUniformLocation(shader, "light_pos");
	lightSwitch = glGetUniformLocation(shader, "onoff");
	uniformTexture = glGetUniformLocation(shader, "sel_texture");
}

void Model::readPLY(){
	string buffer;
	ifstream file;
	file.open(fName);

	if (!file.is_open()) {
		printf("Failed to read %s! File doesn't exist.\n", fName);
		return;
	}
	buffer = "";
	while(buffer!="vertex"){
		file>>buffer;
	}
	file>>no_vertices;
	vertices = (GLfloat*) malloc(3*no_vertices*sizeof(GLfloat));
	vertex_nrml = (GLfloat*) malloc(3*no_vertices*sizeof(GLfloat));
	while(buffer!="face"){
		file>>buffer;
	}
	file>>no_faces;
	indices = (unsigned int*) malloc(3*no_faces*sizeof(unsigned int));
	face_nrml = (GLfloat*) malloc(3*no_faces*sizeof(GLfloat));
	tex_coords = (GLfloat*) malloc(3*no_vertices*sizeof(GLfloat));
	while(buffer != "end_header")
		file>>buffer;
	float num;
	file>>num;
	vertices[0] = num;
	min_x = num;
	max_x = num;
	file>>num;
	vertices[1] = num;
	min_y = num;
	max_y = num;
	file>>num;
	vertices[2] = num;
	min_z = num;
	max_z = num;
	for (int i=3;i<3*no_vertices;i++){
		file>>num;
		vertices[i] = num;
		if(i%3 == 0) {//change x
			min_x = min(min_x,num);
			max_x = max(max_x,num);
		}
		else if(i%3 == 1) {//change y
			min_y = min(num,min_y);
			max_y = max(num,max_y);
		}
		else if(i%3 == 2) {//change z
			min_z = min(num,min_z);
			max_z = max(num,max_z);
		}
	}
	float x = max_x - min_x;
	float y = max_y - min_y;
	float z = max_z - min_z;
	max_diff = max(max(x,y),z);
	scale = 1/max_diff;
	x_center = (min_x+max_x)/2;
	y_center = (min_y+max_y)/2;
	z_center = (min_z+max_z)/2;
	x_center_orgi = (min_x+max_x)/2;
	y_center_orgi = (min_y+max_y)/2;
	z_center_orgi = (min_z+max_z)/2;
	unsigned int ind;
	for(int i=0;i<no_faces;i++){
		file>>ind;
		for(int j=0;j<3;j++){
			file>>ind;
			indices[(3*i)+j]=ind;
		}
	}

	glUseProgram(shader);
  glUniform3fv(Col,1,&color[0]);
}

void Model::ComputeNormals(){
	GLfloat e1x,e1y,e1z,e2x,e2y,e2z;
	glm::dvec3 edge1,edge2,cross_prod;
	unsigned int face1,face2,face3;
	for(int i=0;i<no_faces;i++){
		face1 = indices[3*i];
		face2 = indices[3*i+1];
		face3 = indices[3*i+2];

		e1x = vertices[3*face2]-vertices[3*face1];
		e1y = vertices[3*face2+1]-vertices[3*face1+1];
		e1z = vertices[3*face2+2]-vertices[3*face1+2];
		e2x = vertices[3*face3]-vertices[3*face1];
		e2y = vertices[3*face3+1]-vertices[3*face1+1];
		e2z = vertices[3*face3+2]-vertices[3*face1+2];

		edge1 = glm::dvec3(e1x,e1y,e1z);
		edge2 = glm::dvec3(e2x,e2y,e2z);
		cross_prod = glm::normalize(glm::cross(edge1,edge2));
		face_nrml[3*i] = cross_prod.x;
		face_nrml[3*i+1] = cross_prod.y;
		face_nrml[3*i+2] = cross_prod.z;
	}
	glm::dvec3 cum;
	unsigned int triangle_num,count;
	for(int j=0;j<no_vertices;j++){
		cum.x = 0;
		cum.y = 0;
		cum.z = 0;
		count = 0;
		for(int i=0;i<3*no_faces;i++){
			if(indices[i]==j){
				triangle_num = i/3;
				cum.x += face_nrml[3*triangle_num];
				cum.y += face_nrml[3*triangle_num+1];
				cum.z += face_nrml[3*triangle_num+2];
				count++;
			}
		}
		cum.x = cum.x/count;
		cum.y = cum.y/count;
		cum.z = cum.z/count;
		cum = glm::normalize(cum);
		vertex_nrml[3*j] = cum.x;
		vertex_nrml[3*j+1] = cum.y;
		vertex_nrml[3*j+2] = cum.z;
	}
}

glm::vec2 Model::cylinder_coord(glm::vec3 position){
	glm::vec2 map_coords;
	glm::vec3 n;
	// n = position;
	n.x = position.x - x_center_orgi;
	n.y = position.y - y_center_orgi;
	n.z = position.z - z_center_orgi;
	n = normalize(n);

	// float rad = (max_x - min_x) * 0.5;
	// map_coords.x = acosf(n.x/rad) / (2 * PI);
	// map_coords.y = n.z;

	float height = (max_y-min_y);
	map_coords.x = (atan2(n.z,n.x)+PI)/(2*PI);
	map_coords.y = (position.y - min_y)/height;

	// cout<<map_coords.x<<" "<<map_coords.y<<endl;
	return map_coords;
}

glm::vec2 Model::spherical_coord(glm::vec3 position){
	glm::vec2 map_coords;
	glm::vec3 n;
	// n = position;
	n.x = position.x - x_center_orgi;
	n.y = position.y - y_center_orgi;
	n.z = position.z - z_center_orgi;
	n = normalize(n);

	double theta = atan2(-1*n.z, n.x );
	map_coords.x = (theta + PI) / (2*PI);
	double phi = acos(-1*n.y);
	map_coords.y = (phi) / PI;

	// map_coords.x = 0.5+(atan2(n.z,n.x)/(2*PI));
	// map_coords.y = 0.5-(asin(n.y)/(2*PI));

	// map_coords.x = atan2(sqrt((n.y*n.y)+(n.z*n.z)),n.x)/(2*PI);
	// map_coords.y = atan2(n.z,n.y)/(2*PI);

	// cout<<map_coords.x<<" "<<map_coords.y<<endl;
	return map_coords;
}

void Model::ComputeTextureCoods(int n){
	for(int i=0;i<no_vertices;i++){
		if(n == 0){
			glm::vec2 tex = cylinder_coord(glm::vec3(vertices[3*i],vertices[3*i+1],vertices[3*i+2]));
			tex_coords[3*i] = tex.x;
			tex_coords[3*i+1] = tex.y;
			tex_coords[3*i+2] = 0;
		}
		else if(n == 1){
			glm::vec2 tex = spherical_coord(glm::vec3(vertices[3*i],vertices[3*i+1],vertices[3*i+2]));
			tex_coords[3*i] = tex.x;
			tex_coords[3*i+1] = tex.y;
			tex_coords[3*i+2] = 0;
		}
		else if(n == 2){
			tex_coords[3*i] = (vertices[3*i]-min_x)/(max_x-min_x);
			tex_coords[3*i+1] = (vertices[3*i+1]-min_y)/(max_y-min_y);
			tex_coords[3*i+2] = 0;
		}
	}
}

void Model::CreateMesh(){
	readPLY();
	ComputeNormals();
	ComputeTextureCoods(0);
	curr_texture.GenerateTexture();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, no_faces*3*sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glGenBuffers(3, VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, no_vertices*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, no_vertices*3*sizeof(GLfloat), vertex_nrml, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, no_vertices*3*sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Model::TextureMapping(){
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, no_vertices*3*sizeof(GLfloat), tex_coords, GL_STATIC_DRAW);
}

void Model::DrawMesh(int map){
	ComputeTextureCoods(map); //TODO
	curr_texture.GenerateTexture();
	TextureMapping();
	curr_texture.ApplyTexture();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES,3* no_faces, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::DrawWiredMesh(){
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//to make the drwing triangle unfilled
	glDrawElements(GL_TRIANGLES,3* no_faces, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::DrawVertices(){
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glPointSize(1.5f);
	glDrawElements(GL_POINTS,3* no_faces, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
