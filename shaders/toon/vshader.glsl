#version 420

in vec3 a_Position;
in vec3 a_Normal;

out vec4 v_Normal;

uniform mat4 ModelViewProjectionMatrix;

void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position.xyz, 1.0);
  v_Normal = vec4(a_Normal, 1.0);
}
