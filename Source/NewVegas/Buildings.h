#pragma once

#include "Core/GPU/Mesh.h"
#include "Core/GPU/Texture2D.h"

class Building
{
public:
    struct Comp
    {
        Mesh *mesh;
        Texture2D *texture;
        glm::mat4 modelMat;
    };

    std::vector<Comp> comps;
    glm::mat4 globalModelMat;


    Building(const glm::mat4 &global_model_mat)
        : globalModelMat(global_model_mat) {}

    void AddComp(Mesh *mesh, Texture2D *texture, const glm::mat4 &modelMat)
    {
        comps.push_back(Comp{mesh, texture, modelMat});
    }
};

class Buildings
{
public:
    Mesh *cyl;
    Texture2D *texture[40], *roofTexture;
    std::vector<Building *> buildings;

    Buildings();
    ~Buildings();

    void LoadTextures();

    /** Generate building inside the given rectangular area */
    Building *GenerateBuilding(std::tuple<float, float, float, float> limits);
};
