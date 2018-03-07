/****************************************** 

* File Name : common/Program.hpp

* Creation Date : 06-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

/* Glew */
#include <GL/glew.h>

#include <Shader.hpp>

#ifndef _LOGL_PROGRAM_HPP_
#define _LOGL_PROGRAM_HPP_

/**
 * ******************************************************
 * @brief Shader Program class
 *
 * 
 * ******************************************************
**/
class Program {
    public:
        /**
         * ******************************************************
         * Default constructor
         *
         * @param vertex id
         * @param fragment id
         * ******************************************************
        **/
        Program(uint32_t vertex, uint32_t fragment)
        {
            //TODO some checks are needed here to see wether shader handlers are valid
            id_ = glCreateProgram();
            glAttachShader      (id_, vertex);
            glAttachShader      (id_, fragment);
            glLinkProgram       (id_);
            checkLinkStatus     ();
        }


        /**
         * ******************************************************
         * Use the program
         * ******************************************************
        **/
        void use() { glUseProgram(id_); };

        /**
         * ******************************************************
         * Set a boolean in the program
         *
         * @param name      - uniform name
         * @param value     - value to set
         * ******************************************************
        **/
        void setBool(const char* name, bool value) const
        {         
            glUniform1i(glGetUniformLocation(id_, name), (int)value); 
        }

        /**
         * ******************************************************
         * Set an int in the program
         *
         * @param name      - uniform name
         * @param value     - value to set
         * ******************************************************
        **/
        void setInt(const char* name, int value) const
        { 
            glUniform1i(glGetUniformLocation(id_, name), value); 
        }

        /**
         * ******************************************************
         * Set a float in the program
         *
         * @param name      - uniform name
         * @param value     - value to set
         * ******************************************************
        **/
        void setFloat(const char* name, float value) const
        { 
            glUniform1f(glGetUniformLocation(id_, name), value); 
        }

        /**
         * ******************************************************
         * Check if the program linked successfully
         * ******************************************************
        **/
        bool checkLinkStatus()
        {
            const char* errmsg = "ERROR::PROGRAM_LINKING_ERROR " ;
            const char* errsep = "\n -- --------------------------------------------------- -- \n";
            int32_t success = 0;
            int32_t iLogLen = 0;

            glGetProgramiv(id_, GL_LINK_STATUS, &success);
	        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &iLogLen);
            if (success != GL_TRUE)
            {
                char * infoLog = (char*) malloc(iLogLen);
                glGetProgramInfoLog(id_, iLogLen, NULL, infoLog);
                std::cout << errmsg << "\n" << infoLog << errsep << std::endl;
                free(infoLog);
                return false;
            }
            return true;
        }


    private:
        uint32_t    id_;        /* Program id */
        bool        linked_;    /* Whether it linked successfully */
}; 


#endif
