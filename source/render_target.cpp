#include <vdtgraphics/render_target.h>

#include <glad/glad.h>

#include <vdtgraphics/texture.h>

namespace graphics
{
	RenderTarget::RenderTarget(const int width, const int height, const Color& color)
		: m_width(width)
		, m_height(height)
		, color(color)
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// create the color buffer
		m_texture = std::make_unique<Texture>(nullptr, width, height, 3);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->id(), 0);

		// create depth/stencil buffer
		glGenRenderbuffers(1, &m_depthId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthId);

		// Check for completeness
		int32_t completeStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (completeStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			m_errorMessage = "Invalid FrameBuffer";
			m_state = State::Error;
		}
		else
		{
			m_state = State::Ready;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	RenderTarget::~RenderTarget()
	{
		free();
	}

	void RenderTarget::resize(const int width, const int height)
	{
		if (width == m_width && height == m_height) return;

		m_width = width;
		m_height = height;

		m_texture->bind();
		m_texture->resize(width, height);
		m_texture->unbind();

		glBindRenderbuffer(GL_RENDERBUFFER, m_depthId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void RenderTarget::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glEnable(GL_DEPTH_TEST);
	}

	void RenderTarget::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
	}

	void RenderTarget::free()
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_depthId);
		m_texture->free();
	}
}