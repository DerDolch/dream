/*
 *  Color.cpp
 *  Dream
 *
 *  Created by Samuel Williams on 2/05/10.
 *  Copyright 2010 Samuel Williams. All rights reserved.
 *
 */

#include "Color.h"

namespace Dream
{
	namespace Numerics
	{

#pragma mark -
#pragma mark Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Color)
		{
			Vec3 c1(0.2, 0.4, 0.6), c2(0.4, 1.0, 0.0), c3(0.9, 0.9, 0.9);
			
			Vec3 hsl1(0.58333333333, 0.5, 0.4), hsl2(0.266666666, 1.0, 0.5), hsl3(0.0, 0.0, 0.9);
			Vec3 hsv1(0.58333333333, 0.66666666666, 0.59999999999), hsv2(0.26666666666, 1.0, 1.0), hsv3(0.0, 0.0, 0.9);
			
			Vec3 t1, t2;
			
			testing("RGB <-> HSL");
			
			t1 = RGB2HSL(c1);
			assertEquivalent(t1, hsl1, "RGB -> HSL");
			
			t2 = HSL2RGB(t1);
			assertEquivalent(c1, t2, "RGB -> HSL -> RGB");

			t1 = RGB2HSL(c2);
			assertEquivalent(t1, hsl2, "RGB -> HSL");
			
			t2 = HSL2RGB(t1);
			assertEquivalent(c2, t2, "RGB -> HSL -> RGB");
			
			t1 = RGB2HSL(c3);
			assertEquivalent(t1, hsl3, "RGB -> HSL");
			
			t2 = HSL2RGB(t1);
			assertEquivalent(c3, t2, "RGB -> HSL -> RGB");
			
			testing("RGB <-> HSV");
			
			t1 = RGB2HSV(c1);
			assertEquivalent(t1, hsv1, "RGB -> HSV");
			
			t2 = HSV2RGB(t1);
			assertEquivalent(c1, t2, "RGB -> HSV -> RGB");
			
			t1 = RGB2HSV(c2);
			assertEquivalent(t1, hsv2, "RGB -> HSV");
			
			t2 = HSV2RGB(t1);
			assertEquivalent(c2, t2, "RGB -> HSV -> RGB");
			
			t1 = RGB2HSV(c3);
			assertEquivalent(t1, hsv3, "RGB -> HSV");
			
			t2 = HSV2RGB(t1);
			assertEquivalent(c3, t2, "RGB -> HSV -> RGB");
		}		
#endif
	}
}