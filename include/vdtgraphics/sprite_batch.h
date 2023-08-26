/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>
#include <vector>

#include <vdtmath/matrix4.h>
#include <vdtmath/vector2.h>

#include "color.h"
#include "render_command.h"
#include "texture_rect.h"

namespace graphics
{
	class Context;
    class Renderable;
    class ShaderProgram;
	class Texture;

    enum class SpriteSortMode
    {
        Deferred,
        Immediate,
        Texture,
        BackToFront,
        FrontToBack,
    };

    enum class SpriteEffects : std::size_t
    {
        None = 0,
        FlipHorizontally = 1,
        FlipVertically = 2,
        FlipBoth = FlipHorizontally | FlipVertically,
    };

	class SpriteBatch
	{
	public:

        struct Stats
        {
            int drawCalls{ 0 };
        };

        SpriteBatch(Context* const context);
        virtual ~SpriteBatch() = default;

        void setProjectionMatrix(const math::matrix4& m);
        void setViewMatrix(const math::matrix4& m);
        const math::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
        const math::matrix4& getViewMatrix() const { return m_viewMatrix; }
        const math::matrix4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

        void flush();

        void draw(Texture* const texture, const math::mat4& matrix, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, float rotation, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, float rotation, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);

        const Stats& stats() const { return m_stats; }

        std::size_t batch_size = 10000;

	private:
        std::unique_ptr<ShaderProgram> createProgram(const std::string& name);

        std::vector<std::unique_ptr<RenderCommand>> m_commands;
        std::unique_ptr<ShaderProgram> m_program;
        std::unique_ptr<Renderable> m_renderable;
        
        // stats
        Stats m_stats;

        // matrices
        math::mat4 m_projectionMatrix{ math::mat4::identity };
        math::mat4 m_viewMatrix{ math::mat4::identity };
        math::mat4 m_viewProjectionMatrix{ math::mat4::identity };
	};
}