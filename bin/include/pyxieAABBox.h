/****************************************************************************
  MVGL (Media Vision Game Library)
 
 Axis Aligned Bounding Box
 ****************************************************************************/
#pragma once
#include "pyxieMathutil.h"
namespace pyxie {
	class pyxieAABBox
	{
	public:
		//! Default Constructor.
		pyxieAABBox() : MinEdge(3.402823e+38, 3.402823e+38, 3.402823e+38), MaxEdge(-3.402823e+38, -3.402823e+38, -3.402823e+38) {}

		//! Constructor with min edge and max edge.
		pyxieAABBox(const Vec3& min, const Vec3& max) : MinEdge(min), MaxEdge(max) {}

		//! Constructor with only one point.
		pyxieAABBox(const Vec3& init) : MinEdge(init), MaxEdge(init) {}

		//! Constructor with min edge and max edge as single values, not vectors.
		pyxieAABBox(float minx, float miny, float minz, float maxx, float maxy, float maxz) : MinEdge(minx, miny, minz), MaxEdge(maxx, maxy, maxz) {}

		// operators
		//! Equality operator
		/** \param other box to compare with.
		\return True if both boxes are equal, else false. */
		//inline bool operator==(const AABBox& other) const { return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}
		//! Inequality operator
		/** \param other box to compare with.
		\return True if both boxes are different, else false. */
		//inline bool operator!=(const AABBox& other) const { return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}

		// functions

		//! Resets the bounding box to a one-point box.
		/** \param x X coord of the point.
		\param y Y coord of the point.
		\param z Z coord of the point. */
		void reset(float x, float y, float z)
		{
			MaxEdge = Vec3(x, y, z);
			MinEdge = MaxEdge;
		}

		//! Resets the bounding box.
		/** \param initValue new box to set this one to. */
		void reset(const pyxieAABBox& initValue)
		{
			*this = initValue;
		}

		//! Resets the bounding box to a one-point box.
		/** \param initValue new point. */
		void reset(const Vec3& initValue)
		{
			MaxEdge = initValue;
			MinEdge = initValue;
		}

		//! Adds a point to the bounding box
		/** The box grows bigger, if point was outside of the box.
		\param p: Point to add into the box. */
		void addInternalPoint(const Vec3& p)
		{
			addInternalPoint(p.X(), p.Y(), p.Z());
		}

		//! Adds another bounding box
		/** The box grows bigger, if the new box was outside of the box.
		\param b: Other bounding box to add into this box. */
		void addInternalBox(const pyxieAABBox& b)
		{
			addInternalPoint(b.MaxEdge);
			addInternalPoint(b.MinEdge);
		}

		//! Adds a point to the bounding box
		/** The box grows bigger, if point is outside of the box.
		\param x X coordinate of the point to add to this box.
		\param y Y coordinate of the point to add to this box.
		\param z Z coordinate of the point to add to this box. */
		void addInternalPoint(float x, float y, float z)
		{
			if (x > MaxEdge.X()) MaxEdge.X(x);
			if (y > MaxEdge.Y()) MaxEdge.Y(y);
			if (z > MaxEdge.Z()) MaxEdge.Z(z);

			if (x < MinEdge.X()) MinEdge.X(x);
			if (y < MinEdge.Y()) MinEdge.Y(y);
			if (z < MinEdge.Z()) MinEdge.Z(z);
		}

		//! Get center of the bounding box
		/** \return Center of the bounding box. */
		Vec3 getCenter() const
		{
			return (MinEdge + MaxEdge) / 2;
		}

		//! Get extent of the box (maximal distance of two points in the box)
		/** \return Extent of the bounding box. */
		Vec3 getExtent() const
		{
			return MaxEdge - MinEdge;
		}

		//! Get the volume enclosed by the box in cubed units
		float getVolume() const
		{
			const Vec3 e = getExtent();
			return e.X()* e.Y()* e.Z();
		}

		//! Get the surface area of the box in squared units
		float getArea() const
		{
			const Vec3 e = getExtent();
			return 2 * (e.X() * e.Y() + e.X() * e.Z() + e.Y() * e.Z());
		}

		//! Stores all 8 edges of the box into an array
		/** \param edges: Pointer to array of 8 edges. */
		void getEdges(Vec3 * edges) const
		{
			const Vec3 middle = getCenter();
			const Vec3 diag = middle - MaxEdge;

			/*
			Edges are stored in this way:
			Hey, am I an ascii artist, or what? :) niko.
				   /3--------/7
				  / |       / |
				 /  |      /  |
				1---------5   |
				|  /2- - -|- -6
				| /       |  /
				|/        | /
				0---------4/
			*/

			edges[0] = Vec3(middle.X() + diag.X(), middle.Y() + diag.Y(), middle.Z() + diag.Z());
			edges[1] = Vec3(middle.X() + diag.X(), middle.Y() - diag.Y(), middle.Z() + diag.Z());
			edges[2] = Vec3(middle.X() + diag.X(), middle.Y() + diag.Y(), middle.Z() - diag.Z());
			edges[3] = Vec3(middle.X() + diag.X(), middle.Y() - diag.Y(), middle.Z() - diag.Z());
			edges[4] = Vec3(middle.X() - diag.X(), middle.Y() + diag.Y(), middle.Z() + diag.Z());
			edges[5] = Vec3(middle.X() - diag.X(), middle.Y() - diag.Y(), middle.Z() + diag.Z());
			edges[6] = Vec3(middle.X() - diag.X(), middle.Y() + diag.Y(), middle.Z() - diag.Z());
			edges[7] = Vec3(middle.X() - diag.X(), middle.Y() - diag.Y(), middle.Z() - diag.Z());
		}

		//! Repairs the box.
		/** Necessary if for example MinEdge and MaxEdge are swapped. */
		void repair()
		{
			float t;

			if (MinEdge.X() > MaxEdge.X())
			{
				t = MinEdge.X(); MinEdge.X(MaxEdge.X()); MaxEdge.X(t);
			}
			if (MinEdge.Y() > MaxEdge.Y())
			{
				t = MinEdge.Y(); MinEdge.Y(MaxEdge.Y()); MaxEdge.Y(t);
			}
			if (MinEdge.Z() > MaxEdge.Z())
			{
				t = MinEdge.Z(); MinEdge.Z(MaxEdge.Z()); MaxEdge.Z(t);
			}
		}

		//! Calculates a new interpolated bounding box.
		/** d=0 returns other, d=1 returns this, all other values blend between
		the two boxes.
		\param other Other box to interpolate between
		\param d Value between 0.0f and 1.0f.
		\return Interpolated box. */
		pyxieAABBox getInterpolated(const pyxieAABBox & other, float d) const
		{
			float inv = 1.0f - d;
			return pyxieAABBox((other.MinEdge * inv) + (MinEdge * d),
				(other.MaxEdge * inv) + (MaxEdge * d));
		}

		//! Determines if a point is within this box.
		/** Border is included (IS part of the box)!
		\param p: Point to check.
		\return True if the point is within the box and false if not */
		bool isPointInside(const Vec3 & p) const
		{
			return (p.X() >= MinEdge.X() && p.X() <= MaxEdge.X() &&
				p.Y() >= MinEdge.Y() && p.Y() <= MaxEdge.Y() &&
				p.Z() >= MinEdge.Z() && p.Z() <= MaxEdge.Z());
		}

		//! Determines if a point is within this box and not its borders.
		/** Border is excluded (NOT part of the box)!
		\param p: Point to check.
		\return True if the point is within the box and false if not. */
		bool isPointTotalInside(const Vec3 & p) const
		{
			return (p.X() > MinEdge.X() && p.X() < MaxEdge.X() &&
				p.Y() > MinEdge.Y() && p.Y() < MaxEdge.Y() &&
				p.Z() > MinEdge.Z() && p.Z() < MaxEdge.Z());
		}

		//! Check if this box is completely inside the 'other' box.
		/** \param other: Other box to check against.
		\return True if this box is completly inside the other box,
		otherwise false. */
		bool IsFullInside(const pyxieAABBox & other) const
		{
			return (MinEdge.X() >= other.MinEdge.X() && MinEdge.Y() >= other.MinEdge.Y() && MinEdge.Z() >= other.MinEdge.Z() &&
				MaxEdge.X() <= other.MaxEdge.X() && MaxEdge.Y() <= other.MaxEdge.Y() && MaxEdge.Z() <= other.MaxEdge.Z());
		}

		//! Determines if the axis-aligned box intersects with another axis-aligned box.
		/** \param other: Other box to check a intersection with.
		\return True if there is an intersection with the other box,
		otherwise false. */
		bool intersectsWithBox(const pyxieAABBox & other) const
		{
			return (MinEdge.X() <= other.MaxEdge.X() && MinEdge.Y() <= other.MaxEdge.Y() && MinEdge.Z() <= other.MaxEdge.Z() &&
				MaxEdge.X() >= other.MinEdge.X() && MaxEdge.Y() >= other.MinEdge.Y() && MaxEdge.Z() >= other.MinEdge.Z());
		}

		//! Tests if the box intersects with a line
		/** \param line: Line to test intersection with.
		\return True if there is an intersection , else false. */
		/*
				bool intersectsWithLine(const line3d& line) const
				{
					return intersectsWithLine(line.getMiddle(), line.getVector().normalize(),
							(float)(line.getLength() * 0.5));
				}
		*/
		//! Tests if the box intersects with a line
		/** \param linemiddle Center of the line.
		\param linevect Vector of the line.
		\param halflength Half length of the line.
		\return True if there is an intersection, else false. */
		bool intersectsWithLine(const Vec3 & linemiddle,
			const Vec3 & linevect, float halflength) const
		{
			const Vec3 e = getExtent() * (float)0.5;
			const Vec3 t = getCenter() - linemiddle;

			if ((fabs(t.X()) > e.X() + halflength * fabs(linevect.X())) ||
				(fabs(t.Y()) > e.Y() + halflength * fabs(linevect.Y())) ||
				(fabs(t.Z()) > e.Z() + halflength * fabs(linevect.Z())))
				return false;

			float r = e.Y() * (float)fabs(linevect.Z()) + e.Z() * (float)fabs(linevect.Y());
			if (fabs(t.Y() * linevect.Z() - t.Z() * linevect.Y()) > r)
				return false;

			r = e.X() * (float)fabs(linevect.Z()) + e.Z() * (float)fabs(linevect.X());
			if (fabs(t.Z() * linevect.X() - t.X() * linevect.Z()) > r)
				return false;

			r = e.X() * (float)fabs(linevect.Y()) + e.Y() * (float)fabs(linevect.X());
			if (fabs(t.X() * linevect.Y() - t.Y() * linevect.X()) > r)
				return false;

			return true;
		}

		//! Classifies a relation with a plane.
		/** \param plane Plane to classify relation to.
		\return Returns ISREL3D_FRONT if the box is in front of the plane,
		ISREL3D_BACK if the box is behind the plane, and
		ISREL3D_CLIPPED if it is on both sides of the plane. */

		IntersectionRelation ClassifyPlaneRelation(const pyxiePlane & plane) const
		{
			Vec3 nearPoint(MaxEdge);
			Vec3 farPoint(MinEdge);

			if (plane.Normal.X() > (float)0)
			{
				nearPoint.X(MinEdge.X());
				farPoint.X(MaxEdge.X());
			}

			if (plane.Normal.Y() > (float)0)
			{
				nearPoint.Y(MinEdge.Y());
				farPoint.Y(MaxEdge.Y());
			}

			if (plane.Normal.Z() > (float)0)
			{
				nearPoint.Z(MinEdge.Z());
				farPoint.Z(MaxEdge.Z());
			}

			if (Vec3::Dot(plane.Normal, nearPoint) + plane.D > (float)0)
				return ISREL_FRONT;

			if (Vec3::Dot(plane.Normal, farPoint) + plane.D > (float)0)
				return ISREL_CLIPPED;

			return ISREL_BACK;
		}


		// for use in frustum computations
		Vec3 GetVertexP(const Vec3 & normal) const
		{
			Vec3 res = MinEdge;

			if (normal.X() > 0)
				res.X(MaxEdge.X());

			if (normal.Y() > 0)
				res.Y(MaxEdge.Y());

			if (normal.Z() > 0)
				res.Z(MaxEdge.Z());

			return(res);
		}
		Vec3 GetVertexN(const Vec3 & normal) const
		{
			Vec3 res = MinEdge;

			if (normal.X() < 0)
				res.X(MaxEdge.X());

			if (normal.Y() < 0)
				res.Y(MaxEdge.Y());

			if (normal.Z() < 0)
				res.Z(MaxEdge.Z());

			return(res);
		}

		float LengthSqr()
		{
			return Vec3::LengthSqr(MaxEdge - MinEdge);
		}


		//! The near edge
		Vec3 MinEdge;

		//! The far edge
		Vec3 MaxEdge;
	};
}
