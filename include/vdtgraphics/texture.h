/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>

#include "image.h"

namespace graphics
{
	class Texture
	{
	public:
		struct Options
		{
			enum class Filter
			{
				Linear,
				LinearMipmap,
				Nearest,
				NearestMipmap,
			};

			enum class Repeat
			{
				Disabled,
				Enabled,
				Mirror
			};

			Filter filter{ Filter::Linear };
			Repeat repeat{ Repeat::Disabled };
		};

		Texture(const unsigned char* const data, unsigned int width, unsigned int height,
			unsigned int channels, const Options& options = Options{});
		Texture(const Image& image, const Options& options = Options{});
		~Texture();

		void fillSubData(int offsetX, int offsetY, int width, int height, unsigned char* const data);
		void resize(int width, int height);

		inline unsigned int id() const { return m_id; }
		inline bool isValid() const { return m_id != 0; }

		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }

		void bind(unsigned int slot = 0);
		void unbind();
		void free();

	protected:

		// texture id
		unsigned int m_id;
		// texture size
		unsigned int m_width, m_height;
		// format of the texture object
		unsigned int m_format;
	};

	typedef std::shared_ptr<Texture> TexturePtr;
}