#pragma once

#include "../common/Buffer3.h"
#include "../common/Buffer4.h"
#include "../common/BufferGL.h"
#include "../math/Transform4.h"
#include "Projector4.h"
#include <fstream>
#include <sstream>
#include <stdio.h>

/** OpenGL Drawing Macro
 *
 * @model Model4
 * @vb GLuint to a vertex buffer
*/
#define FORTH_GL_DRAW(model, vb)                                                                                    \
	do                                                                                                              \
	{                                                                                                               \
		glBindBuffer(GL_ARRAY_BUFFER, vb);                                                                          \
		glBufferData(GL_ARRAY_BUFFER, model.driver.vb.size() * sizeof(float), &model.driver.vb[0], GL_STREAM_DRAW); \
		glEnableVertexAttribArray(0);                                                                               \
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(0));                            \
		glEnableVertexAttribArray(1);                                                                               \
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));            \
		glDrawArrays(GL_TRIANGLES, 0, model.driver.vb.size() / 6);                                                  \
	} while (0)

namespace Forth
{
	class Model4
	{
	  public:
		Buffer4 input = Buffer4();
		Buffer3 output = Buffer3();
		BufferGL driver = BufferGL();
		Transform4 matrix;

		Model4(void) : matrix(Transform4::identity()) {}

		void Render(Projector4 &projector)
		{
			projector.Project(input, matrix, output);
			this->driver.Copy(output);
		}

		bool ReadStreamOBJ(std::istream &stream)
		{
			input.Clear();

			while (1)
			{

				char lineHeader[128];
				// read the first word of the line
				stream >> lineHeader;
				if (stream.eof())
					break; // EOF = End Of File. Quit the loop.

				// else : parse lineHeader

				if (strcmp(lineHeader, "v") == 0)
				{
					Vector4 vertex;

					stream >> vertex.x >> vertex.y >> vertex.z >> vertex.w;

					input.AddVertex(vertex);
				}
				else if (strcmp(lineHeader, "f") == 0)
				{
					int indices[4];

					stream >> indices[0] >> indices[1] >> indices[2] >> indices[3];

					input.AddSimplex(indices[0], indices[1], indices[2], indices[3]);
				}
				else
				{
					// Probably a comment, eat up the rest of the line
					stream.get();
				}
			}

			return true;
		}

		bool WriteStreamOBJ(std::ostream &stream, bool apply_matrix = false)
		{
			stream << "# Forth Engine Modified OBJ Format" << std::endl;
			stream << "# Version: 0.1" << std::endl;

			for (int i = 0; i < input.verticeCount; ++i)
			{
				Vector4 &v = input.vertices[i];
				if (apply_matrix) {
					v = matrix * v;
				}
				stream << "v " << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
			}

			for (int i = 0; i < input.indiceCount;)
			{
				stream << "f ";
				for (int j = 0; j <= input.simplex; ++j)
				{
					stream << input.indices[i++] << " ";
				}
				stream << std::endl;
			}

			return true;
		}
	};
} // namespace Forth