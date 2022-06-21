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

int nbFrames = 0;
double lastTime = 0;
double deltaTime = 0;
int drawCalls = 0;

void showFPS(GLFWwindow* pWindow)
{
	// Measure speed
	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	nbFrames++;
	if (deltaTime >= 1.0) { // If last cout was more than 1 sec ago
		// cout << 1000.0 / double(nbFrames) << endl;

		int fps = double(nbFrames) / deltaTime;

		std::stringstream ss;
		ss << "vdtgraphics" << " " << "1.0" << " [" << fps << " FPS] DrawCalls[" << drawCalls << "]";

		glfwSetWindowTitle(pWindow, ss.str().c_str());

		nbFrames = 0;
		lastTime = currentTime;
	}
}

std::unique_ptr< Renderer> renderer;
math::vec3 mouse;

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	renderer = std::make_unique< Renderer>(640, 480);
	renderer->init();

	init();

	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow*, int width, int height)
		{
			renderer->setViewport(width, height);
		}
	);

	glfwSetCursorPosCallback(window, 
		[](GLFWwindow* window, double xpos, double ypos)
		{
			mouse.x = (float)xpos;
			mouse.y = (float)ypos;
		}
	);

	/* Loop until the user closes the window */
	bool run = true;
	while (!glfwWindowShouldClose(window) && run)
	{
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

Image potetoeImg;
std::unique_ptr<Texture> potatoeTexture;

void init()
{
	renderer->setClearColor(Color(0.0f, 0.0f, 0.2, 1.0f));
	potetoeImg = Image::load("../../../assets/spritesheet.png");
	potatoeTexture = std::make_unique<Texture>(potetoeImg);

	renderer->setProjectionMatrix(math::matrix4::orthographic(-1.f, 1.f, -1.f, 1.f, -30.f, 1000.f));
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

void testCase1()
{
	static const int accuracy = 100;
	static const float radius = 1.f;
	const float step = (2 * math::pi) / accuracy;
	float angle = 0.0f;
	for (int i = 0; i < accuracy; ++i)
	{
		renderer->drawLine(
			math::vec3(radius * std::sin(angle), radius * std::cos(angle), 0),
			math::vec3(-radius * std::sin(angle), -radius * std::cos(angle), 0),
			Color(math::random(0.f, 1.f), math::random(0.f, 1.f), math::random(0.f, 1.f))
		);
		angle += step;
	}

	renderer->drawRect(math::vec3::zero, 1.f, 1.f, Color::Magenta);
	renderer->drawRect(math::vec3(.4f, .3f, 2.1f), .5f, .5f, Color::Green);

	renderer->drawCircle(math::vec3::zero, .5f, Color::Yellow);

	const float s = 1.f / 11;
	renderer->drawTexture(potatoeTexture.get(), math::vec3(.3f, .3f, 2.f), TextureRect(s * 9, s * 1, s, s));
}

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

	statsBegin();

	if (s_entities.empty())
	{
		for (int i = 0; i < 1000; ++i)
		{
			Entity entity;

			const float size = math::random(.05f, .2f);

			entity.transform.position.x = math::random(-.9f, .9f);
			entity.transform.position.y = math::random(-.9f, .9f);
			entity.transform.position.z = 0.0f;

			entity.transform.rotation.z = math::random(0.f, 360.f);

			entity.transform.scale.x = entity.transform.scale.y = size;

			entity.transform.update();

			entity.rotate = math::random(0, 1) == 1;
			if (entity.rotate)
			{
				entity.rotateSpeed = math::random(.2f, .8f);
			}

			const float s = 1.f / 11;
			entity.rect = TextureRect(s * 9, s * math::random(1, 5), s, s);

			s_entities.push_back(entity);
		}

		statsEnd("generate data", true);
	}

	for (Entity& entity : s_entities)
	{
		if (entity.rotate)
		{
			entity.transform.rotation.z += entity.rotateSpeed;
			entity.transform.update();
		}
		renderer->drawTexture(potatoeTexture.get(), entity.transform.matrix(), entity.rect);
	}

	statsEnd("draw textures");
}

void testCase3()
{
	renderer->setStyle(Renderer::StyleType::fill);
	auto worldCoords = renderer->screenToWorldCoords({ mouse.x, mouse.y });
	worldCoords.z = 30.f;
	cout << worldCoords.x << ", " << worldCoords.y;
	renderer->drawRect(worldCoords, .05f, .05f, Color::Cyan);

	renderer->setViewMatrix(math::matrix4::translate({ .3f, .0f, .0f }));
}

void render_loop()
{
	renderer->begin();

	testCase2();
	testCase3();

	drawCalls = renderer->flush();
}