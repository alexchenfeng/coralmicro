set(libs_micro_ros_SOURCES
    default_transport.cc
)

add_library_m7(libs_micro_ros_freertos STATIC
    ${libs_micro_ros_SOURCES}
)

target_link_libraries(libs_micro_ros_freertos
    libs_base-m7_freertos
)