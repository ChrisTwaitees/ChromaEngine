#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


in GS_OUT{
	bool X;
	bool Y;
	bool Z;
	bool Joint;
} gs_in;

uniform vec3 color;

void main()
{

	if (gs_in.X)
		FragColor = vec4(1.0, 0.0, 0.0 , 1.0);
	else if(gs_in.Y)
		FragColor = vec4(0.0, 1.0, 0.0 , 1.0);
	else if(gs_in.Z)
		FragColor = vec4(0.0, 0.0, 1.0 , 1.0);
	else if(gs_in.Joint)
		FragColor = vec4(color , 1.0);
	else
		FragColor = vec4(color , 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}