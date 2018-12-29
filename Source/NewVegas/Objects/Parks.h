#pragma once

#include "Core/GPU/Mesh.h"
#include <unordered_set>

class Parks
{
    Parks();

public:
    Mesh *mesh;
    std::vector<glm::mat4> instances;
    std::unordered_set<int> hasPark; // checks if a cell in the streets' grid contains a park

    static Parks *GetInstance();

    ~Parks();
    void GeneratePark(std::tuple<float, float, float, float> limits);
};
