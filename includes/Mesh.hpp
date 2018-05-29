/***************************************** 

* File Name : includes/Mesh.cpp

* Creation Date : 22-05-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

#ifndef _LOGL_MESH_HPP_
#define _LOGL_MESH_HPP_

/* STD */
#include <vector>

#include <glm.hpp>
#include <string>

struct Vertex {
    glm::vec3 pos_;
    glm::vec3 normal_;
    glm::vec2 texCoords_;
};

struct TextureDescr {
    uint32_t id_;
    std::string type_;
    std::string path_;
};


/**
 * ******************************************************
 * Mesh class
 * ******************************************************
**/
class Mesh {
    public: /* Constructors */

        /**
         * ******************************************************
         * Mesh constructor
         *
         * @param[in] vertices
         * @param[in] indices
         * @param[in] textures
         * @param[in] drawType
         * ******************************************************
        **/
        Mesh(std::vector<Vertex> vertices, 
                std::vector<unsigned int> indices,
                std::vector<TextureDescr> textures, 
                uint32_t drawType, 
                std::vector<float> data) :
            vertices_(vertices),
            indices_(indices),
            textures_(textures),
            drawType_(drawType),
            VAO_(),
            VBO_(GL_ARRAY_BUFFER, drawType_ , (float*)vertices_.data(), vertices_.size()*sizeof(Vertex)),
            EBO_(GL_ELEMENT_ARRAY_BUFFER, drawType_, indices_.data(), indices_.size()*sizeof(unsigned int))
        {
            //LOG(L_ERR, "MESH EBO:");
            //VBO_.hexDump();
            //EBO_.hexDump();
            /* Positions */

            int retCmp = memcmp(data.data(), vertices_.data(), vertices_.size()*sizeof(Vertex));
            LOG(L_ERR, "============================================================ CMP = %d", retCmp);
            VAO_.attribPointer(3, sizeof(Vertex), 0);
            VAO_.attribPointer(3, sizeof(Vertex), offsetof(Vertex, normal_));
            VAO_.attribPointer(2, sizeof(Vertex), offsetof(Vertex, texCoords_));
            VAO_.enableAllAttribArrays();
            VAO_.print();

            glBindVertexArray(0);
        }

    public: /* Metods */
        /**
         * ******************************************************
         * Draw function
         * 
         * @param[in] program 
         * ******************************************************
        **/
        void draw(Program & program)
        {
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			for(unsigned int i = 0; i < textures_.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
                std::string number;
                std::string name = textures_[i].type_;
				if(name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if(name == "texture_specular")
					number = std::to_string(specularNr++);

				program.setInt((name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
			}
			glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(VAO_.getHandler());
            glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    private: /* Members */
        /*  Mesh Data  */
        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;
        std::vector<TextureDescr> textures_;

        uint32_t drawType_;

        /*  Render data  */
        VertexArray VAO_;
        Buffer<float> VBO_;
        Buffer<uint32_t> EBO_;

};  

#endif
