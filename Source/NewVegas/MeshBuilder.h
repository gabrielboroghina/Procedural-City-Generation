#pragma once

#include <vector>
#include "Core/GPU/Mesh.h"
#include <tuple>

using namespace std;

class MeshBuilder
{
public:
    MeshBuilder();
    ~MeshBuilder();

    static Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices,
                            const std::vector<unsigned short> &indices);

    static Mesh *CreateDisk(float radius, glm::vec3 centerPos);

    static Mesh *CreateRect(glm::vec3 center, float height, float width, glm::vec3 color = glm::vec3(0));

    static Mesh *CreateVertRect(glm::vec3 center, float height, float width);

    static std::tuple<Mesh*, Mesh*, tuple<float, float, float, float>> CreateCylinder(
        float height, float rad, int numFaces, bool degenerate);
};
