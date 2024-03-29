#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vdtgraphics/graphics.h>
#include <vdtmath/math.h>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD 
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "texture_viewer.h"

using namespace std;
using namespace graphics;
using namespace math;

void init();
void render_loop();
void update();

float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

double lastTick = 0;
double deltaTime = 0;
int drawCalls = 0;

math::vector2_t<int> screenSize(720, 480);

std::unique_ptr<Context> context;
std::unique_ptr<PrimitiveBatch> primitive_batch;
std::unique_ptr<SpriteBatch> sprite_batch;
std::unique_ptr<TextBatch> text_batch;
std::unique_ptr<RenderTarget> render_target;
std::unique_ptr<TextureViewer> texture_viewer;

math::vec3 mouse;
math::transform camera;


// last mouse wheel position
math::vec2 lastMouseWheelPosition;
// mouse wheel position
math::vec2 mouseWheelPosition;
// delta mouse wheel position
math::vec2 deltaMouseWheelPosition;
// zoom
float zoom_speed{ 2.0f };

int input_number = 0;

void setMouseWheelPosition(const double x, const double y)
{
	lastMouseWheelPosition = mouseWheelPosition;
	mouseWheelPosition = { static_cast<float>(x), static_cast<float>(y) };
	deltaMouseWheelPosition = mouseWheelPosition - lastMouseWheelPosition;
}

void showFPS(GLFWwindow* pWindow)
{
	static int s_frames = 0;
	static double s_timer = 1.0;

	s_timer -= deltaTime;
	++s_frames;
	if (s_timer <= 0.0)
	{
		std::stringstream ss;
		const std::size_t draw_calls = sprite_batch->stats().draw_calls
			+ primitive_batch->stats().draw_calls
			+ text_batch->stats().draw_calls
			;
		const std::size_t draw_sprites = sprite_batch->stats().draw_entities;
		const std::size_t draw_primitives = primitive_batch->stats().draw_entities;
		const std::size_t draw_text = text_batch->stats().draw_entities;
		ss << "vdtgraphics" << " " << "1.0" << " [" << s_frames << " FPS] draw_calls[" << draw_calls << "] draw_sprites[" << draw_sprites << "] draw_primitives[" << draw_primitives << "] draw_text[" << draw_text << "]";

		glfwSetWindowTitle(pWindow, ss.str().c_str());

		s_frames = 0;
		s_timer = 1.0;
	}
}

int main(void)
{
	GLFWwindow* window;
	bool test_ui = false;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(screenSize.x, screenSize.y, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	context = std::make_unique<Context>();
	if (context->initialize() != Context::State::Initialized)
	{
		glfwTerminate();
		return -1;
	}

	if (test_ui)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	}

	primitive_batch = std::make_unique<PrimitiveBatch>(context.get());
	sprite_batch = std::make_unique<SpriteBatch>(context.get());
	text_batch = std::make_unique<TextBatch>(context.get());
	render_target = std::make_unique<RenderTarget>(screenSize.x, screenSize.y, Color::Black);
	texture_viewer = std::make_unique<TextureViewer>(context.get());

	init();

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow*, const int width, const int height)
		{
			screenSize.x = width;
			screenSize.y = height;
			render_target->resize(width, height);
			context->viewport(width, height);
		}
	);

	glfwSetCursorPosCallback(window,
		[](GLFWwindow* window, double xpos, double ypos)
		{
			mouse.x = (float)xpos;
			mouse.y = (float)ypos;
		}
	);

	// mouse wheel
	glfwSetScrollCallback(window,
		[](GLFWwindow*, const double x, const double y)
		{
			setMouseWheelPosition(x, y);
		}
	);

	// remove the fps cap
	glfwSwapInterval(0);

	/* Loop until the user closes the window */
	bool run = true;
	while (!glfwWindowShouldClose(window) && run)
	{
		/* Poll for and process events */
		glfwPollEvents();
		// renderer->submitClear(graphics::Color::Black);
		// renderer->draw();

		// delta time
		{
			double currentTime = glfwGetTime();
			deltaTime = currentTime - lastTick;
			lastTick = currentTime;
		}

		// display the FPS
		showFPS(window);

		update();

		if (test_ui)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			// ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		}

		// render logic
		render_loop();

		if (test_ui)
		{
			// render your GUI
			ImGui::Begin("Demo window");
			ImGui::SetWindowSize(ImVec2(200, 200));
			ImGui::Button("Hello!");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("Hint text");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
			ImGui::InputInt("Numero", &input_number);
			ImGui::End();

			// Render dear imgui into screen
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
				
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		context->viewport(display_w, display_h);
		glfwSwapBuffers(window);
	}

	if (test_ui)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	glfwTerminate();
	return 0;
}

Font font;
TexturePtr circleTexture;
TexturePtr potatoeTexture;
TexturePtr squareTexture;
TexturePtr testTexture;
TexturePtr heroTexture;

void init()
{
	Texture::Options options2D;
	options2D.filter = Texture::Options::Filter::Nearest;
	options2D.repeat = Texture::Options::Repeat::Disabled;

	circleTexture = std::make_unique<Texture>(Image::load("../../../assets/circle.png"));
	potatoeTexture = std::make_unique<Texture>(Image::load("../../../assets/spritesheet.png"), options2D);
	squareTexture = std::make_unique<Texture>(Image::load("../../../assets/square.png"));
	heroTexture = std::make_unique<Texture>(Image::load("../../../assets/hero_spritesheet.png"), options2D);

	Image potatoeImage = Image::load("../../../assets/spritesheet.png");
	testTexture = std::make_unique<Texture>(nullptr, potatoeImage.width, potatoeImage.height, potatoeImage.channels);
	testTexture->fillSubData(0, 0, potatoeImage.width, potatoeImage.height, potatoeImage.data.get());

	font = Font::load("../../../assets/OCRAEXT.ttf", 24);
	font.texture->save("font.png");
}

std::chrono::steady_clock::time_point startTime;
void statsBegin()
{
	startTime = std::chrono::steady_clock::now();
}

void statsEnd(const std::string& context, const bool refresh = false)
{
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	std::cout << "Elapsed Time = " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << "[µs]" << " context[" << context << "]" << std::endl;

	if (refresh)
	{
		statsBegin();
	}
}

// Draw lines
void testCase1()
{
	primitive_batch->drawRect(ShapeRenderStyle::fill, math::vec3::zero, 10.f, 10.f, Color::Magenta);
	primitive_batch->drawRect(ShapeRenderStyle::stroke, math::vec3(-5.4f, 5.3f, 0.0f), 5.f, 5.f, Color::Green);
	primitive_batch->drawCircle(ShapeRenderStyle::stroke, math::vec3::zero, 15.f, Color::Yellow);
	primitive_batch->drawLine(math::vec3(-10.f, -10.f, 0.f), Color::Red, math::vec3(10.f, 10.f, 0.f), Color::Yellow);
	sprite_batch->draw(potatoeTexture.get(), math::vec3::zero);
	sprite_batch->draw(circleTexture.get(), math::vec3::zero, math::vec3(10.f, 10.f, 0.1f), {}, Color::Cyan);
	sprite_batch->draw(squareTexture.get(), math::vec3(5.f, 5.f, 0.f), math::vec3(5.f, 5.f, 0.1f), {}, Color::Green);
}

// Draw different entities
void testCase2()
{
	struct Entity
	{
		math::transform transform;
		bool rotate;
		float rotateSpeed;
		TextureRect rect;
	};

	static std::vector<Entity> s_entities;

	// statsBegin();

	if (s_entities.empty())
	{
		for (int i = 0; i < 8000; ++i)
		{
			Entity entity;

			const float size = math::random(1.f, 1.f);

			entity.transform.position.x = math::random(-20.f, 20.f);
			entity.transform.position.y = math::random(-20.f, 20.f);
			entity.transform.position.z = 0.0f;

			entity.transform.rotation.z = math::random(0.f, 360.f);

			entity.transform.scale.x = entity.transform.scale.y = size;

			entity.transform.update();

			entity.rotate = math::random(0, 1) == 1;
			if (entity.rotate)
			{
				entity.rotateSpeed = math::random(10.f, 30.f);
			}

			const float s = 1.f / 11;
			entity.rect = TextureRect(s * 9, s * math::random(0, 5), s, s);

			s_entities.push_back(entity);
		}

		// statsEnd("generate data", true);
	}

	for (Entity& entity : s_entities)
	{
		if (entity.rotate)
		{
			entity.transform.rotation.z += entity.rotateSpeed * static_cast<float>(deltaTime);
			entity.transform.update();
		}
		sprite_batch->draw(potatoeTexture.get(), entity.transform.matrix(), entity.rect);
	}

	// statsEnd("draw textures");
}

// Screen (input) coords to World coords
// draw a rectangle on mouse position
void testCase3()
{
	const float depth = 0.f;
	math::vec4 viewport = math::vec4(0, 0, static_cast<float>(screenSize.x), static_cast<float>(screenSize.y));
	math::vec3 screencoords = math::vec3(mouse.x, screenSize.y - mouse.y - 1, depth);
	math::vec3 objcoords = math::mat4::unproject(screencoords, sprite_batch->getViewMatrix(), sprite_batch->getProjectionMatrix(), viewport);
	objcoords.z = 0.5f;
	sprite_batch->draw(circleTexture.get(), objcoords, math::vec3(1.f, 1.f, 1.0f), {}, Color::Coral);
}

void update()
{
	if (deltaMouseWheelPosition.y != 0.f)
	{
		math::vec3& scale = camera.scale;
		scale.x = scale.y = scale.y + zoom_speed * static_cast<float>(deltaTime) * -deltaMouseWheelPosition.y;
	}
	setMouseWheelPosition(0, 0);
}

void render_loop()
{
	context->clear(Color::Gray20);

	const float aspectRatio = 1.0f;
	primitive_batch->setProjectionMatrix(Camera::ortho(-10.f, 100.f, screenSize.x / 64, screenSize.y / 64, aspectRatio)); // 32 pixel per unit
	primitive_batch->setViewMatrix(Camera::view(camera));
	sprite_batch->setProjectionMatrix(Camera::ortho(-10.f, 100.f, screenSize.x / 64, screenSize.y / 64, aspectRatio)); // 32 pixel per unit
	sprite_batch->setViewMatrix(Camera::view(camera));

	text_batch->setProjectionMatrix(math::mat4::orthographic(0.f, static_cast<float>(screenSize.x), static_cast<float>(screenSize.y), 0.f, -10.f, 100.f));
	text_batch->setViewMatrix(math::mat4::identity);
	
	testCase1();
	testCase2();
	testCase3();

	sprite_batch->draw(heroTexture.get(), math::vec3(0.f, 0.f, 2.f), math::vec3(screenSize.x / 64, screenSize.y / 64, 1.f));
	
	text_batch->draw(&font, "vdtgraphics!", math::vec3(static_cast<float>(screenSize.x) / 2.f - 240.f, static_cast<float>(screenSize.y) / 2.f, 5.f), 72, Color::White);

	primitive_batch->flush();
	sprite_batch->flush();
	text_batch->flush();
}