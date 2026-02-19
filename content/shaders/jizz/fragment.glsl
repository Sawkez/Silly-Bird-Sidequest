#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here
const float paletteMult = 255.0 / 25.0;

uniform vec3[10] palette;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);

    // keep pixel, don't apply palette
    bool keep = texelColor.g > 0.0;

    // find palette color
    int paletteIdx = int(texelColor.b * paletteMult);
    vec3 paletteClr = palette[paletteIdx];

    // mixing
    finalColor.rgb = texelColor.rgb * float(keep);
    finalColor.rgb += paletteClr * float(!keep);

    finalColor.rgb *= colDiffuse.rgb * fragColor.rgb;
    
    finalColor.a = texelColor.a;
}
