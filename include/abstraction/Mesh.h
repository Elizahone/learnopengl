#pragma once
#include <cstddef>
#include <vector>
#include <assimp/types.h>
#include <glm/glm.hpp>
#include <abstraction/Shader.h>
#include <glad/glad.h>


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    std::string  type;
    aiString     path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    }

    void draw(Shader& shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
            // 获取纹理序号（diffuse_textureN 中的 N）
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setUniform(("material." + name + number).c_str(), (int)i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // 绘制网格
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // position, normal, texture
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};
