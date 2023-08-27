/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>
#include <vector>

#include <vdtmath/matrix4.h>
#include <vdtmath/vector2.h>

#include "color.h"
#include "font.h"
#include "render_command.h"
#include "texture_rect.h"

namespace graphics
{
    class Context;
    class Renderable;
    class ShaderProgram;
    class Texture;

    class TextBatch
    {
    public:

        struct Stats
        {
            int drawCalls{ 0 };
        };

        TextBatch(Context* const context);
        virtual ~TextBatch() = default;

        void setProjectionMatrix(const math::matrix4& m);
        void setViewMatrix(const math::matrix4& m);
        const math::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
        const math::matrix4& getViewMatrix() const { return m_viewMatrix; }
        const math::matrix4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

        void flush();

        void draw(Font* const font, const std::string& text, const math::vec3& position, std::size_t size = 16, const Color& color = Color::White);

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