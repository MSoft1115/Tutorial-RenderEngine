
Uniform
{
	u_Asecpt FLOAT1
	u_WorldMatrix WORLD_MATRIX
}

VertexShader {

	attribute vec4 POSITION;
	attribute vec4 COLOR;

	varying vec4 v_color;

	uniform mat4 u_WorldMatrix;
	uniform float u_Asecpt;

	void main()
	{
		gl_Position = u_WorldMatrix * POSITION;
		v_color = COLOR;
		gl_Position.x *= u_Asecpt;
	}

}

PixelShader {

	varying vec4 v_color;

	void main()
	{
		gl_FragColor = v_color;
	}

}