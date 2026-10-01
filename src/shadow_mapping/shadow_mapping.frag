#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;
uniform sampler2D woodTexture;
uniform sampler2D shadowMap;


float shadowCalculation(vec4 fragPosLightSpace) {
    vec3 frag_light_ndc = fragPosLightSpace.xyz / fragPosLightSpace.w;
    frag_light_ndc = frag_light_ndc * 0.5 + 0.5;
    float closest_depth = texture(shadowMap, frag_light_ndc.xy).r;
    float current_depth = frag_light_ndc.z;
    float bias = 0.005;
    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    vec3 lighting = texture(woodTexture, fs_in.TexCoords).rgb;

    float shadow = shadowCalculation(fs_in.FragPosLightSpace);
    if (shadow > 0.9) {
        lighting = lighting * 0.5;
    }
    FragColor = vec4(lighting, 1.0);
}
