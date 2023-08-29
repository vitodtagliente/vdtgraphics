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
            std::size_t draw_calls{ 0 };
            std::size_t draw_entities{ 0 };
        };

        SpriteBatch(Context* const context);
        virtual ~SpriteBatch() = default;

        void setProjectionMatrix(const math::matrix4& m);
        void setViewMatrix(const math::matrix4& m);
        const math::matrix4& getProjectionMatrix() const { return m_projectionMatrix; }
        const math::matrix4& getViewMatrix() const { return m_viewMatrix; }
        const math::matrix4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

        void clear();
        void flush();

        void draw(Texture* const texture, const math::mat4& matrix, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, float rotation, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);
        void draw(Texture* const texture, const math::vec3& position, float rotation, const math::vec3& scale, const TextureRect& rect = {}, const Color& color = Color::White);

        const Stats& stats() const { return m_stats; }

        std::size_t batch_size = 10000;

	private:        
        class RenderSpriteCommand : public RenderCommand
        {
        public:
            RenderSpriteCommand(Renderable* const renderable, ShaderProgram* const program, size_t capacity);

            size_t capacity() const { return m_capacity; }
            size_t size() const { return m_size; }
            bool hasCapacity(const size_t numOfTextures) const { return m_capacity - m_size >= numOfTextures; }

            inline const std::vector<Texture*>& getTextures() const { return m_textures; }
            bool hasCapacity(Texture* const texture) const;

            bool push(const SpriteVertex& vertex, Texture* const texture);

            void clear();
            virtual RenderCommandResult execute(const math::mat4& viewProjectionMatrix) override;

        private:
            size_t m_capacity;
            std::vector<float> m_data;
            ShaderProgram* m_program;
            Renderable* m_renderable;
            size_t m_size;
            std::vector<Texture*> m_textures;

            static constexpr size_t max_texture_units = 16;
        };
        
        std::unique_ptr<ShaderProgram> createProgram(const std::string& name);

        std::vector<RenderSpriteCommand> m_commands;
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