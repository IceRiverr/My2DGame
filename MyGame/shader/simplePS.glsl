#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D spriteTexture2D;

void main()
{
    vec4 color = texture(spriteTexture2D, TexCoord);
	if(color.a < 0.01f)
	{
		discard;
	}
	FragColor = vec4(color.rgb, 1.0f);
}