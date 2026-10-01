#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <initializer_list>
#include <memory>
#include <stdint.h>
#include <assert.h>
#include <string>
#include <vector>


class IndexBuffer {
    public:

        IndexBuffer(uint32_t *indices, uint32_t count) : m_count(count){
            glGenBuffers(1, &m_ID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        }
        ~IndexBuffer() {
            glDeleteBuffers(1, &m_ID);
        }

        void bind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        }
        void unbind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    private:
        uint32_t m_ID;
        uint32_t m_count;
};

enum class ShaderDataType {
    None = 0,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    Int, Int2, Int3, Int4,
    Bool,
};

static uint32_t shaderDataTypeSize(ShaderDataType type) {
    switch(type) {
        case ShaderDataType::Float:     return 4;
        case ShaderDataType::Float2:    return 4 * 2;
        case ShaderDataType::Float3:    return 4 * 3;
        case ShaderDataType::Float4:    return 4 * 4;
        case ShaderDataType::Mat3:      return 4 * 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4 * 4;
        case ShaderDataType::Int:       return 4;
        case ShaderDataType::Int2:      return 4 * 2;
        case ShaderDataType::Int3:      return 4 * 3;
        case ShaderDataType::Int4:      return 4 * 4;
        case ShaderDataType::Bool:      return 1;
        case ShaderDataType::None:      return 0;
    }
    return 0;
}

struct BufferElement {
    std::string name;
    ShaderDataType type;
    uint32_t size;

    uint32_t offset;
    bool normalized;

    BufferElement() {}
    BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
        : name(name), type(type), size(shaderDataTypeSize(type)), offset(0), normalized(normalized) {}

    uint32_t getComponentCount() const {
        switch(type) {
            case ShaderDataType::Float:     return 1;
            case ShaderDataType::Float2:    return 2;
            case ShaderDataType::Float3:    return 3;
            case ShaderDataType::Float4:    return 4;
            case ShaderDataType::Mat3:      return 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4;
            case ShaderDataType::Int:       return 1;
            case ShaderDataType::Int2:      return 2;
            case ShaderDataType::Int3:      return 3;
            case ShaderDataType::Int4:      return 4;
            case ShaderDataType::Bool:      return 1;
            case ShaderDataType::None:      return 0;
        }
        return 0;
    }
    GLenum baseType() const {
        switch(type) {
            case ShaderDataType::Float: 
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
            case ShaderDataType::None: return 0;
        }
        return 0;
    }

};

class BufferLayout {
    public:
        BufferLayout() {}
        BufferLayout(const std::initializer_list<BufferElement>& elements) : m_elements(elements) {
            calculateOffsetAndStride();
        }

        inline uint32_t getStride() const { return m_stride; }
        inline const std::vector<BufferElement>& getElements() const { return m_elements; }

        std::vector<BufferElement>::iterator        begin() { return m_elements.begin(); }
        std::vector<BufferElement>::iterator        end() { return m_elements.end(); }
        std::vector<BufferElement>::const_iterator  begin() const { return m_elements.begin(); }
        std::vector<BufferElement>::const_iterator  end() const { return m_elements.end(); }

    private:
        std::vector<BufferElement> m_elements;
        uint32_t m_stride;

        void calculateOffsetAndStride() {
            uint32_t offset = 0;
            m_stride = 0;
            for (auto& element: m_elements) {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }
};


class VertexBuffer {
    public:
        VertexBuffer(float *vertices, uint32_t size) {
            glGenBuffers(1, &m_ID);
            glBindBuffer(GL_ARRAY_BUFFER, m_ID);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        }
        ~VertexBuffer() { glDeleteBuffers(1, &m_ID); }
        void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_ID); }
        void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

        const BufferLayout& getLayout() const { return m_layout; }

        void setLayout(const BufferLayout layout) { m_layout = layout; }
    private:
        uint32_t m_ID;
        BufferLayout m_layout;
};

class VertexArray {
    public:
        VertexArray() { glGenVertexArrays(1, &m_ID); }
        ~VertexArray() { glDeleteVertexArrays(1, &m_ID); }
        inline void bind() const { glBindVertexArray(m_ID); }
        inline void unbind() const { glBindVertexArray(0); }

        void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
            glBindVertexArray(m_ID);
            vertexBuffer->bind();
            const auto& layout = vertexBuffer->getLayout();
            uint32_t stride = layout.getStride();
            for (const auto& element: layout) {
                glEnableVertexAttribArray(m_location_index);
                glVertexAttribPointer(m_location_index,
                                      element.getComponentCount(),
                                      element.baseType(),
                                      element.normalized,
                                      stride,
                                      (const void*) (unsigned long long)element.offset);
                m_location_index++;
            }
            m_vertexBuffers.push_back(vertexBuffer);
        }

        void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
            glBindVertexArray(m_ID);
            indexBuffer->bind();
            m_indexBuffer = indexBuffer;
        }
    private:
        uint32_t m_ID;
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        uint32_t m_location_index = 0;
};
