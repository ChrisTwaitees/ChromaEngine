#ifndef _CHROMA_SCREEN_CONSTANTS_
#define _CHROMA_SCREEN_CONSTANTS_

#include "./glm/glm.hpp"

// SCREEN
static const unsigned int SCREEN_WIDTH{ 1500 };
static const unsigned int SCREEN_HEIGHT{ 1500 };
static const unsigned int MSAA_SAMPLES{ 4 };

// BG COLOR
static const glm::vec3 SCREEN_DEFAULT_COLOR(0.3f, 0.3f, 0.3f);

// INPUT
static double MOUSE_XOFFSET, MOUSE_YOFFSET;

// CAMERA
static float CAM_FOV{ 45.0f };
static const float CAM_ASPECT{ SCREEN_WIDTH / SCREEN_HEIGHT };
static const float CAM_NEAR{ 0.1f };
static const float CAM_FAR{ 100.0f };

#endif