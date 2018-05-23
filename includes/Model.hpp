/****************************************** 

* File Name : includes/Model.hpp

* Creation Date : 22-05-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _LOGL_MODEL_HPP_
#define _LOGL_MODEL_HPP_

/* STD */
#include <vector>
#include <memory>

/* ASSIMP */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Program.hpp"


class Model 
{
    public: /* Constructors */
        Model(char *path, uint32_t drawType)
        {
            loadModel(path, drawType);
        }

        void draw(Program & program)
		{
            for (uint32_t i = 0; i< meshes.size(); i++) {
                meshes[i].get()->draw(program);
            }
        }

    private: /* Methods */

        /**
         * ******************************************************
         * Load Model
         *
         * @param[in] path
         * ******************************************************
        **/
        void loadModel(std::string path, uint32_t drawType)
        {
            Assimp::Importer import;
			const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
			
			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
			{
				LOG(L_ERR, "ERROR::ASSIMP::%s", import.GetErrorString());
				return;
			}
			directory = path.substr(0, path.find_last_of('/'));

			processNode(scene->mRootNode, scene, drawType);
        }

        /**
         * ******************************************************
         * Process Node
         * TODO Doc this 
         * ******************************************************
        **/
        void processNode(aiNode *node, const aiScene *scene, uint32_t drawType)
        {
            // process all the node's meshes (if any)
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
                meshes.push_back(std::unique_ptr<Mesh>(processMesh(mesh, scene, drawType)));			
            }
            // then do the same for each of its children
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene, drawType);
            }
        }


        /**
         * ******************************************************
         * Process Mesh
         * TODO DOC THIS
         * ******************************************************
        **/
        Mesh* processMesh(aiMesh *mesh, const aiScene *scene, uint32_t drawType)
        {
            // data to fill
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<TextureDescr> textures;

            // Walk through each of the mesh's vertices
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; 
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.pos_ = vector;
                // normals
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal_ = vector;
                // texture coordinates
                
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. 
                    // We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture
                    // coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.texCoords_ = vec;
                }
                else
                {
                    vertex.texCoords_ = glm::vec2(0.0f, 0.0f);
                }

                // tangent
               // vector.x = mesh->mTangents[i].x;
               // vector.y = mesh->mTangents[i].y;
               // vector.z = mesh->mTangents[i].z;
               // vertex.Tangent = vector;
               // // bitangent
               // vector.x = mesh->mBitangents[i].x;
               // vector.y = mesh->mBitangents[i].y;
               // vector.z = mesh->mBitangents[i].z;
               // vertex.Bitangent = vector;
                vertices.push_back(vertex);
            }

            // now wak through each of the mesh's faces
            // (a face is a mesh its triangle) 
            // and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
            // we assume a convention for sampler names in the shaders. 
            // Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a 
            // sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 

            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
            std::vector<TextureDescr> diffuseMaps = loadMaterialTextures(material, 
                    aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // 2. specular maps
            std::vector<TextureDescr> specularMaps = loadMaterialTextures(material, 
                    aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            // 3. normal maps
            std::vector<TextureDescr> normalMaps = loadMaterialTextures(material, 
                    aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            // 4. height maps
            std::vector<TextureDescr> heightMaps = loadMaterialTextures(material, 
                    aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            
            // return a mesh object created from the extracted mesh data
            return new Mesh(vertices, indices, textures, drawType);
        }


        /**
         * ******************************************************
         * TODO Doc this
         * ******************************************************
        **/
		unsigned int TextureFromFile(const char *path, const std::string &directory)
		{
			std::string filename = std::string(path);
			filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return textureID;
		}


        /**
         * ******************************************************
         * TODO Doc This
         * ******************************************************
        **/
        std::vector<TextureDescr> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName)
        {
			std::vector<TextureDescr> textureDescrs;
			for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				TextureDescr textureDescr;
				//textureDescr.id_ = TextureFromFile(str.C_Str(), directory);
                std::string path = directory + std::string(str.C_Str());
                Texture texture(path.c_str() , GL_RGB, 0);
                textureDescr.id_ = texture.getHandler();
				textureDescr.type_ = typeName;
				textureDescr.path_ = std::string(str.C_Str());
				textureDescrs.push_back(textureDescr);
			}
			return textureDescrs;
        }



    private: /* Members */
        /*  Model Data  */
        std::vector<std::unique_ptr<Mesh>> meshes;
        std::string directory;
};



#endif
