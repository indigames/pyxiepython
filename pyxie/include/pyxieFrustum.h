///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

namespace pyxie {
	class pyxieViewFrustum
	{
	public:
		enum VFPLANES
		{
			//! Far plane of the frustum. That is the plane farest away from the eye.
			VF_FAR_PLANE = 0,
			//! Near plane of the frustum. That is the plane nearest to the eye.
			VF_NEAR_PLANE,
			//! Left plane of the frustum.
			VF_LEFT_PLANE,
			//! Right plane of the frustum.
			VF_RIGHT_PLANE,
			//! Bottom plane of the frustum.
			VF_BOTTOM_PLANE,
			//! Top plane of the frustum.
			VF_TOP_PLANE,
			//! Amount of planes enclosing the view frustum. Should be 6.
			VF_PLANE_COUNT
		};
		enum { OUTSIDE, INTERSECT, INSIDE };


		//! Default Constructor
		pyxieViewFrustum() {}

		//! Copy Constructor
		pyxieViewFrustum(const pyxieViewFrustum& other);

		void SetCamInternals(float angle, float ratio, float nearD, float farD, float l = -1.0f, float r = 1.0f, float t = 1.0f, float b = -1.0f)
		{
			this->ratio = ratio;
			this->angle = angle;
			this->nearD = nearD;
			this->farD = farD;

			tang = (float)tan(angle * 0.5);

			float nh = nearD * tang;
			float nw = nh * ratio;
			float fh = farD * tang;
			float fw = fh * ratio;

			nl = nw * l;
			nr = nw * r;
			nt = nh * t;
			nb = nh * b;
			fl = fw * l;
			fr = fw * r;
			ft = fh * t;
			fb = fh * b;
		}

		void SetCamDef(const Mat4& invViewMatrix)
		{
			Vec3 dir, nc, fc, X, Y, Z, p;

			X = (const Vec3&)invViewMatrix.GetVec(0);
			Y = (const Vec3&)invViewMatrix.GetVec(1);
			Z = (const Vec3&)invViewMatrix.GetVec(2);
			p = (const Vec3&)invViewMatrix.GetVec(3);

			nc = p - Z * nearD;
			fc = p - Z * farD;

			ntl = nc + Y * nt + X * nl;
			ntr = nc + Y * nt + X * nr;
			nbl = nc + Y * nb + X * nl;
			nbr = nc + Y * nb + X * nr;

			ftl = fc + Y * ft + X * fl;
			ftr = fc + Y * ft + X * fr;
			fbl = fc + Y * fb + X * fl;
			fbr = fc + Y * fb + X * fr;

			planes[VF_TOP_PLANE].Set3Points(ntr, ntl, ftl);
			planes[VF_BOTTOM_PLANE].Set3Points(nbl, nbr, fbr);
			planes[VF_LEFT_PLANE].Set3Points(ntl, nbl, fbl);
			planes[VF_RIGHT_PLANE].Set3Points(nbr, ntr, fbr);
			planes[VF_NEAR_PLANE].Set3Points(ntl, ntr, nbr);
			planes[VF_FAR_PLANE].Set3Points(ftr, ftl, fbl);
		}

		int PointInFrustum(const Vec3& p) const
		{
			int result = INSIDE;
			for (int i = 0; i < 6; i++) {

				if (planes[i].Distance(p) < 0)
					return OUTSIDE;
			}
			return(result);
		}

		int SphereInFrustum(const Vec3& p, float raio) const
		{
			int result = INSIDE;
			float distance;

			for (int i = 0; i < 6; i++) {
				distance = planes[i].Distance(p);
				if (distance < -raio)
					return OUTSIDE;
				else if (distance < raio)
					result = INTERSECT;
			}
			return(result);
		}

		int BoxInFrustum(const pyxieAABBox & b) const
		{
			int result = INSIDE;
			for (int i = 0; i < 6; i++) {
				Vec3 vp = b.GetVertexP(planes[i].Normal);
				if (planes[i].Distance(vp) < 0)
				{
					return OUTSIDE;
				}
				else
				{
					Vec3 vn = b.GetVertexN(planes[i].Normal);
					if (planes[i].Distance(vn) < 0)
						result = INTERSECT;
				}
			}
			return(result);
		}


		//! transforms the frustum by the matrix
		/** \param mat: Matrix by which the view frustum is transformed.*/
		void transform(const Mat4 & mat);

		//! returns the point which is on the far left upper corner inside the the view frustum.
		Vec3 getFarLeftUp() const
		{
			Vec3 p;
			planes[pyxieViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
				planes[pyxieViewFrustum::VF_TOP_PLANE],
				planes[pyxieViewFrustum::VF_LEFT_PLANE], p);

			return p;
		}


		//! returns the point which is on the far left bottom corner inside the the view frustum.
		Vec3 getFarLeftDown() const
		{
			Vec3 p;
			planes[pyxieViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
				planes[pyxieViewFrustum::VF_BOTTOM_PLANE],
				planes[pyxieViewFrustum::VF_LEFT_PLANE], p);

			return p;
		}

		//! returns the point which is on the far right top corner inside the the view frustum.
		Vec3 getFarRightUp() const
		{
			Vec3 p;
			planes[pyxieViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
				planes[pyxieViewFrustum::VF_TOP_PLANE],
				planes[pyxieViewFrustum::VF_RIGHT_PLANE], p);

			return p;
		}

		//! returns the point which is on the far right bottom corner inside the the view frustum.
		Vec3 getFarRightDown() const
		{
			Vec3 p;
			planes[pyxieViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
				planes[pyxieViewFrustum::VF_BOTTOM_PLANE],
				planes[pyxieViewFrustum::VF_RIGHT_PLANE], p);

			return p;
		}

		//! returns a bounding box enclosing the whole view frustum
		const pyxieAABBox& getBoundingBox() const
		{
			return boundingBox;
		}

		//! recalculates the bounding box member based on the planes
		//inline void recalculateBoundingBox();

		//! the position of the camera
		Vec3 cameraPosition;

		//! all planes enclosing the view frustum.
		pyxiePlane planes[VF_PLANE_COUNT];

		//! bounding box around the view frustum
		pyxieAABBox boundingBox;


		/*
			// calculate: 1 / sqrtf ( x )
			float reciprocal_squareroot(const float f)
			{
				return 1.f / sqrtf(f);
			}

		*/

		/*
			void ViewFrustum::recalculateBoundingBox()
			{
				boundingBox.reset ( cameraPosition );

				boundingBox.addInternalPoint(getFarLeftUp());
				boundingBox.addInternalPoint(getFarRightUp());
				boundingBox.addInternalPoint(getFarLeftDown());
				boundingBox.addInternalPoint(getFarRightDown());
			}
		*/

		//! Clips a line to the frustum
	/*
		inline bool ViewFrustum::clipLine(core::line3d<f32>& line) const
		{
			bool wasClipped = false;
			for (u32 i=0; i < VF_PLANE_COUNT; ++i)
			{
				if (planes[i].classifyPointRelation(line.start) == core::ISREL3D_FRONT)
				{
					line.start = line.start.getInterpolated(line.end,
							planes[i].getKnownIntersectionWithLine(line.start, line.end));
					wasClipped = true;
				}
				if (planes[i].classifyPointRelation(line.end) == core::ISREL3D_FRONT)
				{
					line.end = line.start.getInterpolated(line.end,
							planes[i].getKnownIntersectionWithLine(line.start, line.end));
					wasClipped = true;
				}
			}
			return wasClipped;
		}
	*/


	private:
		Vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
		float nearD, farD, ratio, angle, tang;
		//float nw,nh,fw,fh;
		float nl, nr, nt, nb, fl, fr, ft, fb;
	};
}

