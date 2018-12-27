#include "NewVegas.h"

#include <vector>
#include <iostream>
#include <Core/Engine.h>
#include <Core/GPU/Texture2D.h>
#include "MeshBuilder.h"

using namespace std;

NewVegas::NewVegas() {}

NewVegas::~NewVegas() {}

void NewVegas::Init()
{
    LoadShader("TextureByPos", "TextureByPos", "TextureByPos");
    LoadShader("Texture", "Texture", "Texture");
    LoadShader("LightColor", "Texture", "Color");

    // initialize camera
    camera = new Camera(80, window->props.aspectRatio);

    {
        // build floor
        using namespace UIConstants;
        vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(Map::MIN, -0.01f, Map::MIN), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(Map::MIN, -0.01f, Map::MAX), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(Map::MAX, -0.01f, Map::MAX), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(Map::MAX, -0.01f, Map::MIN), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
        };

        vector<unsigned short> indices = {
            0, 1, 2,
            0, 2, 3
        };

        floorMesh = MeshBuilder::CreateMesh("floor", vertices, indices);
        floorTexture = new Texture2D();
        floorTexture->Load2D((RESOURCE_PATH::TEXTURES + "asphalt2.jpg").c_str(), GL_MIRRORED_REPEAT);
    }

    // initialize objects
    streetSign = new StreetSign();
    streets = Streets::GetInstance();
    parks = Grass::GetInstance();
    buildings = new Buildings();
    trees = new Trees();
    trafficLights = new TrafficLights();
    cars = new Cars();
}

void NewVegas::FrameStart()
{
    // clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void NewVegas::Update(float deltaTimeSeconds)
{
    RenderTexturedMesh(floorMesh, shaders["TextureByPos"], glm::mat4(1), {floorTexture});

    RenderStreets();

    for (auto &instance : parks->instances) {
        SetShaderMVP(shaders["Texture"], instance);
        parks->mesh->Render();
    }

    // render buildings
    for (auto building : buildings->buildings)
        for (auto comp : building->comps)
            RenderTexturedMesh(comp.mesh, shaders["Texture"], building->globalModelMat * comp.modelMat, {comp.texture});
}

void NewVegas::RenderStreets()
{
    for (auto street : streets->vertStreets)
        RenderTexturedMesh(street->mesh, shaders["Texture"], street->modelMatrix, {streets->texture[street->type]});

    for (auto street : streets->horizStreets)
        RenderTexturedMesh(street->mesh, shaders["Texture"], street->modelMatrix, {streets->texture[street->type]});

    for (auto &crossroadInstance : streets->crossroadTransf)
        RenderTexturedMesh(streets->crossroad, shaders["Texture"], crossroadInstance, {streets->crossroadTex});

    // render street objects
    for (auto &modelMat : trees->modelMatrices) {
        SetShaderMVP(shaders["Texture"], modelMat);
        trees->mesh->Render();
    }

    for (auto &modelMat : trafficLights->modelMatrices) {
        SetShaderMVP(shaders["Texture"], modelMat);
        trafficLights->mesh->Render();
    }

    for (auto &instance : cars->instances) {
        SetShaderMVP(shaders["Texture"], instance.second);
        instance.first->Render();
    }
}

void NewVegas::FrameEnd()
{
    // DrawCoordinatSystem(camera->GetViewMatrix(), camera->GetProjectionMatrix());
}

void NewVegas::SetShaderMVP(const Shader *shader, const glm::mat4 &modelMatrix) const
{
    // render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    // Bind light position
    glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 1, glm::value_ptr(UIConstants::Light::LIGHT_POS));

    // Bind eye position (camera position)
    glm::vec3 eyePosition = modelMatrix * glm::vec4(camera->position, 1);
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    // Bind material property uniforms (shininess, kd, ks) 
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), UIConstants::Light::MATERIAL_SHININESS);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), UIConstants::Light::MATERIAL_KD);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), UIConstants::Light::MATERIAL_KS);

    // Bind model, view and projection matrices
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
}

void NewVegas::RenderTexturedMesh(const Mesh *mesh, const Shader *shader, const glm::mat4 &modelMatrix,
                                  const vector<Texture2D *> &textures) const
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    SetShaderMVP(shader, modelMatrix);

    // Bind textures
    for (unsigned i = 0; i < textures.size(); i++) {
        textures[i]->BindToTextureUnit(GL_TEXTURE0 + i);
        glUniform1i(glGetUniformLocation(shader->GetProgramID(), ("textureImg" + to_string(i)).c_str()), i);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void NewVegas::RenderColoredMesh(const Mesh *mesh, const glm::mat4 &modelMatrix, const glm::vec3 &color) const
{
    Shader *shader = shaders.at("LightColor");
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    SetShaderMVP(shader, modelMatrix);

    // Bind color vector
    glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(color));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void NewVegas::OnInputUpdate(float deltaTime, int mods)
{
    // process camera movement
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        camera->Update(deltaTime, window->KeyHold(GLFW_KEY_W), window->KeyHold(GLFW_KEY_A), window->KeyHold(GLFW_KEY_S),
                       window->KeyHold(GLFW_KEY_D), window->KeyHold(GLFW_KEY_Q), window->KeyHold(GLFW_KEY_E));
}

void NewVegas::OnKeyPress(int key, int mods) {}

void NewVegas::OnKeyRelease(int key, int mods) {}

void NewVegas::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        // rotate the camera around OX and OY
        camera->RotateOX(-deltaY * UIConstants::Camera::SENSITIVITY);
        camera->RotateOY(-deltaX * UIConstants::Camera::SENSITIVITY);
    }
}

void NewVegas::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void NewVegas::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void NewVegas::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void NewVegas::OnWindowResize(int width, int height) {}
