#!/bin/bash
set -e

# for coral micro project set the base path to "libs/micro_ros/microros_static_library" folder

export BASE_PATH=/project/$MICROROS_LIBRARY_FOLDER

######## Init ########
apt update

cd /uros_ws

source /opt/ros/$ROS_DISTRO/setup.bash
source install/local_setup.bash

ros2 run micro_ros_setup create_firmware_ws.sh generate_lib

######## Adding extra packages ########
pushd firmware/mcu_ws > /dev/null

    # Workaround: Copy just tf2_msgs
    if [ ! -e ros2/tf2_msgs/CMakeLists.txt ]; then
        echo "Cloning tf2_msgs"
        git clone -b jazzy https://github.com/ros2/geometry2
        cp -R geometry2/tf2_msgs ros2/tf2_msgs
        rm -rf geometry2
    else
        echo "tf2_msgs already exists"
    fi

    # Import user defined packages
    mkdir -pv extra_packages
    pushd extra_packages > /dev/null
    	USER_CUSTOM_PACKAGES_DIR=$BASE_PATH/../../microros_component/extra_packages
    	if [ -d "$USER_CUSTOM_PACKAGES_DIR" ]; then
    		cp -R $USER_CUSTOM_PACKAGES_DIR/* . || echo "copying user custom packages failed"
		fi
        if [ -f $USER_CUSTOM_PACKAGES_DIR/extra_packages.repos ]; then
        	vcs import --input $USER_CUSTOM_PACKAGES_DIR/extra_packages.repos
        fi
        cp -R $BASE_PATH/library_generation/extra_packages/* .
        vcs import --input extra_packages.repos
    popd > /dev/null

popd > /dev/null

export TOOLCHAIN_PREFIX=/uros_ws/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-

rm -rf $BASE_PATH/libmicroros
mkdir -p $BASE_PATH/libmicroros/microros_include

######## Build cortex-m7 static lib ########
echo "Building cortex-m7 static lib"
sleep 5
ros2 run micro_ros_setup build_firmware.sh $BASE_PATH/library_generation/toolchain_cortex_m7.cmake $BASE_PATH/library_generation/colcon.meta

######## copy include and lib files ########
echo "copying include files to $BASE_PATH/libmicroros/microros_include/"
sleep 5
find firmware/build/include/ -name "*.c"  -delete
cp -R firmware/build/include/* $BASE_PATH/libmicroros/microros_include/

echo "copying cortex-m7 static lib to $BASE_PATH/libmicroros/libmicroros_coral_cortex_m7.a"
cp -R firmware/build/libmicroros.a $BASE_PATH/libmicroros/libmicroros_coral_cortex_m7.a

######## Build cortex-m4 static lib ########
echo "Building cortex-m4 static lib"
sleep 5
rm -rf firmware/build
ros2 run micro_ros_setup build_firmware.sh $BASE_PATH/library_generation/toolchain_cortex_m4.cmake $BASE_PATH/library_generation/colcon.meta
echo "copying cortex-m4 static lib to $BASE_PATH/libmicroros/libmicroros_coral_cortex_m4.a"
cp -R firmware/build/libmicroros.a $BASE_PATH/libmicroros/libmicroros_coral_cortex_m4.a

######## Fix include paths  ########
pushd firmware/mcu_ws > /dev/null
    INCLUDE_ROS2_PACKAGES=$(colcon list | awk '{print $1}' | awk -v d=" " '{s=(NR==1?s:s d)$0}END{print s}')
popd > /dev/null

echo "change include paths for ros2 packages"
for var in ${INCLUDE_ROS2_PACKAGES}; do
    if [ -d "$BASE_PATH/libmicroros/microros_include/${var}/${var}" ]; then
        rsync -r $BASE_PATH/libmicroros/microros_include/${var}/${var}/* $BASE_PATH/libmicroros/microros_include/${var}
        rm -rf $BASE_PATH/libmicroros/microros_include/${var}/${var}
    fi
done

######## Generate extra files ########
echo "Generating extra files..."
find firmware/mcu_ws/ros2 \( -name "*.srv" -o -name "*.msg" -o -name "*.action" \) | awk -F"/" '{print $(NF-2)"/"$NF}' > $BASE_PATH/libmicroros/available_ros2_types
find firmware/mcu_ws/extra_packages \( -name "*.srv" -o -name "*.msg" -o -name "*.action" \) | awk -F"/" '{print $(NF-2)"/"$NF}' >> $BASE_PATH/libmicroros/available_ros2_types

cd firmware
echo "" > $BASE_PATH/libmicroros/built_packages
for f in $(find $(pwd) -name .git -type d); do pushd $f > /dev/null; echo $(git config --get remote.origin.url) $(git rev-parse HEAD) >> $BASE_PATH/libmicroros/built_packages; popd > /dev/null; done;
echo "coral micro uros static library generated successfully"