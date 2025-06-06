#pragma once

#include "scene/scene.h"
#include <memory>

std::shared_ptr<Scene> makeLitBoxScene();
std::shared_ptr<Scene> makeLambertianScene();
std::shared_ptr<Scene> makeMirrorScene();
std::shared_ptr<Scene> makeGlassScene();
