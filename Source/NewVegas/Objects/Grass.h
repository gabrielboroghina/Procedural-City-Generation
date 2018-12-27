#pragma once

#include "Core/GPU/Mesh.h"
#include <unordered_set>

class Grass
{
    Grass();

public:
    Mesh *mesh;
    std::vector<glm::mat4> instances;
    std::unordered_set<int> hasPark;

    static Grass *GetInstance();

    ~Grass();
    void GenerateGrass(std::tuple<float, float, float, float> limits);
};
