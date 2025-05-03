#pragma once

#include "ray.h"
#include "pixel.h"
#include "scene.h"

class Renderer
{
public:
    Pixel Intersect(Ray& ray, Scene& scene);

private:



};
