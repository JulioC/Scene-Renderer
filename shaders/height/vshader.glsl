#version 330 core

attribute vec3 a_Position;

out float v_Height;

uniform mat4 ModelViewProjectionMatrix;
void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position, 1.0);
  v_Height = 0.5 + (a_Position.y * 2);
}
