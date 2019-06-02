#include "BroadPhase.h"
#include "../dynamics/ContactManager.h"

namespace Forth
{
	namespace Physics
	{
		BroadPhase::BroadPhase(ContactManager *manager) : Tree(), Manager(manager), PairBuffer(), MoveBuffer() {}

		void BroadPhase::InsertShape(Shape *shape, const Bounds4 &aabb)
		{
			int id = Tree.Insert(aabb, shape);
			shape->broadPhaseIndex = id;
			BufferMove(id);
		}

		void BroadPhase::RemoveShape(Shape *shape)
		{
			Tree.Remove(shape->broadPhaseIndex);
		}

		// Generates the contact list.

		void BroadPhase::UpdatePairs()
		{
			PairBuffer.clear();

			// Query the tree with all moving boxs
			for (size_t i = 0; i < MoveBuffer.size(); ++i)
			{
				CurrentIndex = MoveBuffer[i];

				Tree.Query(this, Tree.GetFatAABB(CurrentIndex));
			}

			// Reset the move buffer
			MoveBuffer.clear();

			for (size_t i = 0; i < PairBuffer.size(); ++i)
			{
				// Add contact to manager
				ContactPair &pair = PairBuffer[i];
				Manager->AddContact(Tree.GetShape(pair.A), Tree.GetShape(pair.B));
			}
		}

		void BroadPhase::Update(int id, Bounds4 aabb)
		{
			if (Tree.Update(id, aabb))
				BufferMove(id);
		}

		bool BroadPhase::TestOverlap(int A, int B)
		{
			return IsIntersecting(Tree.GetFatAABB(A), Tree.GetFatAABB(B));
		}

		void BroadPhase::BufferMove(int id)
		{
			MoveBuffer.push_back(id);
		}

		void BroadPhase::TreeCallback(int index)
		{
			// Cannot collide with self
			if (index == CurrentIndex)
				return;

			int iA = Min(index, CurrentIndex);
			int iB = Max(index, CurrentIndex);

			PairBuffer.push_back({iA, iB});
		}

	} // namespace Physics
} // namespace Forth
