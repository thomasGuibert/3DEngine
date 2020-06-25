#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 12) out;

void main()
{
    ////bottom
    //gl_Position = gl_in[0].gl_Position + vec4(-0.05, -0.15, 0.0, 0.0); 
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( 0.05, -0.15, 0.0, 0.0);
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( -0.05, -0.05, 0.0, 0.0);
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( 0.05, -0.05, 0.0, 0.0);
    //EmitVertex();
	//
    ////Left/Right
    //gl_Position = gl_in[0].gl_Position + vec4( -0.12, -0.05, 0.0, 0.0); 
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4(  0.12, -0.05, 0.0, 0.0);
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( -0.12, 0.05, 0.0, 0.0);
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( 0.12, 0.05, 0.0, 0.0);
    //EmitVertex();
	//
    ////Top
    //gl_Position = gl_in[0].gl_Position + vec4( -0.05, 0.05, 0.0, 0.0); 
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( 0.05, 0.05, 0.0, 0.0);
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( -0.05, 0.15, 0.0, 0.0);
    //EmitVertex();
	//
    //gl_Position = gl_in[0].gl_Position + vec4( 0.05, 0.15, 0.0, 0.0);
    //EmitVertex();
	//
    //EndPrimitive();

	gl_Position = gl_in[0].gl_Position + vec4( 0.04, 0.0, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4( -0.04, 0.0, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();

	gl_Position = gl_in[0].gl_Position + vec4( 0.0, 0.05, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4( 0.0, -0.05, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}

