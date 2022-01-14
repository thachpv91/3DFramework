attribute vec3 a_positionL;
attribute vec2 a_uv;

uniform mat4 u_wMatrix;

varying vec2  v_uv;
void main()
{
	vec4 positionL = vec4(a_positionL, 1.0);
	gl_Position = u_wMatrix * positionL;
	v_uv = a_uv;
}