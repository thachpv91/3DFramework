precision mediump float;

#define NUMBER_OF_LIGHT 5
#define POSITION 0
#define DIRECTION 1

struct ambient
{
	vec4 color;
	float weight;
};

struct light
{
	vec3 position;
	int type;
	vec4 color;
};

uniform sampler2D u_texture1;

uniform vec3 u_camPosW;

uniform ambient u_ambient;
uniform light u_light[NUMBER_OF_LIGHT];

varying vec2 v_uv;
varying vec3 v_posW;
varying vec3 v_normW;
varying float v_distance;

vec4 SetFog(vec4 objColor);

void main()
{
	vec4 totalDiffuse = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 totalSpecular = vec4(0.0, 0.0, 0.0, 0.0);
	
	for (int i = 0; i < NUMBER_OF_LIGHT; i++)
	{
		vec3 lightDir;
		if (u_light[i].type == POSITION)
		{
			lightDir = normalize(v_posW - u_light[i].position);
		}
		else if (u_light[i].type == DIRECTION)
		{
			lightDir = u_light[i].position;
		}
		totalDiffuse += (max(dot(v_normW, -lightDir), 0.0) * u_light[i].color);
		
		vec3 reflectVector = normalize(reflect(lightDir, v_normW));
		vec3 toEye = normalize(u_camPosW - v_posW);
		totalSpecular += (pow(max(dot(reflectVector, toEye), 0.0), 20.0) * u_light[i].color);
	}
		
	vec4 objColor = texture2D(u_texture1, vec2(v_uv.x, -v_uv.y));
		
	vec4 finalColor = vec4(((u_ambient.color * u_ambient.weight + totalDiffuse * (1.0 - u_ambient.weight)) * objColor + totalSpecular).xyz, objColor.w);
	
	gl_FragColor = finalColor;
}

vec4 SetFog(vec4 objColor)
{
	float fogStart = 10.0;
	float fogLength = 100.0;
	float lerpValue = clamp((v_distance - fogStart) / fogLength, 0.0, 1.0);
	vec4 fogColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	return (lerpValue * fogColor + (1.0 - lerpValue) * objColor);
}