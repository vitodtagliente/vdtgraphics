#include <vdtgraphics/texture.h>

#include <glad/glad.h>

namespace graphics
{
	Texture::Texture(const unsigned char* const data, const unsigned int width, const unsigned int height,
		const unsigned int channels, const Options& options /* = Options */)
		: m_id()
		, m_width(width)
		, m_height(height)
		, m_format(channels)
	{
		// generate the texture
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		unsigned int filter = 0;
		switch (options.filter)
		{
		case Options::Filter::LinearMipmap: filter = GL_LINEAR_MIPMAP_LINEAR; break;
		case Options::Filter::Nearest: filter = GL_NEAREST; break;
		case Options::Filter::NearestMipmap: filter = GL_NEAREST_MIPMAP_NEAREST; break;
		default:
		case Options::Filter::Linear: filter = GL_LINEAR; break;
		}

		unsigned int repeat = 0;
		switch (options.repeat)
		{
		case Options::Repeat::Enabled: filter = GL_REPEAT; break;
		case Options::Repeat::Mirror: filter = GL_MIRRORED_REPEAT; break;
		default:
		case Options::Repeat::Disabled: filter = GL_CLAMP_TO_EDGE; break;
		}

		/* set the texture wrapping/filtering options (on the currently bound texture object) */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		if (channels == 1)
			m_format = GL_RED;
		else if (channels == 3)
			m_format = GL_RGB;
		else if (channels == 4)
			m_format = GL_RGBA;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height,
			0, m_format, GL_UNSIGNED_BYTE, data
		);
		if (data != nullptr)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	Texture::Texture(const Image& image, const Options& options)
		: Texture(image.data.get(), image.width, image.height, image.channels, options)
	{
	}

	Texture::~Texture()
	{
		free();
	}

	void Texture::fillSubData(const int offsetX, const int offsetY, const int width, const int height, unsigned char* const data)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, m_format, GL_UNSIGNED_BYTE, data);
	}

	void Texture::resize(const int width, const int height)
	{
		m_width = width;
		m_height = height;
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height,
			0, m_format, GL_UNSIGNED_BYTE, nullptr
		);
	}

	void Texture::bind(const unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::free()
	{
		glDeleteTextures(1, &m_id);
	}
}