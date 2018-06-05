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
#include <unordered_map>

/* ASSIMP */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Program.hpp"

/**
 * ******************************************************
 * TODO DOC THIS
 * ******************************************************
**/
class Model 
{
    public: /* Constructors */
        /**
         * ******************************************************
         * TODO DOC THIS
         * ******************************************************
        **/
        Model(char *path, uint32_t drawType)
        {
            loadModel(path, drawType);
        }

        /**
         * ******************************************************
         * TODO DOC THIS
         * ******************************************************
        **/
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
            std::vector<float> data;

            // Walk through each of the mesh's vertices
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; 
                // positions
                vector.x = mesh->mVertices[i].x;
                data.push_back(mesh->mVertices[i].x);
                vector.y = mesh->mVertices[i].y;
                data.push_back(mesh->mVertices[i].y);
                vector.z = mesh->mVertices[i].z;
                data.push_back(mesh->mVertices[i].z);
                vertex.pos_ = vector;
                // normals
                vector.x = mesh->mNormals[i].x;
                data.push_back(mesh->mNormals[i].x);
                vector.y = mesh->mNormals[i].y;
                data.push_back(mesh->mNormals[i].y);
                vector.z = mesh->mNormals[i].z;
                data.push_back(mesh->mNormals[i].z);
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
                    data.push_back(mesh->mTextureCoords[0][i].x);
                    vec.y = mesh->mTextureCoords[0][i].y;
                    data.push_back(mesh->mTextureCoords[0][i].y);
                    vertex.texCoords_ = vec;
                }
                else
                {
                    data.push_back(0);
                    data.push_back(0);
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
                for(unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j]);
                }
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

            std::vector<Texture*> meshTextures;
            // 1. diffuse maps
            loadMaterialTextures(loadedTextures, meshTextures, material, aiTextureType_DIFFUSE, "texture_diffuse");

            // 2. specular maps
            loadMaterialTextures(loadedTextures, meshTextures, material, aiTextureType_SPECULAR, "texture_specular");

            // 3. normal maps 
            loadMaterialTextures(loadedTextures, meshTextures, material, aiTextureType_NORMALS, "texture_normal");

            // 4. height maps
            loadMaterialTextures(loadedTextures, meshTextures, material, aiTextureType_HEIGHT, "texture_height");
            
            // return a mesh object created from the extracted mesh data
            return new Mesh(vertices, indices, meshTextures, drawType, data);
        }

        /**
         * ******************************************************
         * TODO Doc This
         * ******************************************************
        **/
        void loadMaterialTextures(std::unordered_map<std::string, Texture*> & loadedTextures,
                std::vector<Texture*> & meshTextures,
                aiMaterial *mat, aiTextureType type, 
                std::string typeName)
        {
            unsigned int i = 0;
            for( i= 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);

                std::string name = std::string(str.C_Str());
                std::string path = directory + '/' + name; 

                /* Check if the texture was ever loaded */
                if (loadedTextures.find(path) == loadedTextures.end()) {
                    Texture *texture = new Texture(path.c_str(), GL_RGB, typeName);
                    //TODO this can be done better than indexing with 100 character strings.
                    if (texture == NULL) {
                        LOG(L_CRIT, "Missing texture: %s", path.c_str());
                        exit(1);
                    }
                    loadedTextures.insert(std::pair<std::string, Texture*>(path.c_str(), texture));
                }

                /* Create the mesh texture here */
                meshTextures.push_back(loadedTextures[path]);
            }
        }


    private: /* Members */
        /*  Model Data  */
        std::vector<std::unique_ptr<Mesh>> meshes;
        std::string directory;
        std::unordered_map<std::string, Texture*> loadedTextures; //TODO consider a shared_ptr here
};

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

/**
 * ******************************************************
 * Model loader using tinyobjloader TODO doesn't work atm
 * ******************************************************
**/
class TinyObjModel {
    public: /* Constructors */
        /**
         * ******************************************************
         * TODO DOCTHIS
         * ******************************************************
        **/
        TinyObjModel() {
            loadModel();
        }
    private: /* Methods */

        /**
         * ******************************************************
         * TODO DOC THIS
         * ******************************************************
        **/
        void loadModel() {
            //std::string MODEL_PATH = "/store/Code/cpp/learnopengl/models/star_cruiser/scene.gltf";
            std::string MODEL_PATH ="/store/Code/cpp/learnopengl/models/nanosuit.obj"; 
            std::string MATERIAL_BASE_DIR ="/store/Code/cpp/learnopengl/models/"; 
			if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, 
                        MODEL_PATH.c_str(), MATERIAL_BASE_DIR.c_str())) {
                LOG(L_CRIT, "Error Loading model: %s", MODEL_PATH.c_str());
                exit(1);
			}
            LOG(L_CRIT, "TOL ERROR: %s.", err.c_str());

            std::vector<float> data; // TODO TEMP rm this
            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    Vertex vertex = {};
                    vertex.pos_ = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    vertex.texCoords_ = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };

                    vertex.normal_ = {1.0f, 1.0f, 1.0f};

                    vertex.texCoords_ = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };

                    vertices.push_back(vertex);
                    indices.push_back(indices.size());

                }

                LOG(L_DBG, "Shape: %s", shape.name.c_str());
                for(uint32_t m=0; m<shape.mesh.material_ids.size(); m++) {
                    LOG(L_DBG, "Shape Material: %s", materials[m].name.c_str());
                }

                std::string fullTexPath;
                Texture * texture = NULL;
                //for (uint32_t i=0; i<shap.mesh.material_ids.size() - 1; i++) {
                for (uint32_t i=0; i<materials.size(); i++) {
                    //if (materials[i].name.empty()) continue;
                    LOG(L_DBG, "Material: %s", materials[i].name.c_str());
                    LOG(L_DBG, "Material Texture file: %s", materials[i].diffuse_texname.c_str());

                    /* Diffuse */
                    if (!materials[i].diffuse_texname.empty()) {
                        fullTexPath = MATERIAL_BASE_DIR + materials[i].diffuse_texname;
                        texture = new Texture(fullTexPath.c_str(), GL_RGB, "texture_diffuse");
                        meshTextures.push_back(texture);
                    }

                    /* Specular */
                    if (!materials[i].specular_texname.empty()) {
                        fullTexPath = MATERIAL_BASE_DIR + materials[i].specular_texname;
                        texture = new Texture(fullTexPath.c_str(), GL_RGB, "texture_specular");
                        meshTextures.push_back(texture);
                    }

                    /* Normal */
                    if (!materials[i].normal_texname.empty()) {
                        fullTexPath = MATERIAL_BASE_DIR + materials[i].normal_texname;
                        texture = new Texture(fullTexPath.c_str(), GL_RGB, "texture_normal");
                        meshTextures.push_back(texture);
                    }

                    //[> Height <] //TODO HEIGHT VS BUMP
                    if (!materials[i].bump_texname.empty()) {
                        fullTexPath = MATERIAL_BASE_DIR + materials[i].bump_texname;
                        texture = new Texture(fullTexPath.c_str(), GL_RGB, "texture_height");
                        meshTextures.push_back(texture);
                    }

                }
                meshes.push_back(std::unique_ptr<Mesh>(new Mesh(vertices, indices, meshTextures, GL_STATIC_DRAW, data)));            
            }
        }
    public: /* Methods */

        /**
         * ******************************************************
         * TODO DOC THIS
         * ******************************************************
        **/
        void draw(Program & program) {
            for (uint32_t i = 0; i< meshes.size(); i++) {
                meshes[i].get()->draw(program);
            }
        }
    private: /* Members */
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string err;
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            std::vector<std::unique_ptr<Mesh>> meshes;
            std::vector<Texture*> meshTextures;
};


#endif
