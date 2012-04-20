#version 420

in vec4 v_Normal;
in vec4 v_Position;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

void main() {
  vec3 N  = normalize(v_Normal.xyz);
  vec3 L = normalize(LightPosition.xyz - v_Position.xyz);

  vec3 V = normalize(-v_Position).xyz;
  vec3 H = normalize(L + V);

  vec3 ambient = AmbientProduct.rgb;
  vec3 diffuse = DiffuseProduct.rgb * max(dot(L, N), 0.0);
  vec3 specular = SpecularProduct.rgb * pow(max(dot(N, H), 0.0), Shininess);

  gl_FragColor.rgb = ambient + diffuse + specular;
  gl_FragColor.a = 1.0;
}



