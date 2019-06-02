#pragma once

#include "../Common.h"
#include "../collision/Shape.h"
#include "../dynamics/RaycastHit4.h"
#include <stack>

namespace Forth
{
	namespace Physics
	{
		struct ITreeCallback
		{
			virtual void TreeCallback(int id)=0;
		};

		/// Dynamic Bounds4 Tree
		/// This is simplest implementation of Bounds4 tree.
		class DynamicTree
		{
		  public:
			struct Node
			{
				/// -1 is root
				int parent = -1;
				/// -1 is empty
				int left = -1;
				/// -1 is empty
				int right = -1;

				/// Count of branches this node containing.
				/// 0 is the leaf (tip of the tree)
				/// -1 is deallocated (not part of the tree)
				int height = -1;

				/// Used only when deallocated.
				/// Return the next free node
				int next;

				/// Bounds4 of this node
				Bounds4 aabb;

				/// Shape attached to this node (leaf only)
				Shape *data;

				void Replace(int find, int replace);

				void Set(int l, int r, int p);
				void Set(int l, int r);

				Node();
				Node(int next);
			};

			int Root = -1;
			int FreeNode = 0;
			int NodesCap = 0;
			Node *Nodes = new Node[0]{};

			DynamicTree() { Resize(64); }

			// Provide tight-Bounds4
			int Insert(const Bounds4 &aabb, Shape *data);

			void Remove(int id);

			bool Update(int id, Bounds4 aabb);

			Shape *GetShape(int id);

			Bounds4 GetFatAABB(int id);

			::std::stack<int> stack = ::std::stack<int>();

			void Query(ITreeCallback *cb, const Bounds4& aabb);

			void Query(RaycastHit4 &ray);

			void InsertLeaf(int id);

			void RemoveLeaf(int id);

			// Correct Bounds4 hierarchy heights and AABBs starting at supplied
			void SyncHierarchy(int index);

			int AllocateNode();

			void DeallocateNode(int index);

			void Resize(int length);

			static Bounds4 Fatten(Bounds4 aabb);
		};
	} // namespace Physics
} // namespace Forth
