#pragma once

#include <iostream>
#include <stdint.h>
#include <algorithm>
#include "common/Buffer3.h"
#include "common/Buffer4.h"
#include "math/Transform4.h"
#include "rendering/CrossSection.h"
#include "rendering/Frustum.h"
#include "visualizer/SolidVisualizer.h"
#include "visualizer/WireVisualizer.h"
#include "visualizer/ParticleVisualizer.h"
#include "visualizer/CustomVisualizer.h"

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

namespace Forth {

	CrossSection* crossSection;
	Frustum* frustum;
	std::vector<Buffer3*>* buffers;
	SolidVisualizer* solidViz;
	WireVisualizer* wireViz;
	ParticleVisualizer* particleViz;

	DLLEXPORT void DoInitialize(Transform4 view)
	{
		if (crossSection == NULL)
		{
			crossSection = new CrossSection();
			frustum = new Frustum();
			solidViz = new SolidVisualizer();
			wireViz = new WireVisualizer();
			particleViz = new ParticleVisualizer();
			buffers = new std::vector<Buffer3*>();
		}

		crossSection->Setup(view);
		frustum->Setup(view);
	}

	DLLEXPORT void DoCleanup()
	{
		if (crossSection != NULL)
		{
			for (size_t i = 0; i < buffers->size(); i++)
			{
				delete (*buffers)[i];
			}
			delete crossSection;
			delete frustum;
			delete solidViz;
			delete wireViz;
			delete particleViz;
			delete buffers;
		}
	}

	void HotInit(const VisualizeMode mode, Buffer3 **out, Visualizer4 **viz)
	{
		if (*out == NULL)
			buffers->push_back(*out = new Buffer3());

		switch (mode)
		{
		case VM_Particle:
			*viz = particleViz;
			break;
		case VM_Wire:
			*viz = wireViz;
			break;
		case VM_Solid:
		case VM_Custom:
		default:
			*viz = solidViz;
			break;
		}
	}

	DLLEXPORT Buffer3* DoCrossSection(Buffer4 *in, Buffer3 *out, Transform4 matrix, VisualizeMode mode)
	{
		if (crossSection == NULL)
			return 0;

		Visualizer4 *viz;

		HotInit(mode, &out, &viz);

		viz->Initialize(*out);

		crossSection->Project(*in, matrix, viz);

		viz->End();

		return out;
	}

	DLLEXPORT Buffer3* DoFrustum(Buffer4 *in, Buffer3 *out, Transform4 matrix, VisualizeMode mode)
	{
		if (frustum == NULL)
			return 0;

		Visualizer4 *viz;

		HotInit(mode, &out, &viz);

		viz->Initialize(*out);

		frustum->Project(*in, matrix, viz);

		viz->End();

		return out;
	}

	DLLEXPORT void UpdateFrustumParams(Frustum::frustumparams params)
	{
		if (frustum != NULL)
			frustum->Set(params);
	}

	DLLEXPORT void DisposeBuffer(Buffer3 *buff)
	{
		if (buff == NULL)
			return;

		buffers->erase(std::remove(buffers->begin(), buffers->end(), buff));

		delete buff;
	}

	DLLEXPORT SimplexMode ExtractBuffer(Buffer3 *buff,
		_Out_ Vector3 **vertices, _Out_  size_t *vertCount,
		_Out_ int **indices, _Out_ size_t *indiceCount,
		_Out_ VertexProfile **profiles, _Out_ size_t *profilesCount)
	{
		if (buff == NULL)
			// sentinel value, means something must wrong.
			return SimplexMode::SM_Tetrahedron;

		*vertices = &buff->vertices[0];
		*indices = &buff->indices[0];
		*vertCount = buff->vertices.size();
		*indiceCount = buff->indices.size();
		if (&buff->profiles)
		{
			*profiles = &buff->profiles[0];
			*profilesCount = buff->profiles.size();
		}
		return buff->simplex;
	}
}
