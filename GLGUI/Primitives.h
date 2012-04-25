#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <svl\SVL.h>
#include <GL\glew.h>

class Shader;

void DrawBGQuad(Vec4 colour, Vec2 position, int width, int height, int borderwidth);
void DrawBGQuad(Vec4 colour, Vec2 position, int width, int height, int borderwidth, bool opaque);
void PrintText(Vec2& screenSize, Vec2& pos, const char* text, Vec4& colour);
void PrintText(Vec2& pos, const char* text, Vec4& colour);
void DrawTexQuad();
void SetTextShader(Shader* shader);

#endif