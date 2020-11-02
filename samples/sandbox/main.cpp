#include <iostream>
#include <vdtgraphics/api/opengl/opengl.h>
#include <GLFW/glfw3.h>
#include <vdtgraphics/graphics.h>
#include <sstream>
#include <vdtmath/math.h>
#include <vdtgraphics/meshes/quad.h>
#include <vdtgraphics/api/opengl/opengl.h>

using namespace std;
using namespace graphics;
using namespace math;

void init();
void render_loop();
API* api = nullptr;
Renderer2D* renderer2d = nullptr;
Texture* batmanTexture = nullptr, * wallTexture = nullptr;

float RandomFloat(float min, float max)
{
    float r = (float)rand() / (float)RAND_MAX;
    return min + r * (max - min);
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
    api->enableAlpha(true);
    renderer2d = api->createRenderer2D();

    Image batmanImg;
    Image::load("batman_logo.png", batmanImg);
    batmanImg.flipVertically();
    batmanTexture = api->createTexture(batmanImg);

    Image wallImg;
    Image::load("texture_atlas.png", wallImg);
    wallImg.flipVertically();
    wallTexture = api->createTexture(wallImg);

    init();

    /* Loop until the user closes the window */
    bool run = true;
    while (!glfwWindowShouldClose(window) && run)
    {
        // display the FPS
        showFPS(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        api->update();

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

VertexBuffer* vb, * vb1;
IndexBuffer* ib;
Material* mtb, *spriteMat;
Renderable* quad, *triangle;

void init()
{ 
    quad = api->createRenderable(Quad{});
    spriteMat = api->getMaterialLibrary().get(Material::Default::Name::Texture);
    spriteMat->set(Material::Default::Property::Textures, std::vector<Texture*>{ batmanTexture });
    spriteMat->set(Material::Default::Property::ModelViewProjectionMatrix, math::matrix4::identity);
    // 
    // // triangle
    // float vertices[] = {
    // -0.5f, -0.5f, 0.0f,
    //  0.5f, -0.5f, 0.0f,
    //  0.0f,  0.5f, 0.0f
    // };
    // vb = api->createVertexBuffer(sizeof(float) * 3, 3);
    // vb->update(vertices, 3);
    // vb->update({ BufferElement("position", BufferElement::Type::Float, 3, sizeof(float) * 3) });

    mtb = api->getMaterialLibrary().get(Material::Default::Name::Position);
}

void render_loop()
{
    renderer2d->clear(Color(0.1f, 0.0f, 0.1, 1.0f));


    // for (int i = 0; i < 50; ++i)
    // {
    //     renderer2d->drawTexture(batmanTexture, { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) });
    //     renderer2d->drawTexture(wallTexture, { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) });
    // }
    // renderer2d->drawTexture(batmanTexture, { 1.0f, 1.0f });
    // renderer2d->drawTexture(wallTexture, { -1.0f, 1.0f });
    // renderer2d->drawRect(Color::Red, { -.3f, -.3f }, { .2f, .4f });
    // renderer2d->drawCircle(Color::Green, {}, .4f);

    // vb1->bind();
    // mtb->bind();
    // api->draw(vb1);

    // spriteMat->bind();
    // api->draw(quad);

    renderer2d->drawTexture(batmanTexture, { 1.0f, 1.0f });

    renderer2d->render();
    
}