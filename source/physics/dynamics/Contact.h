#pragma once
#include "../Common.h"
#include "../collision/Collide.h"
#include "../collision/Shape.h"

namespace Forth
{
	namespace Physics
	{

		enum ContactFlags
		{
			CF_Colliding = 0x00000001,	// Set when contact collides during a step
			CF_WasColliding = 0x00000002, // Set when two objects stop colliding
			CF_Island = 0x00000004,		  // For marking during island forming
		};

		struct ContactEdge
		{
			Body *other;
			class Contact *contact;

			void Setup(Body *b, Body *o, class Contact *c);
		};

		struct ContactStateUnit
		{
			float depth;
			float bias; // Restitution + baumgarte
			// Per-axis
			float impulses[4];
			float masses[4];
			// COM to CP
			Vector4 rA;
			Vector4 rB;

			void Update(const Manifold &m, const class Contact &c, int i);
		};

		struct ContactStateBody
		{
			/// Inertia matrix
			Tensor4 i;
			/// mass
			float m;
			/// Island index
			int index;

			void Update(const Body &body);
		};

		struct ContactState
		{

			int contacts;

			/// Normal, Tangent, Bitangent, Tritangent
			Vector4 vectors[4];
			ContactStateUnit units[Common::MULTICONTACT_COUNT];

			float restitution;
			float friction;

			ContactStateBody A = ContactStateBody();
			ContactStateBody B = ContactStateBody();

			ContactState();
			void UpdateInput(const class Contact &c);

			void Setup(const class Contact &c);
		};

		class Manifold
		{
		  public:
			Shape *A;
			Shape *B;

			/// Normal that points from A to B
			Vector4 normal;
			Vector4 position[Common::MULTICONTACT_COUNT];
			float depth[Common::MULTICONTACT_COUNT];
			int contacts;

			void Setup(Shape *A, Shape *B)
			{
				this->A = A;
				this->B = B;
			}

			void MakeContact(const Vector4 &n, const Vector4 &p, float d);

			void MakeContact(const Vector4 &p, float d);

			void MakeContact(const Vector4 &p0, const Vector4 &p1);

			void MakeContact(const Vector4 &n);
		};

		class Contact
		{
		  public:
			Shape *A, *B;
			class Body *bodyA, *bodyB;

			ContactEdge edgeA;
			ContactEdge edgeB;

			float friction;
			float restitution;
			bool sensor;
			int hash;

			Manifold manifold;
			ContactState state;

			int flags;

			Contact();

			void Setup(Shape *A, Shape *B);

			void SolveCollision();
		};

	} // namespace Physics
} // namespace Forth