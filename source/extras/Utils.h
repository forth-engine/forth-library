#pragma once
#include "../math/Math.h"

namespace Forth
{
	template <typename T>
	void EnsureCapacity(T **arr, const int count, int *cap, const int target)
	{
		if (*cap < target)
		{
			*cap = Max(target, *cap << 1);
			T *newArr = new T[*cap];

			if (count > 0)
			{
				memcpy(newArr, *arr, count * sizeof(T));
			}

			delete[] * arr;
			*arr = newArr;
		}
	}

#define FORTH_ARRAY(name, T) \
	T *name = new T[4];      \
	int name##_cap = 4;        \
	int name##_count = 0

} // namespace Forth
