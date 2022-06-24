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

math::vector2_t<int> screenSize(640, 480);

void showFPS(GLFWwindow* pWindow)
{
	static int s_frames = 0;
	static double s_timer = 1.0;

	s_timer -= deltaTime;
	++s_frames;
	if (s_timer <= 0.0) 
	{
		std::stringstream ss;
		ss << "vdtgraphics" << " " << "1.0" << " [" << s_frames << " FPS] DrawCalls[" << drawCalls << "]";

		glfwSetWindowTitle(pWindow, ss.str().c_str());

		s_frames = 0;
		s_timer = 1.0;
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
	window = glfwCreateWindow(screenSize.x, screenSize.y, "Hello World", NULL, NULL);
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
			screenSize.x = width;
			screenSize.y = height;
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

Image potetoeImg;
TexturePtr potatoeTexture;
OrthographicCamera camera;
ParticleSystem particles;

void init()
{
	renderer->setClearColor(Color(0.0f, 0.0f, 0.2, 1.0f));
	potetoeImg = Image::load("../../../assets/spritesheet.png");
	potatoeTexture = std::make_unique<Texture>(potetoeImg);

	particles.duration = 60.f;
	particles.spawnTime = .1f;
	particles.maxParticles = 100;
	particles.particleColorRange = { Color::Blue, Color::White };
	particles.particleLifetimeRange = { 1.f, 2.f };
	particles.particleSizeRange = { .2f, .6f };
	particles.spawnAmountRange = { 1,5 };
	particles.particleSpeedRange = { 1.f, 3.f };
	particles.particlePolygonType = PolygonType::rect;
	particles.particlePolygonStyle = PolygonStyle::fill;
	// particles.particleTexture = potatoeTexture;
	// const float s = 1.f / 11;
	// particles.particleTextureRect = { s * 9, s * 1, s, s };
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

	statsBegin();

	if (s_entities.empty())
	{
		for (int i = 0; i < 1000; ++i)
		{
			Entity entity;

			const float size = math::random(1.f, 1.f);

			entity.transform.position.x = math::random(-9.f, 9.f);
			entity.transform.position.y = math::random(-9.f, 9.f);
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
			entity.rect = TextureRect(s * 9, s * math::random(1, 5), s, s);

			s_entities.push_back(entity);
		}

		statsEnd("generate data", true);
	}

	for (Entity& entity : s_entities)
	{
		if (entity.rotate)
		{
			entity.transform.rotation.z += entity.rotateSpeed * deltaTime;
			entity.transform.update();
		}
		renderer->drawTexture(potatoeTexture.get(), entity.transform.matrix(), entity.rect);
	}

	statsEnd("draw textures");
}

// Screen (input) coords to World coords
// draw a rectangle on mouse position
void testCase3()
{
	renderer->setPolygonStyle(PolygonStyle::fill);
	auto worldCoords = camera.screenToWorldCoords({ mouse.x, mouse.y }, screenSize.x, screenSize.y);
	worldCoords.z = 1.0f;

	particles.position = worldCoords;
	particles.play();
	particles.update((float)deltaTime);
	particles.render(*renderer);
}

void render_loop()
{
	camera.pixelPerfect = true;
	renderer->setProjectionMatrix(camera.getProjectionMatrix(screenSize.x, screenSize.y));
	camera.update();
	renderer->setViewMatrix(camera.getViewMatrix());

	renderer->begin();

	testCase2();
	testCase3();
	
	drawCalls = renderer->flush();
}