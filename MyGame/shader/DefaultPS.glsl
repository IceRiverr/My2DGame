#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Sprite2D;

void main()
{
    vec4 color = texture(Sprite2D, TexCoord);
	if(color.a < 0.01f)
	{
		discard;
	}
	FragColor = color;
}