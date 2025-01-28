#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"

void glfwWindowSizeChangedCallback(GLFWwindow* ptr_window, int width, int height);
void glfwKeyCallback(GLFWwindow* ptr_window, int key, int scancode, int action, int mode);

// Размеры создаваемого окна
static int gl_windowSize_X(640), gl_windowSize_Y(480);

static GLfloat points[] = {
     0.f,   0.5f, 0.f,
     0.5f, -0.5f, 0.f,
    -0.5f, -0.5f, 0.f
};

static GLfloat colors[] = {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f
};

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_pos;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main(){"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_pos, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 pixel_color;"
"void main() {"
"   pixel_color = vec4(color, 1.0);"
"}";

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

    std::string vertexShader(vertex_shader);
    std::string fragmentShader(fragment_shader);
    Renderer::ShaderProgram shaderProgramm(vertex_shader, fragment_shader);
    if(!shaderProgramm.isCompiled())
    {
        std::cerr << " Can't create shader program!" << std::endl;
        return -1;
    }
    shaderProgramm.use();

    GLuint points_vbo = 0;                              // Массив из вертексов для видеокарты.
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(ptr_window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgramm.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
        glfwSetWindowShouldClose(ptr_window, GL_TRUE);
    }
}