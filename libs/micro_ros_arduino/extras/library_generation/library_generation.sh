#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
BASE_PATH="$SCRIPT_DIR/../"
SRC_PATH="$SCRIPT_DIR/../../src"
echo "BASE_PATH: $BASE_PATH"
unset RMW_IMPLEMENTATION

PLATFORMS=()
while getopts "p:" o; do
    case "$o" in
        p)
            PLATFORMS+=(${OPTARG})
            ;;
    esac
done

if [ $OPTIND -eq 1 ]; then
    PLATFORMS+=("opencr1")
    PLATFORMS+=("teensy4")
    PLATFORMS+=("teensy32")
    PLATFORMS+=("teensy35")
    PLATFORMS+=("teensy36")
    PLATFORMS+=("cortex_m0")
    PLATFORMS+=("cortex_m3")
    PLATFORMS+=("cortex_m4")
    # PLATFORMS+=("portenta-m4")
    PLATFORMS+=("portenta-m7")
    PLATFORMS+=("kakutef7-m7")
    PLATFORMS+=("esp32")
    PLATFORMS+=("coral_micro_wifi")
    PLATFORMS+=("coral_micro_poe")
    PLATFORMS+=("coral_micro")
fi

shift $((OPTIND-1))

######## Init ########

apt update

cd /opt/uros_ws

source /opt/ros/$ROS_DISTRO/setup.bash
source install/local_setup.bash

ros2 run micro_ros_setup create_firmware_ws.sh generate_lib

######## Adding extra packages ########
pushd firmware/mcu_ws > /dev/null

    # Workaround: Copy just tf2_msgs
    git clone -b jazzy https://github.com/ros2/geometry2
    cp -R geometry2/tf2_msgs ros2/tf2_msgs
    rm -rf geometry2

    # Import user defined packages
    mkdir -pv extra_packages
    pushd extra_packages > /dev/null
        cp -R $BASE_PATH/library_generation/extra_packages/* .
        vcs import --input extra_packages.repos
    popd > /dev/null

popd > /dev/null

######## Clean and source ########
find $SRC_PATH ! -name micro_ros_arduino.h ! -name *.c ! -name *.cpp ! -name *.c.in -delete



######## Build for coral_micro ########

if [[ " ${PLATFORMS[@]} " =~ " coral_micro_wifi " || " ${PLATFORMS[@]} " =~ " coral_micro_poe " || " ${PLATFORMS[@]} " =~ " coral_micro " ]]; then
    rm -rf firmware/build

    export TOOLCHAIN_PREFIX=/opt/coralmicro/third_party/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-
    ros2 run micro_ros_setup build_firmware.sh $BASE_PATH/library_generation/coral_micro_toolchain_m7.cmake $BASE_PATH/library_generation/colcon.meta

    find firmware/build/include/ -name "*.c"  -delete
    cp -R firmware/build/include/* $SRC_PATH/

    mkdir -pv $SRC_PATH/cortex-m7/fpv5-d16-hard
    cp -R firmware/build/libmicroros.a $SRC_PATH/cortex-m7/fpv5-d16-hard/libmicroros.a
fi

######## Fix include paths  ########
pushd firmware/mcu_ws > /dev/null
    INCLUDE_ROS2_PACKAGES=$(colcon list | awk '{print $1}' | awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}')
popd > /dev/null

apt -y install rsync
for var in ${INCLUDE_ROS2_PACKAGES}; do
    rsync -r $SRC_PATH/${var}/${var}/* $SRC_PATH/${var}/ > /dev/null 2>&1
    rm -rf $SRC_PATH/${var}/${var}/ > /dev/null 2>&1
done

######## Generate extra files ########
find firmware/mcu_ws/ros2 \( -name "*.srv" -o -name "*.msg" -o -name "*.action" \) | awk -F"/" '{print $(NF-2)"/"$NF}' > $SRC_PATH/../available_ros2_types
find firmware/mcu_ws/extra_packages \( -name "*.srv" -o -name "*.msg" -o -name "*.action" \) | awk -F"/" '{print $(NF-2)"/"$NF}' >> $SRC_PATH/../available_ros2_types
# sort it so that the result order is reproducible
sort -o $SRC_PATH/../available_ros2_types $SRC_PATH/../available_ros2_types

cd firmware
echo "" > $SRC_PATH/../built_packages
for f in $(find $(pwd) -name .git -type d); do pushd $f > /dev/null; echo $(git config --get remote.origin.url) $(git rev-parse HEAD) >> $SRC_PATH/../built_packages; popd > /dev/null; done;
# sort it so that the result order is reproducible
sort -o $SRC_PATH/../built_packages $SRC_PATH/../built_packages
