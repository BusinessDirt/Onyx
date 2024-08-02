#version 450

layout (location = 0) in vec2 in_FragOffset;
layout (location = 0) out vec4 out_Color;

struct PointLight
{
	vec4 Position; // ignore w
	vec4 Color; // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec4 AmbientLightColor;
	PointLight PointLights[10];
	int numLights;
} ubo;

layout(push_constant) uniform Push
{
	vec4 Position;
	vec4 Color;
	float Radius;
} push;

void main()
{
	float dist = sqrt(dot(in_FragOffset, in_FragOffset));
	if (dist >= 1.0) discard;
	out_Color = vec4(push.Color.xyz, 1.0);
}