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
			friend struct Collide;
			friend class Scene;
			friend class Shape;
			friend class Manifold;
			friend class Island;
			friend class ContactManager;
			friend class ContactSolver;
			friend struct ContactEdge;
			friend struct ContactStateBody;
			friend struct ContactStateUnit;

			Tensor4 invInertiaModel = Tensor4::zero();
			Tensor4 invInertiaWorld = Tensor4::zero();

			float mass = 0;
			float invMass = 0;

			Vector4 linearVelocity = Vector4::zero;
			Euler4 angularVelocity = Euler4::zero();
			Vector4 force = Vector4::zero;
			Euler4 torque = Euler4::zero();
			/// Compound transform
			Transform4 Tx = Transform4::identity;
			/// Local body center
			Vector4 C = Vector4();
			/// World body center
			Vector4 P = Vector4();
			float sleepTime = 0;
			int islandIndex = -1;
			// Use BFL_*
			int flags = BFL_Active | BFL_AllowSleep | BFL_Static | BFL_Awake | BFL_DirtyMass;

			::std::vector<Shape *> shapes = ::std::vector<Shape *>();
			::std::vector<ContactEdge *> contactList = ::std::vector<ContactEdge *>();
			class Scene *scene = NULL;
			int layers = 0x1;
			float gravityScale = 1;
			float linearDamping = 0.1f;
			float angularDamping = 0.1f;

			void CalculateMassData();
			void SynchronizeProxies();

		  public:
			void *Tag = NULL;
			void SetScene(Scene *scene);
			void AddShape(Shape *shape);
			void RemoveShape(Shape *shape);
			void RemoveAllShapes();
			void ApplyForce(Vector4 force);
			void ApplyForceAtWorldPoint(Vector4 force, Vector4 point);
			void ApplyLinearImpulse(Vector4 impulse);
			void ApplyLinearImpulseAtWorldPoint(Vector4 impulse, Vector4 point);
			void ApplyTorque(Euler4 torque);
			void SetToAwake();
			void SetToSleep();

			static bool CanCollide(Body *a, Body *b);

			Body() {}

			inline Transform4 GetTransform() { return Transform4(P, Tx.rotation); }
			inline bool IsAwake() { return (flags & BFL_Awake) > 0; }
			inline bool GetActive() { return (flags & BFL_Active) > 0; }
			inline Vector4 GetLinearVelocity() { return linearVelocity; }
			inline Euler4 GetAngularVelocity() { return angularVelocity; }
			inline float GetLinearDamping() { return linearDamping; }
			inline float GetAngularDamping() { return angularDamping; }
			inline float GetGravityScale() { return gravityScale; }
			inline int GetFlags() { return flags; }

			void SetTransform(const Transform4 &matrix);
			void SetTransform(const Vector4 &position, const Matrix4 &rotation);
			void SetActive(bool active);
			void SetLinearVelocity(const Vector4 &value);
			void SetAngularVelocity(const Euler4 &value);
			void SetLinearDamping(float value);
			void SetAngularDamping(float value);
			void SetGravityScale(float value);
			void SetFlags(bool dynamics, bool kinematics, bool awake, bool allowSleep, bool active);
		};
	} // namespace Physics
} // namespace Forth
