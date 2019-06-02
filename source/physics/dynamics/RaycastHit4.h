#pragma once
#include "../Common.h"

namespace Forth
{
	namespace Physics
	{
		/// <summary>
		/// Raycasting information.
		/// </summary>
		struct RaycastHit4
		{
			/// <summary> Pointer to hinted collider </summary>
			class Shape *collider;

			/// <summary> The distance to impact point </summary>
			float distance;

			/// <summary> The surface normal of the impact </summary>
			Vector4 normal;

			/// <summary> The ray that generates the raycast </summary>
			Ray4 ray;

			/// <summary> The world point to the impact </summary>
			Vector4 point()
			{
				return ray * distance;
			}

			/// <summary>
			/// Create and initialize the raycast.
			/// </summary>
			RaycastHit4(Ray4 ray) : ray(ray), distance(0), normal(Vector4()), collider(NULL) {}

			/// <summary>
			/// Create, initialize, and report the raycast.
			/// </summary>
			RaycastHit4(Ray4 ray, float distance, Vector4 normal, class Shape *collider) : ray(ray), distance(distance), normal(normal), collider(collider)
			{
			}

			/// <summary>
			/// Report the raycast
			/// </summary>
			void Set(float distance, Vector4 normal, class Shape *collider)
			{
				if (!hit() || this->distance > distance)
				{
					this->distance = distance;
					this->normal = normal;
					this->collider = NULL;
				}
			}

			/// <summary> Does raycasting succeeded? </summary>
			bool hit() { return collider != NULL; }
		};
	} // namespace Physics
} // namespace Forth