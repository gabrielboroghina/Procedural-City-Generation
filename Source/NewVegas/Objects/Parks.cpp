#include "Parks.h"

#include "Core/GPU/Mesh.h"
#include "Core/Managers/ResourcePath.h"
#include "NewVegas/Streets.h"
#include "NewVegas/UIConstants.h"
#include <tuple>

static Parks *instance = nullptr; // singleton instance

Parks *Parks::GetInstance()
{
    if (instance == nullptr)
        instance = new Parks();
    return instance;
}

Parks::Parks()
{
    // load grass 3D model
    mesh = new Mesh("grass");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation", "grass.obj");

    Streets *streets = Streets::GetInstance();
    for (int i = 1; i <= UIConstants::Grass::NUM_PARKS; i++) {
        int vs = rand() % (streets->vertStreets.size() - 1) + 1;
        int hs = rand() % (streets->horizStreets.size() - 1) + 1;
        printf("Park at cell: %i %i\n", vs, hs);

        hasPark.insert(vs * 1000 + hs); // mark this zone so that not to generate a building here

        GeneratePark(streets->ZoneBetweenStreets(vs - 1, vs, hs - 1, hs));
    }
}

Parks::~Parks()
{
    delete mesh;
}

void Parks::GeneratePark(std::tuple<float, float, float, float> limits)
{
    float xmin, xmax, zmin, zmax;
    std::tie(xmin, xmax, zmin, zmax) = limits;

    using namespace UIConstants::Grass;
    // fill the rectangular region with grass objects
    for (float x = xmin + MARGINS; x <= xmax - MARGINS; x += 0.07f)
        for (float z = zmin + MARGINS; z <= zmax - MARGINS; z += 0.07f)
            instances.push_back(glm::scale(glm::translate(glm::mat4(1), glm::vec3(x, 0, z)), glm::vec3(0.1f, 0.02f,0.1f)));
}
