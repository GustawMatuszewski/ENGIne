// Scripts/Rotating.cpp
#include "Entity.hpp"
#include "EcsManager.hpp"
#include <glm/glm.hpp>

void effect(TransformComponent transform) {
    static float rotationAngle = 0.0f;

    // Update rotation angle over time
    rotationAngle += 0.01f; // Adjust speed as needed

    // Apply rotation to the transform
    transform.rotation = glm::vec3(0.0f, 0.0f, rotationAngle);

    // Optional: Add some orbiting effect by updating position
    float orbitRadius = 1.0f;
    float orbitSpeed = 0.005f;
    float orbitAngle = rotationAngle * orbitSpeed;

    float x = orbitRadius * cos(orbitAngle);
    float y = orbitRadius * sin(orbitAngle);
    float z = 0.0f;

    transform.position = glm::vec3(x, y, z);
}
