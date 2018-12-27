#include "Grass.h"

#include "Core/GPU/Mesh.h"
#include "Core/Managers/ResourcePath.h"
#include "NewVegas/Streets.h"
#include "NewVegas/UIConstants.h"
#include <tuple>

static Grass *instance = nullptr;

Grass *Grass::GetInstance()
{
    if (instance == nullptr)
        instance = new Grass();
    return instance;
}

Grass::Grass()
{
    mesh = new Mesh("grass");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation", "grass.obj");

    Streets *streets = Streets::GetInstance();
    for (int i = 1; i <= UIConstants::Grass::NUM_PARKS; i++) {
        int vs = rand() % (streets->vertStreets.size() - 1) + 1;
        int hs = rand() % (streets->horizStreets.size() - 1) + 1;

        hasPark.insert(vs * 1000 + hs); // mark this zone so that not to generate a building here

        GenerateGrass(streets->ZoneBetweenStreets(vs - 1, vs, hs - 1, hs));
    }
}

Grass::~Grass() {}

void Grass::GenerateGrass(std::tuple<float, float, float, float> limits)
{
    float xmin, xmax, zmin, zmax;
    std::tie(xmin, xmax, zmin, zmax) = limits;

    for (float x = xmin; x <= xmax; x += 0.08)
        for (float z = zmin; z <= zmax; z += 0.08)
            instances.push_back(glm::scale(glm::translate(glm::mat4(1), glm::vec3(x, 0, z)), glm::vec3(0.1, 0.03f,0.1)));
}
