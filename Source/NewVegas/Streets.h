#pragma once

#include "Core/GPU/Texture2D.h"
#include <vector>
#include "Core/GPU/Mesh.h"

enum StreetType
{
    BLVD,
    STR
};

struct Street
{
    StreetType type;
    Mesh *mesh;
    glm::mat4 modelMatrix;

    Street(StreetType type, Mesh *mesh, const glm::mat4 &model_matrix)
        : type(type),
          mesh(mesh),
          modelMatrix(model_matrix) {}

    ~Street()
    {
        delete mesh;
    }
};

class Streets
{
public:
    Mesh *vertMesh[2], *horizMesh[2], *crossroad;
    Texture2D *texture[2], *crossroadTex;
    std::vector<Street *> vertStreets, horizStreets;
    std::vector<glm::mat4> crossroadTransf; // model matrices for crossroad meshes

    static Streets *GetInstance();

    static float StreetHalfWidth(Street *street);
    std::tuple<float, float, float, float> ZoneBetweenStreets(int v1, int v2, int h1, int h2);

private:
    Streets();
    ~Streets();

    void InitCrossroads();
};
