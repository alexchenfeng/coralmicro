#include "libs/micro_ros/micro_ros.h"

#include "third_party/nxp/rt1176-sdk/middleware/lwip/src/include/lwip/api.h"
#include "third_party/nxp/rt1176-sdk/middleware/lwip/src/include/lwip/sockets.h"

extern "C"
{
    static int sock_fd = -1;
    static const char * agent_domain = "coral-ros-agent.cineneural.com";
    static int agent_port = 8888;

    bool coralmicro_ros_tcp_transport_open(struct uxrCustomTransport * transport)
    {
        printf("Coral Micro ROS TCP Transport Open! Attempting to use TCP...\r\n");
        ip_addr_t lwip_addr;
        err_t err = netconn_gethostbyname(agent_domain, &lwip_addr);
        if (err != ERR_OK)
        {
            return false;
        }
        sock_fd = coralmicro::SocketClient(lwip_addr, agent_port);

        if (sock_fd == -1)
        {
            return false;
        }
        printf("Connected to agent\r\n");
        return true;

    }
    bool coralmicro_ros_tcp_transport_close(struct uxrCustomTransport * transport)
    {
        printf("Coral Micro ROS TCP Transport Close! Attempting to close TCP...\r\n");
        if (sock_fd != -1)
        {
            coralmicro::SocketClose(sock_fd);
            sock_fd = -1;
        }
        return true;
    }

    size_t coralmicro_ros_tcp_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err)
    {
        printf("Coral Micro ROS TCP Transport Write! Attempting to write TCP...\r\n");
        if (sock_fd == -1)
        {
            return 0;
        }
        coralmicro::IOStatus stat = coralmicro::WriteBytes(sock_fd, buf, len, 1024);
        if (stat == coralmicro::IOStatus::kOk)
        {
            printf("Wrote %d bytes\r\n", len);
            return len;
        } else
        {
            return 0;
        }
    }

    size_t coralmicro_ros_tcp_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err)
    {
        printf("Coral Micro ROS TCP Transport Read! Attempting to read TCP...\r\n");
        if (sock_fd == -1)
        {
            return 0;
        }
        coralmicro::IOStatus stat = coralmicro::ReadBytes(sock_fd, buf, len);
        if (stat == coralmicro::IOStatus::kOk)
        {
            printf("Read %d bytes\r\n", len);
            return len;
        } else
        {
            return 0;
        }
    }
}
