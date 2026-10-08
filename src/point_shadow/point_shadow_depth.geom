#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadowViews[6];
uniform mat4 perspective;
out vec4 FragPos;

void main() {
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;
        for (int i = 0; i < 3; i++) {
            FragPos = shadowViews[face] * gl_in[i].gl_Position; // view space
            gl_Position = perspective * FragPos; // ndc
            EmitVertex();
        }
        EndPrimitive();
    }
}
