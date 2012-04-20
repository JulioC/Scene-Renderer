#version 420

in vec3 a_Position;
in vec3 a_Normal;

out vec4 v_Color;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat3 NormalMatrix;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

void main() {
  gl_Position = ModelViewProjectionMatrix * vec4(a_Position, 1.0);

  vec4 position = ModelViewMatrix * vec4(a_Position, 1.0);

  vec3 N = normalize(NormalMatrix * a_Normal.xyz);

  vec3 L = normalize(LightPosition.xyz - position.xyz);

  vec3 V = normalize(-position).xyz;
  vec3 H = normalize(L + V);

  vec3 ambient = AmbientProduct.rgb;
  vec3 diffuse = DiffuseProduct.rgb * max(dot(L, N), 0.0);
  vec3 specular = SpecularProduct.rgb * pow(max(dot(N, H), 0.0), Shininess);

  v_Color.rgb = ambient + diffuse + specular;
  v_Color.a = 1.0;
}
