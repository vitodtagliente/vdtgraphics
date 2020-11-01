/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <string>
#include <vector>
#include "color.h"

namespace graphics
{
	class Image
	{
	public:

		Image();
		Image(const Color& color, const unsigned int width, const unsigned int height);
		Image(unsigned char* const data, const unsigned int width, const unsigned int height, const unsigned int channels);
		Image(const Image& image);
		~Image();

		inline unsigned const char* getData() const { return &m_pixels[0]; }
		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }
		inline unsigned int getChannels() const { return m_channels; }

		Image& operator=(const Image& image);

		void free();

		static bool load(const std::string& filename, Image& image);

		void flipHorizontally();
		void flipVertically();

	private:

		// image data
		std::vector<unsigned char> m_pixels;
		// image width
		unsigned int m_width;
		// image height
		unsigned int m_height;
		// num of components
		unsigned int m_channels;
	};
}