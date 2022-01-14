precision mediump float;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;

uniform float u_time;

varying vec2 v_uv;
varying float v_distance;

vec4 SetFog(vec4 objColor);

#define DISPLACEMENT_TEXTURE u_texture1
#define OBJECT_TEXTURE u_texture2
#define FIRE_MASK_TEXTURE u_texture3

void main()
{
	const float dMax = 0.1;
	
	vec2 disp = texture2D(DISPLACEMENT_TEXTURE, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offset = (2.0 * disp - 1.0) * dMax;
	
	vec2 uv_new = v_uv + offset;
	vec4 fireColor = texture2D(OBJECT_TEXTURE, vec2(uv_new.x, -uv_new.y));
	
	float alpha = texture2D(FIRE_MASK_TEXTURE, vec2(v_uv.x, -v_uv.y)).r;
	vec4 color = fireColor * vec4(1.0, 1.0, 1.0, alpha);
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
