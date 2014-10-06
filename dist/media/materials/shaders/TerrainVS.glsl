#version 150

// Simple vertex shader for terrain.
// Calculates world space normal and texture coordinates.

in vec4 vertex;
in vec3 normal;
in vec2 uv0;
// out vec4 passPos;
out vec3 passNormal;
out vec2 passUV;

uniform mat4 p_mvp;
uniform mat4 p_m;

void main()
{
    gl_Position = p_mvp * vertex;
    passNormal = vec3(p_m * vec4(normal.x, normal.y, normal.z, 0.0));
    passNormal = normal;
    passUV = uv0;
}
