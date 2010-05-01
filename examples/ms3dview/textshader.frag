#version 120

uniform sampler2D texture;

varying vec2 ex_Texcoord;

//out vec4 out_Color;

void main( void ) {
	//out_Color = texture2D( texture, ex_Texcoord );
	gl_FragColor = texture2D( texture, ex_Texcoord );
}
