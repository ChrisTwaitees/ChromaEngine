#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

//uniform sampler2D screenTexture;

uniform sampler2DArray screenTexture;
//uniform int txLayer;
uniform int layer;

void main()
{	
	vec4 screen = texture(screenTexture, vec3(TexCoords.st, layer));
	//vec4 screen = texture(screenTexture, TexCoords.st);

	// OUT
	FragColor = vec4(screen.rgb, 1.0);
	//FragColor = vec4(1.0,1.0,0.0, 1.0);

	// POST FX
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}  