#pragma once

#include "Bounds4.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Plane4.h"

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

		Transform4() { }

		/// <summary>
		/// Create a 4x5 matrix.
		/// </summary>
		Transform4(const Vector4 &position, const Matrix4 &rotation)
			: position(position), rotation(rotation) { }

		/// <summary>
		/// Transforms a point.
		/// </summary>
		/// <remarks>
		/// This operator does translate the vector,
		/// Otherwise you should only multiply with the rotational part of this 4x5 matrix.
		/// </remarks>
		const Vector4 operator *(const Vector4& v) const
		{
			return (rotation * v + position);
		}

		/// <summary>
		/// Combine two transformations.
		/// </summary>
		/// <remarks>
		/// This operation is not commutative.
		/// </remarks>
		const Transform4 operator *(const Transform4& u) const
		{
			return Transform4(rotation * u.position + position, rotation * u.rotation);
		}

		/// <summary>
		/// Transform the right-hand plane.
		/// </summary>
		const Plane4 operator *(const Plane4& p) const
		{
			Vector4 origin = p.origin();
			origin = *this * origin;
			Vector4 normal = rotation * p.normal;

			return Plane4(normal, Dot(origin, normal));
		}

		/// <summary>
		/// Inversely combine two matrices
		/// </summary>
		/// <remarks>
		/// The operation is designed such that A * B / B is equivalent to A.
		/// </remarks>
		const Transform4 operator /(Transform4 t)
		{
			return Transform4(
				Transpose(t.rotation) * (position - t.position),
				Transpose(t.rotation) * rotation);
		}

		void operator=(const Transform4& rhs)
		{
			// guarantee mutability
			position = rhs.position;
			rotation = rhs.rotation;
		}

		/// <summary>
		/// Get a 4x5 identity matrix
		/// </summary>
		static Transform4 identity()
		{
			return Transform4(Vector4(), Matrix4::identity());
		}

		/// <summary> Convert to non-orthogonal matrix by applying a scale </summary>
		/// <remarks> Do not inverse or multiply any matrix produced using this method. </remarks>
		Transform4 ToTRS(Vector4 scale)
		{
			return Transform4(position, rotation * Matrix4(scale));
		}
	};

	/// <summary>
	/// Inversely transforms a point.
	/// </summary>
	/// <remarks>
	/// This operation is equivalent to Inverse(matrix) * vector
	/// </remarks>
	inline const Vector4 operator/(const Vector4& v, const Transform4& tx)
	{
		return Transpose(tx.rotation) * (v - tx.position);
	}

	/// <summary>
	/// Inverse the matrix
	/// </summary>
	inline const Transform4 Inverse(const Transform4& t)
	{
		const Matrix4 &tp = Transpose(t.rotation);
		return Transform4(tp * (-t.position), tp);
	}

}
