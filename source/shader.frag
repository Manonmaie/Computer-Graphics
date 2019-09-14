#version 330

in vec4 vCol;
in vec3 light_dir[4];
in vec3 nrml_dir;
in vec2 tex;

uniform int onoff[4];
uniform sampler2D sel_texture;

out vec4 colour;

vec4 compute_col(vec3 light,vec4 col)
{
  vec3 n = normalize(nrml_dir);
  vec3 l = normalize(light);
  float cosTheta = dot(n,l);
  float dist = length(light);
  if(cosTheta<0)
    cosTheta = 0;
  vec3 E = vec3(0.0,0.0,1.0);
  vec3 R = normalize(reflect(-l,n));
  float cosAlpha = dot(E,R);
  if(cosAlpha<0)
    cosAlpha = 0;
  vec4 SpecCol = col;

  vec4 DiffuseColor = col*cosTheta/(dist*dist);
  vec4 AmbientColor = vec4(0.005,0.005,0.005,1.0) * col;
  vec4 SpecularColor = SpecCol*pow(cosAlpha,25)/(dist*dist);

  return DiffuseColor+AmbientColor+SpecularColor;
}

void main()
{
  colour = vec4(0,0,0,0);
  for(int i=0;i<4;i++){
    if(onoff[i] == 1)
      colour += compute_col(light_dir[i],vCol);
  }
  //colour = vCol;
  colour = texture(sel_texture,tex)*colour;
}
