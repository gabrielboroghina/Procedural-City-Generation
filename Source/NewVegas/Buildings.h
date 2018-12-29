#pragma once

#include "Core/GPU/Mesh.h"
#include "Core/GPU/Texture2D.h"

class Building
{
public:
    /// building component (level)
    struct Comp
    {
        Mesh *mesh;
        Texture2D *texture;
        glm::mat4 modelMat;
    };

    std::vector<Comp> comps;
    glm::mat4 globalModelMat;
    std::vector<std::pair<glm::vec3, glm::vec3>> spots; // spotlights

    Building(const glm::mat4 &global_model_mat)
        : globalModelMat(global_model_mat) {}

    void AddComp(Mesh *mesh, Texture2D *texture, const glm::mat4 &modelMat)
    {
        comps.push_back(Comp{mesh, texture, modelMat});
    }
};

/// Manager class for buildings generation
class Buildings
{
public:
    Texture2D *texture[40], *roofTexture;
    std::vector<Building *> buildings;

    Buildings();
    ~Buildings();

    void LoadTextures();

    /** Generate building inside the given rectangular area */
    Building *GenerateBuilding(const std::tuple<float, float, float, float> &limits) const;
    void GenerateSpot(Building *building, int ang, float cylRad, float height) const;
};
