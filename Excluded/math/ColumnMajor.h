#pragma once
#include "mathlib.h"

typedef mlib::mat4 mat4;
typedef mlib::vec4 vec4;
typedef mlib::vec3 vec3;

mat4::mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3)
{
	mat[0] = v0.x; mat[1] = v1.x; mat[2] = v2.x; mat[3] = v3.x;
	mat[4] = v0.y; mat[5] = v1.y; mat[6] = v2.y; mat[7] = v3.y;
	mat[8] = v0.z; mat[9] = v1.z; mat[10] = v2.z; mat[11] = v3.z;
	mat[12] = v0.w; mat[13] = v1.w; mat[14] = v2.w; mat[15] = v3.w;
}

mat4::mat4(vec4&& v0, vec4&& v1, vec4&& v2, vec4&& v3)
{
	mat[0] = v0.x; mat[1] = v1.x; mat[2] = v2.x; mat[3] = v3.x;
	mat[4] = v0.y; mat[5] = v1.y; mat[6] = v2.y; mat[7] = v3.y;
	mat[8] = v0.z; mat[9] = v1.z; mat[10] = v2.z; mat[11] = v3.z;
	mat[12] = v0.w; mat[13] = v1.w; mat[14] = v2.w; mat[15] = v3.w;
}

float& mat4::operator ()(const    int i, const    int j)
{
	if ((i > 3 || i < 0) || (j > 3 || j < 0)) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return mat[i + j * 4];
}
float& mat4::operator ()(unsigned int i, unsigned int j)
{
	if ((i > 3 || i < 0) || (j > 3 || j < 0)) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return mat[i + j * 4];
}
const float& mat4::operator ()(const    int i, const    int j) const
{
	if ((i > 3 || i < 0) || (j > 3 || j < 0)) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return mat[i + j * 4];
}
const float& mat4::operator ()(unsigned int i, unsigned int j) const
{
	if ((i > 3 || i < 0) || (j > 3 || j < 0)) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return mat[i + j * 4];
}

vec4 mat4::operator()(int idx)
{
	if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return vec4(mat[4 * idx], mat[4 * idx + 1], mat[4 * idx + 2], mat[4 * idx + 3]);
}
vec4 mat4::operator()(unsigned int idx)
{
	if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return vec4(mat[4 * idx], mat[4 * idx + 1], mat[4 * idx + 2], mat[4 * idx + 3]);
}
vec4 mat4::column(int idx)
{
	if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return vec4(mat[0 + idx], mat[4 + idx], mat[8 + idx], mat[12 + idx]);
}
vec4 mat4::column(unsigned int idx)
{
	if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return vec4(mat[0 + idx], mat[4 + idx], mat[8 + idx], mat[12 + idx]);
}
const vec4 mat4::column(int idx) const
{
	if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return vec4(mat[0 + idx], mat[4 + idx], mat[8 + idx], mat[12 + idx]);
}
const vec4 mat4::column(unsigned int idx) const
{
	if (idx > 3 || idx < 0) { std::cout << "INVALID INDEX ERROR -> MAT4"; exit(-1); }
	return vec4(mat[0 + idx], mat[4 + idx], mat[8 + idx], mat[12 + idx]);
}

vec4 mat4::dot(const vec4& v)
{
	return vec4(
		v.x * mat[0] + v.y * mat[1] + v.z * mat[2] + v.w * mat[3],
		v.x * mat[4] + v.y * mat[5] + v.z * mat[6] + v.w * mat[7],
		v.x * mat[8] + v.y * mat[9] + v.z * mat[10] + v.w * mat[11],
		v.x * mat[12] + v.y * mat[13] + v.z * mat[14] + v.w * mat[15]);
}
vec4 mat4::dot(vec4&& v)
{
	return vec4(
		v.x * mat[0] + v.y * mat[1] + v.z * mat[2] + v.w * mat[3],
		v.x * mat[4] + v.y * mat[5] + v.z * mat[6] + v.w * mat[7],
		v.x * mat[8] + v.y * mat[9] + v.z * mat[10] + v.w * mat[11],
		v.x * mat[12] + v.y * mat[13] + v.z * mat[14] + v.w * mat[15]);
}
const vec4 mat4::dot(const vec4& v) const
{
	return vec4(
		v.x * mat[0] + v.y * mat[1] + v.z * mat[2] + v.w * mat[3],
		v.x * mat[4] + v.y * mat[5] + v.z * mat[6] + v.w * mat[7],
		v.x * mat[8] + v.y * mat[9] + v.z * mat[10] + v.w * mat[11],
		v.x * mat[12] + v.y * mat[13] + v.z * mat[14] + v.w * mat[15]);
}
const vec4 mat4::dot(vec4&& v) const
{
	return vec4(
		v.x * mat[0] + v.y * mat[1] + v.z * mat[2] + v.w * mat[3],
		v.x * mat[4] + v.y * mat[5] + v.z * mat[6] + v.w * mat[7],
		v.x * mat[8] + v.y * mat[9] + v.z * mat[10] + v.w * mat[11],
		v.x * mat[12] + v.y * mat[13] + v.z * mat[14] + v.w * mat[15]);
}

mat4 mlib::translate(const vec3& v)
{
	return mat4(
		1.0f, 0.0f, 0.0f, v.x,
		0.0f, 1.0f, 0.0f, v.y,
		0.0f, 0.0f, 1.0f, v.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}
mat4 mlib::translate(vec3&& v)
{
	return mat4(
		1.0f, 0.0f, 0.0f, v.x,
		0.0f, 1.0f, 0.0f, v.y,
		0.0f, 0.0f, 1.0f, v.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

mat4 mlib::model(const vec3& trs, const vec3& rot, const vec3& scl) { return scale(scl).matmul(rotate(rot.x, rot.y, rot.z).matmul(translate(trs))); }
mat4 mlib::model(vec3&& trs, vec3&& rot, vec3&& scl) { return scale(scl).matmul(rotate(rot.x, rot.y, rot.z).matmul(translate(trs))); }

mat4 mlib::mvp(mat4& persp, mat4& view, mat4& model) { return persp.matmul(view.matmul(model)); }
mat4 mlib::mvp(mat4&& persp, mat4&& view, mat4&& model) { return persp.matmul(view.matmul(model)); }

#if CURRENT_COORDINATE_SYSTEM == RIGHT_HANDED
mat4 mlib::lookAt(vec3& eye, vec3& at, vec3& up) {
	vec3 z = (eye - at).normalize();
	vec3 x = (up.cross(z)).normalize();
	vec3 y = z.cross(x);
	return mat4(
		x.x, x.y, x.z, -x.dot(eye),
		y.x, y.y, y.z, -y.dot(eye),
		z.x, z.y, z.z, -z.dot(eye),
		0.0f, 0.0f, 0.0f, 1.0f
	);
}
mat4 mlib::lookAt(vec3&& eye, vec3&& at, vec3&& up) {
	vec3 z = (eye - at).normalize();
	vec3 x = (up.cross(z)).normalize();
	vec3 y = z.cross(x);
	return mat4(
		x.x, x.y, x.z, -x.dot(eye),
		y.x, y.y, y.z, -y.dot(eye),
		z.x, z.y, z.z, -z.dot(eye),
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

#else
mat4 mlib::lookAt(vec3& eye, vec3& at, vec3& up) {
	vec4 z = normal(at - eye)
		vec4 x = normal(cross(up, z))
		vec4 y = cross(z, x)
		return mat4(
			x.x, x.y, x.z, -x.dot(eye),
			y.x, y.y, y.z, -y.dot(eye),
			z.x, z.y, z.z, -z.dot(eye),
			0.0f, 0.0f, 0.0f, 1.0f
		);
}
mat4 mlib::lookAt(vec3&& eye, vec3&& at, vec3&& up) {
	vec4 z = normal(at - eye)
		vec4 x = normal(cross(up, z))
		vec4 y = cross(z, x)
		return mat4(
			x.x, x.y, x.z, -x.dot(eye),
			y.x, y.y, y.z, -y.dot(eye),
			z.x, z.y, z.z, -z.dot(eye),
			0.0f, 0.0f, 0.0f, 1.0f
		);
}
#endif
