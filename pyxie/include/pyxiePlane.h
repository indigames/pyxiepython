///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieMathutil.h"
namespace pyxie {

#define	F32_LOWER_EQUAL_0(n)	((n) <= 0.0f)

	class pyxiePlane
	{
	public:
		Vec3 Normal, Point;
		float D;

		pyxiePlane() : Normal(0, 1, 0), D(0) { }
		pyxiePlane(const Vec3& normal, const float d) : Normal(normal), D(d) { }
		pyxiePlane(const  Vec3& v1, const Vec3& v2, const Vec3& v3) { Set3Points(v1, v2, v3); }
		~pyxiePlane() {}

		void Set3Points(const Vec3& v1, const Vec3& v2, const Vec3& v3)
		{
			Vec3 aux1, aux2;

			aux1 = v1 - v2;
			aux2 = v3 - v2;
			Normal = Cross3D(aux2, aux1);
			Normal.Normalize();

			Point = v2;
			D = -Vec3::Dot(Point, Normal);
		}

		void SetNormalAndPoint(const Vec3& normal, const Vec3& point)
		{
			this->Normal = Vec3::Normalize(normal);
			D = -Vec3::Dot(this->Normal, point);
		}

		void SetCoefficients(float a, float b, float c, float d)
		{
			// set the normal vector
			Normal = Vec3(a, b, c);
			//compute the lenght of the vector
			float l = Vec3::Length(Normal);
			// normalize the vector
			Normal = Vec3(a / l, b / l, c / l);
			// and divide d by th length as well
			this->D = d / l;
		}

		float Distance(const Vec3 & p) const
		{
			return (D + Vec3::Dot(Normal, p));
		}


		//! Get an intersection with a 3d line.
		/** \param lineVect Vector of the line to intersect with.
		\param linePoint Point of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		bool getIntersectionWithLine(const Vec3 & linePoint,
			const Vec3 & lineVect,
			Vec3 & outIntersection) const
		{
			float t2 = Vec3::Dot(Normal, lineVect);

			if (t2 == 0)
				return false;

			float t = -(Vec3::Dot(Normal, linePoint) + D) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		//! Get percentage of line between two points where an intersection with this plane happens.
		/** Only useful if known that there is an intersection.
		\param linePoint1 Point1 of the line to intersect with.
		\param linePoint2 Point2 of the line to intersect with.
		\return Where on a line between two points an intersection with this plane happened.
		For example, 0.5 is returned if the intersection happened exactly in the middle of the two points.
		*/
		float getKnownIntersectionWithLine(const Vec3 & linePoint1,
			const Vec3 & linePoint2) const
		{
			Vec3 vect = linePoint2 - linePoint1;
			float t2 = Vec3::Dot(Normal, vect);
			return -((Vec3::Dot(Normal, linePoint1) + D) / t2);
		}


		//! Returns if this vector interpreted as a point is on a line between two other points.
		/** It is assumed that the point is on the line.
		\param begin Beginning vector to compare between.
		\param end Ending vector to compare between.
		\return True if this vector is between begin and end, false if not. */
		bool isBetweenPoints(const Vec3 & p, const Vec3 & begin, const Vec3 & end) const
		{
			const float f = Vec3::LengthSqr(end - begin);
			return (Vec3::LengthSqr(begin - p) <= f) && (Vec3::LengthSqr(end - p) <= f);
			//const float f = (end - begin).getLengthSQ();
			//return getDistanceFromSQ(begin) <= f &&
			//	getDistanceFromSQ(end) <= f;
		}


		//! Get an intersection with a 3d line, limited between two 3d points.
		/** \param linePoint1 Point 1 of the line.
		\param linePoint2 Point 2 of the line.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		bool getIntersectionWithLimitedLine(
			const Vec3 & linePoint1,
			const Vec3 & linePoint2,
			Vec3 & outIntersection) const
		{
			return (getIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection) &&
				isBetweenPoints(outIntersection, linePoint1, linePoint2));
		}

		//! Classifies the relation of a point to this plane.
		/** \param point Point to classify its relation.
		\return ISREL3D_FRONT if the point is in front of the plane,
		ISREL3D_BACK if the point is behind of the plane, and
		ISREL3D_PLANAR if the point is within the plane. */
		IntersectionRelation classifyPointRelation(const Vec3 & point) const
		{
			const float d = Vec3::Dot(Normal, point) + D;

			if (d < -ROUNDING_ERROR_f32)
				return ISREL_BACK;

			if (d > ROUNDING_ERROR_f32)
				return ISREL_FRONT;

			return ISREL_PLANAR;
		}

		//! Recalculates the distance from origin by applying a new member point to the plane.
		void recalculateD(const Vec3 & MPoint)
		{
			D = -Vec3::Dot(MPoint, Normal);
		}

		//! Gets a member point of the plane.
		Vec3 getMemberPoint() const
		{
			return Normal * -D;
		}

		//! Tests if there is an intersection with the other plane
		/** \return True if there is a intersection. */
		bool existsIntersection(const pyxiePlane & other) const
		{
			Vec3 c = Cross3D(other.Normal, Normal);
			return Vec3::Length(c) > ROUNDING_ERROR_f32;
		}

		//! Intersects this plane with another.
		/** \param other Other plane to intersect with.
		\param outLinePoint Base point of intersection line.
		\param outLineVect Vector of intersection.
		\return True if there is a intersection, false if not. */
		bool getIntersectionWithPlane(const pyxiePlane & other,
			Vec3 & outLinePoint,
			Vec3 & outLineVect) const
		{
			const float fn00 = Vec3::Length(Normal);
			const float fn01 = Vec3::Dot(Normal, other.Normal);
			const float fn11 = Vec3::Length(other.Normal);
			const double det = fn00 * fn11 - fn01 * fn01;

			if (fabs(det) < ROUNDING_ERROR_f64)
				return false;

			const double invdet = 1.0 / det;
			const double fc0 = (fn11 * -D + fn01 * other.D) * invdet;
			const double fc1 = (fn00 * -other.D + fn01 * D) * invdet;

			outLineVect = Cross3D(Normal, other.Normal);
			outLinePoint = Normal * (float)fc0 + other.Normal * (float)fc1;
			return true;
		}

		//! Get the intersection point with two other planes if there is one.
		bool getIntersectionWithPlanes(const pyxiePlane & o1,
			const pyxiePlane & o2, Vec3 & outPoint) const
		{
			Vec3 linePoint, lineVect;
			if (getIntersectionWithPlane(o1, linePoint, lineVect))
				return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

			return false;
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from
		which the triangle is definitely visible when looking into
		the given direction.
		Note that this only works if the normal is Normalized.
		Do not use this method with points as it will give wrong results!
		\param lookDirection: Look direction.
		\return True if the plane is front facing and
		false if it is backfacing. */
		bool isFrontFacing(const Vec3 & lookDirection) const
		{
			const float d = Vec3::Dot(Normal, lookDirection);
			return F32_LOWER_EQUAL_0(d);
		}
	};
}