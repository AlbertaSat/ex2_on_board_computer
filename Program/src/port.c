
#include "packet.h"
#include "server.h"
#include "port.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mib.h"
#include "filesystem_funcs.h"

#ifdef POSIX_PORT
       #include <pthread.h>
       #include <sys/socket.h>
       #include <stdio.h>
       #include <errno.h>
       #include <limits.h>
       #include <stdio.h>
       #include <stdarg.h>
       #include <unistd.h>

#endif
#include "protocol_handler.h"



/*------------------------------------------------------------------------------
    File system port functions, these are used to interchange different 
    File systems
------------------------------------------------------------------------------*/
int ssp_rename(const char *old, const char *new) {
    #ifdef POSIX_PORT
        return rename(old, new);
    #endif
}

int ssp_write(int fd, const void *buf, size_t count) {
    #ifdef POSIX_PORT
        return write(fd, buf, count);
    #endif
}


int ssp_read(int fd, char* buff, size_t size) {
    #ifdef POSIX_PORT
        return read(fd, buff, size);
    #endif

}

//SEEK_END 2  SEEK_CUR 1  SEEK_SET 0 
int ssp_lseek(int fd, int offset, int whence) {
    #ifdef POSIX_PORT
        return lseek(fd, offset, whence);
    #endif
} 

int ssp_open(char *pathname, int flags) {
    #ifdef POSIX_PORT
        //open with read and write permissions
        return open(pathname, flags, 0666);
    #endif
}

int ssp_close(int fd) {
    #ifdef POSIX_PORT
        return close(fd);
    #endif
}


/*------------------------------------------------------------------------------
    Network port functions, these are used to interchange different network
    stacks
------------------------------------------------------------------------------*/

void ssp_sendto(Response res) {
    
    #ifdef POSIX_PORT
    
        struct sockaddr* addr = (struct sockaddr*) res.addr;
        int n = sendto(res.sfd, res.msg, res.packet_len, 0, addr, sizeof(*addr));
        if (n < 0) 
            ssp_error("ERROR in ssp_sendto");
    #endif
}

/*------------------------------------------------------------------------------
    Std lib functions, for custom memory allocation, and stdio
------------------------------------------------------------------------------*/

void *ssp_alloc(uint32_t n_memb, size_t size) {
    #ifdef POSIX_PORT
        return calloc(n_memb, size);
    #endif
}

void ssp_free(void *pointer) {
    #ifdef POSIX_PORT
        free(pointer);
        pointer = NULL;
    #endif
}

void ssp_error(char *error){
    #ifdef POSIX_PORT
        perror(error);
    #endif
}

void ssp_printf( char *stuff, ...) {
    #ifdef POSIX_PORT
        va_list args;
        va_start(args, stuff);
        vfprintf(stdout, stuff, args);
        va_end (args);
        fflush(stdout);
    #endif
}

void *ssp_thread_create(int stack_size, void * (thread_func)(void *params), void *params) {


    #ifdef POSIX_PORT
    pthread_t *handler = ssp_alloc(sizeof(pthread_t), 1);
    checkAlloc(handler, 1);

    pthread_attr_t *attr = ssp_alloc(sizeof(pthread_attr_t), 1); 
    checkAlloc(attr, 1);

    int err = pthread_attr_init(attr);
    if (0 != err) 
        perror("pthread_init failed");


    err = pthread_attr_setstacksize(attr, stack_size);

    if (0 != err)
        perror("ERROR pthread_attr_setstacksize %d");

    if (EINVAL == err) {
        printf("the stack size is less that PTHREAD_STACK_MIN %d\n", PTHREAD_STACK_MIN);
    }

    err = pthread_create(handler, attr, thread_func, params);
    if (0 != err)
        perror("ERROR pthread_create");

    ssp_free(attr);
    #endif

    return handler;

}

void ssp_thread_join(void *thread_handle) {
    #ifdef POSIX_PORT
        pthread_t * handle = (pthread_t*) thread_handle;
        pthread_join(*handle, NULL);
        ssp_free(thread_handle);
    #endif

}