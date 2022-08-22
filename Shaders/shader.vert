#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    vec3 cameraPosition;
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat3 normalMatrix;
} push;



layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragPosition;

void main() {
    gl_Position = ubo.proj * ubo.view * push.modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = push.normalMatrix * inNormal;
    fragPosition = vec3(push.modelMatrix * vec4(inPosition, 1.0));
}