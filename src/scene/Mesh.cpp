//
// Created by Zaiyang Li on 25/01/2018.
//

#include <iostream>
#include "Mesh.hpp"

namespace McRenderFace {
    void Mesh::computeBoundingBox() {
        boundingBox = (meshData != nullptr) ? meshData->computeBoundingBox() : AxisAlignedBoundingBox();
    }

    void Mesh::castRay(const Ray &ray, RayHit& hit) {
        // optimization: if ray does not hit bounding box, terminate.
        //cout << "number of triangles" << meshData->triangles.size() << endl;
        boundingBox.castRay(ray, hit);
        if(!hit.isHit) {
            return;
        }
        hit.isHit = false;
        const auto& triangles = meshData -> triangles;
        RayHit hitTemp;
        for(auto tri : triangles) {
            hitTemp.isHit = false;
            tri.castRay(ray, hitTemp);
            if(hitTemp.isHit&& hitTemp.t > 0 && hitTemp.t < hit.t) {
                 hit = hitTemp;
            }
        }
    }

    void Mesh::initializeMeshFromObj(Mesh &mesh, const ObjModel &obj, bool computeNormalAsNeeded) {
        if(mesh.meshData != nullptr) {
            delete mesh.meshData;
        }

        MeshData* meshData = new MeshData();
        mesh.meshData = meshData;

        //construct triangles
        Triangle tri;
        TriangleUV triUv;

        for(vec3 v: obj.vertices) {
            std::cout << v.x << ' ' << v.y << ' ' << v.z << endl;
        }
        const vector<vec3> vertices = obj.vertices;
        const vector<vec3> normals = obj.normals;
        const vector<vec2> uvCoords = obj.uvCoords;
        int i = 0;
        for(auto& face: obj.faces) {

            tri.vertices[0] = vertices[face.vertex[0]];
            tri.vertices[1] = vertices[face.vertex[1]];
            tri.vertices[2] = vertices[face.vertex[2]];

            if(uvCoords.size() > 0) {
                tri.uvCoords[0] = uvCoords[face.uvCoord[0]];
                tri.uvCoords[1] = uvCoords[face.uvCoord[0]];
                tri.uvCoords[2] = uvCoords[face.uvCoord[0]];
            }

            tri.computeNormal();

            meshData->triangles.push_back(tri);
            i++;
        }
        mesh.computeBoundingBox();
    }

    void Mesh::applyTransform() {
        vec4 transformedVertices[3];
        vec4 transformedNormal;
        mat4 transformationMatrix = transform.matrix();
        bool shouldNormalize = !(transform.scale.x == 1.0f && transform.scale.y == 1.0f && transform.scale.z == 1.0f);
        if(shouldNormalize) {
            for (Triangle &tri: meshData->triangles) {
                for (int i = 0; i < 3; i++) {
                    // vertex has position, so we multiply matrix with (x,y,z,1)
                    transformedVertices[i] = transformationMatrix * vec4(tri.vertices[i], 1.0f);
                    tri.vertices[i] = vec3(transformedVertices[i].x, transformedVertices[i].y,
                                           transformedVertices[i].z);
                }
                // normal represents direction only, so we multiply matrix with (x, y, z, 0);
                // we must re-normalize a normal vector in case of scaling.
                transformedNormal = glm::normalize(transformationMatrix * vec4(tri.normal, 0.0f));
                tri.normal = vec3(transformedNormal.x, transformedNormal.y, transformedNormal.z);
            }
        } else {
            for (Triangle &tri: meshData->triangles) {
                for (int i = 0; i < 3; i++) {
                    // vertex has position, so we multiply matrix with (x,y,z,1)
                    transformedVertices[i] = transformationMatrix * vec4(tri.vertices[i], 1.0f);
                    tri.vertices[i] = vec3(transformedVertices[i].x, transformedVertices[i].y,
                                           transformedVertices[i].z);
                }
                // normal represents direction only, so we multiply matrix with (x, y, z, 0);
                transformedNormal = transformationMatrix * vec4(tri.normal, 0.0f);
                tri.normal = vec3(transformedNormal.x, transformedNormal.y, transformedNormal.z);
            }
        }
        transform.reset();
    }

    AxisAlignedBoundingBox MeshData::computeBoundingBox() {
        AxisAlignedBoundingBox boundingBox;

        if(triangles.size() < 1) {
            return boundingBox;
        }
        boundingBox.max = vec3(-FLT_MAX);
        boundingBox.min = vec3(FLT_MAX);
        for(auto tri : triangles) {
            boundingBox.min = glm::min(boundingBox.min, tri.vertices[0]);
            boundingBox.max = glm::max(boundingBox.max, tri.vertices[0]);

            boundingBox.min = glm::min(boundingBox.min, tri.vertices[1]);
            boundingBox.max = glm::max(boundingBox.max, tri.vertices[1]);

            boundingBox.min = glm::min(boundingBox.min, tri.vertices[2]);
            boundingBox.max = glm::max(boundingBox.max, tri.vertices[2]);
        }
        return boundingBox;
    }

    void MeshData::invertNormals() {
        for(auto& tri : triangles) {
            tri.invertNormal();
        }
    }

    void MeshData::computeNormals() {
        for(auto& tri: triangles) {
            tri.computeNormal();
        }
    }
}