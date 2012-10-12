#version 330 core

struct Light_t {
  vec3 position;
  vec4 brightness;
};

struct Material_t {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

in vec4 v_Position;
in vec4 v_Normal;
in vec4 v_Tangent;
in vec4 v_Bitangent;
in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D texColorMap;
uniform sampler2D texBumpMap;

uniform Material_t Material;
uniform Light_t Lights[1];

// TODO: change this to uniform
const int Shininess = 10;

void main() {
  vec3 normal = normalize(v_Normal.xyz);
  vec3 tangent = normalize(v_Tangent.xyz);
  vec3 bitangent = normalize(v_Bitangent.xyz);

  mat3 TBN = mat3(tangent.x, bitangent.x, normal.x,
                  tangent.y, bitangent.y, normal.y,
                  tangent.z, bitangent.z, normal.z);

  vec3 N  = normalize(texture2D(texBumpMap, v_TexCoords).rgb);

  vec3 L = normalize(TBN * (Lights[0].position - v_Position.xyz));

  vec3 V = normalize(TBN * (-v_Position.xyz));
  vec3 R = reflect(-L, N);

  vec3 ambient = Material.ambient.rgb;
  vec3 diffuse = Material.diffuse.rgb * max(dot(L, N), 0.0);
  vec3 specular = Material.specular.rgb * pow(max(dot(R, V), 0.0), Shininess);

  FragColor.rgb = Lights[0].brightness.rgb * (ambient + diffuse + specular) * texture2D(texColorMap, v_TexCoords).rgb;
  FragColor.a = 1.0;
}



