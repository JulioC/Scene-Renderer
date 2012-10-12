#version 330 core

struct Light_t {
  vec3 position;
  vec4 brightness;
};

struct Material_t {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

in vec4 v_Normal;
in vec4 v_Position;
in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D texMap;

uniform Material_t Material;
uniform Light_t Lights[1];

// TODO: change this to uniform
vec3 SurfaceColor = vec3(0.75, 0.75, 0.75);
vec3 WarmColor    = vec3(1.0, 1.0, 0.0);
vec3 CoolColor    = vec3(0.0, 0.0, 1.0);
float DiffuseWarm = 0.45;
float DiffuseCool = 0.045;

void main() {
FragColor = texture2D(texMap,v_TexCoords);
FragColor.a = 1.0;


}



