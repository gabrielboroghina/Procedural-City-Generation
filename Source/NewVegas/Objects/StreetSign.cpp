#include "StreetSign.h"

#include <GL/glew.h>
#include "Core/Managers/ResourcePath.h"

StreetSign::StreetSign()
{
    stopMesh = new Mesh("StreetSign");
    stopMesh->LoadMesh(RESOURCE_PATH::MODELS + "StreetSign", "neuro_stop_obj.obj");

    stopTex = new Texture2D();
    stopTex->Load2D((RESOURCE_PATH::MODELS + "StreetSign/neuro_stop_diffuse.jpg").c_str(), GL_REPEAT);

    pedestrianMesh = new Mesh("StreetSign");
    pedestrianMesh->LoadMesh(RESOURCE_PATH::MODELS + "StreetSign", "neuro_pedestrian_obj.obj");

    pedestrianTex = new Texture2D();
    pedestrianTex->Load2D((RESOURCE_PATH::MODELS + "StreetSign/neuro_pedestrian_diffuse.jpg").c_str(), GL_REPEAT);

    onewayMesh = new Mesh("StreetSign");
    onewayMesh->LoadMesh(RESOURCE_PATH::MODELS + "StreetSign", "neuro_oneway_obj.obj");

    onewayTex = new Texture2D();
    onewayTex->Load2D((RESOURCE_PATH::MODELS + "StreetSign/neuro_oneway_diffuse.jpg").c_str(), GL_REPEAT);
}

StreetSign::~StreetSign()
{
    delete stopMesh;
    delete pedestrianMesh;
    delete onewayMesh;
    delete stopTex;
    delete pedestrianTex;
    delete onewayTex;
}
