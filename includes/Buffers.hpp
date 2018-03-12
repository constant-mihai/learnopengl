/****************************************** 

* File Name : includes/Buffers.hpp

* Creation Date : 08-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _LOGL_BUFFERS_H_
#define _LOGL_BUFFERS_H_

/* Std */
#include <vector>

#include "BinDataUtils.hpp"

/**
 * ******************************************************
 * @brief Buffer class
 *
 * Templated by buffer data type
 * ******************************************************
**/
template<class T>
class Buffer{
    public: /* Constructors */
        /**
         * ******************************************************
         * Constructor
         *
         * @param[in] bType         - buffer type
         * @param[in] dType         - draw type; how the data will be drawn 
         * @param[in] data          - buffer data
         * @param[in] size          - buffer data size
         * ******************************************************
        **/
        Buffer(uint32_t bType, uint32_t dType, const T* data, size_t size):
            bType_(bType), dType_(dType), size_(size)
        {
            LOG(L_INFO, "Allocating buffer %d, with size %ld.", bType, size_);
            uptrData_.reset( (T*) malloc(size_) );
            memcpy(uptrData_.get(), data, size_);
            glGenBuffers(1 /* Generate one buffer */, &handler_);
            glBindBuffer(bType_, handler_);
            glBufferData(bType_, size_, uptrData_.get(), dType_); 

            hexDumpMask_ = setHexDumpMask(typeid(T).name());
        }

        /**
         * ******************************************************
         * Destructor
         * ******************************************************
        **/
        ~Buffer()
        {
        }

    private: /* Methods */
        /**
         * ******************************************************
         * Sets the dump mask
         * ******************************************************
        **/
        uint8_t setHexDumpMask(const char* type)
        {
            LOG(L_DBG, "Mask type: %s", type);
            if (strcmp("i", type) == 0)
            {
                return HDF_INT_32;
            }
            else if (strcmp("l", type) == 0)
            {
                return HDF_INT_64;
            }
            else if (strcmp("j", type) == 0)
            {
                return HDF_UINT_32;
            } 
            else if (strcmp("m", type) == 0)
            {
                return HDF_UINT_64;
            }
            else if (strcmp("f", type) == 0)
            {
                return HDF_FLOAT;
            }
            else if (strcmp("d", type) == 0)
            {
                return HDF_DOUBLE;
            }
            else return HDF_DEFAULT;
        }
    public: /* Methods */
        /**
         * ******************************************************
         * Dump the buffer in hex form
         * ******************************************************
        **/
        void hexDump()
        {
            DumpHex(uptrData_.get(), size_, hexDumpMask_);
        }

    private: /*Members */
        uint32_t            bType_;     /* Buffer type */
        uint32_t            dType_;     /* Draw type */
        uint32_t            handler_;   /* Buffer handler */
        size_t              size_;      /* Buffer size */
        uint8_t             hexDumpMask_;
        std::unique_ptr<T>  uptrData_;  /* Data to be drawn */ // TODO is this data copied by OpenGL? should I free it?
};


/**
 * ******************************************************
 * @brief Buffer Format class
 * ******************************************************
**/
class BufferFormat {
    public: /* Constructors */
        /**
         * ******************************************************
         * @brief Buffer Format constructor
         *
         * @param[in] size          - how many values the attribute contains
         * @param[in] type          - value type
         * @param[in] normalized    - whether normalized or not
         * @param[in] stride        - total count of attribute values
         * @param[in] offset        - where in the current buffer "line" the attribute starts
         * ******************************************************
        **/
        BufferFormat(size_t size, uint32_t type, 
                bool normalized, size_t stride, size_t offset) :
            size_(size), type_(type), 
            normalized_(normalized), stride_(stride), 
            offset_(offset)
        {

        }

        /**
         * ******************************************************
         * @brief Buffer Format shortened constructor
         *
         * @param[in] size          - how many values the attribute contains
         * @param[in] stride        - total count of attribute values
         * @param[in] offset        - where in the current buffer "line" the attribute starts
         * ******************************************************
        **/
        BufferFormat(size_t size, size_t stride, size_t offset) :
            size_(size), type_(GL_FLOAT), 
            normalized_(GL_FALSE), stride_(stride), 
            offset_(offset)
        {
        }

    private: /* Methods */
    public: /* Methods */
        void print()
        {
            LOG(L_INFO, "Buffer Format:\n\
                    Size: %ld\n \
                    Type: %d\n \
                    Normalized: %d\n \
                    Stride:%ld\n \
                    Offset:%ld\n",
                    size_, type_, normalized_, stride_, offset_);
        }

    private: /* Members */
        size_t      size_;
        uint32_t    type_;
        bool        normalized_;
        size_t      stride_;
        size_t      offset_;
};

/**
 * ******************************************************
 * @brief VertexArray class
 * ******************************************************
**/
class VertexArray {
    public: /* Constructors */
        /**
         * ******************************************************
         * Constructor
         * ******************************************************
        **/
        VertexArray()
        {
            glGenVertexArrays(1 /* Numbers of Vertex arrays */, &handler_);
            glBindVertexArray(handler_);
        }

        /**
         * ******************************************************
         * Destructor
         * ******************************************************
        **/
        ~VertexArray()
        {
        }

    private: /* Methods */
    public: /* Methods */
        /**
         * ******************************************************
         * @brief Print function
         * ******************************************************
        **/
        void print() 
        {
            LOG(L_INFO, "Vertex Array info:");
            LOG(L_INFO, "\t handler: %d\n", handler_);
            LOG(L_INFO, "Attribute pointers:\n");
            for (uint32_t i = 0; i<attrPtrs_.size(); ++i)
            {
                LOG(L_INFO, "Index: %d", i);
                attrPtrs_[i].print();
            }
        }
        /**
         * ******************************************************
         * @bried Get the Vertex Array handler
         * ******************************************************
        **/
        uint32_t getHandler() { return handler_; };
        /**
         * ******************************************************
         * Emplace back for buffer format 
         * ******************************************************
        **/
        void attribPointer(uint32_t id, size_t size, uint32_t type, 
                bool normalized, size_t stride, size_t offset)
        {
            LOG(L_ERR, "NOT IMPLEMENTED!");
            // TODO, not sure if replacing attribute format at id is needed or a good idea
        }

        /**
         * ******************************************************
         * Emplace back for buffer format 
         * ******************************************************
        **/
        void attribPointer(size_t size, uint32_t type, 
                bool normalized, size_t stride, size_t offset)
        {
            attrPtrs_.emplace_back(size, type, normalized, stride, offset);
            glVertexAttribPointer(
               attrPtrs_.size()-1,          /* attribute key. */
               size,                        /* size */
               type,                        /* type */
               normalized,                  /* normalized or not */
               stride,                      /* stride */
               (void*)offset                /* array buffer offset */
            );
        }

        /**
         * ******************************************************
         * Emplace back for buffer format 
         * ******************************************************
        **/
        void attribPointer(size_t size, size_t stride, size_t offset)
        {
            attrPtrs_.emplace_back(size, stride, offset);
            glVertexAttribPointer(
               attrPtrs_.size()-1,          /* attribute key. */
               size,                        /* size */
               GL_FLOAT,                    /* type */
               GL_FALSE,                    /* normalized or not */
               stride,                      /* stride */
               (void*)offset                /* array buffer offset */
            );
        }

        /**
         * ******************************************************
         * @brief enables a vertex attribute array
         *
         * @param[in] id        - the id of the attribute array
         * ******************************************************
        **/
        void enableAttribArray(uint32_t id)
        {
            glEnableVertexAttribArray(id);
        }

        /**
         * ******************************************************
         * @brief Enable all attribute arrays.
         * ******************************************************
        **/
        void enableAllAttribArrays()
        {
            for (uint32_t i = 0; i<attrPtrs_.size(); ++i)
            {
                glEnableVertexAttribArray(i);
            }
        }

    private: /*Members */
        uint32_t                    handler_;
        std::vector<BufferFormat>   attrPtrs_;
};
#endif
