#pragma once

#include "Bounds4.h"
#include "Matrix4.h"
#include "Plane4.h"
#include "Vector4.h"

namespace Forth
{
	/// <summary>
	/// A 4x5 matrix to describe 4D transformation.
	/// </summary>
	struct Transform4
	{
		/// <summary>
		/// Fifth column of the matrix, denoted as a translation vector.
		/// </summary>
		Vector4 position;

		/// <summary>
		/// 4x4 part of the matrix, denoted as a rotational matrix.
		/// </summary>
		Matrix4 rotation;

		Transform4(void) : position(), rotation() {}

		/// <summary>
		/// Create a 4x5 matrix.
		/// </summary>
		Transform4(const Vector4 &position, const Matrix4 &rotation)
			: position(position), rotation(rotation) {}

		/// <summary>
		/// Create non-orthogonal 4x5 matrix.
		/// </summary>
		Transform4(const Vector4 &position, const Matrix4 &rotation, const Vector4 &scale)
			: position(position), rotation(rotation * Matrix4(scale)) {}

		/// <summary>
		/// Transforms a point.
		/// </summary>
		/// <remarks>
		/// This operator does translate the vector,
		/// Otherwise you should only multiply with the rotational part of this 4x5 matrix.
		/// </remarks>
		Vector4 operator*(const Vector4 &v) const
		{
			return (rotation * v + position);
		}

		/// <summary>
		/// Combine two transformations.
		/// </summary>
		/// <remarks>
		/// This operation is not commutative.
		/// </remarks>
		Transform4 operator*(const Transform4 &u) const
		{
			return Transform4(rotation * u.position + position, rotation * u.rotation);
		}

		/// <summary>
		/// Transform the right-hand plane.
		/// </summary>
		Plane4 operator*(const Plane4 &p) const
		{
			Vector4 origin = p.origin();
			origin = *this * origin;
			Vector4 normal = rotation * p.normal;

			return Plane4(normal, Dot(origin, normal));
		}

		/// <summary>
		/// Convert to OBB, transform, then wrap it inside AABB back
		/// </summary>
		Bounds4 operator*(const Bounds4 &u) const
		{
			Bounds4 b = Bounds4::infinite();
			const Vector4 &e = u.extent(), &c = u.center();
			for (float x = -1.f; x <= 1.f; x += 2.f)
				for (float y = -1.f; y <= 1.f; y += 2.f)
					for (float z = -1.f; z <= 1.f; z += 2.f)
						for (float w = -1.f; w <= 1.f; w += 2.f)
						{
							b.Allocate(*this * (e * Vector4(x, y, z, w)) + c);
						}
			return b;
		}

		/// <summary>
		/// Inversely combine two matrices
		/// </summary>
		/// <remarks>
		/// The operation is designed such that A * B / B is equivalent to A.
		/// </remarks>
		Transform4 operator/(const Transform4 &t) const
		{
			return Transform4(
				Transpose(t.rotation) * (position - t.position),
				Transpose(t.rotation) * rotation);
		}

		Transform4& operator=(const Transform4 &rhs)
		{
			// guarantee mutability
			position = rhs.position;
			rotation = rhs.rotation;
			return *this;
		}

		bool operator==(const Transform4 &t) const
		{
			return position == t.position && rotation == t.rotation;
		}

		/// <summary>
		/// Get a 4x5 identity matrix
		/// </summary>
		static const Transform4 identity;

		/// <summary> Convert to non-orthogonal matrix by applying a scale </summary>
		/// <remarks> Do not inverse or multiply any matrix produced using this method. </remarks>
		Transform4 ToTRS(const Vector4 &scale)
		{
			return Transform4(position, rotation * Matrix4(scale));
		}

		inline static Transform4 Position(Vector4 pos) { return Transform4(pos, Matrix4::identity); }

		inline static Transform4 Rotation(Matrix4 rot) { return Transform4(Vector4::zero, rot); }

		inline static Transform4 Scale(Vector4 scl) { return Transform4(Vector4::zero, Matrix4(scl)); }

	};



	/// <summary>
	/// Inversely transforms a point.
	/// </summary>
	/// <remarks>
	/// This operation is equivalent to Inverse(matrix) * vector
	/// </remarks>
	inline const Vector4 operator/(const Vector4 &v, const Transform4 &tx)
	{
		return Transpose(tx.rotation) * (v - tx.position);
	}

	/// <summary>
	/// Inverse the matrix
	/// </summary>
	inline const Transform4 Inverse(const Transform4 &t)
	{
		const Matrix4 tp = Transpose(t.rotation);
		return Transform4(tp * (-t.position), tp);
	}

} // namespace Forth
