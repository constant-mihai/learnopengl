/****************************************** 

* File Name : includes/Textures.hpp

* Creation Date : 08-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _LOGL_TEXTURES_HPP_
#define _LOGL_TEXTURES_HPP_

// Texture loading
#define STB_IMAGE_IMPLEMENTATION 1
#include <stb_image.h>

/**
 * ******************************************************
 * @brief Textures class
 *
 * ******************************************************
**/
class Texture {
    public: /* Constructors */
        /**
         * ******************************************************
         * Constructor
         *
         * @param[in] path          - texture file path
         * ******************************************************
        **/
        Texture(const char* path) :
            path_(path)
        {
            createTexture(path_.c_str());
        }

        /**
         * ******************************************************
         * Destructor
         * ******************************************************
        **/
        ~Texture()
        {
            stbi_image_free(data_);
        }

    public: /* Methods */
    private: /* Methods */
        /**
         * ******************************************************
         * Create Textures
         *
         * @param[in] path
         * ******************************************************
         */
        void createTexture(const char* path)
        {
            /* Set texture wraping parameters */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

            /* Set texture filtering parameters */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            /* Generate textures */
            glGenTextures(1 /* Generate one texture */, &handler_);
            glBindTexture(GL_TEXTURE_2D, handler_);

            /* Load file */
            int width, height, nrChannels;
            data_ = stbi_load(path, &width, &height, &nrChannels, 0);
            if (data_) {
                glTexImage2D(
                        GL_TEXTURE_2D,      // Texture target
                        0,                  // Mipmap level. Level of detail.
                        GL_RGB,             // Internal format. Number of color components
                        width,              // Width of teture image
                        height,             // Height of texture image
                        0,                  // Legacy. Must be 0.
                        GL_RGB,             // Format of pixel data
                        GL_UNSIGNED_BYTE,   // Data type of pixel data
                        data_);             // Pointer to binary image
                glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                LOG(L_ERR, "Could not open texture: %s!", path);
            }

            //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        }

    private: /* Members */
        std::string     path_;      /* Path to texture */
        uint8_t         *data_;     /* Binary image data */
        uint32_t        handler_;   /* Texture handler */
};
#endif
