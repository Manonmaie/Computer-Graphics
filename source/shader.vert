#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrml;
layout (location = 2) in vec3 texture_coordinates;

uniform mat4 model;
uniform mat4 projection;
uniform vec3 clr;
uniform vec3 light_pos[4];
uniform int onoff[4];

out vec4 vCol;
out vec3 light_dir[4];
out vec3 nrml_dir;
out vec2 tex;

void main()
{
	gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0f);
	gl_PointSize = 10000.0;

	vec3 position_world = (model*vec4(pos,1.0f)).xyz;
	for(int i=0;i<4;i++){
		light_dir[i] = light_pos[i] - position_world;
	}
	nrml_dir = (model*vec4(nrml,0.0f)).xyz;
	nrml_dir = normalize(nrml_dir);

	tex = vec2(texture_coordinates.x,texture_coordinates.y);

	vCol = vec4(clr.x,clr.y,clr.z,1.0);
	//vCol = vec4(nrml_dir.x,nrml_dir.y,nrml_dir.z,1.0);
}
