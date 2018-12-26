#pragma once

#include <include/glm.h>

namespace UIConstants
{
namespace Camera
{
    const float SENSITIVITY = 0.001f;
    const float MOVE_SPEED = 5;
    const float DIST_TO_TARGET = 2.5f;
    const glm::vec3 LOOKING_DIR_FIRST_PERSON = glm::vec3(0, 1, 0);
    const glm::vec3 LOOKING_DIR_THIRD_PERSON = glm::vec3(0, 0.5f, 0.45f);
}

namespace Light
{
    const glm::vec3 LIGHT_POS = glm::vec3(0, 2.5f, 0);
    const int MATERIAL_SHININESS = 30;
    const float MATERIAL_KD = 2;
    const float MATERIAL_KS = 1.4f;
}

namespace Map
{
    const float MIN = -20;
    const float MAX = 20;
}

namespace Streets
{
    const float MIN_DIST = 0.7f;
    const float BLVD_WIDTH = 0.5f;
    const float STR_WIDTH = 0.4f;
}

namespace Buildings
{
    const float MIN_HEIGHT = 0.6f;
    const float MAX_HEIGHT = 6.0f;
}
}
