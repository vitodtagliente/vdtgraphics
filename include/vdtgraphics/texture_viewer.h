/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <memory>

#include "color.h"
#include "texture_rect.h"

namespace graphics
{
    class Context;
    class Renderable;
    class ShaderProgram;
    class Texture;

    class TextureViewer
    {
    public:
        TextureViewer(Context* const context);
        virtual ~TextureViewer() = default;

        void draw(Texture* const texture, const Color& color = Color::White);
        void draw(unsigned int texture, unsigned int channels, const Color& color = Color::White);

    private:
        std::unique_ptr<ShaderProgram> createProgram(const std::string& name);

        std::unique_ptr<ShaderProgram> m_program;
        std::unique_ptr<ShaderProgram> m_bitmap_program;
        std::unique_ptr<Renderable> m_renderable;
    };
}