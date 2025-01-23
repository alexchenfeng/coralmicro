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

    size_t coralmicro_ros_transport_write(struct uxrCustomTransport * transport, const uint8_t *buf, size_t len, uint8_t *errcode)
    {
        char ch;
        for (size_t i = 0; i < len; i++)
        {
            ch = buf[i];
            coralmicro::ConsoleM7::GetSingleton()->Write(&ch, 1);
        }

        return len;
    }

    size_t coralmicro_ros_transport_read(struct uxrCustomTransport * transport, uint8_t *buf, size_t len, int timeout, uint8_t *errcode)
    {
        int64_t start = uxr_millis();
        size_t readed = 0;
        char ch;
        for (size_t i = 0; i < len; i++)
        {
            int64_t elapsed_time_us = timeout - (uxr_millis() - start);

            if (elapsed_time_us < 0)
            {
                *errcode = 1;
                break;
            }
            int bytes = coralmicro::ConsoleM7::GetSingleton()->Read(&ch, 1);
            if (bytes == 1)
            {
                buf[i] = ch;
                readed++;
            }
            else
            {
                vTaskDelay( 1 / portTICK_PERIOD_MS );
            }
        }
        return readed;
    }
}

