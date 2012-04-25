#version 330 core

in vec4 v_Position;
in vec4 v_Normal;
in float v_Height;

out vec4 FragColor;

void main() {
  vec3 color = vec3(0.8*v_Height, 0.1*v_Height, 0.6);

  vec3 lightdir = -v_Position.xyz;

  float i = max(dot(normalize(lightdir), normalize(v_Normal.xyz)), 0.0);

  FragColor.xyz = color * i;
  FragColor.a = 1.0;
}
