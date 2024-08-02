#version 450

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_Uv;

layout(location = 0) out vec3 out_FragColor;
layout(location = 1) out vec3 out_FragPosWorld;
layout(location = 2) out vec3 out_FragNormalWorld;

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
	mat4 ModelMatrix;
	mat4 NormalMatrix;
} push;

void main() 
{
	vec4 positionWorld = push.ModelMatrix * vec4(in_Position, 1.0);
	gl_Position = ubo.ProjectionMatrix * ubo.ViewMatrix * positionWorld;

	out_FragNormalWorld = normalize(mat3(push.NormalMatrix) * in_Normal);
	out_FragPosWorld = positionWorld.xyz;
	out_FragColor = in_Color;
}