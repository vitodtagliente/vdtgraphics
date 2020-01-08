#include <vdtgraphics/opengl/texture_gl.h>
#include <vdtgraphics/opengl/opengl.h>

namespace graphics
{
	Texture::Options::Options()
		: wrapS(GL_REPEAT)
		, wrapT(GL_REPEAT)
		, filterMin(GL_LINEAR)
		, filterMax(GL_LINEAR)
	{

	}

	TextureGL::TextureGL(const unsigned char* const data, const unsigned int width, const unsigned int height,
		const unsigned int channels, const Options& options /* = Options */)
		: Texture(data, width, height, channels, options)
	{
		// generate the texture
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		/* set the texture wrapping/filtering options (on the currently bound texture object) */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, options.wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, options.wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, options.filterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, options.filterMax);

		if (data)
		{
			if (channels == 1)
				m_format = GL_RED;
			else if (channels == 3)
				m_format = GL_RGB;
			else if (channels == 4)
				m_format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height,
				0, m_format, GL_UNSIGNED_BYTE, data
			);
		}
	}

	TextureGL::~TextureGL()
	{
		glDeleteTextures(1, &m_id);
	}

	void TextureGL::bind()
	{
		bind(0);
	}

	void TextureGL::bind(const unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void TextureGL::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}