/*!
    @header
    @abstract   This code provides a basic framework for various types of splines.
	@copyright	2007 Samuel Williams. All rights reserved.
*/

#ifndef _DREAM_GEOMETRY_SPLINE_H
#define _DREAM_GEOMETRY_SPLINE_H

#include <Dream/Numerics/Vector.h>
#include <Dream/Numerics/Interpolate.h>
#include <Dream/Geometry/Plane.h>

#include <boost/function.hpp>
#include <boost/assign/std/vector.hpp>

#include <vector>
#include <list>

namespace Dream {
	namespace Geometry {
		using namespace boost;
		
		/** A generic spline interface.

			@templatefield	D	The dimensions of the spline. Can be 2 or 3.
			
			The spline interface exports basic spline methods, such as 
			point and tangent information at time t. There is also a virtual helper
			function normalAtTime, which is not well defined for 3-space. You should
			use SplineWithNormal to combine a normal spline with a regular spline.
		*/
		template <unsigned D>
		class ISpline {
		public:
			typedef Vector<D> VecT;
			typedef std::vector<VecT> PointsT;
			typedef std::vector<RealT> TimesT;
			
			virtual ~ISpline ();
			
			/// Retrieve the point at time t.
			virtual VecT pointAtTime(RealT t) const abstract;
			
			/// Retrieve the spline tangent at time t.
			virtual VecT tangentAtTime(RealT t) const;

			/// Retrieve the spline normal at time t.
			/// Default implementation is not very good in 3-space. Use 
			/// SplineWithNormal to avoid problems (see Hairy Ball Theorem).
			virtual VecT normalAtTime(RealT t) const;
			
			/// Helper method to subdivide the mesh. 
			/// @param      resolution	The higher the resolution, the more detail will be present.
			///	@param		divisions	Divide at least divisions^2 times.
			///	@result     vector of times
			virtual std::vector<RealT> timesAtResolution(RealT resolution, unsigned divisions) const;

			/// Points used for drawing segments. May differ based on closed/open nature of the spline. */
			virtual const PointsT& segmentPoints() const {
				return points();
			}
			
			virtual const PointsT & points() const abstract;
			
			/// The number of segments along the spline.
			virtual unsigned segments () const;
			
			/// The index of the first point given time t.
			unsigned startingPoint(RealT t) const;
			
			/// The fraction between two points given time t.
			RealT fractionalComponent(RealT t) const;
			
			/// Produce an array of times where points are typically defined
			///	@result     vector of times where well defined points exist
			virtual TimesT nominalTimes () const;
			
		private:
			TimesT divideAndAppend(int n, RealT res) const;
			void divideAndAppend(RealT t, RealT d, int n, RealT res2, bool first, TimesT &times) const;
		};
		
		/** A concrete spline base class.
			A point-based spline can inherit from this class to gain a point-based interface.
		*/
		template <unsigned D>
		class Spline : IMPLEMENTS(Spline<D>) {		
		protected:
			typedef Vector<D> PointT;
			typedef std::vector<PointT> PointsT;
			
			PointsT m_points;
			
			mutable PointsT m_segmentPoints;
			void resetSegmentPointsCache();
			
			/// Points used for drawing segments. Additional start point at end if closed.
			virtual PointsT generateSegmentPoints() const;
			
			bool m_closed;
		public:
		
			Spline ();
			virtual ~Spline ();
			
			virtual const PointsT& points() const { return m_points; }
			virtual PointsT& points() { return m_points; }

			virtual bool closed () const;
			virtual void setClosed (bool closed);
			
			/// Points used for drawing segments. Additional start point at end if closed.
			///	@discussion	Will cache points generated by generateSegmentPoints, so you should 
			///	typically not over-ride this function, and over-ride generateSegmentPoints. To indicate
			///	that the cache needs to be regenerated, call resetSegmentPointsCache()
			virtual const PointsT& segmentPoints() const;		
		};
		
		/** A spline plus spline normal combination.
			
			This interface can be used to attach two splines together.
			The primary spline is exposed through pointAtTime and tangentAtTime, 
			while the difference of pointAtTime(t) is used to provide normalAtTime(t)
		*/
		template <unsigned D>
		class SplineWithNormal : IMPLEMENTS(Spline<D>) {
		protected:
			typedef SplineWithNormal<D> this_t;
			typedef ISpline<D> SplineT;
			typedef Vector<D> PointT;
			typedef std::vector<PointT> PointsT;

		protected:		
			const SplineT * m_spline;
			const SplineT * m_normalSpline;
			
		public:
			SplineWithNormal(const SplineT * spline, const SplineT * normalSpline);
			virtual ~SplineWithNormal ();
			
			/// Retrieve the primary spline tangent at time t.
			virtual Vector<D> pointAtTime(RealT t) const;
			
			/// Retrieve the primary spline tangent at time t.
			virtual Vector<D> tangentAtTime(RealT t) const;
			
			/// Retrieve calculated normal information.
			virtual Vector<D> normalAtTime(RealT t) const;
			
			virtual const PointsT & points() const;
			
			static void forSpline (const SplineT * spline, const SplineT * normalSpline);
		};
		
		/**	A simple linear spline.
			
			Tangent approximation may be curved at midpoints.
		*/
		template <unsigned D>
		class LinearSpline : public Spline<D> {		
		public:
			virtual Vector<D> pointAtTime(RealT t) const;
		};
		
		/** A basic 4-point cubic spline.
		*/
		template <unsigned D>
		class CubicSpline : public Spline<D> {
		protected:
			typedef Vector<D> PointT;
			typedef std::vector<PointT> PointsT;
			
			/// Cublic splines require one additional point at the start and at the 
			/// end, so this function is overridden.
			virtual unsigned segments () const;
			
			///	A closed cubic requires in addition to a closed spline, two additional
			/// points to be wrapped, one at the start, and one at the end.
			virtual PointsT generateSegmentPoints() const;
		public:
			/// Retrieve the point at time t.
			virtual Vector<D> pointAtTime(RealT t) const;
		};

		/**	A hermite cubic spline with programmatic mu.
		*/
		template <unsigned D>
		class HermiteSpline : public CubicSpline<D> {
		protected:
			// Tangent Helper
			inline Vector<D> tangent(unsigned n) const {
				return m_tangent(this, n);
			}
			
			// Tangent (MU) Function
			typedef function<Vector<D> (const HermiteSpline<D> *spline, unsigned n)> SplineFunctionT;
			SplineFunctionT m_tangent;
			
			typedef Vector<D> PointT;
			typedef std::vector<PointT> PointsT;
		public:
			/// Catmull-Rom tangent function
			static Vector<D> catmullRomSpline (const ISpline<D> *spline, unsigned n);
			
			static Vector<D> fourPointLinearMu (const ISpline<D> *s, unsigned n);
			static Vector<D> fourPointExponentialMu (const ISpline<D>* s, unsigned n);
			
			/// Simple linear tangent function
			static Vector<D> multiPointLinearMu (const ISpline<D>* s, unsigned n);
			
			/// Default constructor. Tangent generation by catmulRomSpline.
			HermiteSpline () : m_tangent(catmullRomSpline) {
			}
			
			/// Catmull-Rom Tangent function
			///	@param		splineFunc	Function pointer for tangent generation
			HermiteSpline (SplineFunctionT splineFunc) : m_tangent(splineFunc) {
			}
			
			virtual Vector<D> pointAtTime(RealT t) const;
		};
	}
}

#endif