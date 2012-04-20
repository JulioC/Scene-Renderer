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

in vec3 a_Position;
in vec3 a_Normal;

out vec4 v_Color;

uniform Material_t Material;
uniform Light_t Lights[1];

uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat3 NormalMatrix;

// TODO: change this to uniform
const int Shininess = 1000;

void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position, 1.0);

  vec4 position = ModelViewMatrix * vec4(a_Position, 1.0);

  vec3 N = normalize(NormalMatrix * a_Normal.xyz);
  vec3 L = normalize(Lights[0].position - position.xyz);

  vec3 V = normalize(-position.xyz);
  vec3 H = normalize(L + V);

  vec3 ambient = Material.ambient.rgb;
  vec3 diffuse = Material.diffuse.rgb * max(dot(L, N), 0.0);
  vec3 specular = Material.specular.rgb * pow(max(dot(N, H), 0.0), Shininess);

  v_Color.rgb = Lights[0].brightness.rgb * (ambient + diffuse + specular);
  v_Color.a = 1.0;
}
