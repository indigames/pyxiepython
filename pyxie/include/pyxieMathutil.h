///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

#include "pyxieTypes.h"
//#include <vectormath_aos.h>
#include <float.h>
#include "vmath.h"
//using namespace Vectormath::Aos;

namespace pyxie {


	template <int N>
	class PYXIE_EXPORT Vec {
		float v[N];
	public:
		Vec() {
			for (int i = 0; i < N; i++) v[i] = 0.0f;
		}
		Vec(float x, float y) {
			v[0] = x;
			v[1] = y;
			if (N > 2) {
				v[2] = 0.0f;
				if (N > 3) {
					v[3] = 0.0f;
				}
			}
		}
		Vec(float x, float y, float z) {
			v[0] = x;
			v[1] = y;
			if (N > 2) {
				v[2] = z;
				if (N > 3) {
					v[3] = 0.0f;
				}
			}
		}
		Vec(float x, float y, float z, float w) {
			v[0] = x;
			v[1] = y;
			if (N > 2) {
				v[2] = z;
				if (N > 3) {
					v[3] = w;
				}
			}
		}
		Vec(Vec<N - 1> vec, float a) {
			vmath_cpy(vec.P(), N - 1, v);
			v[N - 1] = a;
		}
		inline float& operator [](int idx)
		{
			return v[idx];
		}
		inline float operator [](int idx) const{
			return v[idx];
		}

		inline bool operator ==(const Vec<N>& vec)
		{
			return vmath_almostEqual(v, vec.v, N);
		}

		inline bool operator !=(const Vec<N> vec)
		{
			return !vmath_almostEqual(v, vec.v, N);
		}

		inline const Vec<N> operator +(const Vec<N> & vec) const {
			Vec<N> out;
			vmath_add(v, vec.v, N, out.v);
			return out;
		}
		inline const Vec<N> operator -(const Vec<N> & vec) const {
			Vec<N> out;
			vmath_sub(v, vec.v, N, out.v);
			return out;
		}
		inline const Vec<N> operator *(float scalar) const {
			Vec<N> out;
			vmath_mul(v, scalar, N, out.v);
			return out;
		}

		inline const Vec<N> operator /(float scalar) const {
			Vec<N> out;
			vmath_div(v, scalar, N, out.v);
			return out;
		}

		inline const Vec<N> operator -() const{
			Vec<N> vec;
			vmath_neg(v, N, vec.v);
			return vec;
		}

		inline Vec<N>& operator +=(const Vec<N>& vec)
		{
			*this = *this + vec;
			return *this;
		}

		inline Vec<N>& operator -=(const Vec<N>& vec)
		{
			*this = *this - vec;
			return *this;
		}

		inline Vec<N>& operator *=(float scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		inline Vec<N>& operator /=(float scalar)
		{
			*this = *this / scalar;
			return *this;
		}


		inline Vec<N>& operator =(const Vec& vec)
		{
			vmath_cpy(vec.v, N,v);
			return *this;
		}

		inline void Normalize(){
			vmath_normalize(v, N, v);
		}

		inline float Length()const {
			return vmath_length(v, N);
		}

		inline Vec<N> Abs()const {
			Vec<N> out;
			vmath_absPerElem(v, N, out.v);
			return out;
		}
		inline Vec<N> Negative()const {
			Vec<N> out;
			vmath_neg(v, N, out.v);
			return out;
		}
		inline Vec<N - 1> DownD()const {
			Vec<N - 1> out;
			vmath_cpy(v, N - 1, out.P());
			return out;
		}


		static float Dot(const Vec<N>& v1, const Vec<N>& v2) {
			return vmath_dot(v1.v, v2.v, N);
		}
		static Vec<N> Normalize(const Vec<N>& v1) {
			Vec<N> out;
			vmath_normalize(v1.v, N, out.v);
			return out;
		}
		static float Length(const Vec<N>& v1) {
			return vmath_length(v1.v, N);
		}
		static float LengthSqr(const Vec<N>& v1) {
			return vmath_lengthSqr(v1.v, N);
		}
		static Vec<N> xAxis() {
			Vec<N> out;
			vmath_vect_xaxis(out.v, N);
			return out;
		}
		static Vec<N> yAxis() {
			Vec<N> out;
			vmath_vect_xaxis(out.v, N);
			return out;
		}
		static Vec<N> zAxis() {
			Vec<N> out;
			vmath_vect_xaxis(out.v, N);
			return out;
		}
		static Vec<N> wAxis() {
			Vec<N> out;
			vmath_vect_xaxis(out.v, N);
			return out;
		}


		inline int D() const { return N; }
		inline float X() const { return v[0]; }
		inline float Y() const { return v[1]; }
		inline float Z() const { return N > 2 ? v[2] : 0.0f; }
		inline float W() const { return N > 3 ? v[3] : 0.0f; }
		inline void X(float i) {  v[0] = i; }
		inline void Y(float i) {  v[1] = i; }
		inline void Z(float i) {  if(N > 2) v[2] = i; }
		inline void W(float i) {  if(N > 3) v[3] = i; }
		inline const float* P() const { return v; }
		inline float* P() { return v; }
	};

	using Vec2 = Vec<2>;
	using Vec3 = Vec<3>;
	using Vec4 = Vec<4>;

	template <int N, int M>
	class PYXIE_EXPORT Mat {
		float v[N * M];
	public:
		Mat() {}
		Mat(const Vec<M> col0, const Vec<M> col1) {
			vmath_cpy(col0.P(), M, &(v[0 * M]));
			vmath_cpy(col1.P(), M, &(v[1 * M]));
		}
		Mat(const Vec<M> col0, const Vec<M> col1, const Vec<M> col2) {
			vmath_cpy(col0.P(), M, &(v[0 * M]));
			vmath_cpy(col1.P(), M, &(v[1 * M]));
			if (N > 2) {
				vmath_cpy(col2.P(), M, &(v[2 * M]));
			}
		}
		Mat(const Vec<M> col0, const Vec<M> col1, const Vec<M> col2, const Vec<M> col3) {
			vmath_cpy(col0.P(), M, &(v[0 * M]));
			vmath_cpy(col1.P(), M, &(v[1 * M]));
			if (N > 2) {
				vmath_cpy(col2.P(), M, &(v[2 * M]));
				if (N > 3) {
					vmath_cpy(col3.P(), M, &(v[2 * M]));
				}
			}
		}
		inline void Identity()
		{
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					v[i * M + j] = i == j ? 1.0f : 0.0f;
				}
			}
		}
		inline float* P() { return v; }
		inline const float* P() const{ return v; }
		const Vec<N>& GetVec(int i)const { return *((const Vec<N>*)(&(v[i * M]))); }
		Vec<N>& GetVec(int i) { return *((Vec<N>*)(&(v[i * M]))); }

		inline bool operator ==(const Mat<N,M>& mat)
		{
			return vmath_almostEqual(v, mat.v, N*M);
		}

		inline bool operator !=(const Mat<N,M> mat)
		{
			return !vmath_almostEqual(v, mat.v, N*M);
		}


		inline Vec<M>& operator [](int idx){
			return *((Vec<M>*)(&(v[idx*M])));
		}

		inline const Vec<M>& operator [](int idx) const{
			return *((const Vec<M>*)(&(v[idx*M])));
		}

		inline Vec<N> operator *(Vec<N> vec) const{
			Vec<N> out;
			vmath_mul_matrix_vector(vec.P(), v, N, N, out.P());
			return out;
		}
		inline Vec<N> operator *(Vec<N-1> vec) const {
			Vec<N> out;
			vmath_mul_matrix_vector(vec.P(), v, N, N-1, out.P());
			return out;
		}

		inline Mat<N,M> operator *=(Mat<N,M> mat) const {
			Mat<N, M> out;
			vmath_mul_matrix_matrix(v, mat.v, N, N, out.v);
			return out;
		}
		inline Mat<N, M> operator *(Mat<N, M> mat) const {
			Mat<N, M> out;
			vmath_mul_matrix_matrix(v, mat.v, N, N, out.v);
			return out;
		}
		inline Mat<N, M> Inverse() const {
			Mat<N, M> out;
			vmath_mat_inverse(v, N, out.v);
			return out;
		}
		inline Mat<N, M> Transpose() const {
			Mat<N, M> out;
			vmath_mat_transpose(v, N, out.v);
			return out;
		}



		static Mat<N,M> IdentityMat(){
			Mat<N, M> out;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					out.v[i * M + j] = i == j ? 1.0f : 0.0f;
				}
			}
			return out;
		}

		static Mat<N, M> RotationX(float radian) {
			Mat<N, M> out;
			vmath_mat_rotationX(radian, N, out.P());
			return out;
		}
		static Mat<N, M> RotationY(float radian) {
			Mat<N, M> out;
			vmath_mat_rotationY(radian, N, out.P());
			return out;
		}
		static Mat<N, M> RotationZ(float radian) {
			Mat<N, M> out;
			vmath_mat_rotationZ(radian, N, out.P());
			return out;
		}
		static Mat<N, M> Scale(Vec3 xyz) {
			Mat<N, M> out;
			vmath_mat_scale(xyz.P(), N, out.P());
			return out;
		}



	};
	using Mat2 = Mat<2, 2>;
	using Mat3 = Mat<3, 3>;
	using Mat4 = Mat<4, 4>;
	using Mat3x4 = Mat<3, 4>;

	class PYXIE_EXPORT Quat {
		float v[4];
	public:
		Quat() {
			v[0] = v[1] = v[2] = 0.0f;
			v[3] = 1.0f;
		}
		Quat(const Mat3& mat) {
			vmath_quat(mat.P(), v);
		}
		inline float* P() { return v; }
		inline const float* P() const { return v; }

		inline Quat& operator =(const Quat& q)
		{
			vmath_cpy(q.v, 4, v);
			return *this;
		}
		inline const Quat operator *(const Quat& q) const {
			Quat out;
			vmath_quat_mul(v, q.v, out.v);
			return out;
		}

		inline void Normalize() {
			vmath_normalize(v, 4, v);
		}
		inline Mat3 ToMatrix() const{
			Mat3 out;
			vmath_mat_from_quat(v, 3, out.P());
			return out;
		}

		static Quat Normalize(const Quat& v1) {
			Quat out;
			vmath_normalize(v1.v, 4, out.v);
			return out;
		}
		static Quat RotationX(float radian) {
			Quat out;
			vmath_quat_rotationX(radian, out.v);
			return out;
		}
		static Quat RotationY(float radian) {
			Quat out;
			vmath_quat_rotationY(radian, out.v);
			return out;
		}
		static Quat RotationZ(float radian) {
			Quat out;
			vmath_quat_rotationZ(radian, out.v);
			return out;
		}
		static Quat Rotation(float radian, const Vec3& vec) {
			Quat out;
			vmath_quat_rotation(radian, vec.P(), out.v);
			return out;
		}
	};


	struct Joint {
		Vec4  m_rotation;             // joint rotation (quaternion)
		Vec4  m_translation;          // joint translation 
		Vec4  m_scale;                // joint scale

		Joint() :
			m_rotation(0.0f, 0.0f, 0.0f, 1.0f),
			m_translation(0.0f, 0.0f, 0.0f, 0.0f),
			m_scale(1.0f, 1.0f, 1.0f, 1.0f){}
	};
	Joint MatrixToJoint(const Mat4& mat);

	constexpr float tolerance = 0.0001f;
	inline bool AlmostEquals(float x, float y) {
		return (fabsf(x - y) < tolerance) ? true : false;
	}

	struct Transform
	{
		Quat		rotation;
		Vec3		translation;
		Vec4		scale;
	};

	inline Vec3 Cross3D(const Vec3& v1, const Vec3& v2) {
		Vec3 out;
		vmath_cross3D(v1.P(), v2.P(), out.P());
		return out;
	}

	inline Mat4 LockAt(const Vec3& eyePos, const Vec3& lookAtPos, const Vec3& upVec) {
		Mat4 out;
		vmath_mat4_lookAt(eyePos.P(), lookAtPos.P(), upVec.P(), out.P());
		return out;
	}
	inline Mat4 Perspective(float fovyRadians, float aspect, float zNear, float zFar) {
		Mat4 out;
		vmath_mat4_perspective(fovyRadians, aspect, zNear, zFar, out.P());
		return out;
	}

	inline Mat4 Frustum(float left, float right, float bottom, float top, float zNear, float zFar) {
		Mat4 out;
		vmath_mat4_frustum(left, right, bottom, top, zNear, zFar, out.P());
		return out;
	}
	inline Mat4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
		Mat4 out;
		vmath_mat4_orthographic(left, right, bottom, top, zNear, zFar, out.P());
		return out;
	}

	inline Joint MatrixToJoint(const Mat4& mat)
	{
		Joint joint;

		// Translation
		joint.m_translation = mat.GetVec(3);
		joint.m_translation.W(1.0f);

		// Scale
		joint.m_scale.X(mat.GetVec(0).Length());
		joint.m_scale.Y(mat.GetVec(1).Length());
		joint.m_scale.Z(mat.GetVec(2).Length());
		joint.m_scale.W(1.0f);

		// Ortho normalize rotation
		Vec3 v0, v1, v2;

		v0 = (const Vec3&)mat.GetVec(0);
		v0.Normalize();

		v2 = Cross3D(v0, (const Vec3&)mat.GetVec(1));
		v2.Normalize();

		v1 = Cross3D(v2, v0);
		v1.Normalize();

		// Get quat from rotation (see target/common/Vectormath)
		float xx = v0.X();
		float yx = v0.Y();
		float zx = v0.Z();
		float xy = v1.X();
		float yy = v1.Y();
		float zy = v1.Z();
		float xz = v2.X();
		float yz = v2.Y();
		float zz = v2.Z();
		float trace = ((xx + yy) + zz);
		bool negTrace = (trace < 0.0f);
		bool ZgtX = zz > xx;
		bool ZgtY = zz > yy;
		bool YgtX = yy > xx;
		bool largestXorY = (!ZgtX || !ZgtY) && negTrace;
		bool largestYorZ = (YgtX || ZgtX) && negTrace;
		bool largestZorX = (ZgtY || !YgtX) && negTrace;

		if (largestXorY) {
			zz = -zz;
			xy = -xy;
		}
		if (largestYorZ) {
			xx = -xx;
			yz = -yz;
		}
		if (largestZorX) {
			yy = -yy;
			zx = -zx;
		}

		float radicand = (((xx + yy) + zz) + 1.0f);
		float scale = (0.5f * (1.0f / sqrtf(radicand)));

		float tmpx = ((zy - yz) * scale);
		float tmpy = ((xz - zx) * scale);
		float tmpz = ((yx - xy) * scale);
		float tmpw = (radicand * scale);
		float qx = tmpx;
		float qy = tmpy;
		float qz = tmpz;
		float qw = tmpw;

		if (largestXorY) {
			qx = tmpw;
			qy = tmpz;
			qz = tmpy;
			qw = tmpx;
		}
		if (largestYorZ) {
			tmpx = qx;
			tmpz = qz;
			qx = qy;
			qy = tmpx;
			qz = qw;
			qw = tmpz;
		}

		joint.m_rotation.X(qx);
		joint.m_rotation.Y(qy);
		joint.m_rotation.Z(qz);
		joint.m_rotation.W(qw);

		return joint;
	}


	inline void Matrices4ToTransform(const Mat4& inputMatrices, Transform& outTranfrorm)
	{
		float sx = inputMatrices.GetVec(0).Length();
		float sy = inputMatrices.GetVec(1).Length();
		float sz = inputMatrices.GetVec(2).Length();
		Vec4 scale(sx, sy, sz, 1.0f);

		Mat3 rotmat(
			((const Vec3&)(inputMatrices.GetVec(0))) / sx,
			((const Vec3&)(inputMatrices.GetVec(1))) / sy,
			((const Vec3&)(inputMatrices.GetVec(2))) / sz);

		Quat rotation(rotmat);
		Vec3 translation = (const Vec3&)(inputMatrices.GetVec(4));

		outTranfrorm.rotation = rotation;
		outTranfrorm.rotation.Normalize();
		outTranfrorm.translation = translation;
		outTranfrorm.scale = scale;
	}

	inline void TransformToMatrix4(const Transform& inputTransform, Mat4& outputMatrix)
	{
		vmath_mat4_from_rottrans(inputTransform.rotation.P(), inputTransform.translation.P(), outputMatrix.P());
		vmath_mat_appendScale(outputMatrix.P(), inputTransform.scale.P(), 4, 4, outputMatrix.P());
	}

	inline void JointToMatrix4(const float* joint, Mat4& outMatrix)
	{
		outMatrix.GetVec(0) = Vec4(joint[0], joint[4], joint[8], 0.0f);
		outMatrix.GetVec(1) = Vec4(joint[1], joint[5], joint[9], 0.0f);
		outMatrix.GetVec(2) = Vec4(joint[2], joint[6], joint[10], 0.0f);
		outMatrix.GetVec(3) = Vec4(joint[3], joint[7], joint[11], 1.0f);
	}

	/*
	typedef uint16_t __fp16;
	inline __fp16 F32to16(float fval) {
		uint32_t ival = *(uint32_t*)(&fval);
		if (!ival) {
			return	0;
		}
		int	e = ((ival & 0x7f800000) >> 23) - 127 + 15;
		if (e < 0) {
			return	0;
		}
		else if (e > 31) {
			e = 31;
		}
		uint32_t 	s = ival & 0x80000000;
		uint32_t 	f = ival & 0x007fffff;
		return	(uint16_t)(((s >> 16) & 0x8000) | ((e << 10) & 0x7c00) | ((f >> 13) & 0x03ff));
	}
*/
	inline uint8_t F32to8(float fval) {
		return (uint8_t)(fval * 255);
	}

	inline int16_t F32toS16(float fval) {
		return (int16_t)(fval * 32767.0f);
	}

	constexpr int SKINMATRIXSIZE = 12;

//	extern const Vector3 ZeroVector3;
//	extern const Vector3 OneVector3;
//	extern const Vector4 ZeroVector4;
//	extern const Vector4 OneVector4;
//	extern const Quat UnitQuat;
//	extern const Matrix4 UnitMatrix4;

	//! Enumeration for intersection relations of 3d objects
	enum IntersectionRelation
	{
		ISREL_FRONT = 0,
		ISREL_BACK,
		ISREL_PLANAR,
		ISREL_SPANNING,
		ISREL_CLIPPED
	};
#define	F32_LOWER_EQUAL_0(n)	((n) <= 0.0f)
	const float ROUNDING_ERROR_f32 = 0.000001f;
	const double ROUNDING_ERROR_f64 = 0.00000001;

}

#include "pyxiePlane.h"
#include "pyxieAABBox.h"
#include "pyxieFrustum.h"
