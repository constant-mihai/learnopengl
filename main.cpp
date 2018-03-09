// std
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include <glew.h>

#include <glfw3.h>

#include <glm.hpp>
using namespace glm;

#define WINDOW_WIDTH 1024 
#define WINDOW_HEIGHT 768 

// Includes
#include <Utils.hpp>

// Common 
#include "common/shader.hpp"
#include "common/debug.hpp"

// Includes
#include "Shader.hpp"
#include "Program.hpp"
#include "Window.hpp"
#include "Textures.hpp"
#include "Buffers.hpp"


/**
 * ******************************************************
 * Initialize GLFW and GLEW
 *
 * @param[out] window
 *
 * @return 0 if successfull
 * ******************************************************
**/
int init(std::unique_ptr<Window> &window) {
    /* Initialize GLFW */
    if( !glfwInit() )
    {
        LOG(L_ERR, "Failed to initialize GLFW\n" );
        return -1;
    }

    window.reset(new Window());

    glewExperimental=true;

    /* Initialize GLEW */
    if (glewInit() != GLEW_OK) 
    {
        LOG(L_ERR, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    return 0;
}

/**
 * ******************************************************
 * Create Buffer 
 * ******************************************************
 */
void createBuffer(GLuint * VAO, GLuint * VBO, GLuint * EBO) {
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat vertices[] = {
         // Positions       // Colors              // Texture coords
         0.5f,  0.5f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, /**/ 0.0f, 0.0f, 0.0f, /**/ 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 0.0f, 1.0f, // top left 
    };

    static const uint32_t indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    /* Create VAO */
    glGenVertexArrays       (1, VAO);
    glBindVertexArray       (*VAO);

    /* Bind VBO */
    Buffer<float> vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
    vbo.hexDump();

    /* Bind EBO */
    Buffer<uint32_t> ebo(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
    ebo.hexDump();

    glVertexAttribPointer(
       0,                  // attribute 0.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       8* sizeof(float),   // stride
       (void*)0            // array buffer offset
    );

    glVertexAttribPointer(
       1,
       3,
       GL_FLOAT,
       GL_FALSE,
       8*sizeof(float),
       (void*)(3*sizeof(float))
    );

    glVertexAttribPointer(
       2,
       2,
       GL_FLOAT,
       GL_FALSE,
       8* sizeof(float),
       (void*)(6*sizeof(float))
    );

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

/**
 * ******************************************************
 * Change color with time 
 * ******************************************************
 */
void changeColor(GLuint shaderProgram) {
    float   timeValue               = glfwGetTime();
    float   greenValue              = (sin(timeValue) / 2.0f) + 0.5f;
    int     vertexColorLocation     = glGetUniformLocation(shaderProgram, "ourColor");

    glUseProgram    (shaderProgram);
    glUniform4f     (vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

/**
 * ******************************************************
 * Main
 * ******************************************************
 */
int main( void )
{
    // Open window
    std::unique_ptr<Window> uptrWindow;
    if (init(uptrWindow) != 0)
    {
        LOG(L_ERR, "Could not open window!\n");
        exit(1);
    }

    /* Enable OpenGL debug */
    loglEnableDebug();

    int nrAttributes;
    /* Max vertex attributes */
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    LOG(L_INFO, "Maximum nr of vertex attributes supported: %d\n", nrAttributes );

    // Enable Debug
    //glEnable                    (GL_DEBUG_OUTPUT);
    //glDebugMessageCallback      ((GLDEBUGPROC) MessageCallback, 0);

    /* Dark blue background */
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    /* This will identify our vertex buffer */
    GLuint VBO, VAO, EBO;
    createBuffer(&VAO, &VBO, &EBO);

    Texture crate("/store/Code/cpp/learnopengl/img/textures/container.jpg");

    Shader vShader("/store/Code/cpp/learnopengl/shaders/SimpleVertexShader.vs", GL_VERTEX_SHADER); 
    Shader fShader("/store/Code/cpp/learnopengl/shaders/SimpleFragmentShader.fs", GL_FRAGMENT_SHADER); 
    Program program(vShader.getHandler(), fShader.getHandler());

    /* While window is open */
    while(!uptrWindow.get()->shouldClose())
    {
        /* Clear */
        glClear(GL_COLOR_BUFFER_BIT);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Draw the triangle !
        //changeColor(programProgram);
        //glUseProgram(programProgram);

        /* Run GLSL program */
        program.use();
        program.setFloat("ourColor", (sin(glfwGetTime())/2.0f) + 0.5f);

        /* Bind and draw*/
        glBindVertexArray(VAO);   
        //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        //glDisableVertexAttribArray(0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap buffers */
        uptrWindow.get()->swapBuffers();

        /* Poll for events */
        glfwPollEvents();
    } 

    /* Close OpenGL window and terminate GLFW */
    uptrWindow.reset(NULL);
    glfwTerminate();

    return 0;
}
