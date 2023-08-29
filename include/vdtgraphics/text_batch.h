/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>
#include <vector>

#include <vdtmath/matrix4.h>
#include <vdtmath/vector2.h>

#include "color.h"
#include "common.h"
#include "render_command.h"
#include "texture_rect.h"

namespace graphics
{
    class Context;
    class Font;
    class Renderable;
    class ShaderProgram;
    class Texture;

    class TextBatch
    {
    public:
        struct Stats
        {
            std::size_t draw_calls{ 0 };
            std::size_t draw_entities{ 0 };
        };

        TextBatch(Context* const context);
        virtual ~TextBatch() = default;

        void setProjectionMatrix(const math::matrix4& m);
        void setViewMatrix(const math::matrix4& m);
        const math::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
        const math::matrix4& getViewMatrix() const { return m_viewMatrix; }
        const math::matrix4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

        void clear();
        void flush();

        void draw(Font* const font, const std::string& text, const math::vec3& position, std::size_t size = 16, const Color& color = Color::White);

        const Stats& stats() const { return m_stats; }

        std::size_t batch_size = 10000;

    private:
        class RenderTextCommand : public RenderCommand
        {
        public:
            RenderTextCommand(Renderable* const renderable, ShaderProgram* const program, size_t capacity);

            size_t capacity() const { return m_capacity; }
            size_t size() const { return m_size; }
            bool hasCapacity(const size_t numOfFonts) const { return m_capacity - m_size >= numOfFonts; }

            inline const std::vector<Font*>& getFonts() const { return m_fonts; }
            bool hasCapacity(Font* const font) const;

            void clear();
            bool push(const SpriteVertex& vertex, Font* const font);

            virtual RenderCommandResult execute(const math::mat4& viewProjectionMatrix) override;

        private:
            size_t m_capacity;
            std::vector<float> m_data;
            std::vector<Font*> m_fonts;
            ShaderProgram* m_program;
            Renderable* m_renderable;
            size_t m_size;

            static constexpr size_t max_font_units = 16;
        };

        std::unique_ptr<ShaderProgram> createProgram(const std::string& name);

        std::vector<RenderTextCommand> m_commands;
        std::unique_ptr<ShaderProgram> m_program;
        std::unique_ptr<Renderable> m_renderable;

        // stats
        Stats m_stats;
        std::size_t m_last_used_commands{ 0 };

        // matrices
        math::mat4 m_projectionMatrix{ math::mat4::identity };
        math::mat4 m_viewMatrix{ math::mat4::identity };
        math::mat4 m_viewProjectionMatrix{ math::mat4::identity };
    };
}