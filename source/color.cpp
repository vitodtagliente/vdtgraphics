#include <vdtgraphics/color.h>
#include <algorithm>

namespace graphics
{
	Color::Color()
		: m_red()
		, m_green()
		, m_blue()
		, m_alpha()
	{
	}

	Color::Color(const unsigned int red, const unsigned int green, const unsigned int blue)
		: m_alpha(1.0f)
	{
		m_red = std::clamp(static_cast<float>(red) / 255.0f, 0.0f, 1.0f);
		m_green = std::clamp(static_cast<float>(green) / 255.0f, 0.0f, 1.0f);
		m_blue = std::clamp(static_cast<float>(blue) / 255.0f, 0.0f, 1.0f);
	}

	Color::Color(const float red, const float green, const float blue, const float alpha)
	{
		m_red = std::clamp(red, 0.0f, 1.0f);
		m_green = std::clamp(green, 0.0f, 1.0f);
		m_blue = std::clamp(blue, 0.0f, 1.0f);
		m_alpha = std::clamp(alpha, 0.0f, 1.0f);
	}

	void Color::setRed(const float value)
	{
		m_red = std::clamp(value, 0.0f, 1.0f);
	}

	void Color::setGreen(const float value)
	{
		m_green = std::clamp(value, 0.0f, 1.0f);
	}

	void Color::setBlue(const float value)
	{
		m_blue = std::clamp(value, 0.0f, 1.0f);
	}

	void Color::setAlpha(const float value)
	{
		m_alpha = std::clamp(value, 0.0f, 1.0f);
	}

	bool Color::operator==(const Color& color) const
	{
		return m_red == color.m_red &&
			m_green == color.m_green &&
			m_blue == color.m_blue &&
			m_alpha == color.m_alpha;
	}

	bool Color::operator!=(const Color& color) const
	{
		return !(*this == color);
	}

	Color Color::operator+(const Color& color) const
	{
		return {
			m_red + color.m_red,
			m_green + color.m_green,
			m_blue + color.m_blue,
			m_alpha + color.m_alpha
		};
	}

	Color Color::operator-(const Color& color) const
	{
		return {
			m_red - color.m_red,
			m_green - color.m_green,
			m_blue - color.m_blue,
			m_alpha - color.m_alpha
		};
	}

	Color& Color::operator+=(const Color& color)
	{
		m_red = std::clamp(m_red + color.m_red, 0.0f, 1.0f);
		m_green = std::clamp(m_green + color.m_green, 0.0f, 1.0f);
		m_blue = std::clamp(m_blue + color.m_blue, 0.0f, 1.0f);
		m_alpha = std::clamp(m_alpha + color.m_alpha, 0.0f, 1.0f);
		return *this;
	}

	Color& Color::operator-=(const Color& color)
	{
		m_red = std::clamp(m_red - color.m_red, 0.0f, 1.0f);
		m_green = std::clamp(m_green - color.m_green, 0.0f, 1.0f);
		m_blue = std::clamp(m_blue - color.m_blue, 0.0f, 1.0f);
		m_alpha = std::clamp(m_alpha - color.m_alpha, 0.0f, 1.0f);
		return *this;
	}

	std::string Color::toString() const
	{
		return "color {r: " + std::to_string(m_red) +
			", g: " + std::to_string(m_green) +
			", b: " + std::to_string(m_blue) +
			", a: " + std::to_string(m_alpha) + " }";
	}

	const Color Color::White(1.0f, 1.0f, 1.0f);
	const Color Color::Black(0.0f, 0.0f, 0.0f);
	const Color Color::Red(1.0f, 0.0f, 0.0f);
	const Color Color::Green(0.0f, 1.0f, 0.0f);
	const Color Color::Blue(0.0f, 0.0f, 1.0f);
	const Color Color::Cyan(0.0f, 1.0f, 1.0f);
	const Color Color::Yellow(1.0f, 1.0f, 0.0f);
	const Color Color::Magenta(1.0f, 0.0f, 1.0f);
	const Color Color::Transparent(0.0f, 0.0f, 0.0f, 0.0f);
}