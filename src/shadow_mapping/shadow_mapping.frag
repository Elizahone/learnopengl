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
uniform vec3 lightPos;
uniform mat4 view;


float shadowCalculation(vec4 fragPosLightSpace, float bias) {
    vec3 frag_light_ndc = fragPosLightSpace.xyz / fragPosLightSpace.w;
    frag_light_ndc = frag_light_ndc * 0.5 + 0.5;
    float current_depth = frag_light_ndc.z;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float shadow = 0.0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            float pcfDepth = texture(shadowMap, frag_light_ndc.xy + vec2(i, j) * texelSize).r;
            if (pcfDepth > 0.999) continue;
            shadow += current_depth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
}

void main() {
    vec3 color = texture(woodTexture, fs_in.TexCoords).rgb;
    vec3 normal = fs_in.Normal;
    vec3 lightColor = vec3(1.0);

    vec3 ambient = 0.15 * lightColor;

    // diffuse
    vec3 lightDir = normalize(vec3(view * vec4(lightPos, 1.0)) - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 view_dir = normalize(-fs_in.FragPos);
    vec3 half_vec = normalize(view_dir + lightDir);
    float spec = pow(max(dot(half_vec, normal), 0.0), 64);
    vec3 specular = spec * lightColor;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = shadowCalculation(fs_in.FragPosLightSpace, bias);
    vec3 lighting = (ambient + (1 - shadow) * (specular + diffuse)) * color;

    FragColor = vec4(lighting, 1.0);
}
