//
// Created by Zaiyang Li on 31/01/2018.
//

#ifndef RENDERER_TRIANGLE_HPP
#define RENDERER_TRIANGLE_HPP

#include <glm/glm.hpp>
#include "RayIntersecting.hpp"

namespace McRenderFace {
    using namespace glm;
    struct Triangle : RayIntersecting{
        vec3 vertices[3];
        vec2 uvCoords[3]{vec2(0,0), vec2(1,0), vec2(1,1)};
        vec3 normal {0, 0, 1};
        vec2 barycentricCoordOf(vec3 point);
        vec2 barycentricCoordOf(vec4 point);
        vec3 positionOf(float u, float v);
        void computeNormal(bool normalize = true);
        void invertNormal(bool recompute = false);
        void castRay(const Ray& ray, RayHit& hit) override;
        inline float surfaceArea() {
            vec3 e1 = vertices[1] - vertices[0];
            vec3 e2 = vertices[2] - vertices[0];
            vec3 perp = cross(e1, e2);
            return length(perp);
        }
        Triangle(): Triangle(vec3(0), vec3(0), vec3(0), vec3(0)) { }
        Triangle(vec3 v0, vec3 v1, vec3 v2, vec3 normalIn)
                : vertices{ v0, v1, v2 },
                  normal{normalIn} {
        }
    };
}


#endif //RENDERER_TRIANGLE_HPP
