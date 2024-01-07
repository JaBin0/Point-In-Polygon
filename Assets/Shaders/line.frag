#version 430 
out vec4 FragColor;

uniform vec4 color;
uniform vec2 startPoint;
uniform bool tmpLine = false;

void main() {
    if(tmpLine) {
        float dist = sqrt(pow(startPoint.x - gl_FragCoord.x, 2) + pow(startPoint.y - gl_FragCoord.y, 2));
        float mod_value = 20;
        if(mod(dist, mod_value) > mod_value / 2) {
            discard;
        }
        else {
            FragColor = color;
        }
    }
    else {
        FragColor = color;
    }
}