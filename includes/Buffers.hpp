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
            uptrData_.reset( (T*) malloc(sizeof(T)*size_) );
            glGenBuffers(1 /* Generate one buffer */, &handler_);
            glBindBuffer(bType_, handler_);
            glBufferData(bType_, size_, uptrData_.get(), dType_); 
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
    public: /* Methods */
    private: /*Members */
        uint32_t            bType_;     /* Buffer type */
        uint32_t            dType_;     /* Draw type */
        uint32_t            handler_;   /* Buffer handler */
        uint32_t            size_;      /* Buffer size */
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
