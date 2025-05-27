#pragma once
#include <string>
#include <sstream>
#include <cmath>
#include <time.h>
#include "Array.h"

namespace m3d {
	static bool random_called = false;
	static const double PI = 3.14159265358979323846;

	struct vec4 {
		float x, y, z, w;
		unsigned int dims[2];

		vec4();
		vec4(const vec4&);
		vec4(float);
		vec4(float, float, float, float);

		float& operator [](int);
		const float& operator [](int) const;
		vec4& operator =(const vec4&);
		vec4 operator +(const vec4&);
		vec4 operator -(const vec4&);
		vec4 operator *(const float);
		vec4 operator /(const float);

		vec4& operator +=(const vec4&);
		vec4& operator -=(const vec4&);
		vec4& operator *=(const float);
		vec4& operator /=(const float);

		vec4 operator +(vec4&&);
		vec4 operator -(vec4&&);
		vec4& operator +=(vec4&&);
		vec4& operator -=(vec4&&);

		vec4 divide(const vec4&);
		vec4 multiply(const vec4&);

		vec4& divide_elem(const vec4&);
		vec4& multiply_elem(const vec4&);

		float dot(const vec4&);
		float mag();
		vec4& normalize();
		vec4& T();

		std::string toString();
		std::string shape();
		const std::string shape() const;
		Array<float> arr();
	};

	struct vec3 {
		float x, y, z;
		unsigned int dims[2];

		vec3();
		vec3(const vec3&);
		vec3(float);
		vec3(float, float, float);

		float& operator [](int);
		const float& operator [](int) const;
		vec3& operator =(const vec3&);
		vec3 operator +(const vec3&);
		vec3 operator -(const vec3&);
		vec3 operator *(const float);
		vec3 operator /(const float);

		vec3& operator +=(const vec3&);
		vec3& operator -=(const vec3&);
		vec3& operator *=(const float);
		vec3& operator /=(const float);
		vec3& operator %=(const float);

		vec3 operator +(vec3&&);
		vec3 operator -(vec3&&);
		vec3& operator +=(vec3&&);
		vec3& operator -=(vec3&&);

		vec3 divide(const vec3&);
		vec3 multiply(const vec3&);

		vec3& divide_elem(const vec3&);
		vec3& multiply_elem(const vec3&);

		float dot(const vec3&);
		float mag();
		vec3& normalize();
		vec3& T();
		vec3 cross(const vec3&);
		vec4 homogenous();

		std::string toString();
		std::string shape();
		const std::string shape() const;
		Array<float> arr();
	};

	struct mat4x4 {
		vec4 mat[4];
		mat4x4();
		mat4x4(float* values);
		mat4x4(float&& initVal);
		mat4x4(vec4, vec4, vec4, vec4);
		mat4x4(const mat4x4&);
		~mat4x4();

		mat4x4 operator +(const mat4x4&);
		mat4x4 operator -(const mat4x4&);
		mat4x4 operator *(const float);
		mat4x4 operator /(const float);

		mat4x4& operator =(const mat4x4&);
		mat4x4& operator +=(const mat4x4&);
		mat4x4& operator -=(const mat4x4&);
		mat4x4& operator *=(const float);
		mat4x4& operator /=(const float);

		bool operator ==(const mat4x4&);
		bool operator !=(const mat4x4&);

		float& operator ()(const int, const int);
		const float& operator ()(const int, const int) const;
		mat4x4 multiply(const mat4x4&);
		vec4 multiply(const vec4&);
		mat4x4 matmul(const mat4x4&);
		mat4x4& transpose();
		std::string toString();
		float* get_buffer();
	};

	float sqrt_(float);
	vec4 random();
	Array<float> random(int size);
	int randint(int, int);
	float degToRad(float deg);
	float radToDeg(float rad);

	mat4x4 Identity();
	mat4x4 translate(const vec3& vec);
	mat4x4 scale(const vec3& vec);
	mat4x4 rotate(const float& x, const float& y, const float& z);
	mat4x4 rotate(const vec3& vec);
	mat4x4 perspective(const float& near, const float& far, const float& aspect, const float& fov);
	mat4x4 lookAt(vec3& eye, vec3& at, vec3& up);
	mat4x4 model(const vec3& trs, const vec3& rot, const vec3& scl);
	mat4x4 mvp(mat4x4& persp, mat4x4& view, mat4x4& model);

	mat4x4 translate(vec3&& vec);
	mat4x4 scale(vec3&& vec);
	mat4x4 rotate(float&& x, float&& y, float&& z);
	mat4x4 rotate(vec3&& vec);
	mat4x4 perspective(float&& near, float&& far, float&& aspect, float&& fov);
	mat4x4 lookAt(vec3&& eye, vec3&& at, vec3&& up);
	mat4x4 model(vec3&& trs, vec3&& rot, vec3&& scl);
	mat4x4 mvp(mat4x4&& persp, mat4x4&& view, mat4x4&& model);
}