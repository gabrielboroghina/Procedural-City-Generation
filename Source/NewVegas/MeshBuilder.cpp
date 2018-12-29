#include "MeshBuilder.h"

#include "Core/GPU/Mesh.h"
#include "include/gl.h"
#include "UIConstants.h"

constexpr float PI = 3.14159265358979323846f;

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
        VertexFormat(center + glm::vec3(width, 0, -height), color, glm::vec3(0, 1, 0), glm::vec2(1, 0)),
        VertexFormat(center + glm::vec3(width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(1, 1)),
        VertexFormat(center + glm::vec3(-width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(0, 1))
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
        VertexFormat(center + glm::vec3(width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(1, 16 * UIConstants::Map::MAX)),
        VertexFormat(center + glm::vec3(-width, 0, height), color, glm::vec3(0, 1, 0), glm::vec2(0, 16 * UIConstants::Map::MAX))
    };

    Mesh *rect = new Mesh("rect");
    std::vector<unsigned short> indices = {0, 1, 2, 3};

    // draw 2 triangles. Add the remaining 2 indices
    indices.push_back(0);
    indices.push_back(2);

    rect->InitFromData(vertices, indices);
    return rect;
}

std::tuple<Mesh*, Mesh*, std::tuple<float, float, float, float>>
MeshBuilder::CreateCylinder(float height, float rad, int numFaces, bool degenerate)
{
    Mesh *cyl = new Mesh("cyl");
    Mesh *roof = new Mesh("roof");
    glm::vec3 color(0.5);

    std::vector<VertexFormat> vertices, roofVertices;
    std::vector<unsigned short> indices, roofIndices;

    // centers
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color, glm::vec3(0, 1, 0), glm::vec2()));
    vertices.push_back(VertexFormat(glm::vec3(0, height, 0), color, glm::vec3(0, 1, 0), glm::vec2()));
    roofVertices.push_back(*vertices.rbegin());

    // starting point
    vertices.push_back(VertexFormat(glm::vec3(rad, 0, 0), color, glm::vec3(rad, 0, 0), glm::vec2(1, 0)));
    vertices.push_back(VertexFormat(glm::vec3(rad, height, 0), color, glm::vec3(rad, 0, 0), glm::vec2(1, int(height))));
    roofVertices.push_back(VertexFormat(glm::vec3(rad, height, 0), color, glm::vec3(0, 1, 0), glm::vec2(1, int(height))));

    std::vector<float> angles(numFaces, 2 * PI / numFaces);

    if (degenerate) {
        for (int i = 1; i <= 10; i++) {
            int a = rand() % numFaces;
            int b = rand() % numFaces;
            float amount = min((float)(rand() % 10 + 1) * (PI / 50.0f), PI / 6.0f);

            if (angles[a] - amount > PI / 10 && angles[b] + amount < PI) {
                angles[a] -= amount;
                angles[b] += amount;
            }
        }
    }

    float angle = 0;
    for (int i = 0; i < numFaces; i++) {
        angle += angles[i];
        vertices.push_back(VertexFormat(glm::vec3(rad * cos(angle), 0, rad * sin(angle)), color,
                                        glm::vec3(rad * cos(angle), 0, rad * sin(angle)), glm::vec2(i % 2, 0)));
        vertices.push_back(VertexFormat(glm::vec3(rad * cos(angle), height, rad * sin(angle)), color,
                                        glm::vec3(rad * cos(angle), 0, rad * sin(angle)),
                                        glm::vec2(i % 2, int(height))));
        roofVertices.push_back(VertexFormat(glm::vec3(rad * cos(angle), height, rad * sin(angle)), color, glm::vec3(0, 1, 0),
                                            glm::vec2(i % 2, int(height))));

        unsigned short last = (i + 1) * 2;
        unsigned short current = unsigned short(vertices.size() - 2);
        indices.push_back(current);
        indices.push_back(last + 1);
        indices.push_back(last);

        indices.push_back(current + 1);
        indices.push_back(last + 1);
        indices.push_back(current);

        roofIndices.push_back(0);
        roofIndices.push_back(i + 1);
        roofIndices.push_back(i + 2);
    }

    std::tuple<float, float, float, float> limits;
    for (auto &vertex : vertices) {
        std::get<0>(limits) = min(std::get<0>(limits), vertex.position.x);
        std::get<1>(limits) = max(std::get<1>(limits), vertex.position.x);
        std::get<2>(limits) = min(std::get<2>(limits), vertex.position.z);
        std::get<3>(limits) = max(std::get<3>(limits), vertex.position.z);
    }

    cyl->InitFromData(vertices, indices);
    roof->InitFromData(roofVertices, roofIndices);
    return std::make_tuple(cyl, roof, limits);
}
