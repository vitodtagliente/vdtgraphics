#include <vdtgraphics/render_target.h>

#include <glad/glad.h>

#include <vdtgraphics/texture.h>

namespace graphics
{
	RenderTarget::RenderTarget(const int width, const int height, const Color& color)
		: m_width(width)
		, m_height(height)
		, m_color(color)
	{
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// Create colour texture
		m_texture = std::make_unique<Texture>(nullptr, width, height, 4);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->id(), 0);

		// Create depth/stencil renderbuffer
		glGenRenderbuffers(1, &m_depthId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthId);

		// Check for completeness
		int32_t completeStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (completeStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	RenderTarget::~RenderTarget()
	{
		glDeleteFramebuffers(1, &m_id);
	}
}