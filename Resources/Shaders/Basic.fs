precision mediump float;

uniform sampler2D u_texture1;

varying vec4 v_color;
varying vec2 v_uv;
varying float v_distance;

vec4 SetFog(vec4 objColor);

#define OBJECT_TEXTURE u_texture1

void main()
{
	vec4 color = texture2D(OBJECT_TEXTURE, vec2(v_uv.x, -v_uv.y));
	
	gl_FragColor = vec4((color + v_color).rgb, color.a);
}

vec4 SetFog(vec4 objColor)
{
	float fogStart = 10.0;
	float fogLength = 100.0;
	float lerpValue = clamp((v_distance - fogStart) / fogLength, 0.0, 1.0);
	vec4 fogColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	return (lerpValue * fogColor + (1.0 - lerpValue) * objColor);
}
