#version 150

// Triplanar texture mapping for terrain.
in vec3 passNormal;
in vec2 passUV;

out vec4 fragColour;

uniform sampler2D p_diffuse;
uniform vec4 p_lightPos;

void main()
{
    // Wanna know more about triplanar texturing?
    // Here is the article I took as a source:
    // http://memoirsofatexel.blogspot.de/2010/08/terrain-triplanar-uv-mapping.html

    // // Weights.
    // float tighten = 0.21;
    // float wXY = abs(passNormal.z) - tighten;
    // float wXZ = abs(passNormal.y) - tighten;
    // float wYZ = abs(passNormal.x) - tighten;

    // // Make sum = 1.
    // float total = wXY + wXZ + wYZ;
    // wXY /= total;
    // wXZ /= total;
    // wYZ /= total;

    // // Take three samples.
    // // vec4 cXY = texture2D(p_diffuse, passUV.xy);
    // // vec4 cXZ = texture2D(p_diffuse, passUV.xz);
    // // vec4 cYZ = texture2D(p_diffuse, passUV.yz);
    // vec4 cXY = texture2D(p_diffuse, passUV.xy);
    // vec4 cXZ = texture2D(p_diffuse, vec2(passUV.x, 1));
    // vec4 cYZ = texture2D(p_diffuse, vec2(passUV.y, 1));

    // // Combine samples.
    // fragColour = cXY * wXY + cXZ * wXZ + cYZ * wYZ;
    fragColour *= max(dot(p_lightPos.xyz, passNormal), 0.1);

    fragColour = texture2D(p_diffuse, passUV);
}
