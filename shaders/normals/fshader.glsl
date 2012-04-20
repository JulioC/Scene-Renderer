#version 330

in vec4 v_Normal;

out vec4 FragColor;

void main() {
  FragColor.rgb = v_Normal.xyz;
  FragColor.a = 1.0;
}
