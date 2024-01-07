#version 430 
in vec2 texCoord;

uniform sampler2D image;

out vec4 FragColor;

void main() {
    // FragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
    // FragColor = mix(texture(image, texCoord), vec4(1.0, 0.5, 0.7, 1.0), 0.4);
    FragColor = texture(image, texCoord);
}