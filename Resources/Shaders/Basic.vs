attribute vec3 a_positionL;
attribute vec3 a_normalL;
attribute vec4 a_color;
attribute vec2 a_uv;
attribute vec3 a_tangentL;
attribute vec3 a_bitangentL;

uniform mat4 u_wvpMatrix;
uniform mat4 u_wMatrix;

varying vec4 v_color;
varying vec2 v_uv;
varying float v_distance;
varying vec3 v_posL;
varying vec3 v_posW;
varying vec3 v_normW;
varying vec3 v_tangentW;
varying vec3 v_bitangentW;

void main()
{
	vec4 positionL = vec4(a_positionL, 1.0);
	gl_Position = u_wvpMatrix * positionL;
	v_color = a_color;
	v_uv = a_uv;
	v_distance = length(gl_Position);
	v_posW = (u_wMatrix * positionL).xyz;
	v_normW = normalize((u_wMatrix * vec4(a_normalL, 0.0)).xyz);
	v_tangentW = normalize((u_wMatrix * vec4(a_tangentL, 0.0)).xyz);
	v_bitangentW = normalize((u_wMatrix * vec4(a_bitangentL, 0.0)).xyz);
	v_posL = a_positionL;
}