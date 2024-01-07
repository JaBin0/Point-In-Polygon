#version 430 core
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoord;

uniform mat4 projection;

out vec2 texCoord;

void main() {
    texCoord = vTexCoord;
    gl_Position = projection * vec4(vPos.x, vPos.y, -0.1, 1.0);
}
