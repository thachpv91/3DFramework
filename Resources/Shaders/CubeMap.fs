precision mediump float;

uniform samplerCube u_cubeMap1;

varying vec3 v_posL;
varying float v_distance;

vec4 SetFog(vec4 objColor);

#define CUBE_TEXTURE u_cubeMap1

void main()
{
	vec4 color = textureCube(CUBE_TEXTURE, v_posL);

	gl_FragColor = color;//(lerpValue * fogColor + (1.0 - lerpValue) * color);
}

vec4 SetFog(vec4 objColor)
{
	float fogStart = 10.0;
	float fogLength = 100.0;
	float lerpValue = clamp((v_distance - fogStart) / fogLength, 0.0, 1.0);
	vec4 fogColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	return (lerpValue * fogColor + (1.0 - lerpValue) * objColor);
}
