//
// Created by Zaiyang Li on 30/01/2018.
//

#ifndef RENDERER_VERTEX_HPP
#define RENDERER_VERTEX_HPP

#include <glm/glm.hpp>
namespace McRenderFace {
    using namespace glm;
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec3 color;
        vec2 uvCoord;
    };
}


#endif //RENDERER_VERTEX_HPP
