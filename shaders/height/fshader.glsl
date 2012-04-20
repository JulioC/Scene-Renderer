#version 330 core

in float v_Height;

out vec4 FragColor;

void main() {
  FragColor.g = 0.8*v_Height;
  FragColor.b = 0.1*v_Height;
  FragColor.r = 0.6;

  FragColor.a = 1.0;
}



