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

#ifndef _LOGL_PROGRAM_HPP_
#define _LOGL_PROGRAM_HPP_

/* Glew */
#include <GL/glew.h>

#include <Shader.hpp>

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

            linked_ = checkLinkStatus();
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
         * Set a float matrix in the program
         *
         * @param name      - uniform name
         * @param value     - value to set
         * ******************************************************
        **/
        void setMat4f(const char* name, const float *value) const
        { 
            glUniformMatrix4fv(
                    glGetUniformLocation(id_, name), 
                    1           /* Number of Matrices to send */, 
                    GL_FALSE    /* Whether to transpose matrix */, 
                    value); 
        }

        void setVec2(const std::string &name, const glm::vec2 &value) const
        {
            glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string &name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const
        {
            glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const
        {
            glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string &name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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

        /**
         * ******************************************************
         * @brief id getter
         * ******************************************************
        **/
        uint32_t getId() { return id_; };


    private:
        uint32_t    id_;        /* Program id */
        bool        linked_;    /* Whether it linked successfully */
}; 


#endif
