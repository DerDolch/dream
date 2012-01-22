/*
 *  Client/Graphics/PointCamera.h
 *  This file is part of the "Dream" project, and is licensed under the GNU GPLv3.
 *
 *  Created by Samuel Williams on 16/10/06.
 *  Copyright 2006 Samuel G. D. Williams. All rights reserved.
 *
 */

#ifndef _DREAM_RENDERER_POINTCAMERA_H
#define _DREAM_RENDERER_POINTCAMERA_H

#include "Camera.h"
#include "../Geometry/Line.h"

namespace Dream
{
	namespace Renderer
	{
		using namespace Dream::Geometry;
		
		class PointCamera : public Object, implements ICamera {
		protected:
			Vec3 m_origin;
			Vec3 m_direction;
			Vec3 m_up;
			
		public:
			PointCamera ();
			
			static Mat44 lookAt(const Vec3 & origin, const Vec3 & direction, const Vec3 & up);
			
			void setDirection (const Vec3 &dir) { m_direction = dir; }
			void setOrigin (const Vec3 &origin) { m_origin = origin; }
			void setUp (const Vec3 &up) { m_up = up; }
			
			void set (const Line<3> &l);
			
			virtual Mat44 viewMatrix () const;				
		};
		
	}
}

#endif
