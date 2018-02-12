//
// Created by Zaiyang Li on 31/01/2018.
//
#include <cmath>
#include <iostream>
#include "RayTracer.hpp"


namespace McRenderFace {

    // deprecated. use other render method.
    void RayTracer::render(McRenderFace::SimpleScene &scene, McRenderFace::RenderTarget &target) {
        // convert from screen space coordinate to camera coordinate.

        const int width = target.getWidth();
        const int height = target.getHeight();

        float focalLength = scene.camera.focalLength;
        const Camera& camera = scene.camera;
        Light light = scene.lights[0];
        Ray ray{camera.position, vec3(0, 0, 0)};
        RayHit hit;
        int closestIndex = 0;
        float fovLength = tan(0.5f * camera.fieldOfViewDegrees / 180.0f * (float) M_PI);
        vec3 meshColour(0.75, 0.75, 0.75);
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                float screenX = (float(x) / float(width - 1) - 0.5f) * 2.0f * fovLength;
                float screenY = -(float(y) / float(height - 1) - 0.5f) * 2.0f * fovLength;
                vec3 direction = vec3(screenX, screenY, camera.focalLength);
                vec3 worldCoord = camera.toWorldCoordinate(screenX, screenY);

                ray.forward = glm::normalize(worldCoord - camera.position);

                closestIntersection(scene.model, ray, hit, closestIndex);

                // ray escaped, set pixel colour to background colour: assume black;
                if(!hit.isHit) {
                    target.setColor(x, y, scene.backgroundColour);
                }
                else {
                    Triangle tri = scene.model[closestIndex];
                    target.setColor(x, y, meshColour);
                    vec3 toLight = light.position - hit.position;
                    float distance2 = glm::dot(toLight, toLight);
                    float distance = sqrt(distance2);
                    toLight /= distance;
                    //ray position needs a bias to avoid shadow acne.
                    //https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows
                    Ray shadowRay{hit.position + tri.normal * config.shadowBias, toLight};
                        // material: (diffuse, specular, ambient)
                        // light: (diffuse, specular, ambient, distance)

                    float cosine = glm::dot(toLight, tri.normal) / distance;
                    cosine = cosine > 0 ? cosine : 0;
                    vec3 lightColour = light.colour * cosine / (distance2) * INVERSE2PI * light.intensity;
                    vec3 colour = lightColour * meshColour;

                    if(traceShadow(scene, distance, shadowRay)) {
                        colour *= 0.2;
                    }
                    target.setColor(x, y, colour);

                }
            }
//            loop through triangles
//            find intersection using the equation
//            check intersection distance is closer than before
//            set new intersection
//                    return if any intersection exists
        }
        // hard shadow
    }

    bool RayTracer::traceShadow(SimpleScene &scene, float lightDistance, const Ray &ray) {
        RayHit hit;
        int index = -1;
        closestIntersection(scene.model, ray, hit, index);
        return hit.isHit && (hit.t < lightDistance);
    }

    void RayTracer::render(Scene &scene, RenderTarget &target) {
        const int width = target.getWidth();
        const int height = target.getHeight();

        float focalLength = scene.camera.focalLength;
        const Camera& camera = scene.camera;
        Light* light = scene.lights[0].get();
        Ray ray{camera.position, vec3(0, 0, 0)};
        cout << "camera position" << camera.position.x << ','<< camera.position.y<<',' << camera.position.z << endl;
        RayHit hit;
        int closestIndex = 0;
        const int raysPerPixel = config.samplingLevel;
        float fovLength = tan(0.5f * camera.fieldOfViewDegrees / 180.0f * (float) M_PI);
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < height; y++) {
                float screenX = (float(x) / float(width - 1) - 0.5f) * 2.0f * fovLength;
                float screenY = -(float(y) / float(height - 1) - 0.5f) * 2.0f * fovLength;
                vec3 direction = vec3(screenX, screenY, focalLength);
                vec3 worldCoord = camera.toWorldCoordinate(screenX, screenY);

                ray.forward = glm::normalize(worldCoord - camera.position);
                closestIntersection(scene.models, ray, hit, closestIndex);

                // ray escaped, set pixel colour to background colour: assume black;
                if(!hit.isHit) {
                    target.setColor(x, y, scene.backgroundColour);
                }
                else {
                    auto model = scene.models[closestIndex];
                    vec3 toLight = light->position - hit.position;
                    float distance2 = glm::dot(toLight, toLight);
                    float distance = sqrt(distance2);
                    toLight /= distance;
                    //ray position needs a bias to avoid shadow acne.
                    //https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows
                    Ray shadowRay{hit.position + hit.normal * config.shadowBias, toLight};
                    // material: (diffuse, specular, ambient)
                    // light: (diffuse, specular, ambient, distance)
                    Material* material = scene.materials[model->materialId].get();
                    float cosine = glm::dot(toLight, hit.normal) / distance;
                    cosine = cosine > 0 ? cosine : 0;
                    vec3 lightColour = light->colour / ( cosine * (distance2) * INVERSE2PI * light->intensity);
                    vec3 colour = lightColour * dynamic_cast<LambertMaterial*>(material)->diffuseColour;

                    if(traceShadow(scene, distance, shadowRay)) {
                        colour *= 0.2;
                    }
                    target.setColor(x, y, colour);

                }
            }
//            loop through triangles
//            find intersection using the equation
//            check intersection distance is closer than before
//            set new intersection
//                    return if any intersection exists
        }
    }

    bool RayTracer::traceShadow(Scene &scene, float lightDistance, const Ray &ray) {
        return false;
    }

    void closestIntersection(vector<Triangle> &triangles, const Ray &ray, RayHit& hit, int& closestIndex) {
        RayHit closestHit;
        int closest = -1;
        closestHit.t = MAXFLOAT;
        RayHit hitResult;
        int size = static_cast<int>(triangles.size());
        for(int i = 0; i < size; i++) {
            hitResult = triangles[i].castRay(ray);
            if(hitResult.isHit && hitResult.t > 0.0f && hitResult.t < closestHit.t) {
                closestHit = hitResult;
                closest = i;
            }
        }

        closestIndex = closest;
        hit = closestHit;
    }
    void closestIntersection(vector<shared_ptr<Mesh>> models, const Ray& ray, RayHit& hit, int& closestIndex) {
        int closest = -1;
        RayHit closestHit;
        closestHit.t = MAXFLOAT;
        RayHit hitResult;
        int modelCount = static_cast<int>(models.size());
        for(int i = 0; i < modelCount; i++) {
            hitResult = models[0]->castRay(ray);
            if(hitResult.isHit && hitResult.t > 0.0f && hitResult.t < closestHit.t) {
                closestHit = hit;
                closest = i;
            }
        }
        closestIndex = closest;
        hit = hitResult;
    }
}
