#include <vdtgraphics/api/opengl/texture_gl.h>
#include <vdtgraphics/api/opengl/opengl.h>

namespace graphics
{
	TextureGL::TextureGL(const Image& image, const Options& options /* = Options */)
		: Texture(image, options)
	{
		// generate the texture
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		/* set the texture wrapping/filtering options (on the currently bound texture object) */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, options.wrapS == 0 ? GL_REPEAT : options.wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, options.wrapT == 0 ? GL_REPEAT : options.wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, options.filterMin == 0 ? GL_LINEAR : options.filterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, options.filterMax == 0 ? GL_LINEAR : options.filterMax);

		unsigned int format = 0;
		switch (image.getChannels())
		{
		case 1: format = GL_RED; break;
		case 4: format = GL_RGBA; break;
		case 3: 
		default:
			format = GL_RGB; break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, image.getWidth(), image.getHeight(),
			0, format, GL_UNSIGNED_BYTE, image.getData()
		);
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