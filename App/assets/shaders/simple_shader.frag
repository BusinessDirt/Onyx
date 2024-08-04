#version 450

layout (location = 0) in vec3 in_FragColor;
layout (location = 1) in vec3 in_FragPositionWorld;
layout (location = 2) in vec3 in_FragNormalWorld;

layout (location = 0) out vec4 out_Color;

struct PointLight
{
	vec4 Position; // ignore w
	vec4 Color; // w is intensity
};

layout(std140, binding = 0) uniform GlobalUbo
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
	vec3 diffuseLight = ubo.AmbientLightColor.xyz * ubo.AmbientLightColor.w;
	vec3 surfaceNormal = normalize(in_FragNormalWorld);

	for (int i = 0; i < ubo.numLights; i++)
	{
		PointLight light = ubo.PointLights[i];
		vec3 directionToLight = light.Position.xyz - in_FragPositionWorld;
		float attenuation = 1.0 / dot(directionToLight, directionToLight);
		float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
		vec3 intensity = light.Color.xyz * light.Color.w * attenuation;


		diffuseLight += intensity * cosAngIncidence;
	}

	out_Color = vec4(diffuseLight * in_FragColor, 1.0);
}