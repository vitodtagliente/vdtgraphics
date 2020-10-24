#include <iostream>
#include <vdtgraphics/api/opengl/opengl.h>
#include <GLFW/glfw3.h>
#include <vdtgraphics/graphics.h>
#include <sstream>
#include <vdtmath/math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace graphics;
using namespace math;

void render_loop();
API* api = nullptr;
Renderer2D* renderer2d = nullptr;
Texture* batmanTxt = nullptr;
unsigned char* imageData = nullptr;

bool loadImage(const std::string& filename, Image& image)
{
    int width;
    int height;
    int channels;

    stbi_set_flip_vertically_on_load(1);
    imageData = stbi_load(filename.c_str(), &width, &height, &channels, 4);
    if (imageData != nullptr)
    {
        image = Image(imageData, width, height, channels);
        return true;
    }
    return false;
}

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

    api = API::Factory::get();
    api->startup();
    glEnable(GL_TEXTURE_2D);
    // api->enableAlpha(true);
    renderer2d = api->createRenderer2D();
    Image batmanImg;
    loadImage("batman_logo.png", batmanImg);
    batmanTxt = api->createTexture(batmanImg);

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

    batmanImg.free();

    api->shutdown();
    glfwTerminate();
    return 0;
}

void render_loop()
{
    renderer2d->drawTexture(batmanTxt, {});
    renderer2d->drawRect(Color::Red, { -.3f, -.3f }, { .2f, .4f });
    renderer2d->drawCircle(Color::Green, {}, .4f);
    renderer2d->clear(Color::Blue);
    renderer2d->render();
}