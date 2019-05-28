#pragma once

#include "../common/Buffer3.h"
#include "../common/Buffer4.h"
#include "../common/BufferGL.h"
#include "../math/Transform4.h"
#include "Projector4.h"

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

		Model4() : matrix(Transform4::identity()) {}

		void Render(Projector4 &projector)
		{
			projector.Project(input, matrix, output);
			this->driver.Copy(output);
		}
	};
} // namespace Forth