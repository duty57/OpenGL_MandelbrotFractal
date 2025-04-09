#include <fstream>
#include <iostream>
#include <string>
#include "Shader.h"
#include "Input.h"
#include "glm.hpp"

float vertices[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
};

unsigned int indices[6] = {
    0, 1, 2,
    2, 3, 0
};

int main()
{
    char* vertexShaderSource = Shader::readShaderFile(PROJECT_PATH "/Shaders/shader.vert");
    char* fragmentShaderSource = Shader::readShaderFile(PROJECT_PATH "/Shaders/shader.frag");
    char* computeShaderSource = Shader::readShaderFile(PROJECT_PATH "/Shaders/shader.comp");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "mandelbrot", nullptr, nullptr);
    if (!window) {
        std::cout<<"Window initialization failed!"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//make the context of the specified window current on the calling thread
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetScrollCallback(window, Input::scrollFunction);
    glfwSetKeyCallback(window, Input::keyFunction);
    glfwSetCursorPosCallback(window, Input::mousePosFunction);
    glfwSwapInterval(1);//vsync on

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // buffers initialization
    unsigned int VAO, VBO, IBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //generate 2 textures for ping-pong rendering
    unsigned int textures[2];
    glGenTextures(2, textures);
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexStorage2D(GL_TEXTURE_2D, 1 , GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT);
        glBindImageTexture(i, textures[i], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    }

    //create compute shader
    unsigned int computeShader = Shader::compileShader(GL_COMPUTE_SHADER, computeShaderSource);

    unsigned int computeProgram = glCreateProgram();
    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);

    // compute shader uniforms
    glUseProgram(computeProgram);
    glUniform1d(glGetUniformLocation(computeProgram, "zoom"), Input::zoom);
    glUniform2d(glGetUniformLocation(computeProgram, "offset"), Input::offset.x, Input::offset.y);
    glUniform2f(glGetUniformLocation(computeProgram, "resolution"), WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniform1i(glGetUniformLocation(computeProgram, "colorMode"), Input::colorMode);
    glUniform1f(glGetUniformLocation(computeProgram, "deltaTime"), 0.0);
    glUseProgram(0);

    // initialize shaders
    unsigned int vertexShader = Shader::compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = Shader::compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "fractalTex"), 0);

    glUseProgram(0);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int currentBuffer = 0;
    int nextBuffer = 1;

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Input::showFPS(window);
        if(Input::autoZoom) Input::updateZoom(1.0f);

        // write data to the next buffer
        glUseProgram(computeProgram);
        glUniform1d(glGetUniformLocation(computeProgram, "zoom"), Input::zoom);
        glUniform2d(glGetUniformLocation(computeProgram, "offset"), Input::offset.x, Input::offset.y);
        glUniform2f(glGetUniformLocation(computeProgram, "resolution"), WINDOW_WIDTH, WINDOW_HEIGHT);
        glUniform1ui(glGetUniformLocation(computeProgram, "colorMode"), Input::colorMode);
        glBindImageTexture(0, textures[nextBuffer], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute((WINDOW_WIDTH + 15) / 16, (WINDOW_HEIGHT + 15) / 16, 1);// dispatch the compute shader with a workgroup size calculated based on the window dimensions
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // read data from current buffer and send it to the fragment shader
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[currentBuffer]);
        glUniform1i(glGetUniformLocation(shaderProgram, "fractalTex"), 0);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        std::swap(currentBuffer, nextBuffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clear the program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteTextures(2, textures);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;

}