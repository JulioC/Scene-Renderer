#version 330 core

in vec3 a_Position;
in vec3 a_Normal;
in vec4 a_Tangent;
in vec3 a_Bitangent;
in vec2 a_TexCoords;

out vec4 v_Position;
out vec4 v_Normal;
out vec4 v_Tangent;
out vec4 v_Bitangent;
out vec2 v_TexCoords;

uniform mat4 ModelViewProjectionMatrix;
uniform mat3 NormalMatrix;

void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position, 1.0);
  v_Position = gl_Position;
  v_Normal = vec4(NormalMatrix * a_Normal, 1.0);
  v_Tangent = vec4(NormalMatrix * a_Tangent.xyz, a_Tangent.w);
  v_Bitangent = vec4(NormalMatrix * a_Bitangent.xyz, 1.0);;
  v_TexCoords = a_TexCoords;
}
