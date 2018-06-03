/****************************************** 

* File Name : common/Utils.hpp

* Creation Date : 06-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

//#define USE_ZIGGURAT 1


#ifndef _LOGL_UTILS_HPP_
#define _LOGL_UTILS_HPP_

#ifndef USE_ZIGGURAT
typedef enum {
    L_DEFAULT = 0,
    L_CRIT    = 1,
    L_ERR     = 2,
    L_INFO    = 3,
    L_WARN    = 4,
    L_DBG     = 5,
    L_MEM     = 6,
}LoggingLevels;

#define LOG_LEVEL L_DBG

#define LOG(l_type, msg, ...) \
    do{ if (LOG_LEVEL >= l_type) printf(msg "\n", ##__VA_ARGS__); \
    }while(0)
#else 
#include <utils/Logging.h>
#endif


#endif
