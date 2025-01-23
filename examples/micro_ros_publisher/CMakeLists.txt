# Copyright 2022 Google LLC
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

add_executable_m7(micro_ros_publisher
    micro_ros_publisher.cc
)

target_link_directories(micro_ros_publisher PUBLIC ../../libs/micro_ros/micro_ros_static_lib)

target_include_directories(micro_ros_publisher PUBLIC
    ../../libs/micro_ros/microros_include
)

target_link_libraries(micro_ros_publisher
    libs_base-m7_freertos
    libs_micro_ros_freertos
    libmicroros.a
)
