#include "Collision.h"
#include <algorithm>
using namespace std;

bool Collision::RayCastIntersects(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 boxMin, glm::vec3 boxMax, float& howFar){
    glm::vec3 invDir = 1.0f/rayDirection;

    glm::vec3 t0 = (boxMin-rayOrigin)*invDir;
    glm::vec3 t1 = (boxMax-rayOrigin)*invDir;

    glm::vec3 tmin = glm::min(t0,t1);
    glm::vec3 tmax = glm::max(t0,t1);

    float tnear = glm::max(glm::max(tmin.x,tmin.y),tmin.z);
    float tfar = glm::min(glm::min(tmax.x,tmax.y),tmax.z);

    if(tnear>tfar || tfar<0.0f) return false;

    howFar = tnear;

    return true;
}

glm::vec3 Collision::RayIntersectsFloor(glm::vec3 rayOrigin, glm::vec3 rayDirection, float floorHeight) {
    if (abs(rayDirection.y) < 0.0001f) return glm::vec3(0.0f, floorHeight, 0.0f); 
    float t = (floorHeight - rayOrigin.y) / rayDirection.y;
    return rayOrigin + rayDirection * t;
}

bool Collision::CheckCollision_Sphere(glm::vec3 SphereCenter,float radius,glm::vec3 aabbCenter,glm::vec3 aabbSize){
    // extracting the edges
    glm::vec3 halfExtents = aabbSize/2.0f;
    glm::vec3 aabbMin = aabbCenter - halfExtents;
    glm::vec3 aabbMax = aabbCenter + halfExtents;

    float closestX = max(aabbMin.x,min(SphereCenter.x,aabbMax.x));
    float closestY = max(aabbMin.y,min(SphereCenter.y,aabbMax.y));
    float closestZ = max(aabbMin.z,min(SphereCenter.z,aabbMax.z));

    glm::vec3 closestPoint = glm::vec3(closestX,closestY,closestZ);
    float dist = glm::distance(SphereCenter,closestPoint);

    return dist < radius;
}