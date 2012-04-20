#version 330

in vec4 v_Normal;
in vec4 v_Position;

uniform Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

void main() {

  gl_FragColor.rgb = ambient.rgb;
  gl_FragColor.a = 1.0;
}



