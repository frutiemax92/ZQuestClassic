#ifndef ZC_GUI_SIZE_H
#define ZC_GUI_SIZE_H

#include <cstddef>
#include <cstdint>

namespace GUI
{

/* A simple wrapper to force sizes into units. */
class Size
{
public:
    // These are templates to resolve annoying ambiguity issues.

    template<typename T>
    static inline Size em(T t)
    {
		return Size(static_cast<int32_t>(t * static_cast<T>(emSize())));
    }

    template<typename T>
    static inline Size largePixels(T t) noexcept
    {
        return Size::sized(static_cast<int32_t>(t));
    }

    template<typename T>
    static inline Size smallPixels(T t) noexcept
    {
        return Size::sized(static_cast<int32_t>(t)*3/2);
    }

    template<typename T>
	static inline constexpr Size pixels(T t) noexcept
	{
		return Size(static_cast<int32_t>(t));
	}

    /* Returns the size as the actual number of pixels. */
	inline constexpr int32_t resolve() const
	{
		return value;
	}
	
	inline constexpr operator int32_t() const noexcept
	{
		return value;
	}
	
	inline constexpr Size operator+(const int32_t v) const noexcept
	{
		return Size(value+v);
	}
	
	inline constexpr Size operator+(const Size& other) const noexcept
	{
		return Size(value+other.value);
	}
	
	inline constexpr Size operator*(const int32_t v) const noexcept
	{
		return Size(value*v);
	}
	
	inline constexpr Size operator*(const Size& other) const noexcept
	{
		return Size(value*other.value);
	}
	inline friend Size operator*(const int32_t v, const Size s);
	
	inline constexpr Size operator/(const int32_t v) const noexcept
	{
		return Size(value/v);
	}
	
	inline constexpr Size operator/(const Size& other) const noexcept
	{
		return Size(value/other.value);
	}
	inline friend Size operator/(const int32_t v, const Size s);
	
	inline constexpr Size operator-(const int32_t& other) const noexcept
	{
		return Size(value-other);
	}
	
	inline constexpr Size operator-(const Size& other) const noexcept
	{
		return Size(value-other.value);
	}
	inline friend Size operator-(const Size s, const int32_t v);
	inline friend Size operator-(const int32_t v, const Size s);
	inline constexpr bool operator<(const Size& other) const noexcept
	{
		return value < other.value;
	}

	inline constexpr bool operator>(const Size& other) const noexcept
	{
		return value > other.value;
	}

	inline constexpr bool operator==(const Size& other) const noexcept
	{
		return value == other.value;
	}

	inline constexpr bool operator!=(const Size& other) const noexcept
	{
		return value != other.value;
	}

private:
	int32_t value;

	inline constexpr Size(int32_t raw) noexcept: value(raw) {}
    static int32_t emSize();
    static Size sized(int32_t size) noexcept;
};

inline Size operator ""_em(uint64_t size)
{
	return Size::em(size);
}

inline Size operator ""_em(long double size)
{
	return Size::em(size);
}

inline constexpr Size operator ""_px(uint64_t size)
{
	return Size::pixels(size);
}

inline Size operator ""_lpx(uint64_t size)
{
	return Size::largePixels(size);
}

inline Size operator ""_spx(uint64_t size)
{
	return Size::smallPixels(size);
}

inline Size operator-(const Size s, const int32_t v)
{
	return Size(s.value - v);
}

inline Size operator-(const int32_t v, const Size s)
{
	return Size(v - s.value);
}

inline Size operator*(const int32_t v, const Size s)
{
	return Size(v*s.value);
}

inline Size operator/(const int32_t v, const Size s)
{
	return Size(v/s.value);
}

}

#endif
