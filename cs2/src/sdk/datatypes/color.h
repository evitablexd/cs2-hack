#pragma once

struct col_t {
	col_t() = default;
	col_t(int r, int g, int b) { set(r, g, b, 255); }
	col_t(int r, int g, int b, int a) { set(r, g, b, a); }
	col_t(const col_t& col, int a) { set(col.r(), col.g(), col.b(), a); }

	std::array<uint8_t, 4> m_value = { };

	__forceinline void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { m_value = { r, g, b, a }; }

	__forceinline int r() const { return m_value.at(0u); }

	__forceinline int g() const { return m_value.at(1u); }

	__forceinline int b() const { return m_value.at(2u); }

	__forceinline int a() const { return m_value.at(3u); }

	__forceinline uint32_t hex(bool rgba = false) const
	{
		return rgba
			? ((r() & 0xFF) << 24) + ((g() & 0xFF) << 16) + ((b() & 0xFF) << 8) + (a() & 0xFF)
			: ((a() & 0xFF) << 24) + ((b() & 0xFF) << 16) + ((g() & 0xFF) << 8) + (r() & 0xFF);
	}

	__forceinline col_t alpha(int value) const { return col_t(r(), g(), b(), value); }

	__forceinline col_t convert_imgui()
	{
		return col_t(r() * 255.f, g() * 255.f, b() * 255.f, a() * 255.f);
	}


	__forceinline col_t& operator= (const col_t& value)
	{
		set(value.r(), value.g(), value.b(), value.a());

		return *this;
	}

	__forceinline col_t& operator-= (uint8_t value)
	{
		set(r() - value, g() - value, b() - value, a() - value);

		return *this;
	}

	__forceinline col_t& operator+= (uint8_t value)
	{
		set(r() + value, g() + value, b() + value, a() + value);

		return *this;
	}

	__forceinline col_t& operator/= (uint8_t value)
	{
		set(r() / value, g() / value, b() / value, a() / value);

		return *this;
	}

	__forceinline col_t& operator*= (uint8_t value)
	{
		set(r() * value, g() * value, b() * value, a() * value);

		return *this;
	}

	__forceinline col_t& operator-= (const col_t& value)
	{
		set(r() - value.r(), g() - value.g(), b() - value.b(), a() - value.a());

		return *this;
	}

	__forceinline col_t& operator+= (const col_t& value)
	{
		set(r() + value.r(), g() + value.g(), b() + value.b(), a() + value.a());

		return *this;
	}

	__forceinline col_t operator- (uint8_t value) const { return col_t(r() - value, g() - value, b() - value, a() - value); }

	__forceinline col_t operator+ (uint8_t value) const { return col_t(r() + value, g() + value, b() + value, a() + value); }

	__forceinline col_t operator/ (uint8_t value) const { return col_t(r() / value, g() / value, b() / value, a() / value); }

	__forceinline col_t operator* (uint8_t value) const { return col_t(r() * value, g() * value, b() * value, a() * value); }

	__forceinline col_t operator- (const col_t& value) const { return col_t(r() - value.r(), g() - value.g(), b() - value.b(), a() - value.a()); }

	__forceinline col_t operator+ (const col_t& value) const { return col_t(r() + value.r(), g() + value.g(), b() + value.b(), a() + value.a()); }

	__forceinline bool operator== (const col_t& value) const { return *const_cast<col_t*> (this) == *const_cast<col_t*> (&value); }

	__forceinline bool operator!= (const col_t& value) const { return !(operator== (value)); }

	__forceinline float hue() const
	{
		const auto red = r() / 255.f;
		const auto green = g() / 255.f;
		const auto blue = b() / 255.f;

		const auto max = std::max<float>({ red, green, blue });
		const auto min = std::min<float>({ red, green, blue });

		if (max == min)
			return 0.f;

		const auto delta = max - min;

		auto hue = 0.f;

		if (max == red) {
			hue = (green - blue) / delta;
		}
		else if (max == green) {
			hue = 2.f + (blue - red) / delta;
		}
		else {
			hue = 4.f + (red - green) / delta;
		}

		hue *= 60.f;

		if (hue < 0.f) {
			hue += 360.f;
		}

		return hue / 360.f;
	}

	__forceinline float saturation() const
	{
		const auto red = r() / 255.f;
		const auto green = g() / 255.f;
		const auto blue = b() / 255.f;

		const auto max = std::max<float>({ red, green, blue });
		const auto min = std::min<float>({ red, green, blue });

		const auto delta = max - min;

		if (max == 0.f)
			return delta;

		return delta / max;
	}

	__forceinline float brightness() const { return std::max<float>({ r() / 255.f, g() / 255.f, b() / 255.f }); }

	static col_t from_hsb(float hue, float saturation, float brightness)
	{

		hue = std::clamp(hue, 0.f, 1.f);
		saturation = std::clamp(saturation, 0.f, 1.f);
		brightness = std::clamp(brightness, 0.f, 1.f);

		float h = (hue == 1.f) ? 0.f : (hue * 6.f);
		float f = h - static_cast<int> (h);
		float p = brightness * (1.f - saturation);
		float q = brightness * (1.f - saturation * f);
		float t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1.f)
			return col_t((int)(brightness * 255), (int)(t * 255), (int)(p * 255));
		else if (h < 2.f)
			return col_t((int)(q * 255), (int)(brightness * 255), (int)(p * 255));
		else if (h < 3.f)
			return col_t((int)(p * 255), (int)(brightness * 255), (int)(t * 255));
		else if (h < 4)
			return col_t((int)(p * 255), (int)(q * 255), (int)(brightness * 255));
		else if (h < 5)
			return col_t((int)(t * 255), (int)(p * 255), (int)(brightness * 255));
		else
			return col_t((int)(brightness * 255), (int)(p * 255), (int)(q * 255));
	}

	static col_t lerp(col_t a, col_t b, float t)
	{
		return col_t
		(
			a.r() + (b.r() - a.r()) * t,
			a.g() + (b.g() - a.g()) * t,
			a.b() + (b.b() - a.b()) * t,
			a.a() + (b.a() - a.a()) * t
		);
	}

	struct palette_t {
		static col_t red(int alpha = 255) { return col_t(255, 0, 0, alpha); }
		static col_t green(int alpha = 255) { return col_t(0, 255, 0, alpha); }
		static col_t blue(int alpha = 255) { return col_t(0, 0, 255, alpha); }

		static col_t white(int alpha = 255) { return col_t(255, 255, 255, alpha); }
		static col_t black(int alpha = 255) { return col_t(13, 13, 13, alpha); }
		static col_t grey(int alpha = 255) { return col_t(60, 60, 60, alpha); }
		static col_t orange(int alpha = 255) { return col_t(245, 171, 53, alpha); }

		static col_t light_black(int alpha = 255) { return col_t(32, 32, 32, alpha); }
		static col_t light_grey(int alpha = 255) { return col_t(77, 77, 77, alpha); }
		static col_t light_green(int alpha = 255) { return col_t(127, 255, 0, alpha); }
		static col_t light_red(int alpha = 255) { return col_t(236, 98, 95, alpha); }
		static col_t light_blue(int alpha = 255) { return col_t(42, 87, 233, alpha); }
		static col_t light_violet(int alpha = 255) { return col_t(153, 153, 255, alpha); }
		static col_t light_purple(int alpha = 255) { return col_t(127, 73, 208, alpha); }

		static col_t dark_grey(int alpha = 255) { return col_t(44, 44, 44, alpha); }
		static col_t dark_blue(int alpha = 255) { return col_t(14, 18, 134, alpha); }

		static col_t purple(int alpha = 255) { return col_t(220, 0, 220, alpha); }
		static col_t pink(int alpha = 255) { return col_t(255, 105, 180, alpha); }
	};
};