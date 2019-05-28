#pragma once

#include "Projector4.h"

namespace Forth
{

	void Projector4::Project(const Buffer4 &from, const Transform4 &transform, Buffer3 &to) {
		Visualizer4* viz = ((Visualizer4*)defaultVisualizers[from.simplex - 1]);
		viz->Initialize(to);
		this->Project(from, transform, viz);
		viz->End();
	}

	Projector4::Projector4()
	{
		defaultVisualizers[SM_Point] = new ParticleVisualizer();
		defaultVisualizers[SM_Line] = new WireVisualizer();
		defaultVisualizers[SM_Triangle] = new SolidVisualizer();
	}

	Projector4::~Projector4()
	{
		delete defaultVisualizers[SM_Point];
		delete defaultVisualizers[SM_Line];
		delete defaultVisualizers[SM_Triangle];
	}
}