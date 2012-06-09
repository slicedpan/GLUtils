#include "shaderDefs.h"

char* textDrawVertexSource = "#version 330 \n\
\n\
layout(location=0) in vec2 coord; \n\
layout(location=1) in vec2 texCoord; \n\
\n\
const int masks[4] = int[4] (0x000000ff,  0x0000ff00, 0x00ff0000, 0xff000000); \n\
\n\
uniform vec2 basePosition; \n\
uniform vec4 colour = vec4(1.0, 0.0, 0.0, 1.0); \n\
uniform vec2 screenSize; \n\
uniform int text[256]; \n\
\n\
smooth out vec4 inColour; \n\
smooth out vec2 fragTexCoord; \n\
\n\
void main() \n\
{ \n\
	inColour = colour; \n\
	vec2 outPos = basePosition + vec2(gl_InstanceID * 16 + (16 * texCoord.x), texCoord.y * -32); \n\
	\n\
	outPos /= screenSize; \n\
	\n\
	int arrayPos = gl_InstanceID / 4; \n\
	\n\
	int character = (text[arrayPos] & masks[gl_InstanceID %4]) >> (8 * (gl_InstanceID % 4)); \n\
	\n\
	//character = 70 + gl_InstanceID; \n\
	\n\
	fragTexCoord = vec2((texCoord.x + character) / 256.0, texCoord.y); \n\
	//fragTexCoord = vec2(texCoord.x / 64, texCoord.y); \n\
	\n\
	gl_Position = vec4(outPos.x, outPos.y, 0.0, 1.0); \n\
} \n\
";

char* textDrawFragmentSource = "#version 330 \n\
\n\
uniform sampler2D fontTexture; \n\
\n\
out vec4 colour; \n\
smooth in vec4 inColour; \n\
\n\
smooth in vec2 fragTexCoord; \n\
\n\
void main() \n\
{ \n\
	colour = inColour * texture(fontTexture, fragTexCoord).a; \n\
}\n\
";
