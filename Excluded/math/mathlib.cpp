#include "mathlib.h"

// CURRENT_VECTOR_ORIENTATION && CURRENT_COORDINATE_SYSTEM can be changed in mlib.
#if CURRENT_VECTOR_ORIENTATION == COLUMN_MAJOR
#include "ColumnMajor.h"
#else
#include "RowMajor.h"
#endif

/*
   Column Major Matrix-vector Multiplication. [3, 3] * [3, 1] (M * v)
   [a, b, c]   [x]   [x']
   [d, e, f] * [y] = [y']
   [g, h, i]   [z]   [z']

   Row Major Matrix-vector Multiplication. [1, 3] * [3, 3] (v * M)
   [x]   [a, b, c]   [x']
   [y] * [d, e, f] = [y']
   [z]   [g, h, i]   [z']
*/

namespace mlib
{
	vec4::vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	vec4::vec4(const float& n) : x(n), y(n), z(n), w(n) {}
	vec4::vec4(float&& n) : x(n), y(n), z(n), w(n) {}
	vec4::vec4(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {}
	vec4::vec4(float&& x, float&& y, float&& z, float&& w) : x(x), y(y), z(z), w(w) {}
	vec4::vec4(const vec4& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}
	vec4::vec4(vec4&& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {}

	vec4& vec4::operator=(const vec4& v) noexcept { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	vec4& vec4::operator=(vec4&& v) noexcept { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

	vec4 vec4::operator+(const vec4& v) { return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	vec4 vec4::operator-(const vec4& v) { return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	vec4 vec4::operator/(const vec4& v) { return vec4(x / v.x, y / v.y, z / v.z, w / v.w); }
	vec4 vec4::operator*(const vec4& v) { return vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	vec4 vec4::operator*(const float& s) { return vec4(x * s, y * s, z * s, w * s); }
	vec4 vec4::operator/(const float& s) { return vec4(x / s, y / s, z / s, w / s); }

	vec4 vec4::operator+(vec4&& v) { return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	vec4 vec4::operator-(vec4&& v) { return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	vec4 vec4::operator*(vec4&& v) { return vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	vec4 vec4::operator/(vec4&& v) { return vec4(x / v.x, y / v.y, z / v.z, w / v.w); }
	vec4 vec4::operator*(float&& s) { return vec4(x * s, y * s, z * s, w * s); }
	vec4 vec4::operator/(float&& s) { return vec4(x / s, y / s, z / s, w / s); }

	vec4& vec4::operator+=(const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	vec4& vec4::operator-=(const vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	vec4& vec4::operator*=(const vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	vec4& vec4::operator/=(const vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	vec4& vec4::operator*=(const float& s) { x *= s; y *= s; z *= s; w *= s;         return *this; }
	vec4& vec4::operator/=(const float& s) { x /= s; y /= s; z /= s; w /= s;         return *this; }

	vec4& vec4::operator+=(vec4&& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	vec4& vec4::operator-=(vec4&& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	vec4& vec4::operator*=(vec4&& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	vec4& vec4::operator/=(vec4&& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	vec4& vec4::operator*=(float&& s) { x *= s; y *= s; z *= s; w *= s;         return *this; }
	vec4& vec4::operator/=(float&& s) { x /= s; y /= s; z /= s; w /= s;         return *this; }

	float& vec4::operator ()(unsigned int idx)
	{
		if (idx > 3) { std::cout << "INVALID INDEX ERROR -> VEC4"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	const float& vec4::operator ()(unsigned int idx) const
	{
		if (idx > 3) { std::cout << "INVALID INDEX ERROR -> VEC4"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	float& vec4::operator ()(int idx)
	{
		if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> VEC4"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	const float& vec4::operator ()(int idx) const
	{
		if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> VEC4"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}

	float vec4::dot(const vec4& v) { return x * v.x + y * v.y + z * v.z + w * v.w; }
	float vec4::dot(vec4&& v) { return x * v.x + y * v.y + z * v.z + w * v.w; }
	const float vec4::dot(const vec4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	const float vec4::dot(vec4&& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	float vec4::sum() { return x + y + z + w; }
	float vec4::mag() { return sqrt(x * x + y * y + z * z + w * w); }
	vec4& vec4::normalize() { operator/=(mag()); return *this; }
	std::string vec4::str() { std::stringstream ss(""); ss << "\n[" << x << ", " << y << ", " << z << ", " << w << "]"; return ss.str(); }

	vec3::vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	vec3::vec3(const float& n) : x(n), y(n), z(n) {}
	vec3::vec3(float&& n) : x(n), y(n), z(n) {}
	vec3::vec3(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {}
	vec3::vec3(float&& x, float&& y, float&& z) : x(x), y(y), z(z) {}
	vec3::vec3(const vec3& v) noexcept : x(v.x), y(v.y), z(v.z) {}
	vec3::vec3(vec3&& v) noexcept : x(v.x), y(v.y), z(v.z) {}

	vec3& vec3::operator=(const vec3& v) noexcept { x = v.x; y = v.y; z = v.z; return *this; }
	vec3& vec3::operator=(vec3&& v) noexcept { x = v.x; y = v.y; z = v.z; return *this; }

	vec3 vec3::operator+(const vec3& v) { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 vec3::operator-(const vec3& v) { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 vec3::operator*(const vec3& v) { return vec3(x * v.x, y * v.y, z * v.z); }
	vec3 vec3::operator/(const vec3& v) { return vec3(x / v.x, y / v.y, z / v.z); }
	vec3 vec3::operator*(const float& s) { return vec3(x * s, y * s, z * s); }
	vec3 vec3::operator/(const float& s) { return vec3(x / s, y / s, z / s); }

	vec3 vec3::operator+(vec3&& v) { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 vec3::operator-(vec3&& v) { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 vec3::operator*(vec3&& v) { return vec3(x * v.x, y * v.y, z * v.z); }
	vec3 vec3::operator/(vec3&& v) { return vec3(x / v.x, y / v.y, z / v.z); }
	vec3 vec3::operator*(float&& s) { return vec3(x * s, y * s, z * s); }
	vec3 vec3::operator/(float&& s) { return vec3(x / s, y / s, z / s); }

	vec3& vec3::operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	vec3& vec3::operator-=(const vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	vec3& vec3::operator*=(const vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	vec3& vec3::operator/=(const vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	vec3& vec3::operator*=(const float& s) { x *= s;   y *= s;   z *= s;   return *this; }
	vec3& vec3::operator/=(const float& s) { x /= s;   y /= s;   z /= s;   return *this; }

	vec3& vec3::operator+=(vec3&& v) { x += v.x; y += v.y; z += v.z;  return *this; }
	vec3& vec3::operator-=(vec3&& v) { x -= v.x; y -= v.y; z -= v.z;  return *this; }
	vec3& vec3::operator*=(vec3&& v) { x *= v.x; y *= v.y; z *= v.z;  return *this; }
	vec3& vec3::operator/=(vec3&& v) { x /= v.x; y /= v.y; z /= v.z;  return *this; }
	vec3& vec3::operator*=(float&& s) { x *= s;   y *= s;   z *= s;    return *this; }
	vec3& vec3::operator/=(float&& s) { x /= s;   y /= s;   z /= s;    return *this; }

	float& vec3::operator ()(unsigned int idx)
	{
		if (idx > 2) { std::cout << "INVALID INDEX ERROR -> VEC3"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	const float& vec3::operator ()(unsigned int idx) const
	{
		if (idx > 2) { std::cout << "INVALID INDEX ERROR -> VEC3"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	float& vec3::operator ()(int idx)
	{
		if (idx > 2 || idx < 0) { std::cout << "INVALID INDEX ERROR -> VEC3"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	const float& vec3::operator ()(int idx) const
	{
		if (idx > 2 || idx < 0) { std::cout << "INVALID INDEX ERROR -> VEC3"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}

	vec4  vec3::homogenous() { return vec4(x, y, z, 1.0f); }
	vec3  vec3::cross(const vec3& v) { return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	vec3  vec3::cross(vec3&& v) { return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	float vec3::dot(const vec3& v) { return x * v.x + y * v.y + z * v.z; }
	float vec3::dot(vec3&& v) { return x * v.x + y * v.y + z * v.z; }
	float vec3::sum() { return x + y + z; }
	float vec3::mag() { return sqrt(x * x + y * y + z * z); }
	vec3& vec3::normalize() { operator/=(mag()); return *this; }
	std::string vec3::str() { std::stringstream ss(""); ss << "[" << x << ", " << y << ", " << z << "]"; return ss.str(); }

	vec2::vec2() : x(0.0f), y(0.0f) {}
	vec2::vec2(const float& n) : x(n), y(n) {}
	vec2::vec2(float&& n) : x(n), y(n) {}
	vec2::vec2(const float& x, const float& y) : x(x), y(y) {}
	vec2::vec2(float&& x, float&& y) : x(x), y(y) {}
	vec2::vec2(const vec3& v) noexcept : x(v.x), y(v.y) {}
	vec2::vec2(vec3&& v) noexcept : x(v.x), y(v.y) {}

	vec2& vec2::operator=(const vec2& v) noexcept { x = v.x; y = v.y; return *this; }
	vec2& vec2::operator=(vec2&& v) noexcept { x = v.x; y = v.y; return *this; }

	vec2 vec2::operator+(const vec2& v) { return vec2(x + v.x, y + v.y); }
	vec2 vec2::operator-(const vec2& v) { return vec2(x - v.x, y - v.y); }
	vec2 vec2::operator*(const vec2& v) { return vec2(x * v.x, y * v.y); }
	vec2 vec2::operator/(const vec2& v) { return vec2(x / v.x, y / v.y); }
	vec2 vec2::operator*(const float& s) { return vec2(x * s, y * s); }
	vec2 vec2::operator/(const float& s) { return vec2(x / s, y / s); }

	vec2 vec2::operator+(vec2&& v) { return vec2(x + v.x, y + v.y); }
	vec2 vec2::operator-(vec2&& v) { return vec2(x - v.x, y - v.y); }
	vec2 vec2::operator*(vec2&& v) { return vec2(x * v.x, y * v.y); }
	vec2 vec2::operator/(vec2&& v) { return vec2(x / v.x, y / v.y); }
	vec2 vec2::operator*(float&& s) { return vec2(x * s, y * s); }
	vec2 vec2::operator/(float&& s) { return vec2(x / s, y / s); }

	vec2& vec2::operator+=(const vec2& v) { x += v.x; y += v.y; return *this; }
	vec2& vec2::operator-=(const vec2& v) { x -= v.x; y -= v.y; return *this; }
	vec2& vec2::operator*=(const vec2& v) { x *= v.x; y *= v.y; return *this; }
	vec2& vec2::operator/=(const vec2& v) { x /= v.x; y /= v.y; return *this; }
	vec2& vec2::operator*=(const float& s) { x *= s;   y *= s;   return *this; }
	vec2& vec2::operator/=(const float& s) { x /= s;   y /= s;   return *this; }

	vec2& vec2::operator+=(vec2&& v) { x += v.x; y += v.y; return *this; }
	vec2& vec2::operator-=(vec2&& v) { x -= v.x; y -= v.y; return *this; }
	vec2& vec2::operator*=(vec2&& v) { x *= v.x; y *= v.y; return *this; }
	vec2& vec2::operator/=(vec2&& v) { x /= v.x; y /= v.y; return *this; }
	vec2& vec2::operator*=(float&& s) { x *= s;   y *= s;   return *this; }
	vec2& vec2::operator/=(float&& s) { x /= s;   y /= s;   return *this; }

	float& vec2::operator ()(unsigned int idx)
	{
		if (idx > 1) { std::cout << "INVALID INDEX ERROR -> VEC2"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	const float& vec2::operator ()(unsigned int idx) const
	{
		if (idx > 1) { std::cout << "INVALID INDEX ERROR -> VEC2"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	float& vec2::operator ()(int idx)
	{
		if (idx != 1 && idx != 0) { std::cout << "INVALID INDEX ERROR -> VEC2"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}
	const float& vec2::operator ()(int idx) const
	{
		if (idx != 1 && idx != 0) { std::cout << "INVALID INDEX ERROR -> VEC2"; exit(-1); }
		return *(&x + idx * sizeof(float));
	}

	vec3  vec2::homogenous() { return vec3(x, y, 1.0f); }
	float vec2::dot(const vec2& v) { return x * v.x + y * v.y; }
	float vec2::dot(vec2&& v) { return x * v.x + y * v.y; }
	float vec2::sum() { return x + y; }
	float vec2::mag() { return sqrt(x * x + y * y); }
	vec2& vec2::normalize() { vec2::operator/=(mag()); return *this; }
	std::string vec2::str() { std::stringstream ss(""); ss << "\n[" << x << ", " << y << "]"; return ss.str(); }

	mat4::mat4(float&& a0, float&& a1, float&& a2, float&& a3,
		float&& a4, float&& a5, float&& a6, float&& a7,
		float&& a8, float&& a9, float&& a10, float&& a11,
		float&& a12, float&& a13, float&& a14, float&& a15)
	{
		mat[0] = a0;  mat[1] = a1;  mat[2] = a2;  mat[3] = a3;
		mat[4] = a4;  mat[5] = a5;  mat[6] = a6;  mat[7] = a7;
		mat[8] = a8;  mat[9] = a9;  mat[10] = a10; mat[11] = a11;
		mat[12] = a12; mat[13] = a13; mat[14] = a14; mat[15] = a15;
	}

	mat4::mat4(const float& a0, const float& a1, const float& a2, const float& a3,
		const float& a4, const float& a5, const float& a6, const float& a7,
		const float& a8, const float& a9, const float& a10, const float& a11,
		const float& a12, const float& a13, const float& a14, const float& a15)
	{
		mat[0] = a0;  mat[1] = a1;  mat[2] = a2;  mat[3] = a3;
		mat[4] = a4;  mat[5] = a5;  mat[6] = a6;  mat[7] = a7;
		mat[8] = a8;  mat[9] = a9;  mat[10] = a10; mat[11] = a11;
		mat[12] = a12; mat[13] = a13; mat[14] = a14; mat[15] = a15;
	}

	mat4::mat4(float* values) { memcpy(mat, values, 16 * sizeof(float)); }
	mat4::mat4() { for (int i = 0; i < 16; i++) mat[i] = 0.0f; }
	mat4::mat4(const float& n) { for (int i = 0; i < 16; i++) mat[i] = n; }
	mat4::mat4(float&& n) { for (int i = 0; i < 16; i++) mat[i] = n; }
	mat4::mat4(const mat4& m) { for (int i = 0; i < 16; i++) mat[i] = m.mat[i]; }
	mat4::mat4(mat4&& m) { for (int i = 0; i < 16; i++) mat[i] = m.mat[i]; }

	mat4& mat4::operator=(const mat4& m) noexcept { for (int i = 0; i < 16; i++) { mat[i] = m.mat[i]; } return *this; }
	mat4& mat4::operator=(mat4&& m) noexcept { for (int i = 0; i < 16; i++) { mat[i] = m.mat[i]; } return *this; }

	mat4 mat4::operator+(const mat4& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] + m.mat[i]; } return result.mat; }
	mat4 mat4::operator-(const mat4& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] - m.mat[i]; } return result.mat; }
	mat4 mat4::operator*(const mat4& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] * m.mat[i]; } return result.mat; }
	mat4 mat4::operator/(const mat4& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] / m.mat[i]; } return result.mat; }
	mat4 mat4::operator*(const float& s) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] * s; } return result.mat; }
	mat4 mat4::operator/(const float& s) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] / s; } return result.mat; }

	mat4 mat4::operator+(mat4&& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] + m.mat[i]; } return result.mat; }
	mat4 mat4::operator-(mat4&& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] - m.mat[i]; } return result.mat; }
	mat4 mat4::operator*(mat4&& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] * m.mat[i]; } return result.mat; }
	mat4 mat4::operator/(mat4&& m) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] / m.mat[i]; } return result.mat; }
	mat4 mat4::operator*(float&& s) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] * s; } return result.mat; }
	mat4 mat4::operator/(float&& s) { mat4 result(0.0f); for (int i = 0; i < 16; i++) { result.mat[i] = mat[i] / s; } return result.mat; }

	mat4& mat4::operator+=(const mat4& m) { for (int i = 0; i < 16; i++) { mat[i] += m.mat[i]; } return *this; }
	mat4& mat4::operator-=(const mat4& m) { for (int i = 0; i < 16; i++) { mat[i] -= m.mat[i]; } return *this; }
	mat4& mat4::operator*=(const mat4& m) { for (int i = 0; i < 16; i++) { mat[i] *= m.mat[i]; } return *this; }
	mat4& mat4::operator/=(const mat4& m) { for (int i = 0; i < 16; i++) { mat[i] /= m.mat[i]; } return *this; }
	mat4& mat4::operator*=(const float& s) { for (int i = 0; i < 16; i++) { mat[i] *= s; } return *this; }
	mat4& mat4::operator/=(const float& s) { for (int i = 0; i < 16; i++) { mat[i] /= s; } return *this; }

	mat4& mat4::operator+=(mat4&& m) { for (int i = 0; i < 16; i++) { mat[i] += m.mat[i]; } return *this; }
	mat4& mat4::operator-=(mat4&& m) { for (int i = 0; i < 16; i++) { mat[i] -= m.mat[i]; } return *this; }
	mat4& mat4::operator*=(mat4&& m) { for (int i = 0; i < 16; i++) { mat[i] *= m.mat[i]; } return *this; }
	mat4& mat4::operator/=(mat4&& m) { for (int i = 0; i < 16; i++) { mat[i] /= m.mat[i]; } return *this; }
	mat4& mat4::operator*=(float&& s) { for (int i = 0; i < 16; i++) { mat[i] *= s; } return *this; }
	mat4& mat4::operator/=(float&& s) { for (int i = 0; i < 16; i++) { mat[i] /= s; } return *this; }

	mat4 mat4::dot(const mat4& m)
	{
		mat4 result(0.0f);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				result.mat[j + 4 * i] = operator()(i).dot(m.column(j));
			}
		return result;
	}
	mat4 mat4::dot(mat4&& m)
	{
		mat4 result(0.0f);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				result.mat[j + 4 * i] = operator()(i).dot(m.column(j));
		return result;
	}

	mat4		mat4::matmul(const mat4& m)
	{
		mat4 result(0.0f);
		// used this for reference: https://i.stack.imgur.com/iRxxe.png
		// mat = a
		// m.mat = b     a         b          a         b          a         b           a         b
		result.mat[0] = mat[0] * m.mat[0] + mat[1] * m.mat[4] + mat[2] * m.mat[8] + mat[3] * m.mat[12];
		result.mat[1] = mat[0] * m.mat[1] + mat[1] * m.mat[5] + mat[2] * m.mat[9] + mat[3] * m.mat[13];
		result.mat[2] = mat[0] * m.mat[2] + mat[1] * m.mat[6] + mat[2] * m.mat[10] + mat[3] * m.mat[14];
		result.mat[3] = mat[0] * m.mat[3] + mat[1] * m.mat[7] + mat[2] * m.mat[11] + mat[3] * m.mat[15];

		result.mat[4] = mat[4] * m.mat[0] + mat[5] * m.mat[4] + mat[6] * m.mat[8] + mat[7] * m.mat[12];
		result.mat[5] = mat[4] * m.mat[1] + mat[5] * m.mat[5] + mat[6] * m.mat[9] + mat[7] * m.mat[13];
		result.mat[6] = mat[4] * m.mat[2] + mat[5] * m.mat[6] + mat[6] * m.mat[10] + mat[7] * m.mat[14];
		result.mat[7] = mat[4] * m.mat[3] + mat[5] * m.mat[7] + mat[6] * m.mat[11] + mat[7] * m.mat[15];

		result.mat[8] = mat[8] * m.mat[0] + mat[9] * m.mat[4] + mat[10] * m.mat[8] + mat[11] * m.mat[12];
		result.mat[9] = mat[8] * m.mat[1] + mat[9] * m.mat[5] + mat[10] * m.mat[9] + mat[11] * m.mat[13];
		result.mat[10] = mat[8] * m.mat[2] + mat[9] * m.mat[6] + mat[10] * m.mat[10] + mat[11] * m.mat[14];
		result.mat[11] = mat[8] * m.mat[3] + mat[9] * m.mat[7] + mat[10] * m.mat[11] + mat[11] * m.mat[15];

		result.mat[12] = mat[12] * m.mat[0] + mat[13] * m.mat[4] + mat[14] * m.mat[8] + mat[15] * m.mat[12];
		result.mat[13] = mat[12] * m.mat[1] + mat[13] * m.mat[5] + mat[14] * m.mat[9] + mat[15] * m.mat[13];
		result.mat[14] = mat[12] * m.mat[2] + mat[13] * m.mat[6] + mat[14] * m.mat[10] + mat[15] * m.mat[14];
		result.mat[15] = mat[12] * m.mat[3] + mat[13] * m.mat[7] + mat[14] * m.mat[11] + mat[15] * m.mat[15];
		return result;
	}
	mat4		mat4::matmul(mat4&& m)
	{
		mat4 result(0.0f);
		// used this for reference: https://i.stack.imgur.com/iRxxe.png
		// mat = a
		// m.mat = b     a         b          a         b          a         b           a         b
		result.mat[0] = mat[0] * m.mat[0] + mat[1] * m.mat[4] + mat[2] * m.mat[8] + mat[3] * m.mat[12];
		result.mat[1] = mat[0] * m.mat[1] + mat[1] * m.mat[5] + mat[2] * m.mat[9] + mat[3] * m.mat[13];
		result.mat[2] = mat[0] * m.mat[2] + mat[1] * m.mat[6] + mat[2] * m.mat[10] + mat[3] * m.mat[14];
		result.mat[3] = mat[0] * m.mat[3] + mat[1] * m.mat[7] + mat[2] * m.mat[11] + mat[3] * m.mat[15];

		result.mat[4] = mat[4] * m.mat[0] + mat[5] * m.mat[4] + mat[6] * m.mat[8] + mat[7] * m.mat[12];
		result.mat[5] = mat[4] * m.mat[1] + mat[5] * m.mat[5] + mat[6] * m.mat[9] + mat[7] * m.mat[13];
		result.mat[6] = mat[4] * m.mat[2] + mat[5] * m.mat[6] + mat[6] * m.mat[10] + mat[7] * m.mat[14];
		result.mat[7] = mat[4] * m.mat[3] + mat[5] * m.mat[7] + mat[6] * m.mat[11] + mat[7] * m.mat[15];

		result.mat[8] = mat[8] * m.mat[0] + mat[9] * m.mat[4] + mat[10] * m.mat[8] + mat[11] * m.mat[12];
		result.mat[9] = mat[8] * m.mat[1] + mat[9] * m.mat[5] + mat[10] * m.mat[9] + mat[11] * m.mat[13];
		result.mat[10] = mat[8] * m.mat[2] + mat[9] * m.mat[6] + mat[10] * m.mat[10] + mat[11] * m.mat[14];
		result.mat[11] = mat[8] * m.mat[3] + mat[9] * m.mat[7] + mat[10] * m.mat[11] + mat[11] * m.mat[15];

		result.mat[12] = mat[12] * m.mat[0] + mat[13] * m.mat[4] + mat[14] * m.mat[8] + mat[15] * m.mat[12];
		result.mat[13] = mat[12] * m.mat[1] + mat[13] * m.mat[5] + mat[14] * m.mat[9] + mat[15] * m.mat[13];
		result.mat[14] = mat[12] * m.mat[2] + mat[13] * m.mat[6] + mat[14] * m.mat[10] + mat[15] * m.mat[14];
		result.mat[15] = mat[12] * m.mat[3] + mat[13] * m.mat[7] + mat[14] * m.mat[11] + mat[15] * m.mat[15];
		return result;
	}
	mat4		mat4::inverse()
	{
		vec4 f0(mat[10] * mat[15] - mat[14] * mat[11],
			mat[10] * mat[15] - mat[14] * mat[11],
			mat[6] * mat[15] - mat[14] * mat[7],
			mat[6] * mat[11] - mat[10] * mat[7]);
		vec4 f1(mat[9] * mat[15] - mat[13] * mat[11],
			mat[9] * mat[15] - mat[13] * mat[11],
			mat[5] * mat[15] - mat[13] * mat[7],
			mat[5] * mat[11] - mat[9] * mat[7]);
		vec4 f2(mat[9] * mat[14] - mat[13] * mat[10],
			mat[9] * mat[14] - mat[13] * mat[10],
			mat[5] * mat[14] - mat[13] * mat[6],
			mat[5] * mat[10] - mat[9] * mat[6]);
		vec4 f3(mat[8] * mat[15] - mat[12] * mat[11],
			mat[8] * mat[15] - mat[12] * mat[11],
			mat[4] * mat[15] - mat[12] * mat[7],
			mat[4] * mat[11] - mat[8] * mat[7]);
		vec4 f4(mat[8] * mat[14] - mat[12] * mat[10],
			mat[8] * mat[14] - mat[12] * mat[10],
			mat[4] * mat[14] - mat[12] * mat[6],
			mat[4] * mat[10] - mat[8] * mat[6]);
		vec4 f5(mat[8] * mat[13] - mat[12] * mat[9],
			mat[8] * mat[13] - mat[12] * mat[9],
			mat[4] * mat[13] - mat[12] * mat[5],
			mat[4] * mat[9] - mat[8] * mat[5]);

		vec4 v0(mat[4], mat[0], mat[0], mat[0]);
		vec4 v1(mat[5], mat[1], mat[1], mat[1]);
		vec4 v2(mat[6], mat[2], mat[2], mat[2]);
		vec4 v3(mat[7], mat[3], mat[3], mat[3]);

		*this = mat4(
			(v1 * f0 - v2 * f1 + v3 * f2) * vec4(+1, -1, +1, -1),
			(v0 * f0 - v2 * f3 + v3 * f4) * vec4(-1, +1, -1, +1),
			(v0 * f1 - v1 * f3 + v3 * f5) * vec4(+1, -1, +1, -1),
			(v0 * f2 - v1 * f4 + v2 * f5) * vec4(-1, +1, -1, +1)
		);
		*this /= (column(0) * mat[0]).sum();
		return *this;
	}
	mat4& mat4::T()
	{
		mat4 result(0.0f);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) {
				result.mat[i * 4 + j] = mat[j * 4 + i];
			}
		memcpy(mat, &result.mat[0], 16 * sizeof(float));
		return *this;
	}
	float* mat4::buffer() { return &mat[0]; }
	std::string mat4::str()
	{
		std::stringstream ss("");
		for (int i = 0; i < 4; i++)
		{
			ss << "\n[";
			for (int j = 0; j < 3; j++)
				ss << mat[j + 4 * i] << ", ";
			ss << mat[3 + 4 * i] << "]";
		}
		return ss.str();
	}

	float		 rad(float deg) { return PIf * deg / 180; }
	float		 deg(float rad) { return 180.0f * rad / PIf; }
	float		 sqrt(float n)
	{
		float lo = 0, hi = n, mid;
		for (int i = 0; i < 10000; i++)
		{
			mid = (lo + hi) / 2;
			if (mid * mid == n) return mid;
			if (mid * mid > n) hi = mid;
			else lo = mid;
		}
		return mid;
	}
	float        randint(float min, float max) {
		srand((unsigned int)time(NULL));
		return min + (rand() * (max - min) / RAND_MAX);
	}
	vec3         random3() {
		srand((unsigned)time(NULL));
		return vec3(
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX
		);
	}
	vec4         random4() {
		srand((unsigned)time(NULL));
		return vec4(
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX
		);
	}
	Array<float> random(int size) {
		srand((unsigned int)time(NULL));
		Array<float> a(size);
		for (int i = 0; i < size; i++) a.push_back((float)rand() / RAND_MAX);
		return a;
	}
	mat4         random4x4()
	{
		mat4 result;
		for (int i = 0; i < 16; i++) result.mat[i] = (float)rand() / RAND_MAX;
		return result.mat;
	}

	mat4 identity()
	{
		return mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4 scale(const vec3& v)
	{
		return mat4(
			(float)v.x, 0.0f, 0.0f, 0.0f,
			0.0f, (float)v.y, 0.0f, 0.0f,
			0.0f, 0.0f, (float)v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	mat4 rotate(const float& x, const float& y, const float& z) {
		return mat4(
			cos(z) * sin(y), cos(z) * sin(y) * sin(x) - sin(z) * cos(x), cos(z) * sin(y) * cos(x) + sin(z) * sin(x), 0.0f,
			sin(z) * cos(y), sin(z) * sin(y) * sin(x) + cos(z) * cos(x), sin(z) * sin(y) * cos(x) - cos(z) * sin(x), 0.0f,
			-sin(y), cos(y) * sin(x), cos(y) * cos(x), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 rotate(const vec3& v)
	{
		return mat4(
			cos(v.z) * sin(v.y), cos(v.z) * sin(v.y) * sin(v.x) - sin(v.z) * cos(v.x), cos(v.z) * sin(v.y) * cos(v.x) + sin(v.z) * sin(v.x), 0.0f,
			sin(v.z) * cos(v.y), sin(v.z) * sin(v.y) * sin(v.x) + cos(v.z) * cos(v.x), sin(v.z) * sin(v.y) * cos(v.x) - cos(v.z) * sin(v.x), 0.0f,
			-sin(v.y), cos(v.y) * sin(v.x), cos(v.y) * cos(v.x), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 perspective(const float& near, const float& far, const float& aspect, const float& fov) {
		return mat4(
			1 / (aspect * tan(fov / 2)), 0.0f, 0.0f, 0.0f,
			0.0f, 1 / tan(fov / 2), 0.0f, 0.0f,
			0.0f, 0.0f, (far + near) / (near - far), -1.0f,
			0.0f, 0.0f, (2 * far * near) / (near - far), 0.0f
		);
	}

	mat4 scale(vec3&& v)
	{
		return mat4(
			(float)v.x, 0.0f, 0.0f, 0.0f,
			0.0f, (float)v.y, 0.0f, 0.0f,
			0.0f, 0.0f, (float)v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	mat4 rotate(float&& x, float&& y, float&& z) {
		return mat4(
			cos(z) * sin(y), cos(z) * sin(y) * sin(x) - sin(z) * cos(x), cos(z) * sin(y) * cos(x) + sin(z) * sin(x), 0.0f,
			sin(z) * cos(y), sin(z) * sin(y) * sin(x) + cos(z) * cos(x), sin(z) * sin(y) * cos(x) - cos(z) * sin(x), 0.0f,
			-sin(y), cos(y) * sin(x), cos(y) * cos(x), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 rotate(vec3&& v)
	{
		return mat4(
			cos(v.z) * sin(v.y), cos(v.z) * sin(v.y) * sin(v.x) - sin(v.z) * cos(v.x), cos(v.z) * sin(v.y) * cos(v.x) + sin(v.z) * sin(v.x), 0.0f,
			sin(v.z) * cos(v.y), sin(v.z) * sin(v.y) * sin(v.x) + cos(v.z) * cos(v.x), sin(v.z) * sin(v.y) * cos(v.x) - cos(v.z) * sin(v.x), 0.0f,
			-sin(v.y), cos(v.y) * sin(v.x), cos(v.y) * cos(v.x), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 perspective(float&& near, float&& far, float&& aspect, float&& fov) {
		mat4 result(0.0f);
		result(0, 0) = 1 / (aspect * tan(fov / 2));
		result(1, 1) = 1 / tan(fov / 2);
		result(2, 2) = (far + near) / (near - far);
		result(2, 3) = -1.0f;
		result(3, 2) = (2 * far * near) / (near - far);
		result(3, 3) = 0.0f;
		return result;
	}

	mat4 rotateX(const float& a)
	{
		return mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos(a), -sin(a), 0.0f,
			0.0f, sin(a), cos(a), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	mat4 rotateY(const float& a)
	{
		return mat4(
			cos(a), 0.0f, sin(a), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(a), 0.0f, cos(a), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 rotateZ(const float& a)
	{
		return mat4(
			cos(a), -sin(a), 0.0f, 0.0f,
			sin(a), cos(a), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 rotateX(float&& a)
	{
		return mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos(a), -sin(a), 0.0f,
			0.0f, sin(a), cos(a), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	mat4 rotateY(float&& a)
	{
		return mat4(
			cos(a), 0.0f, sin(a), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(a), 0.0f, cos(a), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 rotateZ(float&& a)
	{
		return mat4(
			cos(a), -sin(a), 0.0f, 0.0f,
			sin(a), cos(a), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	mat4 inverse(mat4& m)
	{
		mat4 out;
		float det;

		out.mat[0] = m.mat[5] * m.mat[10] * m.mat[15] -
			m.mat[5] * m.mat[11] * m.mat[14] -
			m.mat[9] * m.mat[6] * m.mat[15] +
			m.mat[9] * m.mat[7] * m.mat[14] +
			m.mat[13] * m.mat[6] * m.mat[11] -
			m.mat[13] * m.mat[7] * m.mat[10];

		out.mat[4] = -m.mat[4] * m.mat[10] * m.mat[15] +
			m.mat[4] * m.mat[11] * m.mat[14] +
			m.mat[8] * m.mat[6] * m.mat[15] -
			m.mat[8] * m.mat[7] * m.mat[14] -
			m.mat[12] * m.mat[6] * m.mat[11] +
			m.mat[12] * m.mat[7] * m.mat[10];

		out.mat[8] = m.mat[4] * m.mat[9] * m.mat[15] -
			m.mat[4] * m.mat[11] * m.mat[13] -
			m.mat[8] * m.mat[5] * m.mat[15] +
			m.mat[8] * m.mat[7] * m.mat[13] +
			m.mat[12] * m.mat[5] * m.mat[11] -
			m.mat[12] * m.mat[7] * m.mat[9];

		out.mat[12] = -m.mat[4] * m.mat[9] * m.mat[14] +
			m.mat[4] * m.mat[10] * m.mat[13] +
			m.mat[8] * m.mat[5] * m.mat[14] -
			m.mat[8] * m.mat[6] * m.mat[13] -
			m.mat[12] * m.mat[5] * m.mat[10] +
			m.mat[12] * m.mat[6] * m.mat[9];

		out.mat[1] = -m.mat[1] * m.mat[10] * m.mat[15] +
			m.mat[1] * m.mat[11] * m.mat[14] +
			m.mat[9] * m.mat[2] * m.mat[15] -
			m.mat[9] * m.mat[3] * m.mat[14] -
			m.mat[13] * m.mat[2] * m.mat[11] +
			m.mat[13] * m.mat[3] * m.mat[10];

		out.mat[5] = m.mat[0] * m.mat[10] * m.mat[15] -
			m.mat[0] * m.mat[11] * m.mat[14] -
			m.mat[8] * m.mat[2] * m.mat[15] +
			m.mat[8] * m.mat[3] * m.mat[14] +
			m.mat[12] * m.mat[2] * m.mat[11] -
			m.mat[12] * m.mat[3] * m.mat[10];

		out.mat[9] = -m.mat[0] * m.mat[9] * m.mat[15] +
			m.mat[0] * m.mat[11] * m.mat[13] +
			m.mat[8] * m.mat[1] * m.mat[15] -
			m.mat[8] * m.mat[3] * m.mat[13] -
			m.mat[12] * m.mat[1] * m.mat[11] +
			m.mat[12] * m.mat[3] * m.mat[9];

		out.mat[13] = m.mat[0] * m.mat[9] * m.mat[14] -
			m.mat[0] * m.mat[10] * m.mat[13] -
			m.mat[8] * m.mat[1] * m.mat[14] +
			m.mat[8] * m.mat[2] * m.mat[13] +
			m.mat[12] * m.mat[1] * m.mat[10] -
			m.mat[12] * m.mat[2] * m.mat[9];

		out.mat[2] = m.mat[1] * m.mat[6] * m.mat[15] -
			m.mat[1] * m.mat[7] * m.mat[14] -
			m.mat[5] * m.mat[2] * m.mat[15] +
			m.mat[5] * m.mat[3] * m.mat[14] +
			m.mat[13] * m.mat[2] * m.mat[7] -
			m.mat[13] * m.mat[3] * m.mat[6];

		out.mat[6] = -m.mat[0] * m.mat[6] * m.mat[15] +
			m.mat[0] * m.mat[7] * m.mat[14] +
			m.mat[4] * m.mat[2] * m.mat[15] -
			m.mat[4] * m.mat[3] * m.mat[14] -
			m.mat[12] * m.mat[2] * m.mat[7] +
			m.mat[12] * m.mat[3] * m.mat[6];

		out.mat[10] = m.mat[0] * m.mat[5] * m.mat[15] -
			m.mat[0] * m.mat[7] * m.mat[13] -
			m.mat[4] * m.mat[1] * m.mat[15] +
			m.mat[4] * m.mat[3] * m.mat[13] +
			m.mat[12] * m.mat[1] * m.mat[7] -
			m.mat[12] * m.mat[3] * m.mat[5];

		out.mat[14] = -m.mat[0] * m.mat[5] * m.mat[14] +
			m.mat[0] * m.mat[6] * m.mat[13] +
			m.mat[4] * m.mat[1] * m.mat[14] -
			m.mat[4] * m.mat[2] * m.mat[13] -
			m.mat[12] * m.mat[1] * m.mat[6] +
			m.mat[12] * m.mat[2] * m.mat[5];

		out.mat[3] = -m.mat[1] * m.mat[6] * m.mat[11] +
			m.mat[1] * m.mat[7] * m.mat[10] +
			m.mat[5] * m.mat[2] * m.mat[11] -
			m.mat[5] * m.mat[3] * m.mat[10] -
			m.mat[9] * m.mat[2] * m.mat[7] +
			m.mat[9] * m.mat[3] * m.mat[6];

		out.mat[7] = m.mat[0] * m.mat[6] * m.mat[11] -
			m.mat[0] * m.mat[7] * m.mat[10] -
			m.mat[4] * m.mat[2] * m.mat[11] +
			m.mat[4] * m.mat[3] * m.mat[10] +
			m.mat[8] * m.mat[2] * m.mat[7] -
			m.mat[8] * m.mat[3] * m.mat[6];

		out.mat[11] = -m.mat[0] * m.mat[5] * m.mat[11] +
			m.mat[0] * m.mat[7] * m.mat[9] +
			m.mat[4] * m.mat[1] * m.mat[11] -
			m.mat[4] * m.mat[3] * m.mat[9] -
			m.mat[8] * m.mat[1] * m.mat[7] +
			m.mat[8] * m.mat[3] * m.mat[5];

		out.mat[15] = m.mat[0] * m.mat[5] * m.mat[10] -
			m.mat[0] * m.mat[6] * m.mat[9] -
			m.mat[4] * m.mat[1] * m.mat[10] +
			m.mat[4] * m.mat[2] * m.mat[9] +
			m.mat[8] * m.mat[1] * m.mat[6] -
			m.mat[8] * m.mat[2] * m.mat[5];

		det = m.mat[0] * out.mat[0] + m.mat[1] * out.mat[4] + m.mat[2] * out.mat[8] + m.mat[3] * out.mat[12];
		if (det == 0)
			return identity();
		det = 1.0f / det;
		for (int i = 0; i < 16; i++)
			out.mat[i] *= det;

		return out;
	}
}