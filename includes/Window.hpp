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
 * Error callback
 * ******************************************************
**/
void error_callback(int error, const char* descr)
{
    LOG(L_ERR, "Error: %s", descr);
}

/**
 * ******************************************************
 * Key callback
 * ******************************************************
**/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1 /* Set */);
}

/**
 * ******************************************************
 * Window close callback
 * ******************************************************
**/
void window_close_callback(GLFWwindow* window)
{
    LOG(L_INFO, "Window is closing");
}

/**
 * ******************************************************
 * @brief Window class.
 * 
 * ******************************************************
**/
class Window {
    public: /* Constructors */
        /**
         * ******************************************************
         * Default constructor 
         * ******************************************************
        **/
        Window()
        {
            registerDefaultHints();

            /* Open a window and create its OpenGL context */
            window_ = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Playground", NULL, NULL);
            if( window_ == NULL ){
                LOG(L_ERR, "Failed to open GLFW window. If you have an Intel GPU");
                LOG(L_ERR, "they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
                glfwTerminate();
                exit(1);
            }

            glfwMakeContextCurrent(window_);
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

            /* Ensure we can capture the escape key being pressed below */
            glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

            /* Buffer swap interval */
            glfwSwapInterval( 1 /* Number of frames to wait */);

            registerDefaultCallbacks();
        }

        /**
         * ******************************************************
         * Destructor
         * ******************************************************
        **/
        ~Window()
        {
            LOG(L_DBG, "Window destructor");
            glfwDestroyWindow(window_);
        }

    public: /* Methods */
        /**
         * ******************************************************
         * Process input
         * ******************************************************
        **/
        void processInput() 
        {
            if(glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window_, true);
            }
        }

        /**
         * ******************************************************
         * Swap buffers
         * ******************************************************
        **/
        void swapBuffers()
        {
            glfwSwapBuffers(window_);
        }

        /**
         * ******************************************************
         * Return window
         * ******************************************************
        **/
        GLFWwindow* getWindow() { return window_; };

        /**
         * ******************************************************
         * Check if esc was pressed
         * ******************************************************
        **/
        bool isEscPressed()
        {
            return glfwGetKey(window_, GLFW_KEY_ESCAPE ) == GLFW_PRESS;
        }

        /**
         * ******************************************************
         * Check if window was closed
         * ******************************************************
        **/
        bool windowShouldClose()
        {
            return glfwWindowShouldClose(window_);
        }

        /**
         * ******************************************************
         * All the checks for open window here
         * ******************************************************
        **/
        bool isOpen()
        {
            /* Check if the ESC key was pressed or the window was closed */
            bool ret = (!isEscPressed() && !windowShouldClose());

            return ret;
        }

        /**
         * ******************************************************
         * Window should close flag is set
         * ******************************************************
        **/
        bool shouldClose()
        {
            return glfwWindowShouldClose(window_);
        }

    private: /* Methods */

        /**
         * ******************************************************
         * Default window options
         * ******************************************************
        **/
        void registerDefaultHints()
        {
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            /* To make MacOS happy; should not be needed */
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        }
        
        /**
         * ******************************************************
         * Register callack
         * ******************************************************
        **/
        void registerDefaultCallbacks()
        {
            glfwSetErrorCallback                (error_callback);
            glfwSetFramebufferSizeCallback      (window_,       framebuffer_size_callback);
            glfwSetKeyCallback                  (window_,       key_callback);
            glfwSetWindowCloseCallback          (window_,       window_close_callback);
        }

    private: /* Members */

        GLFWwindow* window_;
};


#endif
