#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vdtgraphics/graphics.h>
#include <vdtmath/math.h>

using namespace std;
using namespace graphics;
using namespace math;

void init();
void render_loop();

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
std::unique_ptr<RenderTarget> renderTarget;
std::unique_ptr<Renderer> renderer;
math::vec3 mouse;
math::transform camera;

void showFPS(GLFWwindow* pWindow)
{
	static int s_frames = 0;
	static double s_timer = 1.0;

	s_timer -= deltaTime;
	++s_frames;
	if (s_timer <= 0.0)
	{
		std::stringstream ss;
		ss << "vdtgraphics" << " " << "1.0" << " [" << s_frames << " FPS] DrawCalls[" << renderer->stats.drawCalls << "]";

		glfwSetWindowTitle(pWindow, ss.str().c_str());

		s_frames = 0;
		s_timer = 1.0;
	}
}

int main(void)
{
	GLFWwindow* window;

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

	renderer = std::make_unique<Renderer>();
	renderer->init(context.get());
	renderTarget = std::make_unique<RenderTarget>(screenSize.x, screenSize.y, Color::Cyan);
	// renderer->setRenderTarget(renderTarget.get());

	init();

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow*, const int width, const int height)
		{
			screenSize.x = width;
			screenSize.y = height;
			renderer->setViewport(width, height);
			renderTarget->resize(width, height);
		}
	);

	glfwSetCursorPosCallback(window,
		[](GLFWwindow* window, double xpos, double ypos)
		{
			mouse.x = (float)xpos;
			mouse.y = (float)ypos;
		}
	);

	// remove the fps cap
	glfwSwapInterval(0);

	/* Loop until the user closes the window */
	bool run = true;
	while (!glfwWindowShouldClose(window) && run)
	{
		// delta time
		{
			double currentTime = glfwGetTime();
			deltaTime = currentTime - lastTick;
			lastTick = currentTime;
		}

		// display the FPS
		showFPS(window);

		// render logic
		render_loop();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

Font font;
TexturePtr circleTexture;
TexturePtr potatoeTexture;
TexturePtr squareTexture;

void init()
{
	circleTexture = std::make_unique<Texture>(Image::load("../../../assets/circle.png"));
	potatoeTexture = std::make_unique<Texture>(Image::load("../../../assets/spritesheet.png"));
	squareTexture = std::make_unique<Texture>(Image::load("../../../assets/square.png"));
	font = Font::load("../../../assets/font.ttf");
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
	renderer->submitDrawRect(ShapeRenderStyle::fill, math::vec3::zero, 10.f, 10.f, Color::Magenta);
	renderer->submitDrawRect(ShapeRenderStyle::stroke, math::vec3(-5.4f, 5.3f, 0.0f), 5.f, 5.f, Color::Green);
	renderer->submitDrawCircle(ShapeRenderStyle::stroke, math::vec3::zero, 15.f, Color::Yellow);
	renderer->submitDrawLine(math::vec3(-10.f, -10.f, 0.f), Color::Red, math::vec3(10.f, 10.f, 0.f), Color::Yellow);
	renderer->submitDrawTexture(potatoeTexture.get(), math::vec3::zero);
	renderer->submitDrawTexture(circleTexture.get(), math::vec3::zero, math::vec3(10.f, 10.f, 1.f), {}, Color::Cyan);
	renderer->submitDrawTexture(squareTexture.get(), math::vec3(5.f, 5.f, 0.f), math::vec3(5.f, 5.f, 1.f), {}, Color::Green);
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
		for (int i = 0; i < 5000; ++i)
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
		renderer->submitDrawTexture(potatoeTexture.get(), entity.transform.matrix(), entity.rect);
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
	math::vec3 objcoords = math::mat4::unproject(screencoords, renderer->getViewMatrix(), renderer->getProjectionMatrix(), viewport);
	objcoords.z = 0.5f;
	renderer->submitDrawTexture(circleTexture.get(), objcoords, math::vec3(.5f, .5f, 1.0f), {}, Color::Cyan);
}

// text rendering
void testCase4()
{
	renderer->submitDrawText(&font, "Hello vdtgraphics!", math::vec3(-4.f, 0.f, 1.f), 1.f, Color::White);
}

void render_loop()
{
	static bool s_test_render_target = false;

	renderer->setWireframeMode(false);

	if (s_test_render_target)
	{
		renderer->setRenderTarget(renderTarget.get());
	}

	const float aspectRatio = 1.0f;
	renderer->setViewMatrix(Camera::view(camera));
	// 32 pixel per unit
	renderer->setProjectionMatrix(Camera::ortho(-10.f, 100.f, screenSize.x / 32, screenSize.y / 32, aspectRatio));

	renderer->clear(Color(0.0f, 0.0f, 0.2f, 1.0f));
	renderer->setViewport(screenSize.x, screenSize.y);
	testCase1();
	testCase2();
	testCase3();
	testCase4();

	if (s_test_render_target)
	{
		renderer->setRenderTarget(nullptr);
		renderer->clear(Color(0.0f, 0.0f, 0.2f, 1.0f));
		renderer->setViewport(screenSize.x, screenSize.y);
		renderer->setProjectionMatrix(math::mat4::scale({ 1.f, -1.f, 1.f }));
		renderer->setViewMatrix(math::mat4::identity);
		renderer->submitDrawTexture(renderTarget->getTexture(), math::vec3::zero);
	}

	renderer->flush();
}