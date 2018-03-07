#ifndef _LOGL_SHADER_HPP_
#define _LOGL_SHADER_HPP_

#include <GL/glew.h>

#include "Utils.hpp"
#include "Files.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * ******************************************************
 * @brief Shader class
 * 
 * Reads the GLSL code on construction and holds
 * a compiled shader and the shader code location.
 * ******************************************************
 */
class Shader {
    public:
        /**
         * ******************************************************
         * Shader default constructor
         *
         * @param shader path 
         * @param shader type 
         * ******************************************************
         */
        Shader(const char* path, uint32_t type)
            : file_(path), type_(type)
        {
            const char* buff = 0;
            /* Compile shaders */
            buff        = file_.getBuff();
            handler_    = compileShader(type, 1, &buff, NULL);
            compiled_   = checkCompileErrors(); 

        }

        /**
         * ******************************************************
         * Return shader handler
         * ******************************************************
        **/
        uint32_t getHandler() { return handler_; };

    private:
        /**
         * ******************************************************
         * @brief Compile shader
         * Compiles the given shader code.
         *
         * @param type      - the type of shader
         * @param count     - number of elements in the code and lengths arrays. 
         * @param code      - pointer to array of code strings
         * @param lengths   - an array of code lengths. If null it assumes each string is NULL term. 
         * ******************************************************
        **/
        uint32_t compileShader(uint32_t type, size_t count, const char** code, int32_t* lengths) {
            uint32_t handler = glCreateShader(type);

            glShaderSource      (handler, count, code, lengths);
            //LOG(L_INFO, "BUFF: \n ====================================================n %s", *code);
            glCompileShader     (handler);

            return handler;
        }

        /**
         * ******************************************************
         * Return a const char from the shader type
         * ******************************************************
        **/
        const char* type2Str(uint32_t type) {
            struct pair { 
                uint32_t t;
                const char* str;
            } p[] = {
                {GL_VERTEX_SHADER, "Vertex Shader"},
                {GL_FRAGMENT_SHADER, "Fragment Shader"},
                {0,0}
            };

            int i = 0;
            while (p[i].t != 0 && p[i].str != 0) {
                if (p[i].t == type) return p[i].str;
                i++;
            }
            return "Unkown type of shader!";
        };

        /**
         * ******************************************************
         * Checks for compilation errors 
         * ******************************************************
        **/
        bool checkCompileErrors()
        {
            const char* errmsg = "ERROR::SHADER_COMPILATION_ERROR for ";
            const char* errsep = "\n -- --------------------------------------------------- -- \n";
            int32_t success = 0;
            int32_t iLogLen = 0;

            glGetShaderiv(handler_, GL_COMPILE_STATUS, &success);
	        glGetShaderiv(handler_, GL_INFO_LOG_LENGTH, &iLogLen);
            
            if (iLogLen < 0) {
                LOG(L_ERR, "Log len cannot be negative");
                return false;
            }

            if (success != GL_TRUE)
            {
                char * infoLog = (char*) malloc(iLogLen);
                glGetShaderInfoLog(handler_, iLogLen, NULL, infoLog);
                std::cout << errmsg << type2Str(type_) << "\n" << infoLog <<  errsep << std::endl;
                free(infoLog);
                return false;
            }
            
            return true;
        }

    private:
        File        file_;          /* Vertex file */
        bool        compiled_;      /* Whether it compiled successfully */
        uint32_t    handler_;       /* Shader handler */
        uint32_t    type_;          /* The shader type */
};


#endif
