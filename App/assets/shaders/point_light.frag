#version 450

layout(location = 0) in vec2 in_FragOffset;
layout(location = 0) out vec4 out_Color;


// Struct for point light data
struct PointLight
{
    vec4 Position; // Ignore w component
    vec4 Color; // w is intensity
};

// Uniform block for global data
layout(std140, binding = 0) uniform GlobalUbo
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
    vec4 AmbientLightColor;
    PointLight PointLights[10];
    int numLights;
} ubo;

// Push constants for temporary data
layout(push_constant) uniform Push
{
    vec4 Position;
    vec4 Color;
    float Radius;
} push;

void main()
{
    if (length(in_FragOffset) >= 1.0) discard;
    out_Color = vec4(push.Color.rgb, 1.0); // Ensure correct color assignment
}