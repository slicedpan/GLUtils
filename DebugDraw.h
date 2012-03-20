#include <svl\SVL.h>
#include <svl\SVLgl.h>
#include <GL\glew.h>

inline void DrawArrow(Vec3& point, Vec3& direction, Vec3& colour)
{	
	Vec3 v1 = Vec3(0.0, 0.0, 1.0);
	if (fabs(dot(direction, v1)) > 0.999f)
		v1 = Vec3(1.0, 0.0, 0.0);
	v1 += direction;
	v1 = cross(v1, direction);
	Vec3 end = point + direction;
	glColor(colour);		
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glVertex(point);
	glEnd();
	glBegin(GL_LINES);
	glVertex(point);
	glVertex(end);
	glVertex(end);
	glVertex(point + (direction * 0.6) + (v1 * 0.3));
	glVertex(end);
	glVertex(point + (direction * 0.6) - (v1 * 0.3));		
	glEnd();
}