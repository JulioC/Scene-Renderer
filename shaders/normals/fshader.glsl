#version 420

in vec4 v_Normal;

void main() {
  gl_FragColor.rgb = v_Normal.rgb;
  gl_FragColor.a = 1.0;
}
