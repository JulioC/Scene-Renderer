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

out vec4 FragColor;

uniform Material_t Material;
uniform Light_t Lights[1];

// TODO: change this to uniform
vec3 SurfaceColor = vec3(0.75, 0.75, 0.75);
vec3 WarmColor    = vec3(1.0, 1.0, 0.0);
vec3 CoolColor    = vec3(0.0, 0.0, 1.0);
float DiffuseWarm = 0.45;
float DiffuseCool = 0.045;

void main() {
  vec3 N  = normalize(v_Normal.xyz);
  vec3 L = normalize(Lights[0].position - v_Position.xyz);

  vec3 V = normalize(-v_Position.xyz);
  vec3 R = reflect(-L, N);

  vec3 ambient = Material.ambient.rgb;
  vec3 diffuse = Material.diffuse.rgb * max(dot(L, N), 0.0);

  vec3 kcool    = min(CoolColor + DiffuseCool * vec3(1), 1.0);
  vec3 kwarm    = min(WarmColor + DiffuseWarm * vec3(1), 1.0);
  vec3 kfinal   = mix(kcool, kwarm, dot(N,L)) * 1.0 * Material.diffuse.rgb;

  FragColor.rgb = Lights[0].brightness.rgb * (ambient + diffuse * kfinal);

  if (dot(L, N) < 0.2)
    FragColor.rgb *= 0.4;
}



