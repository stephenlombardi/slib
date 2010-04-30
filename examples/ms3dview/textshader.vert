#version 140

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_Position;
in vec2 in_Texcoord;

out vec2 ex_Texcoord;

void main( void ) {
	gl_Position = projection * view * model * vec4(in_Position, 1.0);
	ex_Texcoord = in_Texcoord;
}
