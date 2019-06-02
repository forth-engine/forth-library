#pragma once

#include "../Common.h"
#include "DynamicTree.h"

namespace Forth
{
	namespace Physics
	{
		struct ContactPair
		{
			int A;
			int B;
		};

		class BroadPhase : public ITreeCallback
		{
		  public:
			BroadPhase() {}

			BroadPhase(class ContactManager *manager);

			void InsertShape(Shape *shape, const Bounds4 &aabb);

			void RemoveShape(Shape *shape);

			// Generates the contact list.
			void UpdatePairs();

			void Update(int id, Bounds4 aabb);

			bool TestOverlap(int A, int B);

			class ContactManager *Manager;

			::std::vector<ContactPair> PairBuffer;

			::std::vector<int> MoveBuffer;

			DynamicTree Tree;

			int CurrentIndex;

			void BufferMove(int id);

			void TreeCallback(int index);
		};
	} // namespace Physics
} // namespace Forth
