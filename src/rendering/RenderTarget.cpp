//
// Created by Zaiyang Li on 31/01/2018.
//

#include "RenderTarget.hpp"

namespace McRenderer {

    RenderTarget::~RenderTarget() {

    }

    RenderTarget::RenderTarget(int widthIn, int heightIn)
            : width{widthIn}, height{heightIn} {
        if (width < 1 || height < 1) {
            throw runtime_error{"error"};
        }
        frameBuffer = new vec3[width * height];
    }

    void RenderTarget::setColor(int x, int y, glm::vec3 colour) {
        frameBuffer[x + y * width] = colour;
    }

    int RenderTarget::getWidth() {
        return width;
    }
    int RenderTarget::getHeight() {
        return height;
    }

    void RenderTarget::fillColor(int xMin, int yMin, int xMax, int yMax, vec3 colour) {
        assert(xMin <= xMax);
        assert(yMin <= yMax);

        xMin = xMin < 0 ? 0 : xMin;
        xMax = xMax >= height ? height - 1 : xMax;
        yMin = yMin < 0 ? 0 : yMin;
        yMax = yMax >= height ? height - 1 : yMax;

        for(int x = xMin; x <= xMax; x++) {
            for(int y = yMin; y <= yMax; y++) {
                // fill framebuffer at (x,y) with colour;
                frameBuffer[x + y * width] = colour;
            }
        }
    }
}