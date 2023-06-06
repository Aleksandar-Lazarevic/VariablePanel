//--------------------------
// - Hazel 2D -
// Renderer2D Line Shader
// --------------------------

#type vertex
#version 300 es
precision mediump float;
layout(location=0)in vec3 a_Position;
layout(location=1)in vec4 a_Color;
layout(location=2)in int a_EntityID;

layout(std140)uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
};

out VertexOutput v_data;
flat out int v_EntityID;

void main()
{
	v_data.Color=a_Color;
	v_EntityID=a_EntityID;
	
	gl_Position=u_ViewProjection*vec4(a_Position,1.);
}

#type fragment
#version 300 es
precision mediump float;
layout(location=0) out vec4 o_Color;
layout(location=1) out int o_EntityID;

struct VertexOutput
{
	vec4 Color;
};

in VertexOutput v_data;
flat in int v_EntityID;

void main()
{
	o_Color=v_data.Color;
	o_EntityID=v_EntityID;
}
