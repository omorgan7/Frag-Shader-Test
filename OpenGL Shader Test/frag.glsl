R"(
#version 330 core


vec3 hsvToRGB(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

bool drawCircle(vec2 resolution, vec2 position, float radius, float aspectRatio){
    vec2 uv = (2.0f * gl_FragCoord.xy / resolution) - vec2(1.0f);
    uv.y /= aspectRatio;
    uv = uv - position;

    float distance = uv.x * uv.x + uv.y * uv.y;
//
    return distance > radius * radius || distance < 0.8f * radius * radius;
}

void drawSquare(vec2 resolution, vec2 position, float radiusSquared){
    // do nothing, we are already drawing a quad.
}

// in vec4 gl_FragCoord;
in vec3 fragColour;
//in vec2 outPosition;
//in float outRadius;
//in float aspectRatio;

out vec4 colour;

uniform vec2 resolution;

void main(){
//    bool shouldDraw = drawCircle(resolution, outPosition, outRadius, aspectRatio);
//    colour = vec4(fragColour, shouldDraw ? 1.0f : 0.0f);
    colour = vec4(fragColour, 1.0f);
}
)"
