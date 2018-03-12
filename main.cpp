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
void createBuffer(VertexArray &vertexArray) {
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

    /* Bind VBO */
    Buffer<float> vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
    vbo.hexDump();

    /* Bind EBO */
    Buffer<uint32_t> ebo(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
    ebo.hexDump();

    /* Formating attributes */
    vertexArray.attribPointer(3, 8* sizeof(float), 0);
    vertexArray.attribPointer(3, 8* sizeof(float), 3* sizeof(float));
    vertexArray.attribPointer(2, 8* sizeof(float), 6* sizeof(float));
    vertexArray.enableAllAttribArrays();
    vertexArray.print();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * ******************************************************
 * Main
 * ******************************************************
 */
int main( void )
{
    /* Open window */
    std::unique_ptr<Window> uptrWindow;
    if (init(uptrWindow) != 0)
    {
        LOG(L_ERR, "Could not open window!\n");
        exit(1);
    }

    /* Enable OpenGL debug */
    loglEnableDebug();

    /* Max vertex attributes */
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    LOG(L_INFO, "Maximum nr of vertex attributes supported: %d\n", nrAttributes );

    /* Dark blue background */
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    /* Create VAO */
    VertexArray vertexArray;

    /* This will identify our vertex buffer */
    createBuffer(vertexArray);

    /* Load texture */
    Texture crate("/store/Code/cpp/learnopengl/img/textures/container.jpg");

    /* Compile shaders and link program */
    Shader vShader("/store/Code/cpp/learnopengl/shaders/SimpleVertexShader.vs", GL_VERTEX_SHADER); 
    Shader fShader("/store/Code/cpp/learnopengl/shaders/SimpleFragmentShader.fs", GL_FRAGMENT_SHADER); 
    Program program(vShader.getHandler(), fShader.getHandler());

    /* While window is open */
    while(!uptrWindow.get()->shouldClose())
    {
        /* Clear */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Run GLSL program */
        program.use();
        program.setFloat("ourColor", (sin(glfwGetTime())/2.0f) + 0.5f);

        /* Bind and draw*/
        glBindVertexArray(vertexArray.getHandler());   
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
