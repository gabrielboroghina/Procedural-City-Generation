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
    const glm::vec3 LIGHT_POS = glm::vec3(-2, 26, -2);
    
    const int MATERIAL_SHININESS = 10;
    const float MATERIAL_KD = 2;
    const float MATERIAL_KS = 1.4f;
    
    const float SPOT_ANGLE = 30;
    const float SPOT_DIST_ADDITION = 0.7f;
}

namespace Map
{
    const float MIN = -20;
    const float MAX = 20;
}

namespace Streets
{
    const float MIN_DIST = 0.7f;
    const float BLVD_WIDTH = 0.4f;
    const float STR_WIDTH = 0.4f;
}

namespace Buildings
{
    const float MIN_HEIGHT = 0.6f;
    const float MAX_HEIGHT = 5.0f;
}

namespace Trees
{
    const float DIST = 3;
}

namespace Cars
{
    const float DIST = 0.5f;
}

namespace Grass
{
    const int NUM_PARKS = 3;
    const float MARGINS = 0.1f;
}
}
