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
uniform sampler2D u_texture2;

#define MODEL_TEXTURE u_texture1
#define NORMAL_MAP_TEXTURE u_texture2

uniform vec3 u_camPosW;

uniform ambient u_ambient;
uniform light u_light[NUMBER_OF_LIGHT];

varying vec2 v_uv;
varying vec3 v_posW;
varying vec3 v_normW;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;
varying float v_distance;

vec4 SetFog(vec4 objColor);

void main()
{
	vec4 totalDiffuse = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 totalSpecular = vec4(0.0, 0.0, 0.0, 0.0);

	vec3 normal = texture2D(NORMAL_MAP_TEXTURE, vec2(v_uv.x, -v_uv.y)).xyz;
	mat3 TBN = mat3(normalize(v_tangentW), normalize(v_bitangentW), normalize(v_normW));
	vec3 normalW = normalize(TBN * (2.0 * normal - 1.0));
    
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
		totalDiffuse += (max(dot(normalW, -lightDir), 0.0) * u_light[i].color);
		
		vec3 reflectVector = normalize(reflect(lightDir, normalW));
		vec3 toEye = normalize(u_camPosW - v_posW);
		totalSpecular += (pow(max(dot(reflectVector, toEye), 0.0), 20.0) * u_light[i].color);
	}
		
	vec4 objColor = texture2D(MODEL_TEXTURE, vec2(v_uv.x, -v_uv.y));
		
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