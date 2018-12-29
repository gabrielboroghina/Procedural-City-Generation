#pragma once

#include "Core/GPU/Mesh.h"
#include <unordered_set>

class Parks
{
    Parks();

public:
    Mesh *mesh;
    std::vector<glm::mat4> instances;
    std::unordered_set<int> hasPark;

    static Parks *GetInstance();

    ~Parks();
    void GeneratePark(std::tuple<float, float, float, float> limits);
};
