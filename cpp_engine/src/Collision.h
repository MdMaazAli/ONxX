#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

class Collision{
    public:
        static bool RayCastIntersects(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 boxMin, glm::vec3 boxMax, float& howFar);
        static glm::vec3 RayIntersectsFloor(glm::vec3 rayOrigin, glm::vec3 rayDirection, float floorHeight);
        static bool CheckCollision_Sphere(glm::vec3 SphereCenter,float radius,glm::vec3 aabbCenter,glm::vec3 aabbSize);
};

#endif