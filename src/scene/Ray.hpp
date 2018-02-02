//
// Created by Zaiyang Li on 25/01/2018.
//

#ifndef RENDERER_RAY_HPP
#define RENDERER_RAY_HPP

#include <glm/glm.hpp>
#include "Triangle.hpp"
#include "Sphere.hpp"

namespace McRenderer {
    using namespace glm;
    const float EPSILON = 0.00001f;
    struct Ray {
        vec3 origin;
        vec3 forward;

        Ray(float originX, float originY, float originZ, float forwardX, float forwardY, float forwardZ);
        Ray(const vec3& origin, const vec3& forward);
    };

    bool castRay(const Ray& ray, const Triangle& triangle, vec3& hitLocation);
    bool castRay(const Ray& ray, const Sphere& sphere, vec3& hitLocation);
}


#endif //RENDERER_RAY_HPP
