#version 420

in vec3 a_Position;
in vec3 a_Normal;

out float v_NL;
out vec3 v_Reflect;
out vec3 v_View;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat3 NormalMatrix;

uniform vec4 LightPosition;

void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position.xyz, 1.0);

  vec4 mvpos = (ModelViewMatrix * vec4(a_Position.xyz, 1.0));
  vec3 normal = normalize(a_Normal * NormalMatrix);

  vec3 lightv = normalize(LightPosition.xyz - mvpos.xyz);

  v_Reflect = normalize(reflect(-lightv, normal));
  v_View = normalize(-mvpos.xyz);
  v_NL = 0.5 * (dot(lightv, normal) + 1.0);
}
