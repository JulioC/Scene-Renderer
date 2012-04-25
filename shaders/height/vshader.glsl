#version 330

in vec3 a_Position;
in vec3 a_Normal;

out vec4 v_Position;
out vec4 v_Normal;
out float v_Height;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat3 NormalMatrix;

void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position.xyz, 1.0);
  v_Position = ModelViewMatrix * vec4(a_Position.xyz, 1.0);
  v_Normal = vec4(NormalMatrix * a_Normal, 0.0);
  v_Height = 0.5 + (a_Position.y * 2);
}