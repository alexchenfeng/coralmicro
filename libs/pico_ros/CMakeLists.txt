# Copyright 2024 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(PICO_ROS_DIR ${PROJECT_SOURCE_DIR}/third_party/Pico-ROS-software)

if (NOT DEFINED PICOROS_USER_TYPE_FILE)
    set(PICOROS_USER_TYPE_FILE "example_types.h" CACHE STRING "User types header for Pico-ROS serdes")
endif()

set(LIBS_PICO_ROS_SOURCES
    # Micro-CDR serialization engine
    ${PICO_ROS_DIR}/thirdparty/Micro-CDR/src/c/common.c
    ${PICO_ROS_DIR}/thirdparty/Micro-CDR/src/c/types/array.c
    ${PICO_ROS_DIR}/thirdparty/Micro-CDR/src/c/types/basic.c
    ${PICO_ROS_DIR}/thirdparty/Micro-CDR/src/c/types/sequence.c
    ${PICO_ROS_DIR}/thirdparty/Micro-CDR/src/c/types/string.c

    # Pico-ROS core, serialization, and parameter server
    ${PICO_ROS_DIR}/src/picoros.c
    ${PICO_ROS_DIR}/src/picoserdes.c
    ${PICO_ROS_DIR}/src/picoparams.c
)

add_library_m7(libs_pico_ros STATIC
    ${LIBS_PICO_ROS_SOURCES}
)

target_include_directories(libs_pico_ros PUBLIC
    ${PICO_ROS_DIR}/src
    ${PICO_ROS_DIR}/examples
    ${PICO_ROS_DIR}/thirdparty/Micro-CDR/include
    ${PICO_ROS_DIR}/thirdparty/config
)

target_compile_definitions(libs_pico_ros PUBLIC
    ZENOH_FREERTOS_LWIP=1
    ZENOH_COMPILER_GCC=1
    USER_TYPE_FILE="${PICOROS_USER_TYPE_FILE}"
    Z_TRANSPORT_LEASE=60000
    Z_TRANSPORT_LEASE_EXPIRE_FACTOR=2
)

target_compile_options(libs_pico_ros PRIVATE
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-unused-function
)

target_link_libraries(libs_pico_ros PUBLIC
    libs_zenoh_pico
    libs_base-m7_freertos
    libs_nxp_rt1176-sdk_lwip
)

add_library(libs_pico_ros_software ALIAS libs_pico_ros)
