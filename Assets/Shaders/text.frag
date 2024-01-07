#version 430 core
in vec2 fTex;

out vec4 output_color;

uniform sampler2D text;
uniform vec4 color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, fTex).r);
    output_color = color * sampled;
} 