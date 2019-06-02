#pragma once

#include "../extras/Utils.h"
#include "../math/Bounds4.h"
#include "../math/Matrix4.h"
#include "../math/Plane4.h"
#include "../math/Ray4.h"
#include "../math/Tensor4.h"
#include "../math/Transform4.h"
#include "../math/Vector4.h"
#include "../math/math.h"
#include <algorithm>
#include <map>
#include <stack>
#include <vector>

namespace Forth
{
	namespace Physics
	{
		/// Common constant for physics simulation
		struct Common
		{

			/// <summary>
			/// Default gravity for all simulations.
			/// This parameter can be dynamically adjusted.
			/// </summary>
			static Vector4 GRAVITY;

			/// <summary>
			/// Simulation range helps to avoid wasted time computation of computing
			/// Objects that out of range (e.g. object keeps falling down).
			/// The object is deactivated once becomes so.
			/// </summary>
			static Bounds4 SIM_RANGE;

			/// <summary>
			/// Constant collision solving iteration.
			/// Higher values are more realistic, yet more expensive.
			/// Default is 15, can be put between 5 to 20
			/// </summary>
			static int ITERATIONS;

			/// <summary>
			/// Enables or disables rigid body sleeping.
			/// It's an important optimization on every physics engine.
			/// It's recommended to leave this on.
			/// </summary>
			static bool ALLOW_SLEEP;

			/// <summary>
			/// When two objects lay on each other should the simulator simulate friction?
			/// It's recommended to leave this on.
			/// </summary>
			static bool ENABLE_FRICTION;

			/// <summary>
			/// Constant maximum multi contact count. Don't change.
			/// </summary>
			const static int MULTICONTACT_COUNT = 16;

			/// <summary>
			/// Maximum allowed linear velocity that makes a rigidbody sleeps (Default is 0.01)
			/// </summary>
			static float SLEEP_LINEAR;

			/// <summary>
			///  Maximum allowed angular velocity that makes a rigidbody sleeps (Default is 2 deg)
			/// </summary>
			static float SLEEP_ANGULAR;

			/// <summary>
			/// Simulator don't make rigidbody sleeps instantly.
			/// They have a chance by given time to awake by itself. (Default is 0.5)
			/// </summary>
			static float SLEEP_TIME;

			/// <summary>
			/// How fast the collision must be resolved? (Default is 0.2)
			/// </summary>
			static float BAUMGARTE;

			/// <summary>
			/// Offset of allowed contact depth before kicked out by the solver.
			/// Allow very small number to avoid jitter. (Default is 0.05)
			/// </summary>
			static float PENETRATION_SLOP;

			/// <summary>
			/// Maximum allowed delta time. This prevents objects bypassing
			/// each other because of too large delta time. (Default is 0.02)
			/// </summary>
			static float MAX_DT;

			typedef float (*ShapeMixCB)(class Shape *, class Shape *);

			static ShapeMixCB MixRestitution;
			static ShapeMixCB MixFriction;

			/// <summary>
			/// Restitution (bounce) mixing. Default is max
			/// </summary>
			static float DefMixRestitution(class Shape *A, class Shape *B);

			/// <summary>
			/// Friction (slide) mixing. Default is average
			/// </summary>
			static float DefMixFriction(class Shape *A, class Shape *B);
		};
	} // namespace Physics
} // namespace Forth
