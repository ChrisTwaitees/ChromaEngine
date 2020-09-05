
const float ENCODEX = 1.0/255.0;
const float ENCODEY = 1.0/65025.0;
const float ENCODEZ = 1.0/16581375.0;


float EncodeVec4ToFloat(vec4 vec)
{
	return dot( vec, vec4(1.0, ENCODEX, ENCODEY, ENCODEZ) );
}

vec4 DecodeFloatToVec4(float f)
{
  vec4 enc = vec4(1.0, 255.0, 65025.0, 16581375.0) * f;
  enc = fract(enc);
  enc -= enc.yzww * vec4(ENCODEX,ENCODEX,ENCODEX,0.0);
  return enc;
}


// NORMAL 
const float c_precision = 256.0;
const float c_precisionp1 = c_precision + 1.0;

float PackNormal(vec3 normal)
{
	// pack 
	normal *= 255.0;
	normal = floor(normal);
	return normal.r * 256.0 * 256.0 + normal.g * 256.0 + normal.b - 8388608.0;
}

vec3 UnPackNormal(float a)
{
    a += 8388608.; // this makes values signed
    if(a < 0.) {
        return vec3(0.);
    }
    if(a > 16777216.) {
        return vec3(1.);
    }
    vec3 c = vec3(0.0);
    c.b = mod(a, 256.0);
    a = floor(a/256.0);
    c.g = mod(a, 256.0);
    a = floor(a/256.0);
    c.r = mod(a, 256.0);
    c/=255.0;

    return normalize(c);
}