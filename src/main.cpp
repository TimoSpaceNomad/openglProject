#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

void updateFPSCounter(GLFWwindow* window, double lastTime, unsigned int frames) {
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    frames++;

    if (delta >= 1.0) {
        double fps = double(frames) / delta;

        std::string title = "FPS: " + std::to_string(fps);
        glfwSetWindowTitle(window, title.c_str());

        frames = 0;
        lastTime = currentTime;
    }
}

int main() 
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 1024, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] = {
            -1.f, -1.f, 0.0f, 0.0f, // 0
             1.f, -1.f, 1.0f, 0.0f, // 1
             1.f,  1.f, 1.0f, 1.0f, // 2
            -1.f,  1.f, 0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2, // first triangle
            2, 3, 0  // second triangle
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("res/textures/streamPNG/");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbing(); 
        ib.Unbing();
        shader.Unbind();

        Renderer renderer;

        double lastTime = glfwGetTime();
        int currentTextureIndex = 0;
        double lastTextureChangeTime = 0;
        unsigned int frames = 0;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            renderer.Clear();

            shader.Bind();

            double currentTime = glfwGetTime();
            if (currentTime - lastTextureChangeTime >= 0.25) {
                currentTextureIndex = (currentTextureIndex + 1) % 32;
                lastTextureChangeTime = currentTime;

                texture.Bind(currentTextureIndex);
                shader.SetUniform1i("u_Texture", currentTextureIndex);
            }

            renderer.Draw(va, ib, shader);

            updateFPSCounter(window, lastTime, frames);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}