//--------------------------
// - Hazel 2D -
// Renderer2D Circle Shader
// --------------------------

#type vertex
#version 300 es
precision mediump float;
layout(location=0)in vec3 a_WorldPosition;
layout(location=1)in vec3 a_LocalPosition;
layout(location=2)in vec4 a_Color;
layout(location=3)in float a_Thickness;
layout(location=4)in float a_Fade;
layout(location=5)in int a_EntityID;

layout(std140)uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

out VertexOutput v_data;
flat out int v_EntityID;

void main()
{
	v_data.LocalPosition=a_LocalPosition;
	v_data.Color=a_Color;
	v_data.Thickness=a_Thickness;
	v_data.Fade=a_Fade;
	
	v_EntityID=a_EntityID;
	
	gl_Position=u_ViewProjection*vec4(a_WorldPosition,1.);
}

#type fragment
#version 300 es
precision mediump float;
layout(location=0) out vec4 o_Color;
layout(location=1) out int o_EntityID;

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

in VertexOutput v_data;
flat in int v_EntityID;

void main()
{
	// Calculate distance and fill circle with white
	float distance=1.-length(v_data.LocalPosition);
	float circle=smoothstep(0.,v_data.Fade,distance);
	circle*=smoothstep(v_data.Thickness+v_data.Fade,v_data.Thickness,distance);
	
	if(circle==0.)
	discard;
	
	// Set output color
	o_Color=v_data.Color;
	o_Color.a*=circle;
	
	o_EntityID=v_EntityID;
}
