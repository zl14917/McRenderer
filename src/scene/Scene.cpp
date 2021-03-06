//
// Created by Zaiyang Li on 10/02/2018.
//

#include <iostream>
#include "Scene.hpp"
#include "../brdf/lambert/LamberShader.hpp"
namespace McRenderFace {

    void createTestScene(Scene &scene) {
        scene.camera.position = vec3(0,0, 1);
        scene.camera.forward = vec3(0,0, -1);
        scene.camera.up = vec3(0,1,0);
        scene.camera.computeRightVector();
        scene.camera.focalLength = .055;
        scene.camera.aspectRatio = 1.0f;
        scene.camera.fieldOfViewDegrees = 43.0f;

        // default material;
        PbrMaterial* grayMaterial = new PbrMaterial;

        PbrMaterial* redMaterial = new PbrMaterial;
        redMaterial->diffuseColour = vec3(0.75f, 0.1f, 0.1f);

        PbrMaterial* blueMaterial = new PbrMaterial;
        blueMaterial->diffuseColour = vec3(0.1f, 0.1, 0.75f);

        scene.addMaterial(grayMaterial);
        scene.addMaterial(redMaterial);
        scene.addMaterial(blueMaterial);

        Mesh* model = new Mesh;
        MeshData* data = new MeshData;

        Triangle tri{
                vec3(-2,-2, -2),
                vec3(0,-2, -2),
                vec3(0,0, -2),
                vec3(0, 0, 1)
        };
        Triangle tri2{
                vec3(-2, -2, -2),
                vec3(0, 0, -2),
                vec3(-2, 0, -2),
                vec3(0, 0, 1)
        };

        TriangleUV uv;

        data->triangles.push_back(tri);
        data->triangles.push_back(tri2);

        model->meshData = data;

        model->computeBoundingBox();
        model->materialId = 0;

        scene.addObject(model);
    }

    void Scene::addObject(SceneObject *mesh) {
        objects.push_back(shared_ptr<SceneObject>(mesh));
        mesh->objectId = static_cast<int>(objects.size() - 1);
    }

    void Scene::addLight(Light *light) {
        lights.push_back(shared_ptr<Light>(light));
    }

    void Scene::addMaterial(PbrMaterial *material) {
        materials.push_back(shared_ptr<PbrMaterial>(material));
        material->materialId = static_cast<int>(materials.size() - 1);
    }

    void Scene::preprocessMeshes() {
        for(auto& mesh : objects) {
            mesh->applyTransform();
            mesh->computeBoundingBox();
            std::cout <<"min(" << mesh->boundingBox.min.x << ' '
                      << mesh->boundingBox.min.y << ' '
                      << mesh->boundingBox.min.z << ')'
                      <<"max(" << mesh->boundingBox.max.x << ' '
                      << mesh->boundingBox.max.y << ' '
                      << mesh->boundingBox.max.z << ')'
                      << endl;
        }
    }
}