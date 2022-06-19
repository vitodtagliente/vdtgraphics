#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <vdtgraphics/graphics.h>
#include <vdtmath/math.h>
// #define IMGUI_IMPL_OPENGL_LOADER_GLAD 
// #include <examples/imgui_impl_glfw.h>
// #include <examples/imgui_impl_opengl3.h>

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

	// imgui init
	// ImGui::CreateContext();
	// ImGui::StyleColorsDark();
	// ImGuiIO& io = ImGui::GetIO(); (void)io;
	// ImGui_ImplGlfw_InitForOpenGL(window, true);
	// ImGui_ImplOpenGL3_Init("#version 330 core");

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

	// ImGui_ImplOpenGL3_Shutdown();
	// ImGui_ImplGlfw_Shutdown();
	// ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

Image batmanImg;
std::unique_ptr<Texture> batmanTexture;

void init()
{
	renderer->setClearColor(Color(0.0f, 0.0f, 0.2, 1.0f));
	batmanImg = Image::load("../../../assets/batman_logo.png");
	batmanTexture = std::make_unique<Texture>(batmanImg);

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

	renderer->drawTexture(batmanTexture.get(), math::vec3(.3f, .3f, -1.f));
}

void testCase2()
{

}

void render_loop()
{
	// ImGui_ImplOpenGL3_NewFrame();
	// ImGui_ImplGlfw_NewFrame();
	// ImGui::NewFrame();
	// 
	// ImGui::Begin("Performance Viewer");
	// ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	// ImGui::End();

	renderer->begin();

	testCase1();

	drawCalls = renderer->flush();

	// ImGui::Render();
	// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}