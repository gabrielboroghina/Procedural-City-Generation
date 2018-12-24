#pragma once

#include <vector>
#include "Core/GPU/Mesh.h"

class MeshBuilder
{
public:
    MeshBuilder();
    ~MeshBuilder();

    static Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices,
                            const std::vector<unsigned short> &indices);

    static Mesh *CreateDisk(float radius, glm::vec3 centerPos);

    static Mesh *CreateRect(glm::vec3 center, float height, float width, glm::vec3 color);

    static Mesh *CreateVertRect(glm::vec3 center, float height, float width);
};
