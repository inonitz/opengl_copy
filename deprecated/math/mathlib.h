#pragma once
#include <iostream>
#include <sstream>
#include <time.h>
#include "Array.h"

#define ROW_MAJOR    0
#define COLUMN_MAJOR 1	
#define LEFT_HANDED  2
#define RIGHT_HANDED 3

#define CURRENT_VECTOR_ORIENTATION COLUMN_MAJOR
#define CURRENT_COORDINATE_SYSTEM  RIGHT_HANDED
#define max(a, b) a > b ? a : b
/* The "library" uses column-Major ordering as a default, which can be changed.
   This "library" also uses a right-handed coordinate system, intended for use with openGL. 
   I have implemented Right-Handed and Left-Handed coordinate system functions,
   be sure to know what you're doing!
*/


namespace mlib
{
	static const double PI  = 3.14159265358979323846;
	static const float  PIf = 3.141592653f;

	struct vec4
	{
		float x, y, z, w;
		vec4();
		vec4(const float& n);
		vec4(     float&& n);
		vec4(const float& x, const float& y, const float& z, const float& w);
		vec4(	  float&& x,      float&& y,      float&& z,      float&& w);
		vec4(const vec4& v) noexcept;
		vec4(     vec4&& v) noexcept;

		vec4& operator=(const vec4& v) noexcept;
		vec4& operator=(     vec4&& v) noexcept;

		vec4 operator+(const vec4& v);
		vec4 operator-(const vec4& v);
		vec4 operator/(const vec4& v);
		vec4 operator*(const vec4& v);
		vec4 operator*(const float& s);
		vec4 operator/(const float& s);

		vec4 operator+(vec4&& v);
		vec4 operator-(vec4&& v);
		vec4 operator*(vec4&& v);
		vec4 operator/(vec4&& v);
		vec4 operator*(float&& s);
		vec4 operator/(float&& s);

		vec4& operator+=(const vec4& v);
		vec4& operator-=(const vec4& v);
		vec4& operator*=(const vec4& v);
		vec4& operator/=(const vec4& v);
		vec4& operator*=(const float& s);
		vec4& operator/=(const float& s);

		vec4& operator+=(vec4&& v);
		vec4& operator-=(vec4&& v);
		vec4& operator*=(vec4&& v);
		vec4& operator/=(vec4&& v);
		vec4& operator*=(float&& s);
		vec4& operator/=(float&& s);

			  float& operator ()(unsigned int idx);
		const float& operator ()(unsigned int idx) const;

		      float& operator ()(int idx);
		const float& operator ()(int idx) const;

			  float dot(const vec4& v);
			  float dot(     vec4&& v);
		const float dot(const vec4& v) const;
		const float dot(     vec4&& v) const;

		float sum();
		float mag();
		vec4 inverse();
		vec4& normalize();
		std::string str();
	};

	struct vec3
	{
		float x, y, z;
		vec3();
		vec3(const float& n);
		vec3(     float&& n);
		vec3(const float& x, const float& y, const float& z);
		vec3(     float&& x,      float&& y,      float&& z);
		vec3(const vec3& v) noexcept;
		vec3(     vec3&& v) noexcept;

		vec3& operator=(const vec3& v) noexcept;
		vec3& operator=(     vec3&& v) noexcept;

		vec3 operator+(const vec3& v);
		vec3 operator-(const vec3& v);
		vec3 operator*(const vec3& v);
		vec3 operator/(const vec3& v);
		vec3 operator*(const float& s);
		vec3 operator/(const float& s);

		vec3 operator+(vec3&& v);
		vec3 operator-(vec3&& v);
		vec3 operator*(vec3&& v);
		vec3 operator/(vec3&& v);
		vec3 operator*(float&& s);
		vec3 operator/(float&& s);

		vec3& operator+=(const vec3& v);
		vec3& operator-=(const vec3& v);
		vec3& operator*=(const vec3& v);
		vec3& operator/=(const vec3& v);
		vec3& operator*=(const float& s);
		vec3& operator/=(const float& s);


		vec3& operator+=(vec3&& v);
		vec3& operator-=(vec3&& v);
		vec3& operator*=(vec3&& v);
		vec3& operator/=(vec3&& v);
		vec3& operator*=(float&& s);
		vec3& operator/=(float&& s);

		      float& operator ()(unsigned int idx);
		const float& operator ()(unsigned int idx) const;
		      float& operator ()(int idx);
		const float& operator ()(int idx) const;

		vec4  homogenous();
		vec3 inverse();
		vec3  cross(const vec3& v);
		vec3  cross(     vec3&& v);
		float dot(  const vec3& v);
		float dot(	     vec3&& v);
		float sum();
		float mag();
		vec3& normalize();
		std::string str();
	};

	struct vec2
	{
		float x, y;
		vec2();
		vec2(const float& n);
		vec2(     float&& n);
		vec2(const float& x, const float& y);
		vec2(     float&& x,      float&& y);
		vec2(const vec3& v) noexcept;
		vec2(     vec3&& v) noexcept;

		vec2& operator=(const vec2& v) noexcept;
		vec2& operator=(     vec2&& v) noexcept;

		vec2 operator+(const vec2& v);
		vec2 operator-(const vec2& v);
		vec2 operator*(const vec2& v);
		vec2 operator/(const vec2& v);
		vec2 operator*(const float& s);
		vec2 operator/(const float& s);

		vec2 operator+(vec2&& v);
		vec2 operator-(vec2&& v);
		vec2 operator*(vec2&& v);
		vec2 operator/(vec2&& v);
		vec2 operator*(float&& s);
		vec2 operator/(float&& s);

		vec2& operator+=(const vec2& v);
		vec2& operator-=(const vec2& v);
		vec2& operator*=(const vec2& v);
		vec2& operator/=(const vec2& v);
		vec2& operator*=(const float& s);
		vec2& operator/=(const float& s);

		vec2& operator+=(vec2&& v);
		vec2& operator-=(vec2&& v);
		vec2& operator*=(vec2&& v);
		vec2& operator/=(vec2&& v);
		vec2& operator*=(float&& s);
		vec2& operator/=(float&& s);

		      float& operator ()(unsigned int idx);
		const float& operator ()(unsigned int idx) const;
		      float& operator ()(int idx);
		const float& operator ()(int idx) const;

		vec3  homogenous();
		vec2 inverse();
		float dot(const vec2& v);
		float dot(vec2&& v);
		float sum();
		float mag();
		vec2& normalize();
		std::string str();
	};

	struct mat4
	{
		float mat[16];
		mat4(float&& a0,  float&&  a1, float&&  a2,  float&& a3, 
			 float&& a4,  float&&  a5, float&&  a6,  float&& a7,
			 float&& a8,  float&&  a9, float&&  a10, float&& a11,
			 float&& a12, float&& a13, float&&  a14, float&& a15);
		mat4(const float& a0,  const float&  a1, const float&  a2,  const float& a3, 
			 const float& a4,  const float&  a5, const float&  a6,  const float& a7,
			 const float& a8,  const float&  a9, const float&  a10, const float& a11,
			 const float& a12, const float& a13, const float&  a14, const float& a15);
		mat4(float* values);
		mat4();
		mat4(const float& n);
		mat4(float&& n);
		mat4(const mat4& m);
		mat4(mat4&& m);
		mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3); 
		mat4(vec4&& v0, vec4&& v1, vec4&& v2, vec4&& v3);

		mat4& operator=(const mat4& m) noexcept;
		mat4& operator=(     mat4&& m) noexcept;

		mat4 operator+(const mat4& m);
		mat4 operator-(const mat4& m);
		mat4 operator*(const mat4& m);
		mat4 operator/(const mat4& m);
		mat4 operator*(const float& s);
		mat4 operator/(const float& s);

		mat4 operator+(mat4&& m);
		mat4 operator-(mat4&& m);
		mat4 operator*(mat4&& m);
		mat4 operator/(mat4&& m);
		mat4 operator*(float&& s);
		mat4 operator/(float&& s);

		mat4& operator+=(const mat4& m);
		mat4& operator-=(const mat4& m);
		mat4& operator*=(const mat4& m);
		mat4& operator/=(const mat4& m);
		mat4& operator*=(const float&  s);
		mat4& operator/=(const float&  s);

		mat4& operator+=(mat4&& m);
		mat4& operator-=(mat4&& m);
		mat4& operator*=(mat4&& m);
		mat4& operator/=(mat4&& m);
		mat4& operator*=(float&& s);
		mat4& operator/=(float&& s);

			  float& operator ()(const    int i, const    int j);
			  float& operator ()(unsigned int i, unsigned int j);
		const float& operator ()(const    int i, const    int j) const;
		const float& operator ()(unsigned int i, unsigned int j) const;

		vec4 operator()(         int i);
		vec4 operator()(unsigned int i);
			  vec4 column(         int j);
			  vec4 column(unsigned int j);
		const vec4 column(         int j) const;
		const vec4 column(unsigned int j) const;
		vec4 dot(const vec4& v);
		vec4 dot(     vec4&& v);
		const vec4 dot(const vec4& v) const;
		const vec4 dot(	    vec4&& v) const;


		mat4 dot(const mat4& m);
		mat4 dot(	  mat4&& v);
		mat4 matmul(const mat4& m);
		mat4 matmul(     mat4&& m);
		mat4 inverse();
		mat4& T();
		float* buffer();
		std::string str();

	};

	float rad(float deg);
	float deg(float rad);
	float sqrt(float n);

	float        randint(float min, float max);
	vec3         random3();
	vec4         random4();
	Array<float> random(int size);
	mat4         random4x4();

	mat4 identity();
	mat4 scale(      const vec3& v);
	mat4 translate(  const vec3& v);
	mat4 rotate(     const float& x, const float& y, const float& z);
	mat4 rotate(     const vec3& v);
	mat4 perspective(const float& near, const float& far, const float& aspect, const float& fov);
	mat4 lookAt(vec3& eye, vec3& at, vec3& up);

	mat4 scale(      vec3&& v);
	mat4 translate(  vec3&& v);
	mat4 rotate(     float&& x, float&& y, float&& z);
	mat4 rotate(     vec3&& v);
	mat4 perspective(float&& near, float&& far, float&& aspect, float&& fov);
	mat4 lookAt(     vec3&& eye, vec3&& at, vec3&& up);

	// rotation on individual axes are derived in a right hand coordinate system for column-major 
	mat4 rotateX(const float& a);
	mat4 rotateY(const float& a);
	mat4 rotateZ(const float& a);

	mat4 rotateX(float&& a);
	mat4 rotateY(float&& a);
	mat4 rotateZ(float&& a);

	mat4 model(const vec3& trs, const vec3& rot, const vec3& scl);
	mat4 model(     vec3&& trs,      vec3&& rot,      vec3&& scl);

	mat4 mvp( mat4& persp,  mat4& view,  mat4& model);
	mat4 mvp(mat4&& persp, mat4&& view, mat4&& model);
	mat4 inverse(mat4& m);

}