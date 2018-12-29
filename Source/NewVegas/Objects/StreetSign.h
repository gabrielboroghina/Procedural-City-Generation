#pragma once

#include "Core/GPU/Mesh.h"
#include "Core/GPU/Texture2D.h"

class StreetSign
{
public:
    std::vector<std::pair<Mesh *, Texture2D *>> signModels; // 3D models
    std::vector<std::pair<std::pair<Mesh *, Texture2D *>, glm::mat4>> signs;
    Mesh *tree;

    StreetSign();
    ~StreetSign();
};
