#version 330

struct Light_t {
  vec3 position;
  vec4 brightness;
};

struct Material_t {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

in vec4 v_Position;
in vec4 v_Normal;

out vec4 FragColor;

uniform Material_t Material;
uniform Light_t Lights[1];

// TODO: change this to uniform
const int Shininess = 1000;
const float Toonish = 6.0;

void main()
{
  vec3 N  = normalize(v_Normal.xyz);
  vec3 L = normalize(Lights[0].position - v_Position.xyz);

  vec3 V = normalize(-v_Position.xyz);
  vec3 R = reflect(-L, N);

  vec3 ambient = Material.ambient.rgb;
  vec3 diffuse = Material.diffuse.rgb * max(dot(L, N), 0.0);
  vec3 specular = Material.specular.rgb * pow(max(dot(R, V), 0.0), Shininess);

  FragColor.rgb = Lights[0].brightness.rgb * (ambient + floor((diffuse + specular) * Toonish) / Toonish);
  FragColor.a = 1.0;
}