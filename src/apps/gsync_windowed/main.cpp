#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iomanip>
#include <iostream>

#include <cmath>
#include <cstdlib>
#include <cstdio>

// Usage
// * Enter: toggle between windowed and fullscreen.

const int gVsync = 1;
const bool gPrintTimes = true;

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform vec2 uBasePosition;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vPos + uBasePosition, 0.0, 1.0);\n"
"    //gl_Position = vec4(vPos + vec2(0.5, 0.0), 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

bool gFullscreen = false;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        if (gFullscreen)
        {
            glfwSetWindowMonitor(window, nullptr, 50, 50, 1280, 1024, GLFW_DONT_CARE);
        }
        else
        {
            GLFWmonitor * monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode * mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        gFullscreen = !gFullscreen;
    }
}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint ubase_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1280, 1024, "Gsync sample", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    // VSync
    glfwSwapInterval(gVsync);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    ubase_location = glGetUniformLocation(program, "uBasePosition");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));

    double startTime = glfwGetTime();
    double intermediaryTime = 0;
    double update, draw, swap, poll, print=0;
    unsigned long frame = 0;

    while (!glfwWindowShouldClose(window))
    {
        intermediaryTime = glfwGetTime();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform2f(ubase_location, std::cos(glfwGetTime()), 0.f);
        update = glfwGetTime() - intermediaryTime;
        intermediaryTime = glfwGetTime();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        draw = glfwGetTime() - intermediaryTime;
        intermediaryTime = glfwGetTime();

        glfwSwapBuffers(window);
        swap = glfwGetTime() - intermediaryTime;
        intermediaryTime = glfwGetTime();

        glfwPollEvents();
        poll = glfwGetTime() - intermediaryTime;
        intermediaryTime = glfwGetTime();

        double newTime = glfwGetTime();
        if (gPrintTimes)
        {
            std::cout
                << std::fixed << std::setprecision(2)
                << "Frame #" << std::setw(4) << ++frame
                << " Total: " << std::setw(6) << (newTime - startTime) * 1000 << "ms |"
                << " update: " << std::setw(6) << update * 1000 << "ms."
                << " draw: " << std::setw(6) << draw * 1000 << "ms,"
                << " swap: " << std::setw(6) << swap * 1000 << "ms,"
                << " poll: " << std::setw(6) << poll * 1000 << "ms,"
                << " prev. print: " << std::setw(6) << print * 1000 << "ms,"
                << "\n"
            ;
            print = glfwGetTime() - intermediaryTime;
            intermediaryTime = glfwGetTime();
        }

        startTime = newTime;
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
