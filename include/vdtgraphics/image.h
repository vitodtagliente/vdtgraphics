/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <string>

namespace graphics
{
	class Image
	{
	public:

		Image(unsigned char* const data, const int width, const int height, const int channels);
		~Image();

		inline unsigned char* const getData() const { return m_data; }
		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline int getChannels() const { return m_channels; }

	private:

		// image data
		unsigned char* m_data;
		// image width
		int m_width;
		// image height
		int m_height;
		// num of components
		int m_channels;
	};
}