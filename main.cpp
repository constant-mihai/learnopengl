/* std */
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>

/* GLEW */
#include <glew.h>

/* GLFW */
#include <glfw3.h>

/* GLM */
#include <glm.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <trigonometric.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

#define WINDOW_WIDTH 1024 
#define WINDOW_HEIGHT 768 
#define ASPECT_RATIO WINDOW_WIDTH/WINDOW_HEIGHT

/* Common */
#include "common/shader.hpp"
#include "common/debug.hpp"

#include "glm/gtx/string_cast.hpp"

/* Includes */
#include <Utils.hpp>
#include "Shader.hpp"
#include "Program.hpp"
#include "Window.hpp"
#include "Textures.hpp"
#include "Buffers.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Model.hpp"


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
    GLfloat vertices[] = {
         // Positions       // Colors              // Texture coords
         0.5f,  0.5f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, /**/ 0.0f, 0.0f, 0.0f, /**/ 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 0.0f, 1.0f, // top left 
    };

    uint32_t indices[] = {  // note that we start from 0!
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

void spin() {
    //Transform spin(0, 0.1f, glm::vec3(0, 0, 35));
    //Transform trns( glm::vec3(0.25f, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
    //Transform orbit(0, 1, glm::vec3(0, 0, 5));
    //program.setMat4f("transform", (float*)spin.getModelFloat());
}

/**
 * ******************************************************
 * Create world axes
 * ******************************************************
**/
void createWorldAxes(VertexArray& waVA) {
    float vertices[] = {
        0, 0, 0,    1, 0, 0,
        0, 0, 0,    0, 1, 0,
        0, 0, 0,    0, 0, 1 
    };

    /* Bind VBO */
    Buffer<float> vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
    vbo.hexDump();

    /* Formating attributes */
    waVA.attribPointer(3, 3* sizeof(float), 0);
    waVA.enableAllAttribArrays();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * ******************************************************
 * Create xz plane grid
 * ******************************************************
**/
void createGrid(VertexArray& gridVA) {
    float vertices[300];
    for (int i=0;i<100;i++) {
        vertices[i]=i;      /* x = i */
        vertices[i+1]=0;    /* y = 0 */
        vertices[i+2]=i;    /* z = i */
    }
}

/**
 * ******************************************************
 * Load a default model
 * ******************************************************
**/
Model * loadModel() {
    Model * model = new Model("/store/Code/cpp/learnopengl/models/nanosuit.obj", GL_STATIC_DRAW);

    return model;
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
     
    /* Depth test */
    glEnable (GL_DEPTH_TEST);

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

    VertexArray waVA;
    createWorldAxes(waVA);

    VertexArray gridVA;
    createGrid(gridVA);

    /* Load a new model */
    //Model *nanosuit = loadModel();
    TinyObjModel starDestr;

    /* Compile shaders and link program */
    Shader vShader("/store/Code/cpp/learnopengl/shaders/SimpleVertexShader.vs", GL_VERTEX_SHADER); 
    Shader fShader("/store/Code/cpp/learnopengl/shaders/SimpleFragmentShader.fs", GL_FRAGMENT_SHADER); 
    Program program(vShader.getHandler(), fShader.getHandler());
    program.use();

    Camera camera(glm::vec3(0, 0, 1.0f), 70.0f, ASPECT_RATIO, 0.01f, 1000.0f);

    /* Transform */
    Transform model(glm::vec3(0, 0, -1.0f), glm::vec3(1, 1, 1), glm::vec3(0.0f, 0, 0));
    //glm::mat4 projection;
    //projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 respm = projection * glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    //respm = respm * *(model.getModel());
    //program.setMat4f("transform", &respm[0][0]);
    //
    UserPointer up = { &camera, -90.0f, 0.0f, 800.0f / 2.0, 600.0f / 2.0f,  70.0f, true};
    glfwSetInputMode(uptrWindow.get()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(uptrWindow.get()->getWindow(), (void*)&up); //TODO is this thread safe?


    glm::mat4 mvp = camera.getViewProjection() * model.getModelRef();
    program.setMat4f("transform", &mvp[0][0]);

    /* Load texture */
    //Texture crate("/store/Code/cpp/learnopengl/img/textures/container.png", GL_RGB, 0);
    //Texture smile("/store/Code/cpp/learnopengl/img/textures/awesomeface.png", GL_RGBA, 1);

    /* We need to tell GLSL which texture is where */
    uint32_t texLoc = glGetUniformLocation(program.getId(), "ourTexture1");
    glUniform1i(texLoc, 0);
    /* Or like this */
    program.setInt("ourTexture2", 1);

    double time = 0, deltaTime = 0, lastFrame = 0;//, rotateTime = glfwGetTime();
    glm::mat4 res(1.0f);

    // Rotate camera
    float camX = 0, camZ = 0, radius = 10.0f;
    //camera.fix(glm::vec3(0,0,0)); // TODO fix if you want to rotate around a point
    /* While window is open */
    while(!uptrWindow.get()->shouldClose())
    {
        /* Time */
        time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        /* Input */
        uptrWindow.get()->processInput(deltaTime, camera);
        mvp = camera.getViewProjection() * model.getModelRef();
        program.setMat4f("transform", &mvp[0][0]);

        /* Clear */
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // Rotate camera
        //camX = sin(time) * radius;
        //camZ = cos(time) * radius;
        //mvp = camera.move(glm::vec3(camX, 0, camZ)) * model.getModelRef();
        //program.setMat4f("transform", &mvp[0][0]);

        /* Run GLSL program */
        program.use();
        program.setFloat("ourColor", (sin(time)/2.0f) + 0.5f);
        //nanosuit->draw(program);
        starDestr.draw(program);
        
        /* Bind and draw*/
        glBindVertexArray(vertexArray.getHandler());   
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(waVA.getHandler());
        glDrawArrays(GL_LINES, 0, 18);

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
