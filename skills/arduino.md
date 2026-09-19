Get started with Arduino
========================

The Dev Board Micro is Arduino-compatible so you can write code in the
Arduino IDE and deploy to the Dev Board Micro from there.
All you need to do is install the Coral software package from the Arduino IDE
Board Manager, as described below.

Our software package for
Arduino includes the coralmicro platform with support for the [core Arduino
programming langauge](https://www.arduino.cc/reference/en/). So if you're
experienced with Arduino, you can use all the basic Arduino
APIs you already know to do things such as interact with GPIO pins,
analog pins, and I2C pins. We've also added custom
[Arduino APIs for the Dev Board Micro](/docs/reference/micro/arduino/) that
provide an Arduino-style interface for board-specific tasks such as capturing
images from the camera and receiving audio from the microphone.

![](/static/docs/images/devboard-micro/arduino-ide-buttonled.png)

1. Gather requirements
----------------------

* Coral Dev Board Micro
* Linux or macOS computer with:
  + [Arduino IDE 2.0](https://www.arduino.cc/en/software#experimental-software)
  + Python 3
* USB-C cable (to connect the board to your computer)

**Note:** If you have Arduino IDE version 1.X,
**you must upgrade to version 2.0**.

2. Install the Coral package
----------------------------

To start building apps for the Dev Board Micro with the Arduino IDE,
just install the Coral board package as follows:

1. Open the Arduino IDE. (It must be version 2.0 or higher.)
2. In the toolbar, select **File > Preferences** (Linux) or
   **Arduino IDE > Preferences** (Mac)**.**
3. Locate the textbox for **Additional boards manager URLs** and
   paste this URL:

   `https://dl.google.com/coral/micro/package_coral_index.json`

   Then click **OK**.
4. In the toolbar, select **Tools > Board > Boards Manager.**
5. In the Boards Manager panel, search for "Coral" and click
   **Install** for **Coral by Coral Team**.

   You should see the install progress in the IDE's output window.
6. If you're on Linux, you also need to install udev rules for the Dev Board
   Micro by running these commands in a terminal (skip this if you're on Mac or
   if you already ran `coralmicro/setup.sh`):

   ```
   curl -OL https://dl.google.com/coral/micro/99-coral-micro.rules

   sudo cp 99-coral-micro.rules /etc/udev/rules.d

   sudo udevadm control --reload-rules

   sudo udevadm trigger
   ```

When the packages are done installing, you're ready to flash an Arduino sketch
to the Dev Board Micro.

3. Try the face detection example
---------------------------------

1. Connect the Dev Board Micro to your computer with a USB cable.
2. In the Arduino IDE toolbar, select **Tools > Board > Coral > Dev Board Micro**.

   The board manager should automatically select the appropriate device
   port, and the **Dev Board Micro** board name in the sketch window appears
   bold.

   If not, you can select the port from **Tools > Port**. On
   Linux, the port is something like **/dev/ttyACM0**, or on Mac, it might be
   **/dev/cu.usbmodem1101**. (If you're on Linux and don't see a port,
   make sure you [installed the udev rules](#2-set-up-the-arduino-ide).)
3. Now you should see a section called "Examples for Dev Board Micro" in the
   examples menu: In the toolbar, select **File > Examples > TensorFlow > DetectFaces**.

   If you don't see "Examples for Dev Board Micro," make sure
   you have selected the correct board (step 2).
4. Once the new sketch is loaded, select **Sketch > Upload**.

   The IDE will first compile the sketch and then upload it to the board.

   When it's done, the board reboots and loads the DetectFaces app. The green
   Camera LED turns on to indicate the camera is on and the white Edge TPU
   LED turns on to indicate the Edge TPU is running.
5. Make sure your face is well lit and then hold the board facing you, with
   the USB port facing down.

   The green User LED in the middle of the board should turn on when the
   camera sees your face.

   Try moving the camera around to see how quickly it detects when your face
   appears.

To see the detection resuls, including the
confidence score and bounding-box coordinates for each face (it can detect
multiple faces), open the Arduino Serial Monitor as described next.

**Note:**
When you're done using the board, simply unplug it to turn it off.

**Caution:** Some board components
can become hot during operation and might burn you if touched.
When handling the board, always hold it by the edges. For additional
protection, [consider using
a case](/products/dev-board-micro-case).

4. View output in the Serial Monitor
------------------------------------

The [Arduino Serial Monitor](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor) allows you to see all standard output from your apps when the Dev Board Micro
is connected to your computer via USB. (Output is the same
as if connected to the [serial console via
USB](/docs/dev-board-micro/serial-console/#connect-over-usb-otg)).

To open the Serial Monitor, select **Tools > Serial Monitor** in the toolbar.

While the face detection example is running, you should see results like this:

```
Face count: 1
 score: 0.92 xmin: 0.23 ymin: 0.45 xmax: 0.41 ymax: 0.65
```

To print messages from your Arduino code, use the global
[`Serial`](https://www.arduino.cc/reference/en/language/functions/communication/serial/)
object's
[`println()`](https://www.arduino.cc/reference/en/language/functions/communication/serial/println/)
function, and make sure you set the baud rate to `115200` with
[`begin()`](https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/).
For example:

```
void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Hello world!");
  delay(2000);
}
```

**Note:**
The Dev Board Micro requires a baud rate of
115200 for serial port communication. Many other Arduino boards
use 9600, so if you're reusing Arduino sketches from other boards,
make sure you update `Serial.begin()` to use 115200.

Next steps
----------

Now that you're ready to build apps for the Dev Board Micro with Arduino,
we recommend the following next steps to continue learning:

* **Try more code examples**: We've created dozens of examples to give you
  a head start on your own projects, including basic tasks like blink an
  LED and respond to the on-board button, and more complex tasks using the
  camera and TensorFlow Lite models. You can see them
  all in the IDE menu under **File > Examples > Examples for Dev Board Micro**.

  You can try other "Built-in examples" that are listed under
  **File > Examples**, but beware that some might not
  be compatible with the Dev Board Micro, so we recommend you primarily use
  examples that appear under "Examples for Dev Board Micro."

  **Note:** Examples for **Dev Board Micro + PoE
  Add-on** and **Dev Board Micro + Wireless Add-on** will compile only
  if you select the matching board name under **Tools > Board >
  Coral**.
* [Explore the coralmicro Arduino APIs](/docs/reference/micro/arduino/): This
  page provides details about all the major APIs available in Arduino for the
  Dev Board Micro, including [camera
  APIs](/docs/reference/micro/arduino/#camera), [microphone
  APIs](/docs/reference/micro/arduino/#microphone), and much more. You can use
  all the standard Arduino APIs for interaction with GPIOs, but our reference
  also includes definitions for the [Dev Board Micro
  pins](/docs/reference/micro/arduino/#i-o-pins-leds) and some example code.
* **Connect an add-on board**: If you have the [Coral Wireless Add-on
  board](/docs/dev-board-micro/wireless-addon/) or [Coral PoE Add-on
  board](/docs/dev-board-micro/poe-addon/), see these guides for
  setup instructions.
* [Browse our trained models](/models/): Most of these Edge TPU models are compatible
  with the Dev Board Micro, as indicated by the "Micro" column in each
  table. However, some models are not compatible because they are too
  large for the board's memory or because some parts of the model must execute on
  the MCU and require ops that are not supported by
  [TensorFlow Lite for Microcontrollers](https://www.tensorflow.org/lite/microcontrollers).

Troubleshooting
---------------

If something went wrong when uploading your sketch or the board
doesn't seem to be responding the way you expect, first try unplugging the board
and then plug it back in.

If the board still does not respond, your application might have crashed due
to a bug in the code. This can be difficult to diagnose because the board does
not have a general-purpose operating system, so review your code to see if
there's anything that the compiler cannot catch, such as if the
code tries to load a file that can't be found at runtime.

To reflash the board when the app has crashed:

1. Reset the board into Serial Downloader (SDP) mode in one of two ways:

   * Hold the User button while you press the Reset button.
   * Or, hold the User button while you plug in the USB cable. This is often
     better to ensure that the entire system is reset.

   Release the button when the orange LED turns on.
2. Then try uploading the sketch again. However, when the board is in
   Serial Downloader mode, the Arduino IDE cannot identify the serial port so it
   will say that the board is not connected. This is normal and as long as the
   board is plugged in to your computer, it should work (you can use `lsusb`
   to verify the board as shown below).
3. If the upload succeeds but your board does not automatically restart,
   reconnect the board power to restart the board. Sometimes this is necessary
   when the previous image had crashed.

Even though the Arduino IDE cannot recognize the board, you
can verify that the board is in Serial Downloader mode by opening a
terminal and running `lsusb`. When the board is in Serial Downloader mode,
`lsusb` lists the device as:

```
NXP Semiconductors SE Blank RT Family
```

Whereas when the board boots normally, the device appears as:

```
Google Inc. Coral Dev Board Micro
```