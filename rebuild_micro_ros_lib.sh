if [ ! -d /opt/uros_ws/ ]; then
    echo "Micro-ROS workspace not found. Please run the script in a Micro-ROS workspace."
    echo "Install ros2 base system and micro-ros-setup package, then run the following commands:"
    echo "  source /opt/uros_ws/install/setup.bash"
    echo "  bash libs/micro_ros/microros_static_library/library_generation/library_generation.sh"
    echo "  bash libs/micro_ros_arduino/extras/library_generation/library_generation.sh coral_micro_wifi"
    exit 1
fi

bash libs/micro_ros/microros_static_library/library_generation/library_generation.sh
bash libs/micro_ros_arduino/extras/library_generation/library_generation.sh coral_micro_wifi
