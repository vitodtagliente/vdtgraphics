/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <string>

namespace graphics
{
	class Image
	{
	public:

		Image();
		Image(unsigned char* const data, const unsigned int width, const unsigned int height, const unsigned int channels);
		Image(const Image& image);
		~Image();

		inline unsigned char* const getData() const { return m_data; }
		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }
		inline unsigned int getChannels() const { return m_channels; }

		Image& operator=(const Image& image);

		void free();

	private:

		// image data
		unsigned char* m_data;
		// image width
		unsigned int m_width;
		// image height
		unsigned int m_height;
		// num of components
		unsigned int m_channels;
	};
}