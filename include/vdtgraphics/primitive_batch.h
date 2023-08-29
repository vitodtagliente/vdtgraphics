/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>
#include <vector>

#include <vdtmath/matrix4.h>
#include <vdtmath/vector2.h>

#include "common.h"
#include "color.h"
#include "render_command.h"
#include "texture_rect.h"

namespace graphics
{
    class Context;
    class Renderable;
    class ShaderProgram;

    class PrimitiveBatch
    {
    public:
        struct Stats
        {
            int drawCalls{ 0 };
        };

        PrimitiveBatch(Context* const context);
        virtual ~PrimitiveBatch() = default;

        void setProjectionMatrix(const math::matrix4& m);
        void setViewMatrix(const math::matrix4& m);
        const math::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
        const math::matrix4& getViewMatrix() const { return m_viewMatrix; }
        const math::matrix4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

        void flush();

        void drawCircle(ShapeRenderStyle style, const math::vec3& position, float radius, const Color& color);
        void drawLine(const math::vec3& point1, const Color& color1, const math::vec3& point2, const Color& color2);
        void drawShape(ShapeRenderStyle style, const std::vector<Vertex>& vertices);
        void drawRect(ShapeRenderStyle style, const math::vec3& position, float width, float height, const Color& color);

        const Stats& stats() const { return m_stats; }

        std::size_t batch_size = 10000;

    private:
        class RenderPrimitiveCommand final : public RenderCommand
        {
        public:
            RenderPrimitiveCommand(Renderable* const renderable, ShaderProgram* const program, const math::mat4& viewProjectionMatrix, ShapeRenderStyle style, size_t capacity);

            size_t capacity() const { return m_capacity; }
            size_t size() const { return m_size; }
            bool hasCapacity(const size_t numOfVertices) const { return m_capacity - m_size >= numOfVertices; }

            ShapeRenderStyle getStyle() const { return m_style; }

            bool push(const Vertex& vertex);

            virtual RenderCommandResult execute() override;

        private:
            size_t m_capacity;
            std::vector<float> m_data;
            ShaderProgram* m_program;
            Renderable* m_renderable;
            size_t m_size;
            ShapeRenderStyle m_style;
            math::mat4 m_viewProjectionMatrix;
        };

        std::unique_ptr<ShaderProgram> createProgram(const std::string& name);

        std::vector<RenderPrimitiveCommand> m_commands;
        std::unique_ptr<Renderable> m_fill_renderable;
        std::unique_ptr<ShaderProgram> m_program;
        std::unique_ptr<Renderable> m_stroke_renderable;

        // stats
        Stats m_stats;

        // matrices
        math::mat4 m_projectionMatrix{ math::mat4::identity };
        math::mat4 m_viewMatrix{ math::mat4::identity };
        math::mat4 m_viewProjectionMatrix{ math::mat4::identity };
    };
}