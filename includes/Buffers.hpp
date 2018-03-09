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
            LOG(L_INFO, "Allocating buffer %d, with size %d.", bType, size_);
            uptrData_.reset( (T*) malloc(size_) );
            memcpy(uptrData_.get(), data, size);
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
        uint32_t            size_;      /* Buffer size */
        uint8_t             hexDumpMask_;
        std::unique_ptr<T>  uptrData_;  /* Data to be drawn */
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
    private: /*Members */
};
#endif
