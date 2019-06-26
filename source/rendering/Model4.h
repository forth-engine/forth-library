#pragma once

#include "../common/Buffer3.h"
#include "../common/Buffer4.h"
#include "../common/BufferGL.h"
#include "../math/Transform4.h"
#include "../physics/dynamics/Body.h"
#include "Projector4.h"
#include <fstream>
#include <sstream>
#include <stdio.h>

/** OpenGL Drawing Macro
 *
 * @model Model4
 * @vb GLuint to a vertex buffer
*/
#define _FORTH_GL_DRAW(dr, vb)                                                           \
	do                                                                                   \
	{                                                                                    \
		glBindBuffer(GL_ARRAY_BUFFER, vb);                                               \
		glBufferData(GL_ARRAY_BUFFER,                                                    \
					 dr.vb_count * sizeof(float),                                        \
					 &dr.vb[0], GL_STREAM_DRAW);                                         \
		int __f_offsets__ = 0;                                                           \
		for (int __f_iter__ = 0; __f_iter__ < dr.attr.slots; ++__f_iter__)               \
		{                                                                                \
			glEnableVertexAttribArray(__f_iter__);                                       \
			glVertexAttribPointer(__f_iter__,                                            \
								  dr.attr.counts[__f_iter__], GL_FLOAT, GL_FALSE,        \
								  dr.attr.stripe * sizeof(float),                        \
								  (void *)(__f_offsets__ * sizeof(float)));              \
			__f_offsets__ += dr.attr.counts[__f_iter__];                                 \
		}                                                                                \
		glDrawArrays(dr.attr.simplex == 3 ? GL_TRIANGLES                                 \
										  : dr.attr.simplex == 2 ? GL_LINES : GL_POINTS, \
					 0, dr.attr.vertexs);                                                \
	} while (0)

#define FORTH_GL_DRAW(model, vb) _FORTH_GL_DRAW((model).driver, vb)

namespace Forth
{
	class Model4
	{
	protected:
		Transform4 matrix;
		bool matrix_dirty = true;
		unsigned long cached_view_version = ULONG_MAX;
		SphereBounds4 bounds;
		bool bounds_valid = false;

	  public:
		Buffer4 input = Buffer4();
		Buffer3 output = Buffer3();
		BufferGL driver = BufferGL();
		Physics::Body *rigidbody = NULL;

		Model4(void) : matrix(Vector4(), Matrix4(1)) {}

		const Transform4& GetModelMatrix() { return matrix; }
		void SetModelMatrix(const Transform4& value) {
			matrix = value; matrix_dirty = true;
			if (rigidbody != NULL)
				rigidbody->SetTransform(value);
		}

		void Render(Projector4 &projector)
		{
			if (matrix_dirty || cached_view_version != projector.view_version)
			{
				projector.Project(input, matrix, output);
				this->driver.Copy(output);
				matrix_dirty = false;
				cached_view_version = projector.view_version;
			}
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
				if (apply_matrix)
				{
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