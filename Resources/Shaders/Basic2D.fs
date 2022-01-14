precision mediump float;
uniform sampler2D u_texture1;

varying vec2 v_uv;
void main()
{
	 vec4 cl = texture2D(u_texture1, vec2(v_uv.x, v_uv.y));  
     gl_FragColor = cl;
}
