#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D vsNormals;
uniform sampler2D vsPositions;
uniform sampler2D vAlbedo;
uniform sampler2D vDepth;

uniform mat4 vProjection;



vec2 enabled = vec2(1.0);
float maxDistance = 2;
float resolution  = 0.3;
int   steps       = 5;
float thickness   = 0.5;

void main() {

	// initialize parms
	vec2 texSize  = textureSize(vsPositions, 0).xy;
	vec2 texCoord = gl_FragCoord.xy / texSize;

    vec4 uv = vec4(0);

	// Gather current frags positions, normal and reflection around normal
    vec4 positionFrom = texture(vsPositions, texCoord);
    if ( positionFrom.w <= 0 || enabled.x != 1 ) { FragColor = uv; return; } // check whether position in clip space (negative position.w indicates it's outside of clip space) 
    vec3 unitPositionFrom = normalize(positionFrom.xyz);
    vec3 normal           = normalize(texture(vsNormals, texCoord).xyz);
    vec3 pivot            = normalize(reflect(unitPositionFrom, normal));
    vec4 positionTo = positionFrom;

	// Calculalate the start and end point of the reflection ray in view space
    vec4 startView = vec4(positionFrom.xyz + (pivot *           0), 1);
    vec4 endView   = vec4(positionFrom.xyz + (pivot * maxDistance), 1);

	// Transform start and end points from View to ScreenSpace
    vec4 startFrag      = startView;
         startFrag      = vProjection * startFrag;
         startFrag.xyz /= startFrag.w;
         startFrag.xy   = startFrag.xy * 0.5 + 0.5;
         startFrag.xy  *= texSize;
  
    vec4 endFrag      = endView;
         endFrag      = vProjection * endFrag;
         endFrag.xyz /= endFrag.w;
         endFrag.xy   = endFrag.xy * 0.5 + 0.5;
         endFrag.xy  *= texSize;

	// Now in ScreenSpace (x,y coordinates on the screen) we can travel/march along its direction in screenspace
	// We'll use these screen coordinates to sample the frag positions stored in the VS positions framebuffer


	// FIRST PASS
	// Convert frag position to UV coordinate by dividing the coordinates by the texture dimensions
    vec2 frag  = startFrag.xy;
         uv.xy = frag / texSize;

	// We'll establish the number of X and Y pixels we'll need to traverse by calculating the difference between the 
	// x and y coordinates of the start and end positions
    float deltaX    = endFrag.x - startFrag.x;
    float deltaY    = endFrag.y - startFrag.y;

	// To caculate how to traverse this VS to PS projected vector, we'll need to establish whether the line in coordinates 
	// is vertical, horizontal, diagonal etc. 
	// We'll establish which delta between x,y is larger and the delta between the two which will describe the increment 
	// direction we'll take to traverse the screen space vector
	// Resolution will determine how many fragments are skipped during this marching process
    float useX      = abs(deltaX) >= abs(deltaY) ? 1 : 0;
    float delta     = mix(abs(deltaY), abs(deltaX), useX) * clamp(resolution, 0, 1);
    vec2  increment = vec2(deltaX, deltaY) / max(delta, 0.001);
  
	// With an increment offset established we can now traverse the ScreenSpace vector
	// Initialize the first (search0) and second (search1) search coords 
    float search0 = 0;
    float search1 = 0;
  
	// Initialize the first (hit0) and second pass (hit1)  hits 
    int hit0 = 0;
    int hit1 = 0;

	// Viewdistance is how far the camera is from the current fragpos
  
    float viewDistance = startView.y;
    float depth        = thickness;
  
    float i = 0;
  
    for (i = 0; i < int(delta); ++i) {
      frag      += increment;
      uv.xy      = frag / texSize;
      positionTo = texture(vsPositions, uv.xy);
  
      search1 =
        mix
          ( (frag.y - startFrag.y) / deltaY
          , (frag.x - startFrag.x) / deltaX
          , useX
          );
  
      search1 = clamp(search1, 0, 1);
  
      viewDistance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
      depth        = viewDistance - positionTo.y;
  
      if (depth > 0 && depth < thickness) {
        hit0 = 1;
        break;
      } else {
        search0 = search1;
      }
    }
  
    search1 = search0 + ((search1 - search0) / 2);
  
    steps *= hit0;
  
    for (i = 0; i < steps; ++i) {
      frag       = mix(startFrag.xy, endFrag.xy, search1);
      uv.xy      = frag / texSize;
      positionTo = texture(vsPositions, uv.xy);
  
      viewDistance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
      depth        = viewDistance - positionTo.y;
  
      if (depth > 0 && depth < thickness) {
        hit1 = 1;
        search1 = search0 + ((search1 - search0) / 2);
      } else {
        float temp = search1;
        search1 = search1 + ((search1 - search0) / 2);
        search0 = temp;
      }
    }
  
    float visibility =
        hit1
      * positionTo.w
      * ( 1
        - max
           ( dot(-unitPositionFrom, pivot)
           , 0
           )
        )
      * ( 1
        - clamp
            ( depth / thickness
            , 0
            , 1
            )
        )
      * ( 1
        - clamp
            (   length(positionTo - positionFrom)
              / maxDistance
            , 0
            , 1
            )
        )
      * (uv.x < 0 || uv.x > 1 ? 0 : 1)
      * (uv.y < 0 || uv.y > 1 ? 0 : 1);
  
    visibility = clamp(visibility, 0, 1);
  
    uv.ba = vec2(visibility);
  
    //FragColor = uv.bbb;
    //FragColor = vec4(vec3(uv.aaa), 1.0);
    FragColor = texture(vAlbedo, uv.xy);
	//FragColor = vec4(texture(vDepth, TexCoords).xxx, 1.0);

	// POST FX
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}  