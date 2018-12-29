#include "NewVegas.h"

#include <vector>
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
    LoadShader("TextureLight", "Texture", "TextureLight");
    LoadShader("TextureSpotlight", "Texture", "TextureSpotlight");

    // initialize camera
    camera = new Camera(80, window->props.aspectRatio);

    // build sky
    sky = new Mesh("sky");
    sky->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
    sunPosition = UIConstants::Light::LIGHT_POS;

    skyTexture = new Texture2D();
    skyTexture->Load2D((RESOURCE_PATH::TEXTURES + "sky.png").c_str(), GL_MIRRORED_REPEAT);

    // build floor
    using namespace UIConstants::Map;
    floorMesh = MeshBuilder::CreateRect(glm::vec3(0, -0.01f, 0), MAX - MIN, MAX - MIN);
    floorTexture = new Texture2D();
    floorTexture->Load2D((RESOURCE_PATH::TEXTURES + "asphalt2.jpg").c_str(), GL_MIRRORED_REPEAT);

    // initialize objects
    streetSign = new StreetSign();
    streets = Streets::GetInstance();
    parks = Parks::GetInstance();
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
    // render sky
    RenderTexturedMesh(sky, shaders["Texture"], scale(glm::mat4(1), glm::vec3((UIConstants::Map::MAX - UIConstants::Map::MIN) * float(sqrt(2)))),
                       {skyTexture});

    // render floor
    RenderTexturedMesh(floorMesh, shaders["TextureByPos"], glm::mat4(1), {floorTexture});

    RenderStreets();

    // render parks
    for (auto &instance : parks->instances) {
        SetShaderMVP(shaders["TextureLight"], instance);
        parks->mesh->Render();
    }

    // render buildings
    for (auto &building : buildings->buildings)
        for (auto &comp : building->comps)
            RenderTexturedMesh(comp.mesh, shaders["TextureSpotlight"], building->globalModelMat * comp.modelMat,
                               {comp.texture}, building->spots);
}

void NewVegas::RenderStreets()
{
    for (auto &street : streets->vertStreets)
        RenderTexturedMesh(street->mesh, shaders["TextureLight"], street->modelMatrix, {streets->texture[street->type]});

    for (auto &street : streets->horizStreets)
        RenderTexturedMesh(street->mesh, shaders["TextureLight"], street->modelMatrix, {streets->texture[street->type]});

    for (auto &crossroadInstance : streets->crossroadTransf)
        RenderTexturedMesh(streets->crossroad, shaders["TextureLight"], crossroadInstance, {streets->crossroadTex});

    // render street objects
    for (auto &modelMat : trees->modelMatrices) {
        SetShaderMVP(shaders["TextureLight"], modelMat);
        trees->mesh->Render();
    }

    for (auto &modelMat : trafficLights->modelMatrices) {
        SetShaderMVP(shaders["TextureLight"], modelMat);
        trafficLights->mesh->Render();
    }

    for (auto &instance : cars->instances) {
        SetShaderMVP(shaders["TextureLight"], instance.second);
        instance.first->Render();
    }
}

void NewVegas::FrameEnd() {}

void NewVegas::BindCameraMaterialMVP(const Shader *shader, const glm::mat4 &modelMatrix) const
{
    // bind eye position (camera position)
    glm::vec3 eyePosition = modelMatrix * glm::vec4(camera->position, 1);
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    // bind material property uniforms (shininess, kd, ks) 
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), UIConstants::Light::MATERIAL_SHININESS);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), UIConstants::Light::MATERIAL_KD);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), UIConstants::Light::MATERIAL_KS);

    // bind model, view and projection matrices
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
}

void NewVegas::SetShaderSpotsMVP(const Shader *shader, const glm::mat4 &modelMatrix, vector<pair<glm::vec3, glm::vec3>> spots) const
{
    glUseProgram(shader->program);

    // set shader uniforms for light & material properties
    // bind sun position
    glUniform3fv(glGetUniformLocation(shader->program, "sun_pos"), 1, glm::value_ptr(sunPosition));

    for (size_t i = 0; i < spots.size(); i++) {
        glUniform3fv(glGetUniformLocation(shader->program, ("light" + to_string(i + 1) + "_pos").c_str()), 1,
                     glm::value_ptr(spots[i].first));
        glUniform3fv(glGetUniformLocation(shader->program, ("light" + to_string(i + 1) + "_dir").c_str()), 1,
                     glm::value_ptr(spots[i].second));
    }
    glUniform1f(glGetUniformLocation(shader->program, "spot_angle"), UIConstants::Light::SPOT_ANGLE);

    BindCameraMaterialMVP(shader, modelMatrix);
}

void NewVegas::SetShaderMVP(const Shader *shader, const glm::mat4 &modelMatrix) const
{
    glUseProgram(shader->program);

    // set shader uniforms for light & material properties
    // bind sun position
    glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 1, glm::value_ptr(sunPosition));

    // bind eye position (camera position)
    glm::vec3 eyePosition = modelMatrix * glm::vec4(camera->position, 1);
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    BindCameraMaterialMVP(shader, modelMatrix);
}

void NewVegas::RenderTexturedMesh(const Mesh *mesh, const Shader *shader, const glm::mat4 &modelMatrix,
                                  const vector<Texture2D *> &textures, const vector<pair<glm::vec3, glm::vec3>> &spots) const
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    if (!spots.empty())
        SetShaderSpotsMVP(shader, modelMatrix, spots);
    else
        SetShaderMVP(shader, modelMatrix);

    // bind textures
    for (unsigned i = 0; i < textures.size(); i++) {
        textures[i]->BindToTextureUnit(GL_TEXTURE0 + i);
        glUniform1i(glGetUniformLocation(shader->GetProgramID(), ("textureImg" + to_string(i)).c_str()), i);
    }

    // draw the object
    glBindVertexArray(mesh->GetBuffers()->VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void NewVegas::OnInputUpdate(float deltaTime, int mods)
{
    // process camera movement
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float acc = window->KeyHold(GLFW_KEY_LEFT_SHIFT) ? 4 : (window->KeyHold(GLFW_KEY_LEFT_CONTROL) ? 0.2f : 1);
        camera->Update(acc * deltaTime, window->KeyHold(GLFW_KEY_W), window->KeyHold(GLFW_KEY_A), window->KeyHold(GLFW_KEY_S),
                       window->KeyHold(GLFW_KEY_D), window->KeyHold(GLFW_KEY_Q), window->KeyHold(GLFW_KEY_E));
    }

    // move the sun
    float speed = 6;
    if (window->KeyHold(GLFW_KEY_UP)) sunPosition -= glm::vec3(0, 0, 1) * deltaTime * speed;
    if (window->KeyHold(GLFW_KEY_LEFT)) sunPosition -= glm::vec3(1, 0, 0) * deltaTime * speed;
    if (window->KeyHold(GLFW_KEY_DOWN)) sunPosition += glm::vec3(0, 0, 1) * deltaTime * speed;
    if (window->KeyHold(GLFW_KEY_RIGHT)) sunPosition += glm::vec3(1, 0, 0) * deltaTime * speed;
    if (window->KeyHold(GLFW_KEY_KP_ADD)) sunPosition += glm::vec3(0, 1, 0) * deltaTime * speed;
    if (window->KeyHold(GLFW_KEY_KP_SUBTRACT)) sunPosition -= glm::vec3(0, 1, 0) * deltaTime * speed;
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
