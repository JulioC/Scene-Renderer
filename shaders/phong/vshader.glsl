#version 420

in vec3 a_Position;
in vec3 a_Normal;

out vec4 v_Normal;
out vec4 v_Position;

uniform mat4 ModelViewProjectionMatrix;
uniform mat3 NormalMatrix;

void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position, 1.0);
  v_Position = gl_Position;
  v_Normal = normalize(vec4(NormalMatrix * a_Normal, 1.0));
}
