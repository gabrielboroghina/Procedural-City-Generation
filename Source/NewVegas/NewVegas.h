#pragma once

#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Camera.h"
#include "Objects/StreetSign.h"
#include "Streets.h"
#include "Buildings.h"
#include "Objects/Trees.h"
#include "Objects/TrafficLights.h"
#include "Objects/Cars.h"
#include "Objects/Parks.h"

class NewVegas : public SimpleScene
{
public:
    NewVegas();
    ~NewVegas();

    void Init() override;

private:
    Camera *camera;
    glm::vec3 sunPosition;
    Mesh *sky, *floorMesh;
    Texture2D *skyTexture, *floorTexture;

    StreetSign *streetSign;
    Streets *streets;
    Buildings *buildings;
    Trees *trees;
    TrafficLights *trafficLights;
    Cars *cars;
    Parks *parks;

    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void BindCameraMaterialMVP(const Shader *shader, const glm::mat4 &modelMatrix) const;

    void SetShaderMVP(const Shader *shader, const glm::mat4 &modelMatrix) const;

    void SetShaderSpotsMVP(const Shader *shader, const glm::mat4 &modelMatrix, std::vector<std::pair<glm::vec3, glm::vec3>> spots) const;

    void RenderTexturedMesh(const Mesh *mesh, const Shader *shader, const glm::mat4 &modelMatrix,
                            const std::vector<Texture2D *> &textures, const std::vector<std::pair<glm::vec3, glm::vec3>> &spots = {}) const;

    void RenderStreets();

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;
};
