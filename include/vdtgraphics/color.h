/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <string>

namespace graphics
{
	class Color
	{
	public:

		Color();
		Color(const unsigned int red, const unsigned int green, const unsigned int blue);
		Color(const float red, const float green, const float blue, const float alpha = 1.0f);

		Color(const Color& color) = default;
		Color(Color&& color) = default;
		Color& operator= (const Color& color) = default;
		Color& operator=(Color&& color) = default;

		inline float red() const { return m_red; }
		inline float green() const { return m_green; }
		inline float blue() const { return m_blue; }
		inline float alpha() const { return m_alpha; }

		void setRed(const float value);
		void setGreen(const float value);
		void setBlue(const float value);
		void setAlpha(const float value);

		bool operator== (const Color& color) const;
		bool operator!= (const Color& color) const;

		Color operator+ (const Color& color) const;
		Color operator- (const Color& color) const;
		Color& operator+= (const Color& color);
		Color& operator-= (const Color& color);

		std::string toString() const;

		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Cyan;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Transparent;

	private:
		
		float m_red;
		float m_green;
		float m_blue;
		float m_alpha;
	};
}