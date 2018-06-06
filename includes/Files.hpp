/****************************************** 

* File Name : includes/Files.hpp

* Creation Date : 07-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _LOGL_FILES_HPP_
#define _LOGL_FILES_HPP_

/* Std */
#include <string>
#include <string.h>
#include <memory>

/**
 * ******************************************************
 * @brief File class. Reads files into buffers
 *
 * @param filePath
 * ******************************************************
**/
class File {
    public:
        /**
         * ******************************************************
         * Default Constructor
         *
         * @param path
         * ******************************************************
        **/
        File(const char* path)
            : path_(path)
        {
            if (path == NULL) 
            {
                LOG(L_ERR, "Missing file path.");
                exit(1);
            }
        }

        /**
         * ******************************************************
         * Returns read buffer
         * ******************************************************
        **/
        const char* getBuff() {
            /* Open files */
            char* buff = 0;

            FILE *stream = fopen(path_.c_str(), "r");

            if (stream == NULL) 
            {
                char * serr = strerror(errno);
                LOG(L_CRIT, "Couldn't open file: %s, \n error: %s", path_.c_str(), serr);
                exit(1);
            }

            /* Alloc buffers */
            fseek(stream, 0, SEEK_END);
            uint32_t streamSize = ftell(stream);
            fseek(stream, 0, SEEK_SET);

            /* Read files */
            buff = (char*) malloc(streamSize+1);
            if (fread(buff, sizeof(char), streamSize, stream) == 0) {
                /* EOF or error */
                int32_t ferr = 0;
                if (( ferr = ferror(stream)) != 0) {
                    LOG(L_CRIT, "Error reading file: %s, %d", path_.c_str(), ferr);
                    exit(1);
                }
            }
            buff[streamSize] = '\0';
            buff_.reset(buff);

            /* Close the streams */
            fclose(stream);
            return (const char*) buff_.get();
        }

        void append(const char* str, size_t size) {
            FILE *stream = fopen(path_.c_str(), "a");

            if (stream == NULL) 
            {
                char * serr = strerror(errno);
                LOG(L_CRIT, "Couldn't open file: %s, \n error: %s", path_.c_str(), serr);
                exit(1);
            }

            if (fwrite(str, sizeof(char), size, stream) == 0) {
                LOG(L_CRIT, "Error appending in file: %s", path_.c_str());
                exit(1);
            }
            fclose(stream);
        }

    private:
        std::string path_;                 /* File path */

        std::unique_ptr<char> buff_;       /* File buffer */

};

#endif
