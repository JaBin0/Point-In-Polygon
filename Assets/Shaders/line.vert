#version 430 core
layout (location = 0) in vec2 vPos;

uniform mat4 projection;
uniform vec2 startPoint;


void main() {
    gl_Position = projection * vec4(vPos.x, vPos.y, -0.1, 1.0);
    

}
