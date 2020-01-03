#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;
uniform float gamma;

void main()
{             
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(bloom)
        hdrColor += bloomColor; // additive blending

    // exposure
    vec3 result = vec3(1.0) - exp(-hdrColor * 1.0);
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / 2.2));
    FragColor = vec4(hdrColor, 1.0);

}