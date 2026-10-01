#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

void main() {
    mat4 mv = view * model;
    vs_out.FragPos = vec3(mv * vec4(aPos, 1.0));
    vs_out.Normal = vec3(transpose(inverse(mv)) * vec4(aNormal, 0.0));
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);
    gl_Position = projection * mv * vec4(aPos, 1.0);
}
