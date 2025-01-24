#include "libs/micro_ros/micro_ros.h"

extern "C"
{
    /**
     * @brief Connects to Wi-Fi using the Coral Micro ROS library.
     *
     * This function attempts to turn on the Wi-Fi and connect to a network.
     * If the Wi-Fi cannot be turned on, it prints an error message on serial console and suspends the current task.
     */
    void coralmicro_ros_wifi_connect()
    {
        printf("Coral Micro ROS Wi-Fi Connect! Attempting to use Wifi...\r\n");

        bool success = coralmicro::WiFiTurnOn(true);
        if (!success)
        {
            printf("Failed to turn on Wi-Fi\r\n");
            vTaskSuspend(nullptr);
        }

        success = coralmicro::WiFiConnect();
        if (!success)
        {
            printf("Failed to connect to Wi-Fi\r\n");
            vTaskSuspend(nullptr);
        } else 
        {
            printf("Wi-Fi connected\r\n");
        }

        auto board_ip_addr = coralmicro::WiFiGetIp();

        if (board_ip_addr.has_value())
        {
            printf("DHCP succeeded, our IP is %s.\r\n", board_ip_addr.value().c_str());
        }
        else
        {
            printf("We didn't get an IP via DHCP, not progressing further.\r\n");
            vTaskSuspend(nullptr);
        }

    }
}