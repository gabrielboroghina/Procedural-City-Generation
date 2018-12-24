#include "MeshBuilder.h"

#include "Core/GPU/Mesh.h"
#include "include/gl.h"
#include "UIConstants.h"

MeshBuilder::MeshBuilder() {}

MeshBuilder::~MeshBuilder() {}

Mesh *MeshBuilder::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices,
                              const std::vector<unsigned short> &indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Crete the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section describes how the GPU Shader Vertex Shader program receives data

    // set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
                          (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    Mesh *mesh = new Mesh(name);
    mesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
    mesh->vertices = vertices;
    mesh->indices = indices;
    return mesh;
}

Mesh *MeshBuilder::CreateDisk(float radius, glm::vec3 centerPos)
{
    glm::vec3 color(0);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned short> indices;

    // Insert center
    vertices.push_back(VertexFormat(centerPos, color - glm::vec3(10, 10, 10))); // center
    vertices.push_back(VertexFormat(centerPos + glm::vec3(radius, 0, 0), color)); // first point

    for (int u = 1; u <= 360; u++) {
        vertices.push_back(VertexFormat(glm::vec3(centerPos + glm::vec3(radius * cos(u), 0, radius * sin(u))), color));

        indices.push_back(0);
        indices.push_back(u);
        indices.push_back(u + 1);
    }

    // Create a new mesh from the buffer data
    return CreateMesh("disk", vertices, indices);
}

Mesh *MeshBuilder::CreateRect(glm::vec3 center, float height, float width, glm::vec3 color)
{
    height /= 2;
    width /= 2;
    std::vector<VertexFormat> vertices = {
        VertexFormat(center + glm::vec3(-width, 0, -height), color, glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        VertexFormat(center + glm::vec3(width, 0, -height), color, glm::vec3(0, 1, 0), glm::vec2(0, UIConstants::Map::MAX)),
        VertexFormat(center + glm::vec3(width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(1, 4)),
        VertexFormat(center + glm::vec3(-width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(0, 4))
    };

    Mesh *rect = new Mesh("rect");
    std::vector<unsigned short> indices = {0, 1, 2, 3};

    // draw 2 triangles. Add the remaining 2 indices
    indices.push_back(0);
    indices.push_back(2);

    rect->InitFromData(vertices, indices);
    return rect;
}

Mesh *MeshBuilder::CreateVertRect(glm::vec3 center, float height, float width)
{
    height /= 2;
    width /= 2;
    glm::vec3 color(0);
    std::vector<VertexFormat> vertices = {
        VertexFormat(center + glm::vec3(-width, 0, -height), color, glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        VertexFormat(center + glm::vec3(width, 0, -height), color, glm::vec3(0, 1, 0), glm::vec2(1, 0)),
        VertexFormat(center + glm::vec3(width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(1, 10 * UIConstants::Map::MAX)),
        VertexFormat(center + glm::vec3(-width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(0, 10 * UIConstants::Map::MAX))
    };

    Mesh *rect = new Mesh("rect");
    std::vector<unsigned short> indices = {0, 1, 2, 3};

    // draw 2 triangles. Add the remaining 2 indices
    indices.push_back(0);
    indices.push_back(2);

    rect->InitFromData(vertices, indices);
    return rect;
}
