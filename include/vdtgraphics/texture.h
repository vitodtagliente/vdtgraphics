/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include "image.h"

namespace graphics
{
	class Texture
	{
	public:

		struct Options
		{
			Options();

			// Wrapping mode on S axis
			unsigned int wrapS;
			// Wrapping mode on T axis 
			unsigned int wrapT;
			// Filtering mode if texture pixels < screen pixels
			unsigned int filterMin;
			// Filtering mode if texture pixels > screen pixels
			unsigned int filterMax;
		};

		Texture(const Image& image, const Options& options = Options{});

		inline unsigned int id() const { return m_id; }
		inline bool isValid() const { return m_id != INVALID_ID; }

		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }

		virtual void bind() = 0;
		virtual void unbind() = 0;

		static constexpr unsigned int INVALID_ID = 0;

	protected:

		// texture id
		unsigned int m_id;
		// texture size
		unsigned int m_width, m_height;
	};
}