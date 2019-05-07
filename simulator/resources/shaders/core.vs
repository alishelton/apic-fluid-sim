#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoordinate;

out vec2 TexCoord;

uniform mat4 model; //Convert local obj coords to camera coords
uniform mat4 view; //Convert normalized coordinates to screen/window coordinates
uniform mat4 projection; //Convert cam coordinates to normalized coordinates



void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    TexCoord = vec2(texCoordinate.x, 1.0 - texCoordinate.y);
}

