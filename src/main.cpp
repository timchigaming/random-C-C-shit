#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void glfwWindowSizeChangedCallback(GLFWwindow* ptr_window, int width, int height);
void glfwKeyCallback(GLFWwindow* ptr_window, int key, int scancode, int action, int mode);

// Размеры создаваемого окна
static int gl_windowSize_X(640), gl_windowSize_Y(480);

int main(void)
{

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "glwfInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSwapInterval( 0 );

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* ptr_window = glfwCreateWindow(gl_windowSize_X, gl_windowSize_Y, "Hello World", nullptr, nullptr);
    if (!ptr_window)
    {
        std::cerr << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Короче говоря Signal Handler на COMSIG_WINDOW_SIZE_CHANGED
    glfwSetWindowSizeCallback(ptr_window, glfwWindowSizeChangedCallback);

    glfwSetKeyCallback(ptr_window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(ptr_window);

    if(!gladLoadGL())
    {
        std::cerr << "gladLoadGL failed!" << std::endl;
    }

    std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0.1, 0.3, 0.3, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(ptr_window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(ptr_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void glfwWindowSizeChangedCallback(GLFWwindow* ptr_window, int width, int height)
{
    gl_windowSize_X = width;
    gl_windowSize_Y = height;
    glViewport(0, 0, gl_windowSize_X, gl_windowSize_Y);

    // Перекидываем те же функции из основного цикла дабы убрать черноту, сука.
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(ptr_window);

    std::cout << "BLYAYASRU" << std::endl;
}

void glfwKeyCallback(GLFWwindow* ptr_window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(ptr_window, true);
    }
}