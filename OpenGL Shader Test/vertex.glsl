R"(
#version 330 core

layout(location = 0) in vec2 quadPosition;
layout(location = 1) in vec2 position;
layout(location = 2) in float radius;
layout(location = 3) in vec3 colour;

uniform vec2 resolution;

out vec3 fragColour;
out vec2 outPosition;
out float outRadius;
out float aspectRatio;

void main(){
    aspectRatio = resolution.x / resolution.y;
    vec2 shiftedPosition = quadPosition * radius + position;
//    gl_Position = vec4(quadPosition, 0, 1);
    gl_Position = vec4(shiftedPosition.x, shiftedPosition.y * aspectRatio, 0.0f, 1.0f);
    fragColour = colour;
    outPosition = vec2(position.x, position.y);
    outRadius = radius;
}
)"
