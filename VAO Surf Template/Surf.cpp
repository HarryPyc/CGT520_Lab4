#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> //for pi
#include "Surf.h"

static int N = 50;

//The surface to draw.
glm::vec3 surf(int i, int j)
{
   const float center = 0.5f*N;
   const float xy_scale = 20.0f / N;
   const float z_scale = 10.0f;

   float x = xy_scale * (i - center);
   float y = xy_scale * (j - center);

   float r = sqrt(x*x + y * y);
   float z = 1.0f;

   if (r != 0.0f)
   {
      z = sin(r) / r;
   }
   z = z * z_scale;

   return 0.05f*glm::vec3(x, y, z);
}
std::vector<int> indices() {
	std::vector<int> indices(N * N * 6);
	int n = 0;
	for (int i = 0; i < N - 1; i++)
		for (int j = 0; j < N - 1; j++) {
			int index = i * N + j;
			indices[n] = index;
			indices[++n] = index + N;
			indices[++n] = index + 1;
			indices[++n] = index + 1;
			indices[++n] = index + N;
			indices[++n] = index + N + 1;
			n++;

		}
	return indices;
}
//The demo shape being currently draw.
glm::vec3 circle(int i)
{
   const float r = 1.0f;
   float theta = i * 2.0f*glm::pi<float>() / N;
   return glm::vec3(r*cos(theta), r*sin(theta), 0.0f);
}

GLuint create_surf_vbo()
{
   //Declare a vector to hold N vertices
   std::vector<glm::vec3> surf_verts(N * N);

   for(int i = 0;i < N; i++)
	   for (int j = 0; j < N; j++) {
		   surf_verts[i*N + j] = surf(i, j);
	   }

   GLuint vbo;
   glGenBuffers(1, &vbo); //Generate vbo to hold vertex attributes for triangle.

   glBindBuffer(GL_ARRAY_BUFFER, vbo); //Specify the buffer where vertex attribute data is stored.
   
   //Upload from main memory to gpu memory.
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*surf_verts.size(), surf_verts.data(), GL_STATIC_DRAW);

   return vbo;
}
GLuint create_surf_ebo() {
	std::vector<int> Indices = indices();

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(int) * Indices.size(), Indices.data(), GL_STATIC_DRAW);
	
	return ebo;
}
GLuint create_surf_vao()
{
   GLuint vao;

   //Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
   glGenVertexArrays(1, &vao);

   //Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
   glBindVertexArray(vao);

   GLuint vbo = create_surf_vbo();
   GLuint ebo = create_surf_ebo();

   const GLint pos_loc = 0; //See also InitShader.cpp line 164.

   glEnableVertexAttribArray(pos_loc); //Enable the position attribute.

   //Tell opengl how to get the attribute values out of the vbo (stride and offset).
   glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
   glBindVertexArray(0); //unbind the vao

   return vao;
}

void draw_surf(GLuint vao, GLenum mode)
{
    //glDrawArrays(mode, 0, N * N);
	glDrawElements(mode, N * N * 6 , GL_UNSIGNED_INT, 0);
}
