#version 330 core
in vec4 FragPos; // view space
uniform float far_plane;

void main() {
    // light position in view space is (0, 0, 0)
    float depth = length(FragPos.xyz);
    depth /= far_plane;
    gl_FragDepth = depth;
}
