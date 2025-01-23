#include "libs/micro_ros/micro_ros.h"

extern "C"
{

    bool coralmicro_ros_transport_open(struct uxrCustomTransport * transport)
    {
        return true;
    }

    bool coralmicro_ros_transport_close(struct uxrCustomTransport * transport)
    {
        return true;
    }

    size_t coralmicro_ros_transport_write(struct uxrCustomTransport * transport, uint8_t *buf, size_t len, uint8_t *errcode)
    {
        (void)errcode;
        coralmicro::ConsoleM7::GetSingleton()->Write(reinterpret_cast<char*>(buf), len);
        return len;
    }

    size_t coralmicro_ros_transport_read(struct uxrCustomTransport * transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode)
    {
        (void)errcode;
        size_t r_len =  coralmicro::ConsoleM7::GetSingleton()->Read(reinterpret_cast<char*>(buf), len);
        return r_len;
    }

}
