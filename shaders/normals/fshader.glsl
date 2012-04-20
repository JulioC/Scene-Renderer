#version 330

in vec4 v_Normal;

void main() {
  gl_FragColor.rgb = v_Normal.xyz;
  gl_FragColor.a = 1.0;
}
