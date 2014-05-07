#ifndef __NARAN_GEOMETRY_H__
#define __NARAN_GEOMETRY_H__

#include "core/CommonDef.h"

NS_DEF_NARAN{

	template<typename T>
	class CLS_EXPORT Size2D_
	{
	public:
		T w, h;
		inline Size2D_() : w(0), h(0) {}
		inline Size2D_(T _w, T _h) : w(_w), h(_h) {}
		inline Size2D_(const Size2D_ &copy) : w(copy.w), h(copy.h) {}
		inline Size2D_ &operator = (const Size2D_ &copy){
			w = copy.w;
			h = copy.h;
			return *this;
		}
		inline bool operator == (const Size2D_ &other){
			return w == other.w && h == other.h;
		}
		inline bool operator != (const Size2D_ &other){
			return w != other.w && h != other.h;
		}
	};

	template<typename T>
	class CLS_EXPORT Size3D_
	{
	public:
		T w, h, d;
		inline Size3D_() : w(0), h(0), d(0) {}
		inline Size3D_(T _w, T _h, T _d) : w(_w), h(_h), d(_d) {}
		inline Size3D_(const Size3D_ &copy) : w(copy.w), h(copy.h), d(copy.d) {}
		inline Size3D_ &operator = (const Size3D_ &copy){
			w = copy.w;
			h = copy.h;
			d = copy.d;
			return *this;
		}
		inline bool operator == (const Size3D_ &other){
			return w == other.w && h == other.h && d == other.d;
		}
		inline bool operator != (const Size3D_ &other){
			return w != other.w && h != other.h && d != other.d;
		}
	};

	template<typename T>
	class CLS_EXPORT Point2D_
	{
	public:
		T x, y;
		inline Point2D_() : x(0), y(0) {}
		inline Point2D_(T _x, T _y) : x(_x), y(_y) {}
		inline Point2D_(const Point2D_ &copy) : x(copy.x), y(copy.y) {}
		inline Point2D_ &operator = (const Point2D_ &copy){
			x = copy.x;
			y = copy.y;
			return *this;
		}
		inline bool operator == (const Point2D_ &other){
			return x == other.x && y == other.y;
		}
		inline bool operator != (const Point2D_ &other){
			return x != other.x && y != other.y;
		}
		inline Point2D_ operator +(const Size2D_<T> &size){
			return Point2D_(x+size.w, y+size.h);
		}
	};

	template<typename T>
	class CLS_EXPORT Point3D_
	{
	public:
		T x, y, z;
		inline Point3D_() : x(0), y(0), z(0) {}
		inline Point3D_(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
		inline Point3D_(const Point3D_ &copy) : x(copy.x), y(copy.y), z(copy.z) {}
		inline Point3D_ &operator = (const Point3D_ &copy){
			x = copy.x;
			y = copy.y;
			z = copy.z;
			return *this;
		}
		inline bool operator == (const Point2D_ &other){
			return x == other.x && y == other.y && z == other.z;
		}
		inline bool operator != (const Point2D_ &other){
			return x != other.x && y != other.y && z != other.z;
		}
		inline Point3D_ operator +(const Size3D_<T> &size){
			return Point2D_(x+size.w, y+size.h, z+size.d);
		}
	};

	template<typename T>
	class CLS_EXPORT Rect_
	{
	public:
		Point2D_<T> pt;
		Size2D_<T> size;
		inline Rect_() {}
		inline Rect_(const Point2D_<T> &_pt, const Size2D_<T> &_size) : pt(_pt), size(_size) {}
		inline Rect_(T x, T y, T w, T h) : pt(x, y), size(w, h) {}
		inline Rect_(const Point2D_<T> &pt1, const Point2D_<T> &pt2) 
			: pt(MATH_MIN(pt1.x, pt2.x), MATH_MIN(pt1.y, pt2.y))
			, size(MATH_ABS(pt2.x-pt1.x), MATH_ABS(pt2.y-pt1.y)){}
		inline Rect_ &operator = (const Rect_ &copy){
			pt = copy.pt;
			size = copy.size;
			return *this;
		}
		inline bool operator == (const Point2D_<T> &other){
			return pt == other.pt && size == other.size;
		}
		inline bool operator != (const Point2D_<T> &other){
			return pt != other.pt && size != other.size;
		}
		inline Point2D_<T> getFar(){
			return pt + size;
		}
		inline bool isIntersect(const Rect_ &other){
			Point2D_<T> far = getFar();
			Point2D_<T> other_far = other.getFar();
			return (far.x > other.pt.x &&
				far.y > other.pt.y &&
				other_far.x > pt.x &&
				other_far.y > pt.y);
		}
		inline Rect_ intersect(const Rect_ &other){
			Point2D_<T> far = getFar();
			Point2D_<T> other_far = other.getFar();
			T x = MATH_MAX(pt.x, other.pt.x);
			T y = MATH_MAX(pt.y, other.pt.y);
			T fx = MATH_MIN(far.x, other_far.x);
			T fy = MATH_MIN(far.y, other_far.y);
			return Rect_(x, y, (x>fx ? 0 : fx-x), (y>fy ? 0 : fy-y));
		}
		inline Rect_ surround(const Rect_ &other){
			Point2D_<T> far = getFar();
			Point2D_<T> other_far = other.getFar();
			T x = MATH_MIN(pt.x, other.pt.x);
			T y = MATH_MIN(pt.y, other.pt.y);
			T fx = MATH_MAX(far.x, other_far.x);
			T fy = MATH_MAX(far.y, other_far.y);
			return Rect_(x, y, fx-x, fy-y);
		}
	};

	template<typename T>
	class CLS_EXPORT Bound_
	{
	public:
		Point3D_<T> pt;
		Size3D_<T> size;
		inline Bound_() {}
		inline Bound_(const Point3D_<T> &_pt, const Size3D_<T> &_size) : pt(_pt), size(_size) {}
		inline Bound_(T x, T y, T z, T w, T h, T d) : pt(x, y, z), size(w, h, d) {}
		inline Bound_(const Point3D_<T> &pt1, const Point3D_<T> &pt2) 
			: pt(MATH_MIN(pt1.x, pt2.x), MATH_MIN(pt1.y, pt2.y), MATH_MIN(pt1.z, pt2.z))
			, size(MATH_ABS(pt2.x-pt1.x), MATH_ABS(pt2.y-pt1.y), MATH_ABS(pt2.z-pt1.z)){}
		inline Bound_ &operator = (const Bound_ &copy){
			pt = copy.pt;
			size = copy.size;
			return *this;
		}
		inline bool operator == (const Point3D_<T> &other){
			return pt == other.pt && size == other.size;
		}
		inline bool operator != (const Point3D_<T> &other){
			return pt != other.pt && size != other.size;
		}
		inline Point3D_<T> getFar(){
			return pt + size;
		}
		inline bool isIntersect(const Bound_ &other){
			Point3D_<T> far = getFar();
			Point3D_<T> other_far = other.getFar();
			return (far.x > other.pt.x &&
				far.y > other.pt.y &&
				far.z > other.pt.z &&
				other_far.x > pt.x &&
				other_far.y > pt.y &&
				other_far.z > pt.z);
		}
		inline Bound_ intersect(const Bound_ &other){
			Point3D_<T> far = getFar();
			Point3D_<T> other_far = other.getFar();
			T x = MATH_MAX(pt.x, other.pt.x);
			T y = MATH_MAX(pt.y, other.pt.y);
			T z = MATH_MAX(pt.z, other.pt.z);
			T fx = MATH_MIN(far.x, other_far.x);
			T fy = MATH_MIN(far.y, other_far.y);
			T fz = MATH_MIN(far.z, other_far.z);
			return Bound_(x, y, z, (x>fx ? 0 : fx-x), (y>fy ? 0 : fy-y), (z>fz ? 0 : fz-z));
		}
		inline Bound_ surround(const Bound_ &other){
			Point3D_<T> far = getFar();
			Point3D_<T> other_far = other.getFar();
			T x = MATH_MIN(pt.x, other.pt.x);
			T y = MATH_MIN(pt.y, other.pt.y);
			T z = MATH_MIN(pt.z, other.pt.z);
			T fx = MATH_MAX(far.x, other_far.x);
			T fy = MATH_MAX(far.y, other_far.y);
			T fz = MATH_MAX(far.z, other_far.z);
			return Bound_(x, y, z, fx-x, fy-y, fz-z);
		}
	};

	namespace g2d{
		typedef Size2D_<int>		Sizei;
		typedef Size2D_<float>		Sizef;
		typedef Point2D_<int>		Pointi;
		typedef Point2D_<float>		Pointf;
		typedef Rect_<int>			Recti;
		typedef Rect_<float>		Rectf;
	};

	namespace g3d{
		typedef Size3D_<int>		Sizei;
		typedef Size3D_<float>		Sizef;
		typedef Point3D_<int>		Pointi;
		typedef Point3D_<float>		Pointf;
		typedef Bound_<int>			Boundi;
		typedef Bound_<float>		Boundf;
	};

}

#endif

