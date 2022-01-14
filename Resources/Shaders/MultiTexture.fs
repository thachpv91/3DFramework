precision mediump float;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;

//varying vec4 v_color;
varying vec2 v_uv;
varying float v_distance;

vec4 SetFog(vec4 objColor);

#define ROCK_TEXTURE u_texture1
#define GRASS_TEXTURE u_texture2
#define DIRT_TEXTURE u_texture3
#define BLEND_MAP_TEXTURE u_texture4

void main()
{
	vec4 rock = texture2D(ROCK_TEXTURE, vec2(v_uv.x, -v_uv.y) * 100.0);
	vec4 grass = texture2D(GRASS_TEXTURE, vec2(v_uv.x, -v_uv.y) * 100.0);
	vec4 dirt = texture2D(DIRT_TEXTURE, vec2(v_uv.x, -v_uv.y) * 100.0);
	vec4 blend = texture2D(BLEND_MAP_TEXTURE, vec2(v_uv.x, -v_uv.y));
	
	vec4 color = (blend.r * rock + blend.g * grass + blend.b * dirt) / (blend.r + blend.g + blend.b);
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
