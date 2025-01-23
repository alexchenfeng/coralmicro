// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdio>

#include "libs/base/led.h"
#include "third_party/freertos_kernel/include/FreeRTOS.h"
#include "third_party/freertos_kernel/include/task.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>
#include "libs/micro_ros/micro_ros.h"


extern "C" void app_main(void *param) {
  (void)param;
  rmw_uros_set_custom_transport(true,
                                NULL, 
                                coralmicro_ros_transport_open, 
                                coralmicro_ros_transport_close, 
                                reinterpret_cast<write_custom_func>(coralmicro_ros_transport_write), 
                                coralmicro_ros_transport_read);
  
  rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
  
  freeRTOS_allocator.allocate = microros_allocate;
  freeRTOS_allocator.deallocate = microros_deallocate;
  freeRTOS_allocator.reallocate = microros_reallocate;
  freeRTOS_allocator.zero_allocate = microros_zero_allocate;

  if (!rcutils_set_default_allocator(&freeRTOS_allocator)) {
    printf("Error on default allocator (line %d)\n", __LINE__);
  }

  rcl_publisher_t publisher;
  std_msgs__msg__Int32 msg;
  rclc_support_t support;
  rcl_allocator_t allocator;
  rcl_node_t node;

  allocator = rcl_get_default_allocator();

  rclc_node_init_default(&node, "coral_micro_node", "", &support);

  rclc_publisher_init_default(&publisher, 
                              &node, 
                              ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), 
                              "micro_ros_publisher");
  msg.data = 0;

  for(;;){
    rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
    if (ret != RCL_RET_OK) {
      printf("Error publishing message (line %d)\n", __LINE__);
    }
    msg.data++;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
