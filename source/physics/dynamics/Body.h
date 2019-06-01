#pragma once
#include "../Common.h"
#include "../collision/Shape.h"
#include "Contact.h"

namespace Forth
{
	namespace Physics
	{

		enum BodyFlags
		{
			/// The body is static (passive)
			BFL_Static = 0x001,
			/// The body is dynamic (active)
			BFL_Dynamic = 0x002,
			/// The body is kinematic (passive but can interact)
			BFL_Kinematic = 0x004,
			/// The body won't translate in the simulation
			BFL_ZeroMass = 0x008,
			/// The body won't rotate in the simulation
			BFL_ZeroInertia = 0x010,
			/// The body is calculated in any physical interaction
			BFL_Active = 0x020,
			/// The body is allowed to sleep
			BFL_AllowSleep = 0x040,
			/// The body is actively listening and receiving contacts
			BFL_Awake = 0x080,
			/// The body has been solved in island
			BFL_Island = 0x100,
			/// The body has only one shape with identity local transformation
			BFL_Identity = 0x200,
			/// They body mass and inertia need to be recalculated
			BFL_DirtyMass = 0x400,
		};

		class Body
		{
		  public:
			Tensor4 invInertiaModel;

			Tensor4 invInertiaWorld;

			float mass;

			float invMass;

			Vector4 linearVelocity;

			Euler4 angularVelocity;

			Vector4 force;

			Euler4 torque;

			/// Compound transform
			Transform4 Tx = Transform4::identity();

			/// Local body center
			Vector4 C = Vector4();

			/// World body center
			Vector4 P = Vector4();

			float sleepTime;

			int islandIndex;

			// Use BFL_*
			int flags = BFL_Active | BFL_AllowSleep | BFL_Static | BFL_Awake | BFL_DirtyMass;

			::std::vector<Shape*> shapes = ::std::vector<Shape*>();

			::std::vector<ContactEdge *> contactList = ::std::vector<ContactEdge *>();

			class Scene *scene;

			int layers = 0x1;

			float gravityScale = 1;

			float linearDamping = 0.1f;

			float angularDamping = 0.1f;

			void CalculateMassData();

			void SynchronizeProxies();

			void SetScene(Scene* scene);

			// -- Public Accessor

			void AddShape(Shape* shape);

			void RemoveShape(Shape* shape);

			void RemoveAllShapes();

			void ApplyForce(Vector4 force);

			void ApplyForceAtWorldPoint(Vector4 force, Vector4 point);

			void ApplyLinearImpulse(Vector4 impulse);

			void ApplyLinearImpulseAtWorldPoint(Vector4 impulse, Vector4 point);

			void ApplyTorque(Euler4 torque);

			void SetToAwake();

			void SetToSleep();

			bool IsAwake();

			bool Active(void);

			bool Active(bool value);

			// Vector4 LinearVelocity{ get { return linearVelocity; } set { linearVelocity = value; if (LengthSq(value) > 1e-4f) SetToAwake(); } }

			// Euler4 AngularVelocity{ get { return angularVelocity; } set { angularVelocity = value; if (Euler4.LengthSq(value) > 1e-4f) SetToAwake(); } }

			// float LinearDamping{ get { return linearDamping; } set { linearDamping = Max(0, value); } }

			// float AngularDamping{ get { return angularDamping; } set { angularDamping = Max(0, value); } }

			// float GravityScale{ get { return gravityScale; } set { gravityScale = value; } }

			void *Tag;

			static bool CanCollide(Body *a, Body *b);

			int GetFlags();

			void SetFlags(BodyFlags type, bool awake, bool allowSleep, bool active);

			Transform4 GetTransform();

			void GetTransform(Transform4 t);

			void SetTransform(Vector4 position, Matrix4 rotation);

			bool operator==(const Body& o)
			{
				return this == &o || this->Tag == o.Tag;
			}
			Body() {}
		};
	} // namespace Physics
} // namespace Forth
