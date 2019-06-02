#include "Common.h"
#include "collision/Shape.h"

namespace Forth
{
	namespace Physics
	{
		Vector4 Common::GRAVITY = Vector4(0, -9.8f, 0, 0);
		Bounds4 Common::SIM_RANGE = Bounds4(Vector4(-1000), Vector4(1000));
		int Common::ITERATIONS = 15;
		bool Common::ALLOW_SLEEP = true;
		bool Common::ENABLE_FRICTION = true;
		float Common::SLEEP_LINEAR = 0.01f;
		float Common::SLEEP_ANGULAR = (2 / 180.f) * PI;
		float Common::SLEEP_TIME = 0.5f;
		float Common::BAUMGARTE = 0.2f;
		float Common::PENETRATION_SLOP = 0.02f;
		float Common::MAX_DT = 0.02f;

		float Common::DefMixRestitution(Shape *A, Shape *B)
		{
			return A->restitution > B->restitution ? A->restitution : B->restitution;
		}
		float Common::DefMixFriction(Shape *A, Shape *B)
		{
			return (A->friction + B->friction) * 0.5f;
		}

		Common::ShapeMixCB Common::MixRestitution = Common::DefMixRestitution;
		Common::ShapeMixCB Common::MixFriction = Common::DefMixFriction;
	} // namespace Physics
} // namespace Forth