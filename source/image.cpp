#include <vdtgraphics/image.h>

#include <algorithm>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"

namespace graphics
{
	Image::Image()
		: m_pixels()
		, m_width()
		, m_height()
		, m_channels(4)
	{
	}

	Image::Image(const Color& color, const unsigned int width, const unsigned int height)
		: m_pixels()
		, m_width()
		, m_height()
		, m_channels(4)

	{
		if (width && height)
		{
			// Create a new pixel buffer first for exception safety's sake
			std::vector<unsigned char> newPixels(static_cast<std::size_t>(width) * height * 4);

			unsigned char r = static_cast<unsigned char>(color.getRed() * 255);
			unsigned char g = static_cast<unsigned char>(color.getGreen() * 255);
			unsigned char b = static_cast<unsigned char>(color.getBlue() * 255);
			unsigned char a = static_cast<unsigned char>(color.getAlpha() * 255);

			// Fill it with the specified color
			unsigned char* ptr = &newPixels[0];
			unsigned char* end = ptr + newPixels.size();
			while (ptr < end)
			{
				*ptr++ = r;
				*ptr++ = g;
				*ptr++ = b;
				*ptr++ = a;
			}

			// Commit the new pixel buffer
			m_pixels.swap(newPixels);
		}
	}

	Image::Image(unsigned char* const data, const unsigned int width, const unsigned int height, const unsigned int channels)
		: m_pixels()
		, m_width(width)
		, m_height(height)
		, m_channels(channels)
	{
		m_pixels.resize(static_cast<std::size_t>(width) * height * channels);
		std::memcpy(&m_pixels[0], data, m_pixels.size());
	}

	Image::Image(const Image& image)
		: m_pixels(image.m_pixels)
		, m_width(image.m_width)
		, m_height(image.m_height)
		, m_channels(image.m_channels)
	{
	}

	Image::~Image()
	{

	}

	Image& Image::operator=(const Image& image)
	{
		m_pixels = image.m_pixels;
		m_width = image.m_width;
		m_height = image.m_height;
		m_channels = image.m_channels;

		return *this;
	}
	
	void Image::free()
	{
		std::free(&m_pixels[0]);
	}

	bool Image::load(const std::string& filename, Image& image)
	{
		int width = 0;
		int height = 0;
		int channels = 0;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (data != nullptr)
		{
			image.m_pixels.resize(static_cast<std::size_t>(width) * height * channels);
			std::memcpy(&image.m_pixels[0], data, image.m_pixels.size());
			image.m_width = width;
			image.m_height = height;
			image.m_channels = channels;

			stbi_image_free(data);

			return true;
		}
		return false;
	}

	void Image::flipHorizontally()
	{
		if (!m_pixels.empty())
		{
			unsigned int rowSize = m_width * m_channels;

			for (std::size_t y = 0; y < m_height; ++y)
			{
				std::vector<unsigned char>::iterator left = m_pixels.begin() + y * rowSize;
				std::vector<unsigned char>::iterator right = m_pixels.begin() + (y + 1) * rowSize - 4;

				for (std::size_t x = 0; x < m_width / 2; ++x)
				{
					std::swap_ranges(left, left + 4, right);

					left += 4;
					right -= 4;
				}
			}
		}
	}

	void Image::flipVertically()
	{
		if (!m_pixels.empty())
		{
			unsigned int rowSize = m_width * 4;

			std::vector<unsigned char>::iterator top = m_pixels.begin();
			std::vector<unsigned char>::iterator bottom = m_pixels.end() - rowSize;

			for (std::size_t y = 0; y < m_height / 2; ++y)
			{
				std::swap_ranges(top, top + rowSize, bottom);

				top += rowSize;
				bottom -= rowSize;
			}
		}
	}
}