/****************************************** 

* File Name : includes/BinDataUtils.hpp

* Creation Date : 09-03-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _LOGL_BINDATA_UTILS_H_
#define _LOGL_BINDATA_UTILS_H_

#include <stdio.h>

#define bit(n) (1<<(n)) 

typedef enum {
  HDF_DEFAULT       = bit(0),
  HDF_INT_32        = bit(1),
  HDF_INT_64        = bit(2),
  HDF_UINT_32       = bit(3),
  HDF_UINT_64       = bit(4),
  HDF_FLOAT         = bit(5),
  HDF_DOUBLE        = bit(6),
  HDF_LONG_DOUBLE   = bit(7),
} HexDumpFlags;


/**
 * ******************************************************
 * Humans are weak and can't read binary.
 * This appends human readable values.
 *
 * @param flags         - what kind of human readable values to append
 * @param curr          - pointer to current location in the buffer
 * @param lineDatalen   - we parse the buffer in lines of length 'len'
 * @param tableLinelen  - the table displayed has this line len 
 * @param ascii         - the ascii values to be appended at the end
 * ******************************************************
**/
void dumpHumanReadable(uint8_t flags, const char* curr, 
        uint32_t lineDataLen, uint32_t tableLineLen, 
        const char* ascii)
{
    printf("| ");
    uint32_t i = 0;
    size_t jump = 0;
    const char* lineData = curr - (lineDataLen -1); /* Go back to the start of the line */
    if (flags & HDF_INT_32)
    {
        /* Jump through the read line and interpret data */
        int32_t *it = NULL;
        jump = sizeof(int32_t);
        for(i=0; i<lineDataLen; i+=jump)
        {
            it = (int32_t*)(lineData+i);
            printf(" %10d ", *it);
        }
    }
    else if (flags & HDF_INT_64)
    {
        /* Jump through the read line and interpret data */
        int64_t *it = NULL;
        jump = sizeof(int64_t);
        for(i=0; i<lineDataLen; i+=jump)
        {
            it = (int64_t*)(lineData+i);
            printf(" %20ld ", *it);
        }
    }
    else if (flags & HDF_UINT_32)
    {
        /* Jump through the read line and interpret data */
        uint32_t *it = NULL;
        jump = sizeof(uint32_t);
        for(i=0; i<lineDataLen; i+=jump)
        {
            it = (uint32_t*)(lineData+i);
            printf(" %10u ", *it);
        }
    }
    else if (flags & HDF_UINT_64)
    {
        /* Jump through the read line and interpret data */
        uint64_t *it = NULL;
        jump = sizeof(uint64_t);
        for(i=0; i<lineDataLen; i+=jump)
        {
            it = (uint64_t*)(lineData+i);
            printf(" %20lu ", *it);
        }
    }
    else if (flags & HDF_FLOAT)
    {
        /* Jump through the read line and interpret data */
        float *it = NULL;
        jump = sizeof(float);
        for(i=0; i<lineDataLen; i+=jump)
        {
            it = (float*)(lineData+i);
            printf(" %10f ", *it);
        }
    }
    else if (flags & HDF_DOUBLE)
    {
        /* Jump through the read line and interpret data */
        double *it = NULL;
        jump = sizeof(double);
        for(i=0; i<lineDataLen; i+=jump)
        {
            it = (double*)(lineData+i);
            printf(" %20lf ", *it);
        }
    } 
    else if (flags & HDF_LONG_DOUBLE)
    {
        //Not sure how long "long double" is
        //TODO, need to think about this one if LONG DOUBLE=10. 
    } else {
        printf (" %10s", "Mask N/A ");
    }

    /* Padding */
    size_t endOfTheLine = (i) % tableLineLen;
    if ( endOfTheLine != 0 )
    {
        for (size_t j = endOfTheLine; j < tableLineLen; j+=jump) {
            printf(" %10s ", " ");
        }
    }

    printf("|  %s \n", ascii);
}

/**
 * ******************************************************
 * Created by: https://gist.github.com/ccbrown/9722406 
 * & Heavilly mutilated by me.
 * ******************************************************
**/
void DumpHex(const void* data, size_t size, uint8_t flags) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		if (i%16 == 0) printf("%4ld | ", i);
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
                dumpHumanReadable(flags, ((const char*)data)+i, 16 /* lineDataLen */, 16, ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}

                /* Padding, so it looks nice */
                size_t endOfTheLine = (i+1) % 16;
				for (j = endOfTheLine; j < 16; ++j) {
					printf("   ");
				}
                dumpHumanReadable(flags, ((const char*)data)+i, endOfTheLine /* lineDataLen */, 16, ascii);
			}
		}
	}
}

#endif
