#include "Cars.h"

#include "Core/GPU/Mesh.h"
#include "Core/Managers/ResourcePath.h"
#include "NewVegas/Streets.h"
#include "NewVegas/UIConstants.h"
#include "include/math.h"

void Cars::GenerateCar(float x, float z, int side)
{
    if (rand() % 3 == 0) {
        int type = rand() % meshes.size();
        if (type == 0)
            instances.push_back(std::make_pair(meshes[type],
                                               glm::rotate(glm::scale(glm::translate(glm::mat4(1), glm::vec3(x + 0.01f, 0.02f, z + 0.01f)),
                                                                      glm::vec3(0.0006f)),RADIANS((-1-side) * 90), glm::vec3(0, 1, 0))));
        else
            instances.push_back(std::make_pair(meshes[type],
                                               glm::rotate(glm::scale(glm::translate(glm::mat4(1), glm::vec3(x, 0.022f, z)),
                                                                      glm::vec3(0.0003f)), RADIANS(-side * 90), glm::vec3(0, 1, 0))));
    }
}

Cars::Cars()
{
    Mesh *mesh = new Mesh("car1");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Cars/1", "obj_camry.obj");
    meshes.push_back(mesh);

    for (int i = 2; i <= 4; i++) {
        mesh = new Mesh("car" + std::to_string(i));
        mesh->LoadMesh(RESOURCE_PATH::MODELS + "Cars/" + std::to_string(i), "car.obj");
        meshes.push_back(mesh);
    }

    Streets *streets = Streets::GetInstance();
    for (auto hStreet : streets->horizStreets)
        for (float x = UIConstants::Map::MIN + (rand() % 10 + 3) * 0.05f; x <= UIConstants::Map::MAX - 0.3; x += UIConstants::Cars::DIST) {
            int side = rand() % 2;
            if (side == 0) side = -1;
            GenerateCar(x, hStreet->modelMatrix[3].z + 0.03f * side, side);
        }

    for (auto vStreet : streets->vertStreets)
        for (float z = UIConstants::Map::MIN + (rand() % 10 + 3) * 0.05f; z <= UIConstants::Map::MAX - 0.3; z += UIConstants::Cars::DIST) {
            int side = rand() % 2;
            if (side == 0) side = -1;
            GenerateCar(vStreet->modelMatrix[3].x + 0.03f * side, z, 0);
        }
}

Cars::~Cars() {}
