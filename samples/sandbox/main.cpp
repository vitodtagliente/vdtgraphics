#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vdtgraphics/graphics.h>
#include <vdtgraphics/text_renderer.h>
#include <vdtmath/math.h>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD 
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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
std::unique_ptr<TextRenderer> text_renderer;
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
float zoom_speed{ 1.0f };

int input_number = 0;

void showFPS(GLFWwindow* pWindow)
{
	static int s_frames = 0;
	static double s_timer = 1.0;

	s_timer -= deltaTime;
	++s_frames;
	if (s_timer <= 0.0)
	{
		std::stringstream ss;
		const std::size_t draw_calls = sprite_batch->stats().drawCalls
			+ primitive_batch->stats().drawCalls
			+ text_batch->stats().drawCalls
			;
		ss << "vdtgraphics" << " " << "1.0" << " [" << s_frames << " FPS] DrawCalls[" << draw_calls << "]";

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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	}

	primitive_batch = std::make_unique<PrimitiveBatch>(context.get());
	sprite_batch = std::make_unique<SpriteBatch>(context.get());
	text_batch = std::make_unique<TextBatch>(context.get());
	texture_viewer = std::make_unique<TextureViewer>(context.get());

	text_renderer = std::make_unique<TextRenderer>(screenSize.x, screenSize.y);

	init();

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow*, const int width, const int height)
		{
			screenSize.x = width;
			screenSize.y = height;
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
			lastMouseWheelPosition = mouseWheelPosition;
			mouseWheelPosition = { static_cast<float>(x), static_cast<float>(y) };
			deltaMouseWheelPosition = mouseWheelPosition - lastMouseWheelPosition;
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
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		}

		// render logic
		render_loop();

		if (test_ui)
		{
			// render your GUI
			ImGui::Begin("Demo window");
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

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/*
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glfwSwapBuffers(window);
		*/
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

void init()
{
	Texture::Options options2D;
	options2D.filter = Texture::Options::Filter::Nearest;

	circleTexture = std::make_unique<Texture>(Image::load("../../../assets/circle.png"));
	potatoeTexture = std::make_unique<Texture>(Image::load("../../../assets/spritesheet.png"), options2D);
	squareTexture = std::make_unique<Texture>(Image::load("../../../assets/square.png"));

	Image potatoeImage = Image::load("../../../assets/spritesheet.png");
	testTexture = std::make_unique<Texture>(nullptr, potatoeImage.width, potatoeImage.height, potatoeImage.channels);
	testTexture->fillSubData(0, 0, potatoeImage.width, potatoeImage.height, potatoeImage.data.get());

	font = Font::load("../../../assets/Font.ttf", 24);
	font.texture->save("font.png");

	text_renderer->Load("../../../assets/Font.ttf", 24);
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
	sprite_batch->draw(circleTexture.get(), math::vec3::zero, math::vec3(10.f, 10.f, 1.f), {}, Color::Cyan);
	sprite_batch->draw(squareTexture.get(), math::vec3(5.f, 5.f, 0.f), math::vec3(5.f, 5.f, 1.f), {}, Color::Green);
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
	// const float depth = 0.f;
	// math::vec4 viewport = math::vec4(0, 0, static_cast<float>(screenSize.x), static_cast<float>(screenSize.y));
	// math::vec3 screencoords = math::vec3(mouse.x, screenSize.y - mouse.y - 1, depth);
	// math::vec3 objcoords = math::mat4::unproject(screencoords, renderer->getViewMatrix(), renderer->getProjectionMatrix(), viewport);
	// objcoords.z = 0.5f;
	// renderer->submitDrawTexture(circleTexture.get(), objcoords, math::vec3(.5f, .5f, 1.0f), {}, Color::Coral);
}

void update()
{
	if (deltaMouseWheelPosition.y != 0.f)
	{
		math::vec3& scale = camera.scale;
		scale.x = scale.y = scale.x + zoom_speed * static_cast<float>(deltaTime) * -deltaMouseWheelPosition.y;
	}
}

void render_loop()
{
	context->clear(Color::Black);

	const float aspectRatio = 1.0f;
	primitive_batch->setProjectionMatrix(Camera::ortho(-10.f, 100.f, screenSize.x / 64, screenSize.y / 64, aspectRatio)); // 32 pixel per unit
	primitive_batch->setViewMatrix(Camera::view(camera));
	sprite_batch->setProjectionMatrix(Camera::ortho(-10.f, 100.f, screenSize.x / 64, screenSize.y / 64, aspectRatio)); // 32 pixel per unit
	sprite_batch->setViewMatrix(Camera::view(camera));

	text_batch->setProjectionMatrix(math::mat4::orthographic(0.f, screenSize.x, 0.f, screenSize.y, -10.f, 100.f));
	text_batch->setViewMatrix(math::mat4::identity);

	// testCase1();
	// testCase2();
	// testCase3();

	text_batch->draw(&font, "ciao", math::vec3(50.f, 100.f, .1f), 5.f, Color::Red);
	text_renderer->RenderText("foooo", 200.f, 50.f, 1.f, Color::Green);

	primitive_batch->flush();
	sprite_batch->flush();
	text_batch->flush();
}