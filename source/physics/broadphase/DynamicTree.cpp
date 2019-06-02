#include "DynamicTree.h"

namespace Forth
{
	namespace Physics
	{
		// Provide tight-Bounds4

		int DynamicTree::Insert(const Bounds4 &aabb, Shape *data)
		{

			int n = AllocateNode();
			Nodes[n].aabb = Fatten(aabb);
			Nodes[n].data = data;
			Nodes[n].height = 0;

			InsertLeaf(n);

			return n;
		}
		void DynamicTree::Remove(int id)
		{

			RemoveLeaf(id);

			DeallocateNode(id);
		}
		bool DynamicTree::Update(int id, Bounds4 aabb)
		{
			if (Nodes[id].aabb.Contains(aabb))
				return false;

			RemoveLeaf(id);

			Nodes[id].aabb = Fatten(aabb);

			InsertLeaf(id);

			return true;
		}
		Shape *DynamicTree::GetShape(int id)
		{
			return Nodes[id].data;
		}
		Bounds4 DynamicTree::GetFatAABB(int id)
		{
			return Nodes[id].aabb;
		}
		void DynamicTree::Query(ITreeCallback *cb, const Bounds4& aabb)
		{
			if (!stack.empty())
				stack = {};

			stack.push(Root);

			while (!stack.empty())
			{
				int id = stack.top();
				stack.pop();

				Node &n = Nodes[id];

				if (IsIntersecting(aabb, n.aabb))
				{
					if (n.height == 0)
					{
						cb->TreeCallback(id);
					}
					else
					{
						stack.push(n.left);
						stack.push(n.right);
					}
				}
			}
		}
		void DynamicTree::Query(RaycastHit4 &ray)
		{
			if (!stack.empty())
				stack = {};

			stack.push(Root);

			Vector4 p0 = ray.ray.origin;
			Vector4 p1 = ray.ray.direction;

			while (!stack.empty())
			{
				int id = stack.top();

				stack.pop();

				if (id == -1)
					continue;

				Node &n = Nodes[id];
				if (!n.aabb.Raycast(p0, p1))
					continue;

				if (n.height == 0)
				{
					GetShape(id)->Raycast(ray);
				}
				else
				{
					stack.push(n.left);
					stack.push(n.right);
				}
			}
		}
		void DynamicTree::InsertLeaf(int id)
		{
			if (Root == -1)
			{
				// id must be zero at this time..
				Root = id;
				return;
			}

			// Find the closest leaf

			Vector4 c = Nodes[id].aabb.center();
			Node &N = Nodes[Root];
			int n = Root, r, l, p;

			while (N.height > 0)
			{
				Node &R = Nodes[r = N.right];
				Node &L = Nodes[l = N.left];
				if (Compare(c, L.aabb, R.aabb) >= 0)
				{
					N = L;
					n = l;
				}
				else
				{
					N = R;
					n = r;
				}
			}

			{
				Node &P = Nodes[p = AllocateNode()];
				int tmp;
				// Make P becomes parent of N and ID
				SWAP(P.left, N.left, tmp)
				SWAP(P.right, N.right, tmp)
				SWAP(P.parent, N.parent, tmp)

				// Validate grandparent
				if (P.parent == -1)
					Root = p;
				else
					Nodes[P.parent].Replace(n, p);

				// Validate childs
				P.Set(id, n);
				Nodes[id].parent = N.parent = p;

				SyncHierarchy(p);
			}
		}
		void DynamicTree::RemoveLeaf(int id)
		{
			if (id == Root)
			{
				Root = -1;
				return;
			}

			Node &N = Nodes[id], &P = Nodes[N.parent];
			int s = (P.left == id) ? P.right : P.left, p = N.parent, tmp;

			Node &S = Nodes[s];
			// Make S become part of grandparent N, detach N & P
			SWAP(S.parent, P.parent, tmp)

			// Validate grandparent
			if (S.parent == -1)
				Root = s;
			else
				Nodes[S.parent].Replace(p, s);

			DeallocateNode(p);

			SyncHierarchy(S.parent);
		}

		// Correct Bounds4 hierarchy heights and AABBs starting at supplied

		void DynamicTree::SyncHierarchy(int index)
		{
			while (index != -1)
			{
				Node &n = Nodes[index];

				n.height = Max(Nodes[n.left].height, Nodes[n.right].height) + 1;
				n.aabb = Combine(Nodes[n.left].aabb, Nodes[n.right].aabb);

				index = n.parent;
			}
		}
		int DynamicTree::AllocateNode()
		{
			Node &F = Nodes[FreeNode];
			int f = FreeNode;

			if (F.next == NodesCap)
				Resize(NodesCap << 1);

			FreeNode = F.next;
			return f;
		}
		void DynamicTree::DeallocateNode(int index)
		{
			// Use it as the next free node
			Node &N = Nodes[index];
			N.Set(-1, -1, -1);
			N.data = NULL;
			N.next = FreeNode;
			FreeNode = index;
		}
		void DynamicTree::Resize(int length)
		{
			// Resize the array
			int I = NodesCap, J = length;
			EnsureCapacity(&Nodes, NodesCap, &NodesCap, J);
			for (int i = I; i < J; i++)
			{
				Nodes[i] = Node(i + 1);
			}
		}
		Bounds4 DynamicTree::Fatten(Bounds4 aabb)
		{
			Vector4 v = Vector4(0.5f);

			return Bounds4(aabb.min - v, aabb.max + v);
		}

		void DynamicTree::Node::Replace(int find, int replace)
		{
			if (left == find)
				left = replace;
			else
				right = replace;
		}
		void DynamicTree::Node::Set(int l, int r, int p)
		{
			left = l;
			right = r;
			parent = p;
		}
		void DynamicTree::Node::Set(int l, int r)
		{
			left = l;
			right = r;
		}
		DynamicTree::Node::Node() {}
		DynamicTree::Node::Node(int next) : Node() { this->next = next; }
	} // namespace Physics
} // namespace Forth