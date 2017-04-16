
Uniform
{
	u_WorldMatrix WORLD_MATRIX
	u_ViewMatrix VIEW_MATRIX
	u_ProjMatrix PROJ_MATRIX

	u_Time FLOAT1
}

Sampler
{
	// 注意Index对应于SetSamplerTexture的参数
	Index 0
	Name u_Sampler0;
	Filter LINEAR
	AdressMode WRAP
	BorderColor 0 0 0 0
}

VertexShader {

	attribute vec4 POSITION;
	attribute vec4 COLOR;
	attribute vec2 TEXCOORD0;

	varying vec2 v_uv;

	uniform mat4 u_WorldMatrix;
	uniform mat4 u_ViewMatrix;
	uniform mat4 u_ProjMatrix;

	uniform float u_Time;

	void main()
	{
		const float PI = 3.1415926;

		// 波动
		POSITION.y += POSITION.y * sin(POSITION.x * PI + u_Time * PI) * 0.15;

		// 世界变换
		gl_Position = u_WorldMatrix * POSITION;

		// 相机变换
		gl_Position = u_ViewMatrix * gl_Position;

		// 投影变换
		gl_Position = u_ProjMatrix * gl_Position;

		// 输出纹理坐标
		v_uv = TEXCOORD0;
	}

}

PixelShader {

	varying vec2 v_uv;

	uniform sampler2D u_Sampler0;

	void main()
	{
		// 采样纹理
		vec4 texColor = texture2D(u_Sampler0, v_uv);

		// 输出颜色
		gl_FragColor = texColor;
	}

}