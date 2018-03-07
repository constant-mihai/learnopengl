/****************************************** 

* File Name : includes/Window.hpp

* Creation Date : 07-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */
#ifndef _LOGL_WINDOW_H_
#define _LOGL_WINDOW_H_

#include <glfw3.h>

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
 * @brief Window class.
 * 
 * ******************************************************
**/
class Window {
    public:
        /**
         * ******************************************************
         * Default constructor 
         * ******************************************************
        **/
        Window()
        {
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

            // Open a window and create its OpenGL context
            window_ = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Playground", NULL, NULL);
            if( window_ == NULL ){
                LOG(L_ERR, "Failed to open GLFW window. If you have an Intel GPU");
                LOG(L_ERR, "they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
                getchar(); // TODO, wtf?
                glfwTerminate();
                exit(1);
            }

            glfwMakeContextCurrent(window_);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
        }

    private:

        GLFWwindow* window_;
};


#endif
