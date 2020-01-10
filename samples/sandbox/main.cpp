#include <iostream>
#include <vdtgraphics/api/opengl/opengl.h>
#include <GLFW/glfw3.h>
#include <vdtgraphics/graphics.h>

using namespace std;
using namespace graphics;

void render_loop();
GraphicsAPI* api = nullptr;
Renderer* renderer = nullptr;

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
    renderer->drawCircle(Color::Green, {}, .4f);
    renderer->render();
}