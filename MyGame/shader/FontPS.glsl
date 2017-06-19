
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D	CharacterTex;
uniform vec3		FontColor;

void main()
{
	float sample = texture(CharacterTex, TexCoord).r;
	FragColor = vec4(FontColor, sample);
}
