#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// --- LWIP ---
#include "third_party/nxp/rt1176-sdk/middleware/lwip/src/include/lwip/opt.h"
#include "third_party/nxp/rt1176-sdk/middleware/lwip/src/include/lwip/sys.h"
#include "third_party/nxp/rt1176-sdk/middleware/lwip/src/include/lwip/api.h"
#include "third_party/nxp/rt1176-sdk/middleware/lwip/src/include/lwip/sockets.h"

// --- micro-ROS Transports ---
#define UDP_PORT        30755
static int sock_fd = -1;

bool coralmicro_ros_udp_transport_open(struct uxrCustomTransport * transport){
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        return false;
    }

    printf("Coral Micro ROS UDP Transport Open! Attempting to use UDP...\r\n");
    return true;
}

bool coralmicro_ros_udp_transport_close(struct uxrCustomTransport * transport){
    if (sock_fd != -1)
    {
        closesocket(sock_fd);
        sock_fd = -1;
    }
    return true;
}

size_t coralmicro_ros_udp_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err){
    if (sock_fd == -1)
    {
        return 0;
    }
    printf("Coral Micro ROS UDP Transport Write! Attempting to write UDP...\r\n");
    const char * ip_addr = (const char*) transport->args;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_PORT);
    addr.sin_addr.s_addr = inet_addr(ip_addr);
    int ret = 0;
    ret = sendto(sock_fd, buf, len, 0, (struct sockaddr *)&addr, sizeof(addr));
    size_t writed = ret>0? ret:0;
    printf("Wrote %d bytes\r\n", writed);
    return writed;
}

size_t coralmicro_ros_udp_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err){

    printf("Coral Micro ROS UDP Transport Read! Attempting to read UDP...\r\n");
    int ret = 0;
    //set timeout
    struct timeval tv_out;
    tv_out.tv_sec = timeout / 1000;
    tv_out.tv_usec = (timeout % 1000) * 1000;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO,&tv_out, sizeof(tv_out));
    ret = recv(sock_fd, buf, len, MSG_WAITALL);
    size_t readed = ret > 0 ? ret : 0;
    printf("Read %d bytes\r\n", readed);
    return readed;
}