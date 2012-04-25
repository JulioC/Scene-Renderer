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

  float diffuse = dot(L, N);
  if (diffuse > 0.98)
    diffuse = 1.0;
  else if (diffuse > 0.5)
    diffuse = 0.8;
  else if (diffuse > 0.35)
    diffuse = 0.4;
  else
    diffuse = 0.0;

  float specular = pow(max(dot(R, V), 0.0), Shininess);
  specular = step(0.5, specular);

  FragColor.rgb = Lights[0].brightness.rgb * (Material.ambient.rgb + Material.diffuse.rgb * diffuse  + Material.specular.rgb * specular);
  FragColor.a = 1.0;
}