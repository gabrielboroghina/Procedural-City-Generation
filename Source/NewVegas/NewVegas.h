#pragma once

#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Camera.h"
#include "Objects/StreetSign.h"
#include "Streets.h"

class NewVegas : public SimpleScene
{
public:
    NewVegas();
    ~NewVegas();

    void Init() override;

private:
    Camera *camera;
    StreetSign *streetSign;
    Streets streets;

    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void SetShaderMVP(const Shader *shader, const glm::mat4 &modelMatrix) const;

    void RenderTexturedMesh(const Mesh *mesh, const Shader *shader, const glm::mat4 &modelMatrix,
                            const std::vector<Texture2D *> &textures) const;

    void RenderColoredMesh(const Mesh *mesh, const glm::mat4 &modelMatrix, const glm::vec3 &color) const;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;
};
