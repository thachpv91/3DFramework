precision mediump float;

uniform samplerCube u_cubeMap1;
uniform vec3 u_camPosW;

//varying vec4 v_color;
varying vec3 v_posW;
varying vec3 v_normW;
varying float v_distance;

vec4 SetFog(vec4 objColor);

#define CUBE_TEXTURE u_cubeMap1

void main()
{
	vec3 toEye = u_camPosW - v_posW;
	vec3 reflectDir = reflect(normalize(-toEye), v_normW);
	vec4 color = textureCube(CUBE_TEXTURE, reflectDir);//v_color; //vec4(1.0, 0.0, 0.0, 1.0);
	
	gl_FragColor = color;
}

vec4 SetFog(vec4 objColor)
{
	float fogStart = 10.0;
	float fogLength = 100.0;
	float lerpValue = clamp((v_distance - fogStart) / fogLength, 0.0, 1.0);
	vec4 fogColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	return (lerpValue * fogColor + (1.0 - lerpValue) * objColor);
}
