set(libs_micro_ros_SOURCES
    default_transport.cc
    custom_memory_manager.c
    microros_allocators.c
    microros_time.c
    wifi_client.cc
    tcp_transport.cc
    udp_transport.c
)

set(libs_micro_ros_INCLUDE_DIRECTORIES_PUBLIC
    ./microros_static_library/libmicroros/microros_include
)

if (USE_DEBUG)
    target_compile_definitions(libs_micro_ros_freertos PRIVATE
        USE_DEBUG
    )
endif()

add_library_m7(libs_micro_ros_freertos STATIC
    ${libs_micro_ros_SOURCES}
)

target_include_directories(libs_micro_ros_freertos PRIVATE
    ${libs_micro_ros_INCLUDE_DIRECTORIES_PUBLIC}
)

target_link_libraries(libs_micro_ros_freertos
    libs_base-m7_freertos
    libs_base-m7_wifi
)

