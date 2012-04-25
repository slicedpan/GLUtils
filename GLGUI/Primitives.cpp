
#include <svl\SVL.h>
#include <GL\glew.h>
#include "../QuadDrawer.h"
#include "../Shader.h"
#include "fonttex.h"
#include "shaderDefs.h"

//void PrintText(Vec2 pos, const char* text, Vec4& colour)
//{
//    if(!text || !strlen(text)) return;
//    glColor4f(colour[0], colour[1], colour[2], colour[3]);    
//    while (*text) {
//		glRasterPos2f(pos[0], pos[1] + 13);
//		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text);
//		pos[0] += 7;
//        text++;
//    }
//}

GLuint fontTex = 0;
bool textInitialised = false;

int maxInts = 256;
int uniformArray[256];

Shader* textDraw = 0;

void SetTextShader(Shader* shader)
{
	textDraw = shader;
}

void SetupText()
{
	unsigned char buf[131072];
	unsigned char* data = GetData();
	for (int i = 0; i < 32768; ++i)
	{
		buf[i * 4] = data[i * 2];
		buf[(i * 4) + 1] = data[i * 2];
		buf[(i * 4) + 2] = data[i * 2];
		buf[(i * 4) + 3] = data[(i * 2) + 1];
	}
	glGenTextures(1, &fontTex);
	glBindTexture(GL_TEXTURE_2D, fontTex);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 2048, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Error loading texture: %d\n", err);
	else
		printf("Font loaded.\n");

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);	

	if (!textDraw)
	{
		textDraw->SetSource(textDrawVertexSource, textDrawFragmentSource);
		if (!textDraw->Compile())
			printf("%s", textDraw->GetErrorLog());
	}

	memset(uniformArray, 0, sizeof(int) * maxInts);

	textInitialised = true;
}

float offset = 1.0 / 256.0;

void PrintText(Vec2& pos, const char* text, Vec4& colour)
{}

void PrintText(Vec2& screenSize, Vec2& pos, const char* text, Vec4& colour)
{
	if (!textInitialised)
		SetupText();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fontTex);

	int numChars = strlen(text);
	int numInts = (numChars / 4) + 1;

	if (numInts > 256)
		numChars = 1024;
	
	memcpy(uniformArray, text, numChars);

	textDraw->Use();
	textDraw->Uniforms["textLength"].SetValue(numChars);
	textDraw->Uniforms["text[0]"].SetArrayValue(uniformArray, numInts);
	textDraw->Uniforms["basePosition"].SetValue(pos);
	textDraw->Uniforms["colour"].SetValue(colour);
	textDraw->Uniforms["screenSize"].SetValue(screenSize);
	textDraw->Uniforms["fontTexture"].SetValue(0);
	textDraw->Uniforms["baseTex"].SetValue(0);
	QuadDrawer::DrawQuads(numChars);
	//QuadDrawer::DrawQuad(Vec2(-1.0, -1.0), Vec2(1.0, 1.0), Vec2(1 / 800.0f, 1 / 600.0f));
	
}

void DrawTBorder(Vec2 position, int width, int height, int borderwidth, bool repeat)
{
	glVertex2f(position[0], position[1]);
	glVertex2f(position[0] + width, position[1]);
	glVertex2f(position[0] + width - borderwidth, position[1] + borderwidth);
	glVertex2f(position[0] + borderwidth, position[1] + borderwidth);
	if (repeat)
		glVertex2f(position[0], position[1]);
}

void DrawRBorder(Vec2 position, int width, int height, int borderwidth, bool repeat)
{
	glVertex2f(position[0] + width, position[1]);
	glVertex2f(position[0] + width, position[1] + height);
	glVertex2f(position[0] + width - borderwidth, position[1] + height - borderwidth);
	glVertex2f(position[0] + width - borderwidth, position[1] + borderwidth);
	if (repeat)
		glVertex2f(position[0] + width, position[1]);
}

void DrawLBorder(Vec2 position, int width, int height, int borderwidth, bool repeat)
{
	glVertex2f(position[0], position[1]);
	glVertex2f(position[0], position[1] + height);
	glVertex2f(position[0] + borderwidth, position[1] + height - borderwidth);
	glVertex2f(position[0] + borderwidth, position[1] + borderwidth);
	if (repeat)
		glVertex2f(position[0], position[1]);
}

void DrawBBorder(Vec2 position, int width, int height, int borderwidth, bool repeat)
{
	glVertex2f(position[0], position[1] + height);
	glVertex2f(position[0] + width, position[1] + height);
	glVertex2f(position[0] + width - borderwidth, position[1] + height - borderwidth);
	glVertex2f(position[0] + borderwidth, position[1] + height - borderwidth);
	if (repeat)
		glVertex2f(position[0], position[1] + height);
}

void DrawBGQuad(Vec4 colour, Vec2 position, int width, int height, int borderwidth, bool opaque)
{
	glBegin(GL_QUADS);
	if (opaque)
		glColor4f(colour[0], colour[1], colour[2], colour[3]);
	else
		glColor4f(colour[0], colour[1], colour[2], colour[3] / 2.0f);
	glVertex2f(position[0], position[1]);
	glVertex2f(position[0] + width,position[1]);
	glVertex2f(position[0] + width, position[1] + height);
	glVertex2f(position[0], position[1] + height);

	//borders

	glColor4f(colour[0], colour[1], colour[2], colour[3]);
	DrawTBorder(position, width, height, borderwidth, false);
	DrawLBorder(position, width, height, borderwidth, false);
	DrawRBorder(position, width, height, borderwidth, false);
	DrawBBorder(position, width, height, borderwidth, false);
	
	glEnd();

	glColor4f(colour[0] + 0.2f, colour[1] + 0.2f, colour[2] + 0.2f, 1.0f);

	glBegin(GL_LINE_STRIP);

	DrawTBorder(position, width, height, borderwidth, true);
	DrawLBorder(position, width, height, borderwidth, true);
	DrawRBorder(position, width, height, borderwidth, true);
	DrawBBorder(position, width, height, borderwidth, true);

	glEnd();
}

void DrawBGQuad(Vec4 colour, Vec2 position, int width, int height, int borderwidth)
{
	DrawBGQuad(colour, position, width, height, borderwidth, false);
}

void DrawTexQuad()
{
	int startChar = 49;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTex);
	glBegin(GL_QUADS);
	glTexCoord2f(40 * offset, 0.0);
	glVertex2f(300.0, 0.0);
	glTexCoord2f(80 * offset, 0.0);
	glVertex2f(500.0, 0.0);
	glTexCoord2f(80 * offset, 1.0);
	glVertex2f(500.0, 400.0);
	glTexCoord2f(40 * offset, 1.0);
	glVertex2f(300.0, 400.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}