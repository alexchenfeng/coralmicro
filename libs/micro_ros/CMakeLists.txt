set(libs_micro_ros_SOURCES
    default_transport.cc
    custom_memory_manager.c
    microros_allocators.c
    microros_time.c
    wifi_client.cc
    tcp_transport.cc
    udp_transport.c
)

set(libs_FreeRTOS_INCLUDE_DIRECTORIES_PUBLIC
    .
    ${PROJECT_SOURCE_DIR}/third_party/modified/FreeRTOS
    ${PROJECT_SOURCE_DIR}/third_party/modified/nxp/rt1176-sdk/rtos/freertos/freertos_kernel/include
    ${PROJECT_SOURCE_DIR}/third_party/freertos_kernel/include
    ${PROJECT_SOURCE_DIR}/third_party/freertos_kernel/portable/GCC/ARM_CM4F
)

set(libs_micro_ros_INCLUDE_DIRECTORIES_PUBLIC
    ./microros_include
)

add_library_m7(libs_micro_ros_freertos STATIC
    ${libs_micro_ros_SOURCES}
)

target_link_libraries(libs_micro_ros_freertos
    libs_base-m7_freertos
    libs_base-m7_wifi
)

target_include_directories(libs_micro_ros_freertos PUBLIC
    ${libs_FreeRTOS_INCLUDE_DIRECTORIES_PUBLIC}
)

target_include_directories(libs_micro_ros_freertos PUBLIC
    ${libs_micro_ros_INCLUDE_DIRECTORIES_PUBLIC}
)
