
#include "packet.h"
#include "server.h"
#include "port.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mib.h"
#include "filesystem_funcs.h"
#include <fcntl.h>

#ifdef POSIX_PORT
       #include <pthread.h>
       #include <sys/socket.h>
       #include <stdio.h>
       #include <errno.h>
       #include <limits.h>
       #include <stdio.h>
       #include <stdarg.h>
       #include <unistd.h>
       #include <sys/select.h>

#endif
#include "types.h"



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

int ssp_recvfrom(int sfd, void *buff, size_t packet_len, int flags, void *server_addr, uint32_t server_addr_len) {
    int count = 0;
    #ifdef POSIX_PORT
        count = recvfrom(sfd, buff, packet_len, flags, (struct sockaddr*)server_addr, (socklen_t*)&server_addr_len);
    #endif

    return count;
}


void *ssp_init_socket_set(size_t *size) {

    #ifdef POSIX_PORT
        fd_set *socket_set = ssp_alloc(1, sizeof(fd_set));
        *size = sizeof(fd_set);
    #endif
    return (void *)socket_set;
}


void ssp_fd_zero(void *socket_set){
    #ifdef POSIX_PORT
        FD_ZERO((fd_set*) socket_set);
    #endif
}

void ssp_fd_set(int sfd, void *socket_set) {
    #ifdef POSIX_PORT
        FD_SET(sfd, (fd_set*) socket_set);
    #endif
}

int ssp_fd_is_set(int sfd, void *socket_set){
    int is_set = 0;
    #ifdef POSIX_PORT
        is_set = FD_ISSET(sfd, (fd_set*) socket_set);
        
    #endif
    return is_set;
}

int ssp_select(int sfd, void *read_socket_set, void *write_socket_set, void *restrict_socket_set, uint32_t timeout_in_usec) {

    #ifdef POSIX_PORT

    struct timeval timeout = {
        .tv_sec = 0,
        //.tv_usec = 100e3,
        .tv_usec = timeout_in_usec
    };

    int nrdy = select(sfd + 1, read_socket_set, write_socket_set, restrict_socket_set, &timeout);
    #endif

    return nrdy;
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