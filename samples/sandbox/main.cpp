#include <iostream>
#include <vdtgraphics/api/opengl/opengl.h>
#include <GLFW/glfw3.h>
#include <vdtgraphics/graphics.h>
#include <sstream>

using namespace std;
using namespace graphics;

void render_loop();
GraphicsAPI* api = nullptr;
Renderer* renderer = nullptr;

int nbFrames = 0;
double lastTime = 0;
void showFPS(GLFWwindow* pWindow)
{
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if (delta >= 1.0) { // If last cout was more than 1 sec ago
        cout << 1000.0 / double(nbFrames) << endl;

        int fps = double(nbFrames) / delta;

        std::stringstream ss;
        ss << "vdtgraphics" << " " << "1.0" << " [" << fps << " FPS]";

        glfwSetWindowTitle(pWindow, ss.str().c_str());

        nbFrames = 0;
        lastTime = currentTime;
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // load GL
    //gladLoadGL();

    api = GraphicsAPI::Factory::get();
    api->startup();
    renderer = api->createRenderer();
    renderer->initializeMaterials();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // display the FPS
        showFPS(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // render logic
        render_loop();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    api->shutdown();
    glfwTerminate();
    return 0;
}

void render_loop()
{
    renderer->clear(Color::Black);
    renderer->drawRect(Color::Blue, { -.3f, -.3f }, { .2f, .4f });
    ////for(int i = 0; i < 300; ++i)
    //renderer->drawCircle(Color::Green, {}, .4f);
    renderer->render();
}