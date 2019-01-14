//#version 330 core
//
//uniform sampler2D texture1;
//
//in vec2 TexCoord;
//
//void main() {
//	gl_FragColor = vec4(1);//texture(texture1, TexCoord);
//}

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) ;
}