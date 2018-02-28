// std
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

#define WINDOW_WIDTH 1024 
#define WINDOW_HEIGHT 768 

// Shaders
#include "common/shader.hpp"
#include "common/debug.hpp"
#include "common/Shader.hpp"

// Texture loading
#define STB_IMAGE_IMPLEMENTATION 1
#include <stb_image.h>

/**
 * ******************************************************
 * Resize callback
 * ******************************************************
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

/**
 * ******************************************************
 * Open Window
 * ******************************************************
 */
int openWindow() {
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental=true;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    GLint flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        printf("Flags :%x\n", flags);
        printf("Debug initialized\n");

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
		glDebugMessageCallback(glDebugOutput, 0);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

		// debug filtering
		//glDebugMessageControl(GL_DEBUG_SOURCE_API, 
		//					  GL_DEBUG_TYPE_ERROR, 
		//					  GL_DEBUG_SEVERITY_HIGH,
		//					  0, nullptr, GL_TRUE);
    }

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return 0;
}

/**
 * ******************************************************
 * Create Textures
 * ******************************************************
 */
void createTextures() {

    // Set texture wraping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate textures
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Load file
    int width, height, nrChannels;
    static const char * textureImageLoc = "/store/Code/cpp/learnopengl/img/textures/container.jpg";
    unsigned char *data = stbi_load(textureImageLoc, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(
                GL_TEXTURE_2D,      // Texture target
                0,                  // Mipmap level. Level of detail.
                GL_RGB,             // Internal format. Number of color components
                width,              // Width of teture image
                height,             // Height of texture image
                0,                  // Legacy. Must be 0.
                GL_RGB,             // Format of pixel data
                GL_UNSIGNED_BYTE,   // Data type of pixel data
                data);              // Pointer to binary image
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fprintf(stderr, "Could not open texture: %s!", textureImageLoc);
    }

    //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture

    stbi_image_free(data);
}


/**
 * ******************************************************
 * Create Buffer 
 * ******************************************************
 */
void createBuffer(GLuint * VAO, GLuint * VBO, GLuint * EBO) {
    // An array of 3 vectors which represents 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
         // Positions       // Colors              // Texture coords
         0.5f,  0.5f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, /**/ 0.0f, 0.0f, 0.0f, /**/ 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 0.0f, 1.0f, // top left 
    };

    static const unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    // Create VAO, VBO, EBO
    glGenVertexArrays       (1, VAO);
	glGenBuffers            (1, VBO);
	glGenBuffers            (1, EBO);

    glBindVertexArray       (*VAO);

    // Bind VBO
	glBindBuffer            (GL_ARRAY_BUFFER, *VBO);
	glBufferData            (GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); 

    // Bind EBO 
    glBindBuffer            (GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData            (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

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
 * Process Input
 * ******************************************************
 */
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
    if (openWindow() != 0) {
        fprintf(stderr, "Could not open window!");
    }

    int nrAttributes;
    /* Max vertex attributes */
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Maximum nr of vertex attributes supported: %d\n", nrAttributes );

    // Enable Debug
    //glEnable                    (GL_DEBUG_OUTPUT);
    //glDebugMessageCallback      ((GLDEBUGPROC) MessageCallback, 0);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// This will identify our vertex buffer
	GLuint VBO, VAO, EBO;
    createBuffer(&VAO, &VBO, &EBO);
    createTextures();

    //GLuint shaderProgram = LoadShaders(
            //"/store/Code/cpp/ogl/SimpleVertexShader.vs",
            //"/store/Code/cpp/ogl/SimpleFragmentShader.fs" );

    Shader shader("/store/Code/cpp/learnopengl/shaders/SimpleVertexShader.vs", "/store/Code/cpp/learnopengl/shaders/SimpleFragmentShader.fs");
    

	do{
        processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Draw the triangle !
        //changeColor(shaderProgram);
        //glUseProgram(shaderProgram);
        shader.use();
        shader.setFloat("ourColor", (sin(glfwGetTime())/2.0f) + 0.5f);
        glBindVertexArray(VAO);   
        //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        //glDisableVertexAttribArray(0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while( // Check if the ESC key was pressed or the window was closed
		    glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		    glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
