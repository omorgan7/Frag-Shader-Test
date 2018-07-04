R"(
#version 330 core


vec3 hsvToRGB(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float drawRing(vec2 resolution, vec2 position, float radius, float aspectRatio){
    vec2 uv = (2.0f * gl_FragCoord.xy / resolution) - vec2(1.0f);
    uv.y /= aspectRatio;
    uv = uv - position;

    float dist = dot(uv, uv);
//
    float r2 = radius * radius;
    float e = fwidth(dist);
    float alpha1 = 1.0f - smoothstep(r2 - e, r2 + e, dist);
    if (alpha1 <= 0.0f) {
        discard;
    }
    r2 *= 0.4f;
    float alpha2 = smoothstep(r2 - e, r2 + e, dist);
    if (alpha2 <= 0.0f) {
        discard;
    }
    return min(alpha1, alpha2);
}

bool drawRingBool(vec2 resolution, vec2 position, float radius, float aspectRatio){
    vec2 uv = (2.0f * gl_FragCoord.xy / resolution) - vec2(1.0f);
    uv.y /= aspectRatio;
    uv = uv - position;
    
    float distance = dot(uv, uv);
    //
    return distance < radius * radius && distance > 0.4 * radius * radius;
}

float drawCircle(vec2 resolution, vec2 position, float radius, float aspectRatio){
    vec2 uv = (2.0f * gl_FragCoord.xy / resolution) - vec2(1.0f);
    uv.y /= aspectRatio;
    uv = uv - position;
    
    float dist = dot(uv, uv);
    float e = fwidth(dist);
    float alpha = 1.0f - smoothstep(radius * radius - e, radius * radius + e, dist);
    if (alpha <= 0.0f) {
        discard;
    }
    return alpha;
}

void drawSquare(vec2 resolution, vec2 position, float radiusSquared){
    // do nothing, we are already drawing a quad.
}


in vec3 fragColour;
in vec2 outPosition;
in float outRadius;
in float aspectRatio;

out vec4 colour;

uniform vec2 resolution;

void main(){
//    float alpha = drawCircle(resolution, outPosition, outRadius, aspectRatio);
    float alpha = drawRing(resolution, outPosition, outRadius, aspectRatio);
//    drawRing(resolution, outPosition, outRadius, aspectRatio);
//    float shouldDraw = float(drawRingBool(resolution, outPosition, outRadius, aspectRatio));
    
//    colour = vec4(fragColour, shouldDraw ? 1.0f : 0.0f);
    vec3 rgbCol = hsvToRGB(fragColour);
    colour = vec4(rgbCol, alpha);
//    colour = vec4(rgbCol, 1.0f);
}
)"
