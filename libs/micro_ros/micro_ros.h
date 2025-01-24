#ifndef __MICRO_ROS_DEFAULT_TRANSPORT_H__
#define __MICRO_ROS_DEFAULT_TRANSPORT_H__

#include <cassert>
#include "libs/base/console_m7.h"
#include <rmw_microros/rmw_microros.h>
#include "libs/base/led.h"
#include "libs/base/mutex.h"
#include "libs/base/network.h"
#include "libs/base/wifi.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /* default serial transport */
    bool coralmicro_ros_transport_open(struct uxrCustomTransport *transport);
    bool coralmicro_ros_transport_close(struct uxrCustomTransport *transport);
    size_t coralmicro_ros_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *errcode);
    size_t coralmicro_ros_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode);

    /* wifi tcp transport */
    bool coralmicro_ros_tcp_transport_open(struct uxrCustomTransport *transport);
    bool coralmicro_ros_tcp_transport_close(struct uxrCustomTransport *transport);
    size_t coralmicro_ros_tcp_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *errcode);
    size_t coralmicro_ros_tcp_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode);

    /* wifi udp transport */
    bool coralmicro_ros_udp_transport_open(struct uxrCustomTransport *transport);
    bool coralmicro_ros_udp_transport_close(struct uxrCustomTransport *transport);
    size_t coralmicro_ros_udp_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *errcode);
    size_t coralmicro_ros_udp_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode);
    
    void * microros_allocate(size_t size, void * state);
    void microros_deallocate(void * pointer, void * state);
    void * microros_reallocate(void * pointer, size_t size, void * state);
    void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);
    void coralmicro_ros_wifi_connect();

#ifdef __cplusplus
}
#endif
#endif // __MICRO_ROS_DEFAULT_TRANSPORT_H__
