#include "m_simd.h"


/* _mm_load -> to register
   _mm_store -> in memory
   memory -> register
   register -> memory
*/
/*
	==========================================================================
						 IMPLEMENTATION OF VECTOR4 STRUCT
	==========================================================================
*/

float& vec4::operator[](const unsigned int& idx)
{
	ON_CONDITION_DO(idx > 3, ERR_EXIT("INDEX MUST BE IN RANGE [0, 3]"));
	return b[idx];
}
const float& vec4::operator[](const unsigned int& idx) const
{
	ON_CONDITION_DO(idx > 3, ERR_EXIT("INDEX MUST BE IN RANGE [0, 3]"));
	return b[idx];
}

float vec4::mag()			   
{
	vec4 dot{ _mm_dp_ps(m, m, 0b11110001) };
	vec4 inv_dot{_mm_rsqrt_ss(dot.m)}; 
	return dot.b[0] * inv_dot.b[0];
}

float vec4::sum()				
{ 
	return (x + y) + (z + w);
}

float vec4::dot(const vec4& v) const
{
	vec4 result{_mm_dp_ps(m, v.m, 0b11110001)};
	return result.b[0];
}

void vec4::normalize()		   
{
	float tmpmag = mag();
	__m128 tmp = _mm_load_ss(&tmpmag);

	m = _mm_mul_ps(m, _mm_rsqrt_ps(tmp) );
	return;
}

float* vec4::get_c() { return &b[0]; }
void   vec4::print() { printf("[%8.4f, %8.4f, %8.4f, %8.4f]\n", b[0], b[1], b[2], b[3]); }

/*
	============================================================================
						 IMPLEMENTATION OF MATRIX4x4 STRUCT
	============================================================================
*/

mat4::mat4()
{
	v[0] = _mm256_setzero_ps();
	v[1] = _mm256_setzero_ps();
}
mat4::mat4(const float&  d)
{
	v[0] = _mm256_set1_ps(d);
	v[1] = _mm256_set1_ps(d);

}
mat4::mat4(
	const float&  a0,  const float&  a1, const float&  a2,  const float&  a3,
	const float&  a4,  const float&  a5, const float&  a6,  const float&  a7,
	const float&  a8,  const float&  a9, const float&  a10, const float&  a11,
	const float&  a12, const float& a13, const float&  a14, const float&  a15
)
{
	v[0] = _mm256_set_ps(a13, a9,  a5, a1, a12, a8,  a4, a0);
	v[1] = _mm256_set_ps(a15, a11, a7, a3, a14, a10, a6, a2);
}
mat4::mat4(float buffer[16])
{
	v[0] = _mm256_loadu_ps(&buffer[0]);
	v[0] = _mm256_loadu_ps(&buffer[7]);
}
mat4::mat4(const __m256&  a, const __m256&  b)
{
	_mm256_store_ps((float*)&v[0], a);
	_mm256_store_ps((float*)&v[1], b);
}
mat4::mat4(const   mat4&  m) noexcept
{
	_mm256_store_ps((float*)&v[0], m.v[0]);
	_mm256_store_ps((float*)&v[1], m.v[1]);
}
mat4& mat4::operator=(const mat4&  m) noexcept
{
	_mm256_store_ps((float*)&v[0], m.v[0]);
	_mm256_store_ps((float*)&v[1], m.v[1]);
	return *this;
}
void mat4::set(
	const float&  a0,  const float&  a1, const float&  a2,  const float&  a3,
	const float&  a4,  const float&  a5, const float&  a6,  const float&  a7,
	const float&  a8,  const float&  a9, const float&  a10, const float&  a11,
	const float&  a12, const float& a13, const float&  a14, const float&  a15)
{
	v[0] = _mm256_set_ps(a13, a9,  a5, a1, a12, a8,  a4, a0);
	v[1] = _mm256_set_ps(a15, a11, a7, a3, a14, a10, a6, a2);
}

// use [] for column access.
vec4& mat4::operator[](const unsigned int& idx)
{
	ON_CONDITION_DO(idx > 3, ERR_EXIT("INDEX MUST BE IN RANGE [0, 3]"));
	return c[idx];
}
const vec4& mat4::operator[](const unsigned int& idx) const
{
	ON_CONDITION_DO(idx > 3, ERR_EXIT("INDEX MUST BE IN RANGE [0, 3]"));
	return c[idx];
}
// use () for row access.
vec4 mat4::operator()(const unsigned int& idx)
{
	ON_CONDITION_DO(idx > 3, ERR_EXIT("INDEX MUST BE IN RANGE [0, 3]"));
	return vec4(b[idx], b[idx + 4], b[idx + 8], b[idx + 12]);
}
const vec4 mat4::operator()(const unsigned int& idx) const
{
	ON_CONDITION_DO(idx > 3, ERR_EXIT("INDEX MUST BE IN RANGE [0, 3]"));
	return vec4(b[idx], b[idx + 4], b[idx + 8], b[idx + 12]);
}

mat4& mat4::operator+=(const mat4&  m)
{
	v[0] = _mm256_add_ps(v[0], m.v[0]);
	v[1] = _mm256_add_ps(v[1], m.v[1]);
	return *this;
}
mat4& mat4::operator-=(const mat4&  m)
{
	v[0] = _mm256_sub_ps(v[0], m.v[0]);
	v[1] = _mm256_sub_ps(v[1], m.v[1]);
	return *this;
}
mat4& mat4::operator*=(const mat4&  m)
{
	v[0] = _mm256_mul_ps(v[0], m.v[0]);
	v[1] = _mm256_mul_ps(v[1], m.v[1]);
	return *this;
}
mat4& mat4::operator/=(const mat4&  m)
{	 
	v[0] = _mm256_div_ps(v[0], m.v[0]);
	v[1] = _mm256_div_ps(v[1], m.v[1]);
	return *this;
}
mat4& mat4::operator*(const mat4& m)
{
	mat4 tmp(*this);
	Matrix4Vector4Multiply(&tmp, &m.c[1], &c[0]);
	Matrix4Vector4Multiply(&tmp, &m.c[1], &c[1]);
	Matrix4Vector4Multiply(&tmp, &m.c[1], &c[2]);
	Matrix4Vector4Multiply(&tmp, &m.c[1], &c[3]);
	return *this;
}
const mat4& mat4::operator*(const mat4& m) const
{
	mat4 tmp(*this);
	Matrix4Vector4Multiply(&tmp, &m.c[0], (vec4*)&this->x0);
	Matrix4Vector4Multiply(&tmp, &m.c[1], (vec4*)&this->y0);
	Matrix4Vector4Multiply(&tmp, &m.c[2], (vec4*)&this->z0);
	Matrix4Vector4Multiply(&tmp, &m.c[3], (vec4*)&this->w0);
	return *this;
}
vec4  mat4::operator* (const vec4&  m)
{
	return vec4(
		m.dot(operator()(0)),
		m.dot(operator()(1)),
		m.dot(operator()(2)),
		m.dot(operator()(3))
	);
}
const vec4 mat4::operator*(const vec4& m) const
{
	return vec4(
		m.dot(operator()(0)),
		m.dot(operator()(1)),
		m.dot(operator()(2)),
		m.dot(operator()(3))
	);
}


void  mat4::inverse(mat4* const& address)
{
	// credit: Eric Zhang, https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html

	__m128 t0 = _mm_movelh_ps(ci[0], ci[1]);
	__m128 t1 = _mm_movehl_ps(ci[0], ci[1]);
	address->ci[0] = _mm_shuffle_ps(t0, ci[2], mask(0, 2, 0, 3));
	address->ci[1] = _mm_shuffle_ps(t0, ci[2], mask(1, 3, 1, 3));
	address->ci[2] = _mm_shuffle_ps(t1, ci[2], mask(0, 2, 2, 3));

	address->ci[3] = _mm_mul_ps(address->ci[0], _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(ci[3]), digit_mask(0))));
	address->ci[3] = _mm_add_ps(address->ci[3], _mm_mul_ps(address->ci[1], _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(ci[3]), digit_mask(0)))));
	address->ci[3] = _mm_add_ps(address->ci[3], _mm_mul_ps(address->ci[2], _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(ci[3]), digit_mask(0)))));
	address->ci[3] = _mm_sub_ps(_mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f), address->ci[3]);
}
mat4& mat4::transpose() 
{ 
	_MM_TRANSPOSE4_PS(ci[0], ci[1], ci[2], ci[3]); 
	return *this;
}
mat4& mat4::identity()
{
	v[0] = _mm256_set_ps(1, 0, 0, 0, 0, 1, 0, 0);
	v[1] = _mm256_set_ps(0, 0, 1, 0, 0, 0, 0, 1);
	return *this;
}
mat4& mat4::zeros()
{
	v[0] = _mm256_setzero_ps();
	v[1] = _mm256_setzero_ps();
	return *this;
}
float* mat4::get_c () { return &b[0]; }
void mat4::set_zero()
{
	v[0] = _mm256_setzero_ps();
	v[1] = _mm256_setzero_ps();
}
void mat4::set_identity()
{
	v[0] = _mm256_set_ps(1, 0, 0, 0,  0, 1, 0, 0);
	v[1] = _mm256_set_ps(0, 0, 1, 0,  0, 0, 0, 1);
}
void   mat4::print ()
{
	printf("[%8.4f, %8.4f, %8.4f, %8.4f]\n[%8.4f, %8.4f, %8.4f, %8.4f]\n[%8.4f, %8.4f, %8.4f, %8.4f]\n[%8.4f, %8.4f, %8.4f, %8.4f]\n",
		b[0],  b[4],  b[8],  b[12],
		b[1],  b[5],  b[9],  b[13],
		b[2],  b[6],  b[10], b[14],
		b[3],  b[7],  b[11], b[15]);
}
void   mat4::bprint()
{
	printf("[%6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f, %6.4f]\n",
		b[0],  b[1],  b[2],  b[3], b[4],  b[5],  b[6],  b[7], b[8],  b[9],  b[10], b[11], b[12], b[13], b[14], b[15]);
}




void Matrix4x4Multiply(mat4* const& a, mat4* const& b, mat4* const& out)
{
	Matrix4Vector4Multiply(a, &b->c[0], &out->c[0]);
	Matrix4Vector4Multiply(a, &b->c[1], &out->c[1]);
	Matrix4Vector4Multiply(a, &b->c[2], &out->c[2]);
	Matrix4Vector4Multiply(a, &b->c[3], &out->c[3]);
}

void Quaternion_Rotate(quaternion* const& a, vec3* const& v, vec3* const& out)
{
	float
		rsq = a->r * a->r,
		isq = a->i * a->i,
		jsq = a->j * a->j,
		ksq = a->k * a->k;
	out->set(
		v->x * (rsq + isq - jsq - ksq)				+ (v->y + v->y) * (a->i * a->j - a->r * a->k)	+ (v->z + v->z) * (a->r * a->j + a->i * a->k),
		(v->x + v->x) * (a->r * a->k + a->i * a->j) +  v->y * (rsq - isq + jsq - ksq)				+ (v->z + v->z) * (a->j * a->k - a->r * a->i),
		(v->x + v->x) * (a->i * a->k + a->r * a->j) + (v->y + v->y) * (a->r * a->i + a->j * a->k)	+ v->z * (rsq - isq - jsq + ksq)
	);
}