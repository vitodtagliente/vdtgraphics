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
			glViewport(0, 0, width, height);
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
	potetoeImg = Image::load("../../../assets/potatoe.png");
	potatoeTexture = std::make_unique<Texture>(potetoeImg);

	renderer->setProjectionMatrix(math::matrix4::orthographic(-1.f, 1.f, -1.f, 1.f, -30.f, 1000.f));
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

	renderer->drawTexture(potatoeTexture.get(), math::vec3(.3f, .3f, -1.f), math::vec3(1.f, 1.f, 1.f));
}

void testCase2()
{
	static std::vector<math::transform> s_entities;

	if (s_entities.empty())
	{
		for (int i = 0; i < 2000; ++i)
		{
			const float size = math::random(.2f, .4f);
			math::transform transform;

			transform.position.x = math::random(-.9f, .9f);
			transform.position.y = math::random(-.9f, .9f);

			transform.scale.x = transform.scale.y = size;
			transform.update();

			s_entities.push_back(transform);
		}
	}

	for (int i = 0; i < s_entities.size(); ++i)
	{
		renderer->drawTexture(potatoeTexture.get(), s_entities[i].matrix(), {}, Color(math::random(0.f, 1.f), math::random(0.f, 1.f), math::random(0.f, 1.f)));
	}
}

void render_loop()
{
	renderer->begin();

	testCase2();

	drawCalls = renderer->flush();
}