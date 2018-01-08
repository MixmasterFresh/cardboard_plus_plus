#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 leftView;
    mat4 rightView;
    mat4 leftProj;
    mat4 rightProj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

void undistort(inout vec4 position){
    float r2 = dot(position.xy, position.xy) / (position.z * position.z); // currently missing clamp call
    float disp = 1 + (-0.5256 + (0.4278 * r2)) * r2;
    // float r = (position.x + position.y) / (position.z); // currently missing clamp call
    // float disp = 1 + ((((-0.5256 * r + 0.4278) * r - 00.2525) * r + 0.08995) * r - 0.01705) * r;
    position.xy = position.xy * disp;
}

void main() {
    vec4 pos = ubo.leftView * vec4(inPosition, 1.0);
    undistort(pos);
    gl_Position = ubo.leftProj * pos;
    // gl_Position = ubo.proj * ubo.leftView * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
