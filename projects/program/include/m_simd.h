#ifndef MATH_MSIMD_H_
#define MATH_MSIMD_H_
#include <immintrin.h>
#include <stdio.h>
#include "ResourceManager.h"
#include <random>


#define ALIGNED(ptr, BYTE_COUNT) (long)ptr % (long)BYTE_COUNT == 0
#define mask(a, b, c, d)  (a | (b<<2) | (c<<4) | (d<<6))
#define digit_mask(d)			 (d | (d<<2) | (d<<4) | (d<<6))


constexpr double PI = 3.1415926535897932;
constexpr float  pi = 3.141592653f;
constexpr float  MIN_FLOAT = (float)1.0e-20;
constexpr float  MAX_FLOAT = (float)1.0e+20;
constexpr float  epsilon   = (float)1.0e-30;


// element-wise __m128 trignometric evaluation.
// quite ugly, prefer to create wrapper functions or give very distinct names to the macros.
/*
	ve - vector extension
	FAST - aimed towards speed     rather than precision.
	PREC - aimed towards precision rather than speed.
*/
#define sqrt_veFAST(a)     (_mm_sqrt_ss(a))  // returns sqrt ONLY on first element
#define rcpsqrt_veFASTS(a) (_mm_rsqrt_ss(a)) // returns sqrt ONLY on first element

#define sqrt_vePREC(a)    (_mm_sqrt_ps(a))
#define rcpsqrt_vePREC(a) (_mm_rcp_ps(_mm_sqrt_ps(a)))

#define sin_ve(a) (std::sinf(a)) 
#define cos_ve(a) (std::cosf(a)) 
#define tan_ve(a) (std::tanf(a))
#define cot_ve(a) (_mm_rcp_ps(std::tanf(a)))
#define rcp_ve(a) (_mm_rcp_ps (a))
#define MATH_MSIMD_MAX(x, y) ((x) > (y) ? (x) : (y))




/*
	==========================================================================
						 DEFINITION OF MATH FUNCTIONS
	==========================================================================
*/

inline float  rad(const float& ang)
{
	return pi * ang / 180;
}
inline float  ang(const float& rad)
{
	return 180 * rad / pi;
}
inline float clamp(const float& val, const float& lower, const float& upper)
{
	return std::max(lower, std::min(val, upper));
}
inline float fsin(const float& rad) { return sin_ve(rad); }
inline float fcos(const float& rad) { return cos_ve(rad); }
inline float ftan(const float& rad) { return tan_ve(rad); }
inline float fcot(const float& rad) { return 1.0f / tan_ve(rad); }
inline float fsqrtf(const float& x) { return sqrt(x); }
inline float fsqrtp(const float& x) { return sqrt(x); }
// inline __m128 smd_sin(const __m128& v) { return sin_ve(v); }
// inline __m128 smd_cos(const __m128& v) { return cos_ve(v); }
// inline __m128 smd_tan(const __m128& v) { return tan_ve(v); }
// inline __m128 smd_cot(const __m128& v) { return cot_ve(v); }
// inline __m128 smd_sqrtf(const __m128& v) { return sqrt_veFAST(v); }
// inline __m128 smd_sqrtp(const __m128& v) { return sqrt_vePREC(v); }



/*
	=================================================================================
									 VEC4 DEFINITION
	=================================================================================
*/

__declspec(align(16)) struct vec4
{
	vec4(const float&  a, const float&  b,  const float&  c, const float&  d)        { m = _mm_set_ps(d, c, b, a);    }
	vec4(const float&  a, const float&  b,  const float&  c)			             { m = _mm_set_ps(1.0f, c, b, a); }
	vec4(const float&  f)														   	 { m = _mm_set_ps1(f);            }
	vec4(const __m128& v)															 { m = _mm_load_ps((float*)&v);   }
	vec4(float v[4])																 { m = _mm_loadu_ps(&v[0]);       }
	inline void set(const float& a, const float& b, const float& c, const float& d)  { m = _mm_set_ps(d, c, b, a);    }
	inline void set(const float& a, const float& b, const float& c)                  { m = _mm_set_ps(1.0f, c, b, a); }
	inline void set(const float& f)                                                  { m = _mm_set_ps1(f);            }
	vec4(		    const vec4& c) noexcept									         { m = _mm_load_ps(&c.b[0]);      }
	vec4& operator=(const vec4& c) noexcept { _mm_store_ps((float*)&b[0], c.m); return *this;                         }
	
		  float& operator[](const unsigned int& idx);
	const float& operator[](const unsigned int& idx) const;

	inline vec4& operator+=(const vec4&  v) { m = _mm_add_ps(m, v.m);			  return *this; }
	inline vec4& operator-=(const vec4&  v) { m = _mm_sub_ps(m, v.m);			  return *this; }
	inline vec4& operator*=(const vec4&  v) { m = _mm_mul_ps(m, v.m);			  return *this; }
	inline vec4& operator/=(const vec4&  v) { m = _mm_mul_ps(m, _mm_rcp_ps(v.m)); return *this; }
	inline vec4& operator*=(const float& f) { m = _mm_mul_ps(m, _mm_set_ps1(f));  return *this; }
	inline vec4& operator/=(const float& f)
	{
		__m128 tmp = _mm_set_ps1(f);
		tmp = _mm_rcp_ps(tmp);
		m = _mm_mul_ps(m, tmp);
		return *this;
	}

	float mag();
	float sum();
	float dot(const vec4&) const;
	void normalize();

	float* get_c();
	void   print();

	union
	{
		__m128 m;
		float  b[4];
		struct { float x, y, z, w; };
	};
};




/*
	=================================================================================
									 VEC3 DEFINITION
	=================================================================================
*/

__declspec(align(16)) struct vec3
{
	vec3() = delete;
	vec3(const float& a, const float& b, const float& c) { m = _mm_set_ps(1.0f, c, b, a); }
	vec3(const float& f)								 { m = _mm_set_ps1(f); }
	vec3(const vec3& f) noexcept						 { _mm_store_ps(&b[0], f.m);      }
	vec3(const __m128& v) : m(v)               { w = 1.0f; }
	vec3(float v[3])      : m(_mm_loadu_ps(v)) {}
	vec3& operator=(const vec3& c)  noexcept { m = c.m;  return *this; }
	inline void set(const  float& a, const float& b, const float& c) { m = _mm_set_ps(1.0f, c, b, a); }

	inline vec3& operator+= (const vec3& v)  { _mm_store_ps(&b[0], _mm_add_ps(m, v.m));	w = 1.0f;		      return *this; }
	inline vec3& operator-= (const vec3& v)  { _mm_store_ps(&b[0], _mm_sub_ps(m, v.m));	w = 1.0f;		      return *this; }
	inline vec3& operator*= (const vec3& v)  { _mm_store_ps(&b[0], _mm_mul_ps(m, v.m));	w = 1.0f;             return *this; }
	inline vec3& operator/= (const vec3& v)  { _mm_store_ps(&b[0], _mm_mul_ps(m, _mm_rcp_ps(v.m))); w = 1.0f; return *this; }
	inline vec3& operator*= (const float& f) { _mm_store_ps(&b[0], _mm_mul_ps(m, _mm_set_ps1(f)));	w = 1.0f; return *this; }
	inline vec3& operator/= (const float& f) 
	{ 
		__m128 t = _mm_set_ps1(f); t = _mm_rcp_ps(t);
		m = _mm_mul_ps(m, t); 
		w = 1.0f;
		return *this;
	}

	inline float mag()
	{
		return fsqrtf(x * x + y * y + z * z);
	}

	inline float sum()
	{
		return x + y + z;
	}

	inline float dot(const vec3& v) 
	{
		static constexpr __m128 mask{1, 0, 0, 0};
		float  result[4];
		__m128 dot = _mm_dp_ps(m, v.m, 0b11110001);
		
		_mm_maskstore_ps(result, mask, dot);
		return result[0];
	}

	inline void  cross(const vec3& v, vec3* const& out) 
	{ 
		out->set(
			y * v.z - v.y * z,
			z * v.x - v.z * x,
			x * v.y - v.x * y);
	}

	vec3& normalize()
	{
		__m128 tmp = _mm_dp_ps(m, m, 0b11111111); 
		tmp = _mm_sub_ps(tmp, _mm_set_ps1(1.0f));
		tmp = _mm_rsqrt_ps(tmp);
		m = _mm_mul_ps(m, tmp);
		w = 1.0f;
		return *this;
	}

	void  normalized(vec3* const& out)
	{
		out->m = _mm_dp_ps(m, m, 0b11111111);
		out->m = _mm_sub_ps(out->m, _mm_set_ps1(1.0f));
		out->m = _mm_rsqrt_ps(out->m);
		out->m = _mm_mul_ps(m, out->m);
		out->w = 1;
	}

	inline vec4& homogenous()
	{
		return hmg;
	}

	inline float* get_c() { return &b[0]; }
	inline void   print() { printf("[%8.4f, %8.4f, %8.4f]\n", b[0], b[1], b[2]); }

	union
	{
		__m128 m;
		vec4   hmg; // homogenous 3 component vector.
		float  b[4];
		struct { float x, y, z, w; };
	};
};




/*
	=================================================================================
									 MAT4 DEFINITION
	=================================================================================
*/


__declspec(align(32)) struct m256 // helper struct
{
	union
	{
		__m256 v8f;
		__m128 v4f[2];
	};
};
__declspec(align(32)) struct mat4
{
	mat4();
	mat4(const float& d);
	mat4(
		const float&a0, const float&a4, const float&a8,  const float&a12,
		const float&a1, const float&a5, const float&a9,  const float&a13, 
		const float&a2, const float&a6, const float&a10, const float&a14, 
		const float&a3, const float&a7, const float&a11, const float&a15);

	mat4(const __m256& a, const __m256& b);
	mat4(float buffer[16]);
	mat4		   (const mat4& m) noexcept;
	mat4& operator=(const mat4& m) noexcept;
	void set(
		const float& a0, const float& a4, const float& a8, const float& a12,
		const float& a1, const float& a5, const float& a9, const float& a13,
		const float& a2, const float& a6, const float& a10, const float& a14,
		const float& a3, const float& a7, const float& a11, const float& a15
	);


	// use [] for column access.
		  vec4& operator[](const unsigned int& idx); 
	const vec4& operator[](const unsigned int& idx) const;

	// use () for row access.
	/* 
		you can't get references to the floats in the matrix,
		you have to copy the data unfortunately unless
		you access the matrix directly.
		accessing the indices directly is overall worse in performance,
		but theres no other option since the mat4 is stored in column-major.
	*/
		  vec4 operator()(const unsigned int& idx);
	const vec4 operator()(const unsigned int& idx) const;

	mat4& operator+=(const mat4&  m);
	mat4& operator-=(const mat4&  m);
	mat4& operator*=(const mat4&  m);
	mat4& operator/=(const mat4&  m);
		  mat4& operator*(const mat4& m);
	const mat4& operator*(const mat4& m) const;
		  vec4 operator* (const vec4& m);
	const vec4 operator* (const vec4& m) const;

	void  inverse(mat4* const& address);
	mat4& transpose();
	mat4& identity();
	mat4& zeros();

	float* get_c();
	void   set_zero();
	void   set_identity();
	void   print();
	void   bprint();

	union
	{
		__m256  v[2];
		__m128 ci[4];
		vec4    c[4];
		float   b[16];
		struct 
		{ // Column Major order.
			float x0, y0, z0, w0,
				  x1, y1, z1, w1,
				  x2, y2, z2, w2,
				  x3, y3, z3, w3;
		};
	};
};




/*
	===========================================================================
							   QUATERNION DEFINITION
	===========================================================================
*/

__declspec(align(16)) struct quaternion
{
	inline quaternion(const float& a, const float& x, const float& y, const float& z)
	{
		q = _mm_set_ps(z, y, x, a);
	}

	inline quaternion(const float& a, const  vec3& axis)
	{
		q = _mm_set_ps1(fsin(a / 2));
		q = _mm_mul_ps(axis.m, q);
		b[0] = fcos(a / 2);
	}

	inline quaternion(const vec4& q) : q(q.m) {}
	inline quaternion(const __m128& v) : q(v) {}

	inline quaternion& operator=(const quaternion& qu) noexcept
	{
		q = _mm_load_ps(qu.b);
		return *this;
	}

	inline void set(const float& f)
	{
		qv = _mm_set_ps1(f);
	}

	inline quaternion& operator+=(const quaternion& qu) { q = _mm_add_ps(q, qu.q); return *this; }
	inline quaternion& operator-=(const quaternion& qu) { q = _mm_sub_ps(q, qu.q); return *this; }
	inline quaternion& operator/=(const float& f) { q = _mm_mul_ps(q, _mm_rcp_ps(_mm_set_ps1(f))); return *this; }
	inline quaternion& operator*=(const float& f) { q = _mm_mul_ps(q,			  _mm_set_ps1(f)); return *this; }

	// hamilton product
	inline quaternion& operator*=(const quaternion& qu)
	{
		_mm_store_ps(&b[0],
			__m128{
				r * qu.r - i * qu.i - j * qu.j - k * qu.k,
				r * qu.i + i * qu.r + j * qu.k - k * qu.j,
				r * qu.j - i * qu.k + j * qu.r + k * qu.i,
				r * qu.k + i * qu.j - j * qu.j + k * qu.r
			});
		return *this;
	}

	inline float length()		   { return qv.mag();			   }
	inline quaternion& normalize() { qv.normalize(); return *this; }
	inline void normalized(quaternion* const& address)
	{
		address->q = q;
		address->normalize();
	}

	inline void conjugate(quaternion* const& address)
	{
		address->q = _mm_set_ps1(-1);
		address->q = _mm_mul_ps(q, address->q);
		address->b[0] = b[0];
	}

	// stores axis-angle in the format of [axis, w=angle] at address
	inline void toAngleAxis(vec4* const& address)
	{
		float t_rad = std::acosf(r); // theta = 2 * arccos(x)
		float temp  = std::sinf(t_rad);
		
		ON_CONDITION_DO(temp != 0, ERR_EXIT("CANNOT DIVIDE BY 0 <--- quaternion.toAngleAxis()"));
		address->m = _mm_mul_ps(q, _mm_set_ps1(1.0f / temp)); // angle axis = q / sin(theta / 2 [ == arcos(x)])
		address->w = 1;
	}

	inline void toRotationMatrix(mat4* const& address)
	{
		address->set(
			1 - 2 * (j * j + k * k), 2 * (i * j - k * r)    , 2 * (i * k + j * r)    , 0,
			2 * (i * j + k * r)    , 1 - 2 * (i * i + k * k), 2 * (j * k - i * r)    , 0,
			2 * (i * k - j * r)    , 2 * (j * k + i * r)    , 1 - 2 * (i * i + j * j), 0,
			0                      , 0                      , 0                      , 1
		);
	}

	union
	{
		__m128 q;
		vec4   qv;
		float  b[4];
		float  r, i, j, k;
	};
};




/*
	==============================================================================
						 DEFINITION OF BASIC MATRIX FUNCTIONS
	==============================================================================
*/

void Matrix4x4Multiply(mat4* const& a, mat4* const& b, mat4* const& out);
void Quaternion_Rotate(quaternion* const& a, vec3* const& v, vec3* const& out);

inline void Matrix4Vector4Multiply(const mat4* const m, const vec4* const v, vec4* const out)
{
	__m128 result[4];
	result[0] = _mm_dp_ps(m->operator()(0).m, v->m, 0b11110001);
	result[1] = _mm_dp_ps(m->operator()(1).m, v->m, 0b11110001);
	result[2] = _mm_dp_ps(m->operator()(2).m, v->m, 0b11110001);
	result[3] = _mm_dp_ps(m->operator()(3).m, v->m, 0b11110001);


	static constexpr __m128 mask{1, 0, 0, 0};
	static float store[4];
	for(uint32_t i = 0; i < 4; ++i) {
		_mm_maskstore_ps(store, mask, result[i]);
		out->b[i] = store[0];
	}
}

inline void identity(mat4* const& out)
{
	out->set(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}
inline void translate(const vec3& t, mat4* const& out)
{
	out->set(
		1.0f, 0.0f, 0.0f, t.x,
		0.0f, 1.0f, 0.0f, t.y,
		0.0f, 0.0f, 1.0f, t.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}
inline void scale(const vec3& s, mat4* const& out)
{
	out->set(
		s.x,  0.0f, 0.0f, 0.0f,
		0.0f, s.y,  0.0f, 0.0f,
		0.0f, 0.0f, s.z,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

// takes in vec4 in the form of [vec3 axis, float angle] arranged [x, y, z, angle]
// angle is in radians.
inline void RotateAroundAxis(const vec3& vec, const float& a, mat4* const& out)
{
	// source: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	vec3 v = vec;
	v.normalize();
	float c = fcos(a), s = fsin(a), omc = 1 - c;

	out->set(
		c + v.x * v.x * omc,         v.x * v.y * omc - v.z * s,   v.x * v.z * omc + v.y * s,   0,
		v.x * v.y * omc + v.z * s,   c + v.y * v.y * omc,         v.y * v.z * omc - v.x * s,   0,
		v.x * v.z * omc - v.y * s,   v.z * v.y * omc + v.x * s,   c + v.z * v.z * omc,         0,
		0,                           0,                           0,                           1
	);
}

inline void TRS(const vec3& t, const vec3& r, const float& a, const vec3& s, mat4* const& out)
{
	mat4 tmp, tmp2;
	scale(s, out);
	RotateAroundAxis(r, a, &tmp);
	Matrix4x4Multiply(&tmp, out, &tmp2);
	translate(t, &tmp);
	Matrix4x4Multiply(&tmp2, &tmp2, out);
}

inline void TS(const vec3& t, const vec3& s, mat4* const& out)
{
	out->set(
		s.x,  0.0f, 0.0f, t.x,
		0.0f, s.y,  0.0f, t.y,
		0.0f, 0.0f, s.z,  t.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline void lookAt(vec3& eye, vec3& at, vec3& up, mat4* const& out)
{
	vec3 x{ 0.0f }, y{ 0.0f };
	vec3 z = at; z -= eye; z = z.normalize();
	z.cross(up, &x);       x = x.normalize();
	x.cross(z, &y);

	out->set(
		x.x,  x.y,  x.z, -x.dot(eye),
		y.x,  y.y,  y.z, -y.dot(eye),
	   -z.x, -z.y, -z.z,  z.dot(eye),
		0, 0, 0, 1
	);
}

inline void perspective(const float& near, const float& far, const float& aspect, const float& fv_rad, mat4* const& out)
{
	ON_CONDITION_DO(near <= 0 || fv_rad <= 0, ERR_EXIT("NEAR CAN NEVER EQUAL 0.\nFIELD OF VIEW MUST BE ABOVE 0."));
	float cotfov = fcot(fv_rad / 2);
	out->set(
		cotfov / aspect, 0,                0,                             0,
		0,            cotfov,              0,                             0,
		0,               0,   (far + near) / (near - far), 2 * far * near / (near - far),
		0,               0,               -1,                             0
	);
}
inline void ModelView(mat4& model, mat4& view, mat4* const& out)
{
	Matrix4x4Multiply(&view, &model, out);
}
inline void ModelViewProjection(mat4& model, mat4& view, mat4& proj, mat4* const& out)
{
	Matrix4x4Multiply(&view, &model, out);
	Matrix4x4Multiply(&proj,  out,   out);
}

// inefficient, use _mm_dp_ps
//inline float dotProduct(vec4* const& a, vec4* const& b)
//{
//	__m128 tmp = _mm_mul_ps(a->m, b->m);
//	tmp = _mm_hadd_ps(tmp, tmp); // [x*x' + y*y' , z*z' + w*w' , x*x' + y*y' , z*z' + w*w' ]
//	tmp = _mm_hadd_ps(tmp, tmp); // [x*x' + y*y' + z*z' + w*w', same thing, same thing, same thing]
//	return tmp.m128_f32[0];
//}


// add, sub, mul, div operations on each struct/class
inline void add(vec4* const& a, vec4* const& b, vec4* const& out)
{
	out->m = _mm_add_ps(a->m, b->m);
}
inline void sub(vec4* const& a, vec4* const& b, vec4* const& out)
{
	out->m = _mm_sub_ps(a->m, b->m);
}
inline void mul(vec4* const& a, vec4* const& b, vec4* const& out)
{
	out->m = _mm_mul_ps(a->m, b->m);
}
inline void div(vec4* const& a, vec4* const& b, vec4* const& out)
{
	out->m = _mm_rcp_ps(b->m);
	out->m = _mm_mul_ps(a->m, out->m);
}
inline void mul(vec4* const& a, const float& f, vec4* const& out)
{
	out->m = _mm_set_ps1(f);
	out->m = _mm_add_ps(a->m, out->m);
}
inline void div(vec4* const& a, const float& f, vec4* const& out)
{
	out->m = _mm_set_ps1(f);
	out->m = _mm_rcp_ps(out->m);
	out->m = _mm_add_ps(a->m, out->m);
}


inline void add(vec3* const& a, vec3* const& b, vec3* const& out)
{
	out->m = _mm_add_ps(a->m, b->m);
	out->w = 1.0f;
}
inline void sub(vec3* const& a, vec3* const& b, vec3* const& out)
{
	out->m = _mm_sub_ps(a->m, b->m);
	out->w = 1.0f;
}
inline void mul(vec3* const& a, vec3* const& b, vec3* const& out)
{
	out->m = _mm_mul_ps(a->m, b->m);
	out->w = 1.0f;
}
inline void div(vec3* const& a, vec3* const& b, vec3* const& out)
{
	out->m = _mm_rcp_ps(b->m);
	out->m = _mm_mul_ps(a->m, out->m);
	out->w = 1.0f;
}
inline void mul(vec3* const& a, const float& f, vec3* const& out)
{
	out->m = _mm_set_ps1(f);
	out->m = _mm_add_ps(a->m, out->m);
	out->w = 1.0f;
}
inline void div(vec3* const& a, const float& f, vec3* const& out)
{
	out->m = _mm_set_ps1(f);
	out->m = _mm_rcp_ps(out->m);
	out->m = _mm_add_ps(a->m, out->m);
	out->w = 1.0f;
}
inline quaternion to_quaternion(const vec3& v)
{
	return { 0, v.x, v.y, v.z };
}

inline void add(mat4* const& a, mat4* const& b, mat4* const& out)
{
	out->v[0] = _mm256_add_ps(a->v[0], b->v[0]);
	out->v[1] = _mm256_add_ps(a->v[1], b->v[1]);
}
inline void sub(mat4* const& a, mat4* const& b, mat4* const& out)
{
	out->v[0] = _mm256_sub_ps(a->v[0], b->v[0]);
	out->v[1] = _mm256_sub_ps(a->v[1], b->v[1]);
}
inline void mul(mat4* const& a, mat4* const& b, mat4* const& out)
{
	out->v[0] = _mm256_mul_ps(a->v[0], b->v[0]);
	out->v[1] = _mm256_mul_ps(a->v[1], b->v[1]);
}
inline void div(mat4* const& a, mat4* const& b, mat4* const& out)
{
	out->v[0] = _mm256_rcp_ps(a->v[0]);
	out->v[0] = _mm256_mul_ps(b->v[0], out->v[0]);

	out->v[1] = _mm256_rcp_ps(a->v[1]);
	out->v[1] = _mm256_mul_ps(b->v[1], out->v[1]);
}



inline void add(quaternion* const& a, quaternion* const& b, quaternion* const& out)
{
	out->q = _mm_add_ps(a->q, b->q);
}
inline void sub(quaternion* const& a, quaternion* const& b, quaternion* const& out)
{
	out->q = _mm_add_ps(a->q, b->q);
}
inline void mul(quaternion* const& a, quaternion* const& b, quaternion* const& out)
{
	_mm_store_ps((float*)out,
		__m128{
			a->r * b->r - a->i * b->i - a->j * b->j - a->k * b->k,
			a->r * b->i + a->i * b->r + a->j * b->k - a->k * b->j,
			a->r * b->j - a->i * b->k + a->j * b->r + a->k * b->i,
			a->r * b->k + a->i * b->j - a->j * b->j + a->k * b->r
		});
}
inline void div(quaternion* const& a, const float& f, quaternion* const& out)
{
	out->q = _mm_set_ps1(f);
	out->q = _mm_rcp_ps(out->q);
	out->q = _mm_mul_ps(a->q, out->q);
}
inline void mul(quaternion* const& a, const float& f, quaternion* const& out)
{
	out->q = _mm_set_ps1(f);
	out->q = _mm_mul_ps(a->q, out->q);
}


inline void addTo(vec3& vec, vec3& vector)
{
	add(&vec, &vector, &vec);
}
template<typename ... Vectors> inline void addTo(vec3& vec, vec3& first, Vectors& ... vec_args)
{
	add(&vec, &first, &vec);
	addTo(vec, vec_args...);
}


inline void addTo(vec4& vec, vec4& vector)
{
	add(&vec, &vector, &vec);
}
template<typename ... Vectors> inline void addTo(vec4& vec, vec4& first, Vectors& ... vec_args)
{
	add(&vec, &first, &vec);
	addTo(vec, vec_args...);
}

inline void addTo(mat4& vec, mat4& vector)
{
	add(&vec, &vector, &vec);
}
template<typename ... Vectors> inline void addTo(mat4& vec, mat4& first, Vectors& ... vec_args)
{
	add(&vec, &first, &vec);
	addTo(vec, vec_args...);
}



/*
	======================================================================================
						 XORSHIFT32 RANDOM NUMBER GENERATOR DEFINITON
	======================================================================================
*/
struct xorshift32
{
	uint64_t m_seed;

	constexpr uint32_t min() { return 0; }
	constexpr uint32_t max() { return UINT32_MAX; }
	xorshift32()
	{
		// using intrinsics.
		//_rdrand64_step(&m_seed);
		//m_seed = m_seed << 31 | m_seed;

		// using standard library.
		std::random_device gen_seed;
		m_seed = gen_seed() << 31 | gen_seed();
	}

	uint32_t operator()()
	{
		uint64_t r = 0xd989bcacc137dcd5ull * m_seed;
		m_seed ^= m_seed >> 11;
		m_seed ^= m_seed << 31;
		m_seed ^= m_seed >> 18;
		return (uint32_t)(r >> 32ull);
	}
};

// GLOBAL GENERATOR DECLARATION
static xorshift32 gen_;


inline float random()
{
	return (float)gen_() / (float)UINT32_MAX;
}
inline double random_prec()
{
	return (double)gen_() / (double)UINT32_MAX;
}
inline vec3  random3()
{
	return vec3(random(), random(), random());
}
inline vec4  random3f()
{
	return vec4(random(), random(), random(), 1.0f);
}
inline vec4  random4f()
{
	return vec4(random(), random(), random(), random());
}

inline mat4 random4x4()
{
	return mat4(
		random(), random(), random(), random(),
		random(), random(), random(), random(),
		random(), random(), random(), random(),
		random(), random(), random(), random()
	);
}



#endif