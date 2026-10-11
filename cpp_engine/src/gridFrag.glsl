#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
uniform vec3 cameraPos; // We need this to calculate the fade distance!

void main() {
    float thickness = 0.02; // How thick the grid lines are

    // fract() returns the decimal part. fract(1.05) = 0.05.
    float gridX = fract(WorldPos.x);
    float gridZ = fract(WorldPos.z);

    // If the decimal is very close to 0.0 or 1.0, we are sitting on a perfect whole number!
    float lineX = step(gridX, thickness) + step(1.0 - thickness, gridX);
    float lineZ = step(gridZ, thickness) + step(1.0 - thickness, gridZ);

    // Combine X and Z lines
    float gridAlpha = max(lineX, lineZ);

    // Fade out smoothly based on distance from the camera
    float distance = length(WorldPos - cameraPos);
    // Starts fading at 10 meters, fully invisible by 50 meters
    float fade = 1.0 - smoothstep(10.0, 50.0, distance); 

    // Final color: An industrial phosphor green line
    FragColor = vec4(0.2, 0.4, 0.2, gridAlpha * fade * 0.8);
}
