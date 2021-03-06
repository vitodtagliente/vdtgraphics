#include <vdtgraphics/renderer_3d.h>

#include <vdtgraphics/api.h>

namespace graphics
{

	Renderer3D::Renderer3D(API* const api)
		: Renderer(api)
	{
	}

	/*
	Renderer::Renderer(GraphicsAPI* const api)
		: m_api(api)
		, m_commandBuffer()
		, m_materialLibrary(new MaterialLibrary(api))
		, m_textureLibrary(new TextureLibrary(api))
		, m_quad(api->createRenderable(Quad{})) 
		, m_circle(api->createRenderable(Circle{})) 
		, m_drawingMode(DrawingMode::Fill)
	{

	}
	
	Renderer::~Renderer()
	{
	}
	
	void Renderer::setDrawingMode(const DrawingMode drawingMode)
	{
		m_drawingMode = drawingMode;
	}

	void Renderer::push(Renderable* const renderable, Material* const material, const matrix4& transform)
	{
		m_commandBuffer.push_back(RenderCommand(renderable, material, transform));
	}

	void Renderer::render()
	{
		for (const RenderCommand& command : m_commandBuffer.getCommands())
		{
			// bind the material
			command.material->bind();
			// bind the data to render
			command.renderable->bind();
			// render the command
			drawIndexed(static_cast<unsigned int>(command.renderable->getMesh().indices.size()));
		}

		m_commandBuffer.clear();
	}
	
	void Renderer::drawTexture(Texture* const texture, const vector2& position)
	{
		drawTexture(
			texture, 
			matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTexture(Texture* const texture, const vector2& position, const vector2& scale)
	{
		drawTexture(
			texture, 
			matrix4::scale(to_vec3(scale)) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTexture(Texture* const texture, const vector2& position, const float theta)
	{
		drawTexture(
			texture,
			matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTexture(Texture* const texture, const vector2& position, const float theta, const vector2& scale)
	{
		drawTexture(
			texture,
			matrix4::scale(to_vec3(scale)) * matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTexture(Texture* const texture, const matrix4& transform)
	{
		Material* const material = m_materialLibrary->get(Shaders::names::TextureShader);

		if (material != nullptr)
		{
			Material* const materialInstance = material->createInstance();

			materialInstance->set(Shaders::params::ModelViewProjectionMatrix, transform);
			materialInstance->set(Shaders::params::Texture, texture);
			push(m_quad, materialInstance, transform);
		}
	}

	void Renderer::drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position)
	{
		drawTextureCrop(
			texture,
			rect,
			matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position, const vector2& scale)
	{
		drawTextureCrop(
			texture,
			rect,
			matrix4::scale(to_vec3(scale)) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position, const float theta)
	{
		drawTextureCrop(
			texture,
			rect,
			matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTextureCrop(Texture* const texture, const rect& rect, const vector2& position, const float theta, const vector2& scale)
	{
		drawTextureCrop(
			texture,
			rect,
			matrix4::scale(to_vec3(scale)) * matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawTextureCrop(Texture* const texture, const rect& rect, const matrix4& transform)
	{
		static Material* const material = m_materialLibrary->get(Shaders::names::CroppedTextureShader);

		if (material != nullptr)
		{
			Material* const materialInstance = material->createInstance();

			materialInstance->set(Shaders::params::ModelViewProjectionMatrix, transform);
			materialInstance->set(Shaders::params::Texture, texture);
			materialInstance->set(Shaders::params::TextureCropping, vec4(
				rect.x, rect.y, rect.width, rect.height
			));
			push(m_quad, materialInstance, transform);
		}
	}

	void Renderer::drawRect(const Color& color, const vector2& position)
	{
		drawRect(
			color,
			matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawRect(const Color& color, const vector2& position, const vector2& scale)
	{
		drawRect(
			color,
			matrix4::scale(to_vec3(scale)) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawRect(const Color& color, const vector2& position, const float theta)
	{
		drawRect(
			color,
			matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawRect(const Color& color, const vector2& position, const float theta, const vector2& scale)
	{
		drawRect(
			color,
			matrix4::scale(to_vec3(scale)) * matrix4::rotate_z(theta) * matrix4::translate(to_vec3(position))
		);
	}

	void Renderer::drawRect(const Color& color, const matrix4& transform)
	{
		static Material* const material = m_materialLibrary->get(Shaders::names::ColorShader);
		
		if (material != nullptr)
		{
			// memory leak issue
			Material* const materialInstance = material; // material->createInstance();
			materialInstance->set(Shaders::params::Color, color);
			materialInstance->set(Shaders::params::ModelViewProjectionMatrix, transform);
			push(m_quad, materialInstance, transform);
		}
	}

	void Renderer::drawCircle(const Color& color, const vector2& position, const float radius)
	{
		static Material* const material = m_materialLibrary->get(Shaders::names::ColorShader);

		if (material != nullptr)
		{
			matrix4 transform = matrix4::scale(vec3(radius, radius, 0.0f)) * matrix4::translate(to_vec3(position));
			Material* const materialInstance = material->createInstance();
			materialInstance->set(Shaders::params::Color, color);
			materialInstance->set(Shaders::params::ModelViewProjectionMatrix, transform);
			push(m_circle, materialInstance, transform);
		}
	}
	
	void Renderer::initializeMaterials()
	{
		for (const std::pair<std::string, std::string>& shader : getDefaultShaderSources())
		{
			std::map<Shader::Type, std::string> sources;
			Shader::Reader::parse(shader.second, sources);
			// create shaders
			auto vertex = m_api->createShader(Shader::Type::Vertex, sources[Shader::Type::Vertex]);
			auto fragment = m_api->createShader(Shader::Type::Fragment, sources[Shader::Type::Fragment]);
			auto program = m_api->createShaderProgram({ vertex, fragment });

			// free shaders
			delete vertex;
			delete fragment;

			if (program->getState() == ShaderProgram::State::Linked)
			{
				Material* const material = new Material(program);
				getMaterialLibrary()->add(shader.first, material);
			}
			else
			{
				delete program;
			}
		}
	}
	*/
}