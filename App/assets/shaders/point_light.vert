#version 450

const vec2 OFFSETS[6] = vec2[](
	vec2(-1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0)
);

layout (location = 0) out vec2 out_FragOffset;

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
	vec4 Position;
	vec4 Color;
	float Radius;
} push;

void main()
{
	out_FragOffset = OFFSETS[gl_VertexIndex];
	vec3 cameraRightWorld = { ubo.ViewMatrix[0][0], ubo.ViewMatrix[1][0], ubo.ViewMatrix[2][0] };
	vec3 cameraUpWorld = { ubo.ViewMatrix[0][1], ubo.ViewMatrix[1][1], ubo.ViewMatrix[2][1] };

	vec3 positionWorld = push.Position.xyz
		+ push.Radius * out_FragOffset.x * cameraRightWorld
		+ push.Radius * out_FragOffset.y * cameraUpWorld;

	gl_Position = ubo.ProjectionMatrix * ubo.ViewMatrix * vec4(positionWorld, 1.0);

}