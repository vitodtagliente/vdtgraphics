#include <vdtgraphics/shader_library.h>

namespace graphics
{
	ShaderLibrary::ShaderLibrary()
		: m_shaders()
	{
		m_shaders.insert(std::make_pair(names::ColorShader, R"(
			#shader vertex

			#version 330 core
 
			// an attribute is an input (in) to a vertex shader.
			// It will receive data from a buffer
			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec4 a_color;

			out vec4 v_color;
 
			// all shaders have a main function
			void main() {
 
				// gl_Position is a special variable a vertex shader
				// is responsible for setting
				gl_Position = a_position;
				v_color = a_color;
			}

			#shader fragment

			#version 330 core

			// fragment shaders don't have a default precision so we need
			// to pick one. highp is a good default. It means "high precision"
			precision highp float;
 
			in vec4 v_color;
			out vec4 outColor;
 
			void main() {
				// Just set the output to a constant reddish-purple
				outColor = v_color;
			}		
		)"
		));
		m_shaders.insert(std::make_pair(names::PolygonBatchShader, R"(
			#shader vertex

			#version 330 core
 
			// an attribute is an input (in) to a vertex shader.
			// It will receive data from a buffer
			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec4 a_color;

			out vec4 v_color;

			uniform mat4 u_matrix;
 
			// all shaders have a main function
			void main() {
 
				// gl_Position is a special variable a vertex shader
				// is responsible for setting
				gl_Position = u_matrix * a_position;
				v_color = a_color;
			}

			#shader fragment

			#version 330 core

			// fragment shaders don't have a default precision so we need
			// to pick one. highp is a good default. It means "high precision"
			precision highp float;
 
			in vec4 v_color;
			out vec4 outColor;
 
			void main() {
				// Just set the output to a constant reddish-purple
				outColor = v_color;
			}		
		)"
		));
		m_shaders.insert(std::make_pair(names::SpriteBatchShader, R"(
			#shader vertex

			#version 330 core
 
			// an attribute is an input (in) to a vertex shader.
			// It will receive data from a buffer
			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec2 a_texcoord;
			layout(location = 2) in float a_textureIndex;
			layout(location = 3) in vec4 a_crop;
			layout(location = 4) in vec4 a_color;
			layout(location = 5) in mat4 a_transform;

			uniform mat4 u_matrix;
 
			// a varying to pass the texture coordinates to the fragment shader
			out vec2 v_texcoord;
			out float v_textureIndex;
			out vec4 v_crop;
			out vec4 v_color;
 
			void main() {
				// Multiply the position by the matrix.
				gl_Position = u_matrix * a_transform * a_position;
 
				// Pass the texcoord to the fragment shader.
				v_texcoord = a_texcoord;
				v_textureIndex = a_textureIndex;
				v_crop = a_crop;
				v_color = a_color;
			}

			#shader fragment

			#version 330 core
			precision highp float;
 
			// Passed in from the vertex shader.
			in vec2 v_texcoord;
			in float v_textureIndex;
			in vec4 v_crop;
			in vec4 v_color;
 
			// The textures
			uniform sampler2D u_texture0;
			uniform sampler2D u_texture1;
			uniform sampler2D u_texture2;
			uniform sampler2D u_texture3;
			uniform sampler2D u_texture4;
			uniform sampler2D u_texture5;
			uniform sampler2D u_texture6;
			uniform sampler2D u_texture7;
			uniform sampler2D u_texture8;
			uniform sampler2D u_texture9;
			uniform sampler2D u_texture10;
			uniform sampler2D u_texture11;
			uniform sampler2D u_texture12;
			uniform sampler2D u_texture13;
			uniform sampler2D u_texture14;
			uniform sampler2D u_texture15;
 
			out vec4 outColor;
 
			void main() {
				if (v_textureIndex == 0) outColor = texture(u_texture0, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 1) outColor = texture(u_texture1, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 2) outColor = texture(u_texture2, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 3) outColor = texture(u_texture3, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 4) outColor = texture(u_texture4, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 5) outColor = texture(u_texture5, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 6) outColor = texture(u_texture6, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 7) outColor = texture(u_texture7, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 8) outColor = texture(u_texture8, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 9) outColor = texture(u_texture9, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 10) outColor = texture(u_texture10, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 11) outColor = texture(u_texture11, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 12) outColor = texture(u_texture12, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 13) outColor = texture(u_texture13, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 14) outColor = texture(u_texture14, v_texcoord * v_crop.zw + v_crop.xy) * v_color;
				else if (v_textureIndex == 15) outColor = texture(u_texture15, v_texcoord * v_crop.zw + v_crop.xy) * v_color;	
				else outColor = vec4(1, 1, 1, 1);
			
				if (outColor.a < 0.5) discard;
			}
		)"
		));
		m_shaders.insert(std::make_pair(names::TextShader, R"(
			#shader vertex

			#version 330 core
 
			// an attribute is an input (in) to a vertex shader.
			// It will receive data from a buffer
			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec2 a_texcoord;
			layout(location = 2) in float a_textureIndex;
			layout(location = 3) in vec4 a_crop;
			layout(location = 4) in vec4 a_color;
			layout(location = 5) in mat4 a_transform;

			uniform mat4 u_matrix;
 
			// a varying to pass the texture coordinates to the fragment shader
			out vec2 v_texcoord;
			out float v_textureIndex;
			out vec4 v_crop;
			out vec4 v_color;
 
			void main() {
				// Multiply the position by the matrix.
				gl_Position = u_matrix * a_transform * a_position;
 
				// Pass the texcoord to the fragment shader.
				v_texcoord = a_texcoord;
				v_textureIndex = a_textureIndex;
				v_crop = a_crop;
				v_color = a_color;
			}

			#shader fragment

			#version 330 core
			precision highp float;
 
			// Passed in from the vertex shader.
			in vec2 v_texcoord;
			in float v_textureIndex;
			in vec4 v_crop;
			in vec4 v_color;
 
			// The textures
			uniform sampler2D u_texture0;
			uniform sampler2D u_texture1;
			uniform sampler2D u_texture2;
			uniform sampler2D u_texture3;
			uniform sampler2D u_texture4;
			uniform sampler2D u_texture5;
			uniform sampler2D u_texture6;
			uniform sampler2D u_texture7;
			uniform sampler2D u_texture8;
			uniform sampler2D u_texture9;
			uniform sampler2D u_texture10;
			uniform sampler2D u_texture11;
			uniform sampler2D u_texture12;
			uniform sampler2D u_texture13;
			uniform sampler2D u_texture14;
			uniform sampler2D u_texture15;
 
			out vec4 outColor;
 
			void main() {
				if (v_textureIndex == 0) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture0, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 1) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture1, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 2) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture2, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 3) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture3, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 4) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture4, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 5) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture5, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 6) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture6, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 7) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture7, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 8) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture8, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 9) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture9, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 10) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture10, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 11) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture11, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 12) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture12, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 13) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture13, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 14) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture14, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;
				else if (v_textureIndex == 15) outColor = vec4(1.0, 1.0, 1.0, texture(u_texture15, v_texcoord * v_crop.zw + v_crop.xy).r) * v_color;	

				if (outColor.a < 0.5) discard;
			}
		)"
		));
		m_shaders.insert(std::make_pair(names::TextTextureShader, R"(
			#shader vertex

			#version 330 core
 
			// an attribute is an input (in) to a vertex shader.
			// It will receive data from a buffer
			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec2 a_texcoord;

			out vec2 v_texcoord;
 
			// all shaders have a main function
			void main() {
 
				// gl_Position is a special variable a vertex shader
				// is responsible for setting
				gl_Position = a_position;
				v_texcoord = a_texcoord;
			}

			#shader fragment

			#version 330 core

			// fragment shaders don't have a default precision so we need
			// to pick one. highp is a good default. It means "high precision"
			precision highp float;
 
			in vec2 v_texcoord;

			// The texture.
			uniform vec4 u_color;
			uniform sampler2D u_texture;

			out vec4 outColor;
 
			void main() {
				outColor = vec4(1.0, 1.0, 1.0, texture(u_texture0, v_texcoord).r) * u_color;
			}		
		)"
		));
		m_shaders.insert(std::make_pair(names::TextureShader, R"(
			#shader vertex

			#version 330 core
 
			// an attribute is an input (in) to a vertex shader.
			// It will receive data from a buffer
			layout(location = 0) in vec4 a_position;
			layout(location = 1) in vec2 a_texcoord;

			out vec2 v_texcoord;
 
			// all shaders have a main function
			void main() {
 
				// gl_Position is a special variable a vertex shader
				// is responsible for setting
				gl_Position = a_position;
				v_texcoord = a_texcoord;
			}

			#shader fragment

			#version 330 core

			// fragment shaders don't have a default precision so we need
			// to pick one. highp is a good default. It means "high precision"
			precision highp float;
 
			in vec2 v_texcoord;

			// The texture.
			uniform sampler2D u_texture;
			uniform vec4 u_color;

			out vec4 outColor;
 
			void main() {
				outColor = texture(u_texture, v_texcoord) * u_color;
			}		
		)"
		));
	}

	const std::string ShaderLibrary::names::ColorShader = "Color";
	const std::string ShaderLibrary::names::PolygonBatchShader = "PolygonBatch";
	const std::string ShaderLibrary::names::SpriteBatchShader = "SpriteBatch";
	const std::string ShaderLibrary::names::TextShader = "Text";
	const std::string ShaderLibrary::names::TextTextureShader = "TextTexture";
	const std::string ShaderLibrary::names::TextureShader = "Texture";
}