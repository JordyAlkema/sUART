
// Pretend that we have a cstdlib to please kvasir-toolchain.
extern "C" void __libc_init_array();
extern "C" void __libc_init_array() { }

// If you want to use virtual functions without cstdlib, you must use this one


extern "C" {
    void __cxa_pure_virtual();
    /**
     * @brief Function needed for pure-virtual inheritance
     * 
     */
    void __cxa_pure_virtual() { 
        while (true) {} 
    }
}




extern "C"{ 
    
/* 
 * Memory functions provided 
 * Copyright (C) 2005, 2007, 2012 Free Software Foundation, Inc.
 * This file is part of the GNU C Library.
 * The GNU C Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * The GNU C Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, see
 * <http://www.gnu.org/licenses/>.  
 */
    #include "stdint.h"
    #include "stddef.h"


    /**
     * @brief Memcopy function, usually provided by the std-library
     * 
     * @copyright LGPL 3.0
     * @author GNU-GCC compiler standard library
     * @param dest Destination adress
     * @param src Source adress
     * @param len Length 
     * @return void* The
     */
    void * memcpy (void *dest, const void *src, size_t len) {
        char *d = (char*)dest;
        const char *s = (const char*)src;
        while (len--)
            *d++ = *s++;
        return dest;
    }
    /**
     * @brief Memset function, usually provided by the std-library
     * 
     * @copyright LGPL 3.0
     * @author GNU-GCC compiler standard library
     * @param dest Destination adress
     * @param src Source adress
     * @param len Length 
     * @return void* The
     */
    void * memset (void * dest, register int val, register size_t len) {
        register unsigned char *ptr = (unsigned char*)dest;
        while (len-- > 0)
            *ptr++ = val;
        return dest;
    }
}



