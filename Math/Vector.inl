#pragma once

#include "../Util.h"

namespace math
{
	template<typename T> struct vec2;
	template<typename T> struct vec3;
	template<typename T> struct vec4;

	template<typename T> float ComputeAngle(vec2<T> v1, vec2<T> v2);
	template<typename T> float ComputeAngle(vec3<T> v1, vec3<T> v2);
	template<typename T> float ComputeAngle(vec4<T> v1, vec4<T> v2);

	template<typename T> T DotProduct(vec2<T> v1, vec2<T> v2);
	template<typename T> T DotProduct(vec3<T> v1, vec3<T> v2);
	template<typename T> T DotProduct(vec4<T> v1, vec4<T> v2);

	template<typename T> vec3<T> CrossProduct(vec3<T> v1, vec3<T> v2);

	template<typename T>
	constexpr T sq(T x)
	{
		return x * x;
	}

	typedef vec2<uint32> vec2u;
	typedef vec3<uint32> vec3u;
	typedef vec4<uint32> vec4u;

	typedef vec2<int32> vec2i;
	typedef vec3<int32> vec3i;
	typedef vec4<int32> vec4i;

	typedef vec2<float> vec2f;
	typedef vec3<float> vec3f;
	typedef vec4<float> vec4f;

	template<typename T>
	struct vec2
	{
		typedef typename T contentType;

		union
		{
			struct { T x, y; };
			struct { T s, t; };
			struct { T u, v; };
		};

		constexpr vec2();
		constexpr vec2(T x, T y);

		vec2& operator+=(vec2 v);
		vec2& operator-=(vec2 v);
		vec2& operator*=(vec2 v);
		vec2& operator/=(vec2 v);

		vec2 operator+(vec2 v);
		vec2 operator-(vec2 v);
		vec2 operator*(vec2 v);
		vec2 operator/(vec2 v);

		vec2& operator+=(T val);
		vec2& operator-=(T val);
		vec2& operator*=(T val);
		vec2& operator/=(T val);

		vec2 operator+(T val);
		vec2 operator-(T val);
		vec2 operator*(T val);
		vec2 operator/(T val);

		vec2&	Normalize();
		T		Dot(vec2 v);
		T		Length();
	};

	template<typename T>
	struct vec3
	{
		union
		{
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T s, t, r; };
			struct { T u, v, w; };
		};

		vec3& operator+=(vec3 v);
		vec3& operator-=(vec3 v);
		vec3& operator*=(vec3 v);
		vec3& operator/=(vec3 v);

		vec3 operator+(vec3 v);
		vec3 operator-(vec3 v);
		vec3 operator*(vec3 v);
		vec3 operator/(vec3 v);

		vec3& operator+=(T val);
		vec3& operator-=(T val);
		vec3& operator*=(T val);
		vec3& operator/=(T val);

		vec3 operator+(T val);
		vec3 operator-(T val);
		vec3 operator*(T val);
		vec3 operator/(T val);

		vec3	Cross(vec3 v);
		T		Dot(vec3 v);

		vec3&	Normalize();
		T		Length();
	};

	template<typename T>
	struct vec4
	{
		union
		{
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T s, t, r, q; };
			struct { T u, v, w; };
		};

		vec4& operator+=(vec4 v);
		vec4& operator-=(vec4 v);
		vec4& operator*=(vec4 v);
		vec4& operator/=(vec4 v);

		vec4 operator+(vec4 v);
		vec4 operator-(vec4 v);
		vec4 operator*(vec4 v);
		vec4 operator/(vec4 v);

		vec4& operator+=(T val);
		vec4& operator-=(T val);
		vec4& operator*=(T val);
		vec4& operator/=(T val);

		vec4 operator+(T val);
		vec4 operator-(T val);
		vec4 operator*(T val);
		vec4 operator/(T val);

		vec3<T>	Cross3D(vec3<T> v);
		T		Dot(vec4 v);

		vec4&	Normalize();
		T		Length();
	};

	template<typename T>
	inline constexpr vec2<T>::vec2() :
		x(T(0)), y(T(0))
	{
	}

	template<typename T>
	inline constexpr vec2<T>::vec2(T x, T y) :
		x(x), y(y)
	{

	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator+=(vec2 v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator-=(vec2 v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator*=(vec2 v)
	{
		x *= v.x;
		y *= v.y;

		return *this;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator/=(vec2 v)
	{
		x /= v.x;
		y /= v.y;

		return *this;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator+(vec2 v)
	{
		vec2 other = *this;
		other += v;

		return other;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator-(vec2 v)
	{
		vec2 other = *this;
		other -= v;

		return other;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator*(vec2 v)
	{
		vec2 other = *this;
		other *= v;

		return other;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator/(vec2 v)
	{
		vec2 other = *this;
		other /= v;

		return other;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator+=(T val)
	{
		x += val;
		y += val;
		return *this;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator-=(T val)
	{
		x -= val;
		y -= val;
		return *this;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator*=(T val)
	{
		x *= val;
		y *= val;
		return *this;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::operator/=(T val)
	{
		x /= val;
		y /= val;
		return *this;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator+(T val)
	{
		vec2 other = *this;
		other += val;

		return other;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator-(T val)
	{
		vec2 other = *this;
		other += val;

		return other;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator*(T val)
	{
		vec2 other = *this;
		other *= val;

		return other;
	}

	template<typename T>
	inline vec2<T> vec2<T>::operator/(T val)
	{
		vec2 other = *this;
		other /= val;

		return other;
	}

	template<typename T>
	inline vec2<T> & vec2<T>::Normalize()
	{
		T length = Length();
		x /= length;
		y /= length;

		return *this;
	}

	template<typename T>
	inline T vec2<T>::Dot(vec2 v)
	{
		return x * v.x + y * v.y;
	}

	template<typename T>
	inline T vec2<T>::Length()
	{
		return std::sqrt(sq(x) + sq(y));
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator+=(vec3 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator-=(vec3 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator*=(vec3 v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator/=(vec3 v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator+(vec3 v)
	{
		vec3 other = *this;
		other += v;

		return other;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator-(vec3 v)
	{
		vec3 other = *this;
		other -= v;

		return other;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator*(vec3 v)
	{
		vec3 other = *this;
		other *= v;

		return other;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator/(vec3 v)
	{
		vec3 other = *this;
		other /= v;

		return other;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator+=(T val)
	{
		x += val;
		y += val;
		z += val;

		return *this;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator-=(T val)
	{
		x -= val;
		y -= val;
		z -= val;

		return *this;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator*=(T val)
	{
		x *= val;
		y *= val;
		z *= val;

		return *this;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::operator/=(T val)
	{
		x /= val;
		y /= val;
		z /= val;

		return *this;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator+(T val)
	{
		vec3 other = *this;
		other += val;

		return other;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator-(T val)
	{
		vec3 other = *this;
		other += val;

		return other;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator*(T val)
	{
		vec3 other = *this;
		other *= val;

		return other;
	}

	template<typename T>
	inline vec3<T> vec3<T>::operator/(T val)
	{
		vec3 other = *this;
		other /= val;

		return other;
	}

	template<typename T>
	inline vec3<T> vec3<T>::Cross(vec3 v)
	{
		vec3 orthogonal;
		orthogonal.x = y * v.z - z * v.y;
		orthogonal.y = z * v.x - x * v.z;
		orthogonal.z = x * v.y - y * v.x;

		return orthogonal;
	}

	template<typename T>
	inline vec3<T> & vec3<T>::Normalize()
	{
		auto length = Length();
		x /= length;
		y /= length;
		z /= length;

		return *this;
	}

	template<typename T>
	inline T vec3<T>::Dot(vec3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	template<typename T>
	inline T vec3<T>::Length()
	{
		return std::sqrt(sq(x) + sq(y) + sq(z));
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator+=(vec4 v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator-=(vec4 v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator*=(vec4 v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;

		return *this;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator/=(vec4 v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;

		return *this;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator+(vec4 v)
	{
		vec4 other = *this;
		other += v;

		return other;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator-(vec4 v)
	{
		vec4 other = *this;
		other -= v;

		return other;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator*(vec4 v)
	{
		vec4 other = *this;
		other *= v;

		return other;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator/(vec4 v)
	{
		vec4 other = *this;
		other /= v;

		return other;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator+=(T val)
	{
		x += val;
		y += val;
		z += val;
		w += val;

		return *this;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator-=(T val)
	{
		x -= val;
		y -= val;
		z -= val;
		w -= val;

		return *this;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator*=(T val)
	{
		x *= val;
		y *= val;
		z *= val;
		w *= val;

		return *this;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::operator/=(T val)
	{
		x /= val;
		y /= val;
		z /= val;
		w /= val;

		return *this;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator+(T val)
	{
		vec4 other = *this;
		other += val;

		return other;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator-(T val)
	{
		vec4 other = *this;
		other += val;

		return other;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator*(T val)
	{
		vec4 other = *this;
		other *= val;

		return other;
	}

	template<typename T>
	inline vec4<T> vec4<T>::operator/(T val)
	{
		vec4 other = *this;
		other /= val;

		return other;
	}

	template<typename T>
	inline vec3<T> vec4<T>::Cross3D(vec3<T> v)
	{
		vec3<T> orthogonal;

		orthogonal.x = y * v.z - v.y * z;
		orthogonal.y = z * v.x - v.z * x;
		orthogonal.z = x * v.y - v.x * y;

		return orthogonal;
	}

	template<typename T>
	inline vec4<T> & vec4<T>::Normalize()
	{
		auto length = Length();

		x /= length;
		y /= length;
		z /= length;
		w /= length;

		return *this;
	}

	template<typename T>
	inline T vec4<T>::Dot(vec4 v)
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	template<typename T>
	inline T vec4<T>::Length()
	{
		return std::sqrt(sq(x) + sq(y) + sq(z) + sq(z));
	}

	template<typename T>
	float ComputeAngle(vec2<T> v1, vec2<T> v2)
	{
		return std::acos(v1.Dot(v2));
	}

	template<typename T>
	float ComputeAngle(vec3<T> v1, vec3<T> v2)
	{
		return std::acos(v1.Dot(v2));
	}

	template<typename T>
	float ComputeAngle(vec4<T> v1, vec4<T> v2)
	{
		return std::acos(v1.Dot(v2));
	}

	template<typename T>
	T DotProduct(vec2<T> v1, vec2<T> v2)
	{
		return v1.Dot(v2);
	}

	template<typename T>
	T DotProduct(vec3<T> v1, vec3<T> v2)
	{
		return v1.Dot(v2);
	}

	template<typename T>
	T DotProduct(vec4<T> v1, vec4<T> v2)
	{
		return v1.Dot(v2);
	}

	template<typename T>
	vec3<T> CrossProduct(vec3<T> v1, vec3<T> v2)
	{
		return v1.Cross(v2);
	}
}