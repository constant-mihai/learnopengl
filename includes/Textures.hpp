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
        Texture(const char* path, uint32_t pixelDataFormat, uint32_t unit) :
            path_(path), pixelDataFormat_(pixelDataFormat), unit_(unit)
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
            glDeleteTextures(1, &handler_);
        }

    public: /* Methods */
        void bind(uint32_t unit)
        {
            if (unit > 31) {
                LOG(L_ERR, "Unit number must be <=31. Unit is: %d", unit);
                exit(1);
            }
            /* Bind textures */
            glActiveTexture(GL_TEXTURE0 + unit); // activate the texture unit first before binding texture
            glBindTexture(GL_TEXTURE_2D, handler_);
        }

        uint32_t getHandler() { return handler_; }

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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            /* Set texture filtering parameters */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            /* Generate textures */
            glGenTextures(1 /* Generate one texture */, &handler_);
            bind(unit_);

            /* Load file */
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            data_ = stbi_load(path, &width, &height, &nrChannels, 0);
            if (data_) {
                glTexImage2D(
                        GL_TEXTURE_2D,      // Texture target
                        0,                  // Mipmap level. Level of detail.
                        GL_RGBA,             // Internal format. Number of color components
                        width,              // Width of teture image
                        height,             // Height of texture image
                        0,                  // Legacy. Must be 0.
                        pixelDataFormat_,   // Format of pixel data
                        GL_UNSIGNED_BYTE,   // Data type of pixel data
                        data_);             // Pointer to binary image
                glGenerateMipmap(GL_TEXTURE_2D);
				//stbi_image_free(data); // TODO Free here as well?
            } else {
                LOG(L_ERR, "Could not open texture: %s!", path);
				stbi_image_free(data_);
            }
        }

    private: /* Members */
        std::string     path_;              /* Path to texture */
        uint8_t         *data_;             /* Binary image data */
        uint32_t        handler_;           /* Texture handler */
        uint32_t        pixelDataFormat_;   /* Pixel data format */
        uint32_t        unit_;
};

/**
 * ******************************************************
 * @brief Texture 2d class
 * ******************************************************
**/
class Texture2d : public Texture {
    public: /* Constructors */
    private: /* Methods */
    public: /* Methods */
    private: /* Members */
};
#endif
