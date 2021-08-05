#pragma once
#pragma warning( disable : 4244)

template <typename T> class Rect {
public:
	T x;
	T y;
	T width;
	T height;

	Rect():
	x(T()), y(T()), width(T()), height(T())
	{
	}
	
	Rect(T x, T y, T w, T h):
	 x(x), y(y), width(w), height(h)
	{
	}

		
	T& area() const
	{
		T area_ = T(width * height);
		return area_;
	}

	inline static const Rect<T>& zero() { return Rect(T(), T(), T(), T()); }

	inline bool operator==(const Rect<T>& rhs)
	{
		return  this->area() == rhs.area();
	}
/*	inline bool operator< (const Rect<T>& rhs) { return (T&)this->area()	 < (T&)rhs.area();	}
	inline bool operator> (const Rect<T>& rhs) { return (T&)rhs.area()	< (T&)this->area(); }
	inline bool operator<=(const Rect<T>& rhs) { return !((T&)this->area() > (T&)rhs.area());	}
	inline bool operator>=(const Rect<T>& rhs) { return !((T&)this->area() < (T&)rhs.area());	}
	*/
private:
	
};

using iRect = Rect<int>;
using lRect = Rect<long>;
using fRect = Rect<float>;
using dRect = Rect<double>;