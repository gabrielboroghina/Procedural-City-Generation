#pragma once

#include "Core/GPU/Mesh.h"
#include "Core/GPU/Texture2D.h"

class StreetSign
{
public:
    Mesh *stopMesh, *pedestrianMesh, *onewayMesh;
    Texture2D *stopTex, *pedestrianTex, *onewayTex;

    StreetSign();
    ~StreetSign();
};
