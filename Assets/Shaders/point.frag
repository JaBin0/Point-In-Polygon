#version 430 
out vec4 FragColor;

uniform vec4 color;
uniform vec2 center;
uniform float radius;

void main() {
    vec2 pos = (center - gl_FragCoord.xy);
    float distanceSquared = dot(pos, pos);
    FragColor = (distanceSquared) < (radius * radius)
                ? color
                : vec4(1.0, 1.0, 1.0, 0.0);
}