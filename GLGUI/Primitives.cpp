
#include <svl\SVL.h>
#include "glFunctions.h"
#include "fonttex.h"

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
bool fontInitialised = false;

void SetupFont()
{
	unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char) * 131072);
	for (int i = 0; i < 32768; ++i)
	{
		buf[i * 4] = data[i * 2];
		buf[(i * 4) + 1] = data[i * 2];
		buf[(i * 4) + 2] = data[i * 2];
		buf[(i * 4) + 3] = data[(i * 2) + 1];
	}
	glGenTextures(1, &fontTex);
	glBindTexture(GL_TEXTURE_2D, fontTex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);	
	free(buf);
	fontInitialised = true;
}

float offset = 1.0 / 256.0;

void PrintText(Vec2 pos, const char* text, Vec4& colour)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTex);
	if (!fontInitialised)
		SetupFont();
	while (*text)
	{		
		glBegin(GL_QUADS);
		glTexCoord2f((*text) * offset, 0.0);
		glVertex2f(pos[0], pos[1]);
		glTexCoord2f((*text + 1) * offset, 0.0);
		glVertex2f(pos[0] + 8, pos[1]);
		glTexCoord2f((*text + 1) * offset, 1.0);
		glVertex2f(pos[0] + 8, pos[1] + 16);
		glTexCoord2f((*text) * offset, 1.0);
		glVertex2f(pos[0], pos[1] + 16);
		glEnd();
		pos += Vec2(7, 0);
		++text;
	}
	glDisable(GL_TEXTURE_2D);
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