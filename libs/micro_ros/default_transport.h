#ifndef __MICRO_ROS_DEFAULT_TRANSPORT_H__
#define __MICRO_ROS_DEFAULT_TRANSPORT_H__

#include <cassert>
#include "libs/base/console_m7.h"

#ifdef __cplusplus
extern "C"
{
#endif

    bool coralmicro_ros_transport_open(struct uxrCustomTransport *transport);

    bool coralmicro_ros_transport_close(struct uxrCustomTransport *transport);

    size_t coralmicro_ros_transport_write(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, uint8_t *errcode);

    size_t coralmicro_ros_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode);

#ifdef __cplusplus
}
#endif
#endif // __MICRO_ROS_DEFAULT_TRANSPORT_H__
