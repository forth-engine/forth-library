#include "Vector4.h"

namespace Forth
{
	/// <summary>
	/// Represent of a line that extends to infinity
	/// </summary>
	struct Ray4
	{

		/// <summary>
		/// Direction of the ray
		/// </summary>
		Vector4 direction;

		/// <summary>
		/// Starting position of the ray
		/// </summary>
		Vector4 origin;

		Ray4() : direction(Vector4()), origin(Vector4()) {}

		/// <summary>
		/// Create new Ray.
		/// </summary>
		Ray4(const Vector4 &origin, const Vector4 &direction) : direction(direction), origin(origin) {}

		/// <summary>
		/// Get a point that travels for given distance
		/// </summary>
		Vector4 GetPoint(float distance)
		{
			return origin + direction * distance;
		}

		/// <summary>
		/// The GetPoint's exotic alternative
		/// </summary>
		Vector4 operator*(float distance)
		{
			return GetPoint(distance);
		}
	};
} // namespace Forth
