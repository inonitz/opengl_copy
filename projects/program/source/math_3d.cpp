#include "math_3d.h"
#include <iostream>

namespace m3d {
	vec3::vec3() : x(0), y(0), z(0) { this->dims[0] = 1; this->dims[1] = 3; }
	vec3::vec3(float f) : x(f), y(f), z(f) { this->dims[0] = 1; this->dims[1] = 3; }
	vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) { this->dims[0] = 1; this->dims[1] = 3; }
	vec3::vec3(const vec3& vec) : x(vec.x), y(vec.y), z(vec.z) { this->dims[0] = vec.dims[0]; this->dims[1] = vec.dims[1]; }
	float& vec3::operator [](int index) {
		switch (index)
		{
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		}
		throw std::invalid_argument("index must be in range [0, 3]. index given: " + std::to_string(index) + "\n");
	}

	const float& vec3::operator [](int index) const
	{
		switch (index)
		{
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		}
		throw std::invalid_argument("index must be in range [0, 3]. index given: " + std::to_string(index) + "\n");
	}

	vec3& vec3::operator =(const vec3& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		memcpy(this->dims, vec.dims, sizeof(unsigned int[2]));
		return *this;
	}
	vec3 vec3::operator +(const vec3& vec) { return vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z); }
	vec3 vec3::operator -(const vec3& vec) { return vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z); }
	vec3 vec3::operator *(const float scalar) { return vec3(this->x * scalar, this->y * scalar, this->z * scalar); }
	vec3 vec3::operator /(const float scalar) { return vec3(this->x / scalar, this->y / scalar, this->z / scalar); }

	vec3& vec3::operator +=(const vec3& vec) { this->x += vec.x; this->y += vec.y; this->z += vec.z; return *this; }
	vec3& vec3::operator -=(const vec3& vec) { this->x -= vec.x; this->y -= vec.y; this->z -= vec.z; return *this; }
	vec3& vec3::operator *=(const float scalar) { this->x *= scalar; this->y *= scalar; this->z *= scalar; return *this; }
	vec3& vec3::operator /=(const float scalar) { this->x /= scalar; this->y /= scalar; this->z /= scalar; return *this; }

	vec3& vec3::operator%=(const float m) { x = std::fmod(x, m); y = std::fmod(y, m); y = std::fmod(z, m);   return *this; }

	vec3 vec3::operator+(vec3&& vec) { return vec3(x + vec.x, y + vec.y, z + vec.z); }
	vec3 vec3::operator-(vec3&& vec) { return vec3(x - vec.x, y - vec.y, z - vec.z); }
	vec3& vec3::operator+=(vec3&& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
	vec3& vec3::operator-=(vec3&& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }

	// Need to add (4, 1) == (1, 4) for multiplication! (shape checking)
	vec3 vec3::divide(const vec3& vec) { return vec3(this->x / vec.x, this->y / vec.y, this->z / vec.z); }
	vec3 vec3::multiply(const vec3& vec) { return vec3(this->x * vec.x, this->y * vec.y, this->z * vec.z); }
	vec3& vec3::divide_elem(const vec3& vec) { this->x /= vec.x; this->y /= vec.y; this->z /= vec.z; return *this; }
	vec3& vec3::multiply_elem(const vec3& vec) { this->x *= vec.x; this->y *= vec.y; this->z *= vec.z; return *this; }
	////////////////////////////////////////////////////////////////////

	float vec3::dot(const vec3& vec) {
		if (vec.dims[0] == this->dims[1] && vec.dims[1] == this->dims[0])
			return this->x * vec.x + this->y * vec.y + this->z * vec.z;
		std::string invalid_arg = "shapes " + this->shape() + " and " + vec.shape() + " not aligned: ";
		invalid_arg += std::to_string(this->dims[1]) + " (dim 1) != " + std::to_string(vec.dims[0]) + " (dim 0)";
		throw std::invalid_argument(invalid_arg);
	}

	float vec3::mag() { return sqrt_(x * x + y * y + z * z); }
	vec3& vec3::normalize() { *this /= this->mag(); return *this; }
	vec3& vec3::T() { unsigned int temp = this->dims[0]; this->dims[0] = this->dims[1]; this->dims[1] = temp; return *this; }
	vec3 vec3::cross(const vec3& vec) { return vec3(this->y * vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x * vec.y - this->y * vec.x); }
	vec4 vec3::homogenous() { return vec4(this->x, this->y, this->z, 1); }

	std::string vec3::toString() { return "[" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + "]\n"; }
	std::string vec3::shape() { return "(" + std::to_string(this->dims[0]) + "," + std::to_string(this->dims[1]) + ")"; }
	const std::string vec3::shape() const { return "(" + std::to_string(this->dims[0]) + "," + std::to_string(this->dims[1]) + ")"; }
	Array<float> vec3::arr() { return { this->x, this->y, this->z }; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//vec4
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	vec4::vec4() : x(0), y(0), z(0), w(0) { this->dims[0] = 1; this->dims[1] = 4; }
	vec4::vec4(float f) : x(f), y(f), z(f), w(f) { this->dims[0] = 1; this->dims[1] = 4; }
	vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { this->dims[0] = 1; this->dims[1] = 4; }
	vec4::vec4(const vec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) { this->dims[0] = vec.dims[0]; this->dims[1] = vec.dims[1]; }
	float& vec4::operator [](int index) {
		switch (index)
		{
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		case 3:
			return this->w;
		}
		throw std::invalid_argument("index must be in range [0, 3]. index given: " + std::to_string(index) + "\n");
	}

	const float& vec4::operator [](int index) const
	{
		switch (index)
		{
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		case 3:
			return this->w;
		}
		throw std::invalid_argument("index must be in range [0, 3]. index given: " + std::to_string(index) + "\n");
	}

	vec4& vec4::operator =(const vec4& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = vec.w;
		memcpy(this->dims, vec.dims, sizeof(unsigned int[2]));
		return *this;
	}
	vec4 vec4::operator +(const vec4& vec) { return vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w); }
	vec4 vec4::operator -(const vec4& vec) { return vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w); }
	vec4 vec4::operator *(const float scalar) { return vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar); }
	vec4 vec4::operator /(const float scalar) { return vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar); }

	vec4& vec4::operator +=(const vec4& vec) { this->x += vec.x; this->y += vec.y; this->z += vec.z; this->w += vec.w; return *this; }
	vec4& vec4::operator -=(const vec4& vec) { this->x -= vec.x; this->y -= vec.y; this->z -= vec.z; this->w -= vec.w;return *this; }
	vec4& vec4::operator *=(const float scalar) { this->x *= scalar; this->y *= scalar; this->z *= scalar; this->w *= scalar; return *this; }
	vec4& vec4::operator /=(const float scalar) { this->x /= scalar; this->y /= scalar; this->z /= scalar; this->w /= scalar; return *this; }

	vec4 vec4::operator+(vec4&& vec) { return vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	vec4 vec4::operator-(vec4&& vec) { return vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	vec4& vec4::operator+=(vec4&& vec) { x += vec.x; y += vec.y; z += vec.z; w += vec.w; return *this; }
	vec4& vec4::operator-=(vec4&& vec) { x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w; return *this; }

	// Need to add (4, 1) == (1, 4) for multiplication! (shape checking)
	vec4 vec4::divide(const vec4& vec) { return vec4(this->x / vec.x, this->y / vec.y, this->z / vec.z, this->w / vec.w); }
	vec4 vec4::multiply(const vec4& vec) { return vec4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w); }
	vec4& vec4::divide_elem(const vec4& vec) { this->x /= vec.x; this->y /= vec.y; this->z /= vec.z; this->w /= vec.w; return *this; }
	vec4& vec4::multiply_elem(const vec4& vec) { this->x *= vec.x; this->y *= vec.y; this->z *= vec.z; this->w *= vec.w; return *this; }
	////////////////////////////////////////////////////////////////////

	float vec4::dot(const vec4& vec) {
		if (vec.dims[0] == this->dims[1] && vec.dims[1] == this->dims[0])
			return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w;

		std::string invalid_arg = "shapes " + this->shape() + " and " + vec.shape() + " not aligned: ";
		invalid_arg += std::to_string(this->dims[1]) + " (dim 1) != " + std::to_string(vec.dims[0]) + " (dim 0)";
		throw std::invalid_argument(invalid_arg);
	}

	float vec4::mag() { return sqrt_(x * x + y * y + z * z + w * w); }
	vec4& vec4::normalize() { *this /= this->mag(); return *this; }
	vec4& vec4::T() { unsigned int temp = this->dims[0]; this->dims[0] = this->dims[1]; this->dims[1] = temp; return *this; }
	std::string vec4::toString() { return "[" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ", " + std::to_string(this->w) + "]\n"; }
	std::string vec4::shape() { return "(" + std::to_string(this->dims[0]) + "," + std::to_string(this->dims[1]) + ")"; }
	const std::string vec4::shape() const { return "(" + std::to_string(this->dims[0]) + "," + std::to_string(this->dims[1]) + ")"; }
	Array<float> vec4::arr() { return { this->x, this->y, this->z, this->w }; }

	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	///////////Matrix Operations///////////

	mat4x4::mat4x4() { for (int i = 0; i < 4; i++) this->mat[i] = vec4(); }
	mat4x4::mat4x4(float* values) { memcpy(this->mat, values, sizeof(float) * 16); }
	mat4x4::mat4x4(float&& initVal) { for (int i = 0; i < 4; i++) { this->mat[i] = vec4(initVal); } }
	mat4x4::mat4x4(vec4 r0, vec4 r1, vec4 r2, vec4 r3) { this->mat[0] = r0; this->mat[1] = r1; this->mat[2] = r2; this->mat[3] = r3; }
	mat4x4::mat4x4(const mat4x4& matrix) { memcpy(this->mat, matrix.mat, sizeof(mat4x4)); }
	mat4x4::~mat4x4() {}

	mat4x4 mat4x4::operator +(const mat4x4& matrix) { mat4x4 result = mat4x4(); for (int i = 0; i < 4; i++) { result.mat[i] = this->mat[i] + matrix.mat[i]; } return result; }
	mat4x4 mat4x4::operator -(const mat4x4& matrix) { mat4x4 result = mat4x4(); for (int i = 0; i < 4; i++) { result.mat[i] = this->mat[i] - matrix.mat[i]; } return result; }
	mat4x4 mat4x4::operator *(const float scalar) { mat4x4 result = mat4x4(); for (int i = 0; i < 4; i++) { result.mat[i] = this->mat[i] * scalar; } return result; }
	mat4x4 mat4x4::operator /(const float scalar) { mat4x4 result = mat4x4(); for (int i = 0; i < 4; i++) { result.mat[i] = this->mat[i] / scalar; } return result; }
	mat4x4& mat4x4::operator =(const mat4x4& matrix) { for (int i = 0; i < 4; i++) { this->mat[i] = matrix.mat[i]; } return *this; }
	mat4x4& mat4x4::operator +=(const mat4x4& matrix) { for (int i = 0; i < 4; i++) { this->mat[i] += matrix.mat[i]; } return *this; }
	mat4x4& mat4x4::operator -=(const mat4x4& matrix) { for (int i = 0; i < 4; i++) { this->mat[i] -= matrix.mat[i]; } return *this; }
	mat4x4& mat4x4::operator *=(const float scalar) { for (int i = 0; i < 4; i++) { this->mat[i] *= scalar; } return *this; }
	mat4x4& mat4x4::operator /=(const float scalar) { for (int i = 0; i < 4; i++) { this->mat[i] /= scalar; } return *this; }

	bool mat4x4::operator==(const mat4x4& m)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (this->operator()(i, j) != m(i, j))
					return false;
		return true;
	}

	bool mat4x4::operator!=(const mat4x4& m) { return !this->operator==(m); }

	float& mat4x4::operator ()(const int row, const int col) { return this->mat[row][col]; }
	const float& mat4x4::operator ()(const int row, const int col) const { return this->mat[row][col]; }

	// Elementwise multiplication
	mat4x4 mat4x4::multiply(const mat4x4& matrix) {
		mat4x4 result = mat4x4();
		for (int i = 0; i < 4; i++)
		{
			result.mat[i] = this->mat[i].multiply(matrix.mat[i]);
		}
		return result;
	}

	vec4 mat4x4::multiply(const vec4& vec) {
		vec4 result = vec4();
		if (vec.dims[0] == 4 && vec.dims[1] == 1)
		{
			for (int i = 0; i < 4; i++) { result[i] = this->mat[i].dot(vec); }
		}
		else
		{
			for (int i = 0; i < 4; i++) {
				vec4 column = vec4(this->mat[0][i], this->mat[1][i], this->mat[2][i], this->mat[3][i]).T();
				result[i] = column.dot(vec);
			}
		}
		return result;
	}

	mat4x4 mat4x4::matmul(const mat4x4& matrix)
	{
		mat4x4 result = mat4x4();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				vec4 column = vec4(matrix(0, j), matrix(1, j), matrix(2, j), matrix(3, j)).T();
				result(i, j) = this->mat[i].dot(column);
			}
		}
		return result;
	}

	mat4x4& mat4x4::transpose() { mat4x4 temp = mat4x4(*this); for (int i = 0; i < 4; i++) { for (int j = 0; j < 4; j++) { this->mat[i][j] = temp(j, i); } } return *this; }
	std::string mat4x4::toString() { std::string info = "\n"; for (int i = 0; i < 4; i++) { info += this->mat[i].toString(); } return info + "\n"; }

	float* mat4x4::get_buffer()
	{
		return &this->mat[0][0];
	}

	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	///////////General functions///////////
	float degToRad(float deg)
	{
		return (float)PI * deg / 180;
	}

	float radToDeg(float rad)
	{
		return 180.0f * rad / (float)PI;
	}

	float sqrt_(float n)
	{
		float lo = 0, hi = n, mid;
		for (int i = 0; i < 1000; i++)
		{
			mid = (lo + hi) / 2;
			if (mid * mid == n) return mid;
			if (mid * mid > n) hi = mid;
			else lo = mid;
		}
		return mid;
	}

	vec4 random()
	{
		if (!random_called)
			srand((unsigned)time(NULL));
		float ar[4];
		for (int i = 1; i < 4; i++)
			ar[i] = (float)rand() / (float)RAND_MAX;
		return vec4(ar[0], ar[1], ar[2], ar[3]);
	}

	int randint(int min, int max) {
		if (!random_called)
			srand((unsigned int)time(NULL));
		return min + rand() % (max - min + 1);
	}

	Array<float> random(int size) {
		if (!random_called)
			srand((unsigned int)time(NULL));
		Array<float> a(size, true);
		for (int i = 0; i < size; i++)
		{
			a.push_back((float)rand() / (float)RAND_MAX);
		}
		return a;
	}

	mat4x4 Identity() {
		mat4x4 result = mat4x4();
		result(0, 0) = 1;
		result(1, 1) = 1;
		result(2, 2) = 1;
		result(3, 3) = 1;
		return result;
	}

	// matrix operations as Column vector
	mat4x4 translate(const vec3& vec) {
		mat4x4 result = Identity();
		result(0, 3) = vec[0];
		result(1, 3) = vec[1];
		result(2, 3) = vec[2];
		return result;
	}

	mat4x4 scale(const vec3& vec) {
		mat4x4 result = Identity();
		result(0, 0) = vec[0];
		result(1, 1) = vec[1];
		result(2, 2) = vec[2];
		return result;
	}

	mat4x4 rotate(const float& x, const float& y, const float& z) {
		return mat4x4(
			vec4(cos(z) * sin(y), cos(z) * sin(y) * sin(x) - sin(z) * cos(x), cos(z) * sin(y) * cos(x) + sin(z) * sin(x), 0),
			vec4(sin(z) * cos(y), sin(z) * sin(y) * sin(x) + cos(z) * cos(x), sin(z) * sin(y) * cos(z) - cos(z) * sin(x), 0),
			vec4(-sin(y), cos(y) * sin(x), cos(y) * cos(x), 0),
			vec4(0, 0, 0, 1)
		);
	}
	mat4x4 rotate(const vec3& vec)
	{
		return mat4x4(
			vec4(cos(vec.z) * sin(vec.y), cos(vec.z) * sin(vec.y) * sin(vec.x) - sin(vec.z) * cos(vec.x), cos(vec.z) * sin(vec.y) * cos(vec.x) + sin(vec.z) * sin(vec.x), 0),
			vec4(sin(vec.z) * cos(vec.y), sin(vec.z) * sin(vec.y) * sin(vec.x) + cos(vec.z) * cos(vec.x), sin(vec.z) * sin(vec.y) * cos(vec.z) - cos(vec.z) * sin(vec.x), 0),
			vec4(-sin(vec.y), cos(vec.y) * sin(vec.x), cos(vec.y) * cos(vec.x), 0),
			vec4(0, 0, 0, 1)
		);
	}

	mat4x4 perspective(const float& near, const float& far, const float& aspect, const float& fov) {
		mat4x4 result = Identity();
		result(0, 0) = 1 / (aspect * tan(fov / 2));
		result(1, 1) = 1 / tan(fov / 2);
		result(2, 2) = -(near + far) / (far - near);
		result(2, 3) = -(2 * far * near) / (far - near);
		result(3, 2) = -1;
		result(3, 3) = 0;
		return result;
	}

	mat4x4 lookAt(vec3& eye, vec3& at, vec3& up) {
		vec3 z = (eye - at).normalize();
		vec3 x = (up.cross(z)).normalize();
		vec3 y = z.cross(x);
		vec3 eye_ = eye * -1;
		return mat4x4(
			vec4(x.x, x.y, x.z, 0),
			vec4(y.x, y.y, y.z, 0),
			vec4(z.x, z.y, z.z, 0),
			vec4(0, 0, 0, 1)).matmul(translate(eye_)
			);
	}

	mat4x4 model(const vec3& trs, const vec3& rot, const vec3& scl) { return translate(trs).matmul(rotate(rot.x, rot.y, rot.z).matmul(scale(scl))); }
	mat4x4 mvp(mat4x4& persp, mat4x4& view, mat4x4& model) { return persp.matmul(view.matmul(model)); }

	mat4x4 translate(vec3&& vec)
	{
		mat4x4 result = Identity();
		result(0, 3) = vec[0];
		result(1, 3) = vec[1];
		result(2, 3) = vec[2];
		return result;
	}
	mat4x4 scale(vec3&& vec)
	{
		mat4x4 result = Identity();
		result(0, 0) = vec[0];
		result(1, 1) = vec[1];
		result(2, 2) = vec[2];
		return result;
	}
	mat4x4 rotate(float&& x, float&& y, float&& z)
	{
		return mat4x4(
			vec4(cos(z) * sin(y), cos(z) * sin(y) * sin(x) - sin(z) * cos(x), cos(z) * sin(y) * cos(x) + sin(z) * sin(x), 0),
			vec4(sin(z) * cos(y), sin(z) * sin(y) * sin(x) + cos(z) * cos(x), sin(z) * sin(y) * cos(z) - cos(z) * sin(x), 0),
			vec4(-sin(y), cos(y) * sin(x), cos(y) * cos(x), 0),
			vec4(0, 0, 0, 1)
		);
	}
	mat4x4 rotate(vec3&& vec)
	{
		return mat4x4(
			vec4(cos(vec.z) * sin(vec.y), cos(vec.z) * sin(vec.y) * sin(vec.x) - sin(vec.z) * cos(vec.x), cos(vec.z) * sin(vec.y) * cos(vec.x) + sin(vec.z) * sin(vec.x), 0),
			vec4(sin(vec.z) * cos(vec.y), sin(vec.z) * sin(vec.y) * sin(vec.x) + cos(vec.z) * cos(vec.x), sin(vec.z) * sin(vec.y) * cos(vec.z) - cos(vec.z) * sin(vec.x), 0),
			vec4(-sin(vec.y), cos(vec.y) * sin(vec.x), cos(vec.y) * cos(vec.x), 0),
			vec4(0, 0, 0, 1)
		);
	}
	mat4x4 perspective(float&& near, float&& far, float&& aspect, float&& fov)
	{
		mat4x4 result = Identity();
		result(0, 0) = 1 / (aspect * tan(fov / 2));
		result(1, 1) = 1 / tan(fov / 2);
		result(2, 2) = -(near + far) / (far - near);
		result(2, 3) = -(2 * far * near) / (far - near);
		result(3, 2) = -1;
		result(3, 3) = 0;
		return result;
	}
	mat4x4 lookAt(vec3&& eye, vec3&& at, vec3&& up)
	{
		vec3 z = (eye - at).normalize();
		vec3 x = (up.cross(z)).normalize();
		vec3 y = z.cross(x);
		vec3 eye_ = eye * -1;
		mat4x4 result =
			mat4x4
			(
				vec4(x.x, x.y, x.z, 0),
				vec4(y.x, y.y, y.z, 0),
				vec4(z.x, z.y, z.z, 0),
				vec4(0, 0, 0, 1)
			);

		return result.matmul(translate(eye_));
	}
	mat4x4 model(vec3&& trs, vec3&& rot, vec3&& scl) { return translate(trs).matmul(rotate(rot.x, rot.y, rot.z).matmul(scale(scl))); }
	mat4x4 mvp(mat4x4&& persp, mat4x4&& view, mat4x4&& model) { return persp.matmul(view.matmul(model)); }
}