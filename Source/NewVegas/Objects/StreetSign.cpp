#include "StreetSign.h"

#include <GL/glew.h>
#include "Core/Managers/ResourcePath.h"

StreetSign::StreetSign()
{
    Mesh *mesh;
    Texture2D *texture;

    std::string objList[] = {"neuro_stop_obj.obj", "neuro_pedestrian_obj.obj", "neuro_oneway_obj.obj"};
    std::string texList[] = {"neuro_stop_diffuse.jpg", "neuro_pedestrian_diffuse.jpg", "neuro_oneway_diffuse.jpg"};

    for (int i = 0; i < 3; i++) {
        mesh = new Mesh("StreetSign");
        mesh->LoadMesh(RESOURCE_PATH::MODELS + "StreetSign", objList[i]);

        texture = new Texture2D();
        texture->Load2D((RESOURCE_PATH::MODELS + "StreetSign/" + texList[i]).c_str(), GL_REPEAT);

        signModels.push_back(std::make_pair(mesh, texture));
    }
}

StreetSign::~StreetSign()
{
    for (auto &p : signModels) {
        delete p.first;
        delete p.second;
    }
}
