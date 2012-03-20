#include "objLoader.h"
#include "obj_parser.h"
#include <GL/glew.h>

int objLoader::load(char *filename)
{
	int no_error = 1;
	no_error = parse_obj_scene(&data, filename);
	if(no_error)
	{
		this->vertexCount = data.vertex_count;
		this->normalCount = data.vertex_normal_count;
		this->textureCount = data.vertex_texture_count;

		this->faceCount = data.face_count;
		this->sphereCount = data.sphere_count;
		this->planeCount = data.plane_count;

		this->lightPointCount = data.light_point_count;
		this->lightDiscCount = data.light_disc_count;
		this->lightQuadCount = data.light_quad_count;

		this->materialCount = data.material_count;

		this->vertexList = data.vertex_list;
		this->normalList = data.vertex_normal_list;
		this->textureList = data.vertex_texture_list;

		this->faceList = data.face_list;
		this->sphereList = data.sphere_list;
		this->planeList = data.plane_list;

		this->lightPointList = data.light_point_list;
		this->lightDiscList = data.light_disc_list;
		this->lightQuadList = data.light_quad_list;

		this->materialList = data.material_list;

		this->camera = data.camera;
	}

	return no_error;
}

void objLoader::Draw()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < data.face_count; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			glVertex3fv(data.vertex_list[data.face_list[i]->vertex_index[j]]->e);
			if (data.face_list[i]->normal_index > 0)
				glTexCoord3fv(data.vertex_normal_list[data.face_list[i]->normal_index[j]]->e);
		}
	}
	glEnd();
	glBegin(GL_LINES);
	for (int i = 0; i < data.face_count; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			float* pos = data.vertex_list[data.face_list[i]->vertex_index[j]]->e;
			float* n = data.vertex_normal_list[data.face_list[i]->normal_index[j]]->e;
			float vec[3];
			vec[0] = pos[0] + n[0];
			vec[1] = pos[1] + n[1];
			vec[2] = pos[2] + n[2];
			glVertex3fv(pos);
			glVertex3fv(vec);
		}
	}
	glEnd();
}