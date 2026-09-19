Arduino APIs
============

The Coral Dev Board Micro is compatible with the [core Arduino
programming langauge](https://www.arduino.cc/reference/en/). So a lot of
simple Arduino projects can work on the Dev Board Micro without any code
changes.

This page describes additional APIs that provide Arduino-style programming for
Dev Board Micro hardware, such as the camera and
microphone, plus descriptions for features that use standard Arduino
libraries, such as GPIOs and the filesystem.

Even the APIs that have custom implementations for the Dev Board Micro
are designed to allow code-reuse with other Arduino
projects that use similar libraries whenever possible.

To try programming with Arduino for the Dev Board Micro, start by following the
guide to [Build apps with Arduino](/docs/dev-board-micro/arduino/).

Camera
------

The Dev Board Micro includes an on-board camera module with 324 x 324 px
resolution. To use the camera with Arduino, you need to use the global instance
of the [`CameraClass`](#_CPPv4N10coralmicro7arduino11CameraClassE "coralmicro::arduino::CameraClass") object called `Camera`.

To get started, you just need to call
[`begin()`](#_CPPv4N10coralmicro7arduino11CameraClass5beginE16CameraResolution "coralmicro::arduino::CameraClass::begin") and specify the image format
you want, such as the image resolution and rotation.

When you want to capture an image, call
[`grab()`](#_CPPv4N10coralmicro7arduino11CameraClass4grabEP7uint8_t "coralmicro::arduino::CameraClass::grab")
and pass it a buffer where the image should be stored.

When youâre done, call
`end` to turn the camera off.

**Example**:

```
#include <coralmicro_SD.h>
#include <coralmicro_camera.h>
#include <libs/libjpeg/jpeg.h>

#include <cstdint>
#include <memory>

#include "Arduino.h"

using namespace coralmicro::arduino;

FrameBuffer frame_buffer;

void setup() {
  Serial.begin(115200);
  SD.begin();
  // Turn on Status LED to show the board is on.
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("Arduino Camera!");
  int width = 324;
  int height = 324;

  if (Camera.begin(width, height) != CameraStatus::SUCCESS) {
    Serial.println("Failed to start camera");
    return;
  }

  if (!Camera.grab(frame_buffer) == CameraStatus::SUCCESS) {
    return;
  }

  Serial.println("Saving image as \"image.jpeg\"");
  std::vector<uint8_t> jpeg;
  coralmicro::JpegCompressRgb(frame_buffer.getBuffer(), width, height,
                              /*quality=*/75, &jpeg);

  SDFile imageFile = SD.open("image.jpeg", FILE_WRITE);
  imageFile.write(jpeg.data(), jpeg.size());
  imageFile.close();
}

void loop() {}
```

[[coralmicro\_camera.h source]](https://github.com/google-coral/coralmicro/blob/main/arduino/libraries/CoralMicro_Camera/src/coralmicro_camera.h)

*namespace* `coralmicro`[Â¶](#_CPPv410coralmicro "Permalink to this definition")
:   *namespace* `arduino`[Â¶](#_CPPv4N10coralmicro7arduinoE "Permalink to this definition")
    :   *class* `CameraClass`[Â¶](#_CPPv4N10coralmicro7arduino11CameraClassE "Permalink to this definition")
        :   *#include <coralmicro\_camera.h>*

            Exposes the Coral Micro deviceâs native camera.

            You should not initialize this object yourself; instead include `coralmicro_camera.h` and then use the global `Camera` instance.

            Public Functions

            int `begin`([CameraResolution](#_CPPv4N10coralmicro7arduino16CameraResolutionE "coralmicro::arduino::CameraResolution") *resolution* = [CAMERA\_R324x324](#_CPPv4N10coralmicro7arduino16CameraResolution15CAMERA_R324x324E "coralmicro::arduino::CAMERA_R324x324"))[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass5beginE16CameraResolution "Permalink to this definition")
            :   Starts the camera.

                Parameters
                :   **resolution** â A `CameraResolution` value representing the cameraâs resolution.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` once initialization has completed.

            int `begin`(int32\_t *width* = 320, int32\_t *height* = 320, [CameraFormat](../camera/#_CPPv4N10coralmicro12CameraFormatE "coralmicro::CameraFormat") *fmt* = [CameraFormat](../camera/#_CPPv4N10coralmicro12CameraFormatE "coralmicro::CameraFormat")::[kRgb](../camera/#_CPPv4N10coralmicro12CameraFormat4kRgbE "coralmicro::CameraFormat::kRgb"), [CameraFilterMethod](../camera/#_CPPv4N10coralmicro18CameraFilterMethodE "coralmicro::CameraFilterMethod") *filter* = [CameraFilterMethod](../camera/#_CPPv4N10coralmicro18CameraFilterMethodE "coralmicro::CameraFilterMethod")::[kBilinear](../camera/#_CPPv4N10coralmicro18CameraFilterMethod9kBilinearE "coralmicro::CameraFilterMethod::kBilinear"), [CameraRotation](../camera/#_CPPv4N10coralmicro14CameraRotationE "coralmicro::CameraRotation") *rotation* = [CameraRotation](../camera/#_CPPv4N10coralmicro14CameraRotationE "coralmicro::CameraRotation")::[k270](../camera/#_CPPv4N10coralmicro14CameraRotation4k270E "coralmicro::CameraRotation::k270"), bool *preserve\_ratio* = false, bool *auto\_white\_balance* = true)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass5beginE7int32_t7int32_t12CameraFormat18CameraFilterMethod14CameraRotationbb "Permalink to this definition")
            :   Starts the camera.

                Parameters
                :   * **width** â The resolution width in pixels.
                    * **height** â The resolution height in pixels.
                    * **fmt** â The image format.
                    * **filter** â The bayer filtering method.
                    * **rotation** â The image rotation amount.
                    * **auto\_white\_balance** â Applies auto white balance if true.

                Returns
                :   `SUCCESS` once initialization has completed.

            int `grab`(uint8\_t \**buffer*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass4grabEP7uint8_t "Permalink to this definition")
            :   Grabs a camera frame.

                Parameters
                :   **buffer** â The buffer where the frame will be stored.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` if a frame was captured from the camera, `NOT_INITIALIZED` if the camera was not initialized, or `FAILURE` if a frame was not captured.

            int `grab`([FrameBuffer](#_CPPv4N10coralmicro7arduino11FrameBufferE "coralmicro::arduino::FrameBuffer") &*buffer*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass4grabER11FrameBuffer "Permalink to this definition")
            :   Grabs a camera frame.

                Parameters
                :   **buffer** â The buffer where the frame will be stored.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` if a frame was captured from the camera, `NOT_INITIALIZED` if the camera was not initialized, or `FAILURE` if a frame was not captured. Note: buffer does not have to be allocated, this function will automatically allocate the memory needed to store the image data.

            int `setStandby`(bool *enable*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass10setStandbyEb "Permalink to this definition")
            :   Changes the camera mode.

                Parameters
                :   **enable** â Sets the camera to standby mode if true, and sets the camera to streaming mode if false.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` once the camera mode is set.

            int `setTestPattern`(bool *enable*, bool *walking*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass14setTestPatternEbb "Permalink to this definition")
            :   Changes the cameraâs test pattern. Test pattern data is fake data that replaces camera sensor data when the value is not `NONE`.

                Parameters
                :   * **walking** â Sets the test pattern to `WALKING_ONES` if true, and to `NONE` if false.
                    * **enable** â Starts test pattern if true, else only set the test pattern.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` once the pattern has been set.

            int `setTestPattern`(bool *enable*, [coralmicro](#_CPPv410coralmicro "coralmicro")::[CameraTestPattern](../camera/#_CPPv4N10coralmicro17CameraTestPatternE "coralmicro::CameraTestPattern") *pattern*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass14setTestPatternEbN10coralmicro17CameraTestPatternE "Permalink to this definition")
            :   Changes the cameraâs test pattern. Test pattern data is fake data that replaces camera sensor data when the value is not `NONE`.

                Parameters
                :   * **pattern** â The desired new test pattern.
                    * **enable** â Starts test pattern if true, else only set the test pattern.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` once the pattern has been set.

            int `setPreserveRatio`(bool *preserve\_ratio*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass16setPreserveRatioEb "Permalink to this definition")
            :   Sets whether the imageâs aspect ratio is preserved.

                Parameters
                :   **preserve\_ratio** â Images will be scaled to preserve aspect ratio if true.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` once the configuration has been set.

            int `setPixelFormat`([coralmicro](#_CPPv410coralmicro "coralmicro")::[CameraFormat](../camera/#_CPPv4N10coralmicro12CameraFormatE "coralmicro::CameraFormat") *fmt*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass14setPixelFormatEN10coralmicro12CameraFormatE "Permalink to this definition")
            :   Sets the image format.

                Parameters
                :   **fmt** â The image format.

                Returns
                :   A `CameraSttus` value such as `SUCCESS` once the format has been set.

            int `discardFrames`(int *num\_frames*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass13discardFramesEi "Permalink to this definition")
            :   Discards a set amount of frames captured by the camera.

                Parameters
                :   **num\_frames** â The amount of frames to discard.

                Returns
                :   A `CameraStatus` value such as `SUCCESS` once the camera has started discarding frames.

            int `enableMotionDetection`([md\_callback\_t](#_CPPv4N10coralmicro7arduino13md_callback_tE "coralmicro::arduino::md_callback_t") *callback* = nullptr, void \**cb\_param* = nullptr)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass21enableMotionDetectionE13md_callback_tPv "Permalink to this definition")
            :   Enables or disables HW motion detection.

                Parameters
                :   * **callback** â The function to call when the camera detected motion.
                    * **cb\_param** â The callback parameter.

            int `disableMotionDetection`()[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass22disableMotionDetectionEv "Permalink to this definition")
            :   Disables motion detection.

            int `setMotionDetectionWindow`(uint32\_t *x*, uint32\_t *y*, uint32\_t *w*, uint32\_t *h*)[Â¶](#_CPPv4N10coralmicro7arduino11CameraClass24setMotionDetectionWindowE8uint32_t8uint32_t8uint32_t8uint32_t "Permalink to this definition")
            :   Sets the motion detection windows.

                Parameters
                :   * **x** â The top left x coordinate to monitor motion detection.
                    * **y** â the top left y coordinate to monitor motion detection.
                    * **w** â The width of the window to monitor for motion.
                    * **h** â the height of the window to monitor for motion.

        *class* `FrameBuffer`[Â¶](#_CPPv4N10coralmicro7arduino11FrameBufferE "Permalink to this definition")
        :   *#include <coralmicro\_camera.h>*

            Represents an imageâs frame buffer.

            Public Functions

            `FrameBuffer`()[Â¶](#_CPPv4N10coralmicro7arduino11FrameBuffer11FrameBufferEv "Permalink to this definition")
            :   Constructs a new frame buffer object without allocating framebuffer pointer.

            `~FrameBuffer`()[Â¶](#_CPPv4N10coralmicro7arduino11FrameBufferD0Ev "Permalink to this definition")
            :   Destroys and delete the frame buffer if it is allocated. Note: The buffer must be allocated with `new []` and not `malloc()`.

            uint32\_t `getBufferSize`()[Â¶](#_CPPv4N10coralmicro7arduino11FrameBuffer13getBufferSizeEv "Permalink to this definition")
            :   Gets the frame buffer size.

                Returns
                :   The bufferâs size.

            uint8\_t \*`getBuffer`()[Â¶](#_CPPv4N10coralmicro7arduino11FrameBuffer9getBufferEv "Permalink to this definition")
            :   Gets the frame bufferâs data.

                Returns
                :   The pointer to the frame bufferâs data.

            void `setBuffer`(uint8\_t \**buffer*, uint32\_t *frame\_size*)[Â¶](#_CPPv4N10coralmicro7arduino11FrameBuffer9setBufferEP7uint8_t8uint32_t "Permalink to this definition")
            :   Set the frame bufferâs data.

                Parameters
                :   * **buffer** â The frame buffer pointer to set.
                    * **frame\_size** â The frame buffer size to set.

            bool `hasFixedSize`()[Â¶](#_CPPv4N10coralmicro7arduino11FrameBuffer12hasFixedSizeEv "Permalink to this definition")
            :   Checks if the frame buffer has a fixed size.

                Returns
                :   true if the frame buffer has been allocated and the size is known.

            bool `isAllocated`()[Â¶](#_CPPv4N10coralmicro7arduino11FrameBuffer11isAllocatedEv "Permalink to this definition")
            :   Checks if the frame buffer is allocated.

                Returns
                :   true if the framebuffer is allocated, else false.

        Enums

        *enum* `CameraResolution`[Â¶](#_CPPv4N10coralmicro7arduino16CameraResolutionE "Permalink to this definition")
        :   Supported camera resolutions.

            *Values:*

            *enumerator* `CAMERA_R160x120`[Â¶](#_CPPv4N10coralmicro7arduino16CameraResolution15CAMERA_R160x120E "Permalink to this definition")
            :   QQVGA Resolution.

            *enumerator* `CAMERA_R320x240`[Â¶](#_CPPv4N10coralmicro7arduino16CameraResolution15CAMERA_R320x240E "Permalink to this definition")
            :   QVGA Resolution.

            *enumerator* `CAMERA_R320x320`[Â¶](#_CPPv4N10coralmicro7arduino16CameraResolution15CAMERA_R320x320E "Permalink to this definition")
            :   320x320 Resolution

            *enumerator* `CAMERA_R324x324`[Â¶](#_CPPv4N10coralmicro7arduino16CameraResolution15CAMERA_R324x324E "Permalink to this definition")
            :   324x324 Default camera resolution.

            *enumerator* `CAMERA_RMAX`[Â¶](#_CPPv4N10coralmicro7arduino16CameraResolution11CAMERA_RMAXE "Permalink to this definition")

        *enum* `CameraStatus`[Â¶](#_CPPv4N10coralmicro7arduino12CameraStatusE "Permalink to this definition")
        :   Return statuses from camera API functions.

            *Values:*

            *enumerator* `FAILURE`[Â¶](#_CPPv4N10coralmicro7arduino12CameraStatus7FAILUREE "Permalink to this definition")

            *enumerator* `SUCCESS`[Â¶](#_CPPv4N10coralmicro7arduino12CameraStatus7SUCCESSE "Permalink to this definition")

            *enumerator* `NOT_INITIALIZED`[Â¶](#_CPPv4N10coralmicro7arduino12CameraStatus15NOT_INITIALIZEDE "Permalink to this definition")

            *enumerator* `UNIMPLEMENTED`[Â¶](#_CPPv4N10coralmicro7arduino12CameraStatus13UNIMPLEMENTEDE "Permalink to this definition")

        Typedefs

        *using* `md_callback_t` = void (\*)(void \*param)[Â¶](#_CPPv4N10coralmicro7arduino13md_callback_tE "Permalink to this definition")
        :   Motion detection callback.

Variables

[coralmicro](#_CPPv410coralmicro "coralmicro")::[arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[CameraClass](#_CPPv4N10coralmicro7arduino11CameraClassE "coralmicro::arduino::CameraClass") `Camera`[Â¶](#_CPPv46Camera "Permalink to this definition")
:   This is the global `CameraClass` instance you should use instead of creating your own instance.

Microphone
----------

The Dev Board Micro includes one on-board pulse-density modulation (PDM)
microphone. To use the microphone with Arduino, you need to use the global
instance of the global instance of the [`PDMClass`](#_CPPv4N10coralmicro7arduino8PDMClassE "coralmicro::arduino::PDMClass")
object called `Mic`.

This API is designed to be code-compatible with projects that use the
[Arduino PDM library](https://docs.arduino.cc/learn/built-in-libraries/pdm),
but it does not support `setGain()` and `setBufferSize()`.

To get started, specify a
callback function with [`onReceive()`](#_CPPv4N10coralmicro7arduino8PDMClass9onReceiveEPFvvE "coralmicro::arduino::PDMClass::onReceive"),
to be notified whenever the microphone has new data to read.

Then call [`begin()`](#_CPPv4N10coralmicro7arduino8PDMClass5beginEi6size_t6size_t "coralmicro::arduino::PDMClass::begin") to start recording
with the microphone. Inside the function you passed to
[`onReceive()`](#_CPPv4N10coralmicro7arduino8PDMClass9onReceiveEPFvvE "coralmicro::arduino::PDMClass::onReceive"), read the available audio
data with [`read()`](#_CPPv4N10coralmicro7arduino8PDMClass4readERNSt6vectorI7int32_tEE6size_t "coralmicro::arduino::PDMClass::read").

The microphone remains powered and
processes input whenever there is an active audio callback.

When youâre done with the microphone, call
[`end()`](#_CPPv4N10coralmicro7arduino8PDMClass3endEv "coralmicro::arduino::PDMClass::end") to disable the micrphone.

**Example**:

This code listens to input from the microphone and saves data into a
`currentSamples` buffer.

```
#include "Arduino.h"
#include "PDM.h"

volatile int samples_read = 0;
std::vector<int32_t> current_samples;

void setup() {
  Serial.begin(115200);
  // Turn on Status LED to show the board is on.
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("Arduino PDM!");

  Mic.onReceive(onPDMData);
  Mic.begin();
}

void loop() {
  if (samples_read) {
    samples_read = 0;
    Serial.println(current_samples[0]);
  }
}

void onPDMData() {
  samples_read = Mic.available();

  if (samples_read) {
    current_samples.clear();
    Mic.read(current_samples, samples_read);
  }
}
```

[[PDM.h source]](https://github.com/google-coral/coralmicro/blob/main/arduino/libraries/PDM/src/PDM.h)

*namespace* `coralmicro`
:   *namespace* `arduino`
    :   *class* `PDMClass`[Â¶](#_CPPv4N10coralmicro7arduino8PDMClassE "Permalink to this definition")
        :   *#include <PDM.h>*

            Exposes the Coral Micro deviceâs native PDM microphone.

            You should not initialize this object yourself; instead include `PDM.h` and then use the global `Mic` instance.

            Public Functions

            int `begin`(int *sample\_rate* = 16000, size\_t *sample\_size\_ms* = 1000, size\_t *drop\_first\_samples\_ms* = 150)[Â¶](#_CPPv4N10coralmicro7arduino8PDMClass5beginEi6size_t6size_t "Permalink to this definition")
            :   Start recording data with the PDM microphone.

                Parameters
                :   * **sample\_rate** â The sample rate to start, only supports 16000 or 48000.
                    * **sample\_size\_ms** â Number of ms of data that is stored in the internal buffer.
                    * **drop\_first\_samples\_ms** â Number of ms to drop during begin to avoid distortions.

                Returns
                :   0 on failure, 1 on success.

            void `onReceive`(void (\**function*)(void))[Â¶](#_CPPv4N10coralmicro7arduino8PDMClass9onReceiveEPFvvE "Permalink to this definition")
            :   Sets the current audio callback function. The microphone starts as soon as `begin()` is called, however this function adds an extra callback that gets executed as soon as new data are received. Within the callback, you can call `available()` and `read()` to access the microphone data.

                Parameters
                :   **function** â The function to call when audio data is received. The function should not have any arguments and not have a return value.

            void `end`()[Â¶](#_CPPv4N10coralmicro7arduino8PDMClass3endEv "Permalink to this definition")
            :   Removes the current callback, effectively turning off the microphone.

            int `available`()[Â¶](#_CPPv4N10coralmicro7arduino8PDMClass9availableEv "Permalink to this definition")
            :   Gets the amount of available data in the audio buffer. Data is stored in the buffer as `uint32_t`s, and the sizes in this function refer to the amount of values in the buffer.

                Returns
                :   The amount of data values stored in the underlying buffer that are ready to be read.

            int `read`(std::vector<int32\_t> &*buffer*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro7arduino8PDMClass4readERNSt6vectorI7int32_tEE6size_t "Permalink to this definition")
            :   Reads data from the audio buffer. Data is stored in the buffer as `uint32_t`s, and the sizes in this function refer to the amount of values in the buffer.

                Parameters
                :   * **buffer** â The buffer that will receive the copied audio data.
                    * **size** â The amount of audio data values to copy.

                Returns
                :   The amount of audio data values that were copied.

Variables

[coralmicro](#_CPPv410coralmicro "coralmicro")::[arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[PDMClass](#_CPPv4N10coralmicro7arduino8PDMClassE "coralmicro::arduino::PDMClass") `Mic`[Â¶](#_CPPv43Mic "Permalink to this definition")
:   This is the global `PDMClass` instance you should use instead of creating your own instance.

Filesystem
----------

This API allows you to create, read, and write files on the Dev Board Micro
flash storage.

This API is designed to be code-compatible with projects that use the
[Arduino SD library](https://www.arduino.cc/reference/en/libraries/sd/).
However, on the Dev Board Micro, these APIs enable reading and writing files on
the on-board flash memory (not an SD card).

[[coralmicro\_SD.h source]](https://github.com/google-coral/coralmicro/blob/main/arduino/libraries/CoralMicro_SD/src/coralmicro_SD.h)

*namespace* `coralmicro`
:   *namespace* `arduino`
    :   *namespace* `SDLib`[Â¶](#_CPPv4N10coralmicro7arduino5SDLibE "Permalink to this definition")
        :   *class* `File` : *public* Stream[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4FileE "Permalink to this definition")
            :   *#include <coralmicro\_SD.h>*

                Represents a file in the flash memory filesystem (represented by `SDClass`).

                Public Functions

                `File`(void)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File4FileEv "Permalink to this definition")

                *virtual* size\_t `write`(uint8\_t)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File5writeE7uint8_t "Permalink to this definition")
                :   Writes one byte to the file.

                    Parameters
                    :   **val** â The byte to write.

                    Returns
                    :   The amount of data written.

                *virtual* size\_t `write`(*const* uint8\_t \**buf*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File5writeEPK7uint8_t6size_t "Permalink to this definition")
                :   Writes data to the file.

                    Parameters
                    :   * **buf** â The data to write.
                        * **size** â The size of the data.

                    Returns
                    :   The amount of data written.

                *virtual* int `read`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File4readEv "Permalink to this definition")
                :   Reads one value from the file.

                    Returns
                    :   The value read from the file.

                *virtual* int `peek`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File4peekEv "Permalink to this definition")
                :   Gets the next value in the file, without advancing through the file.

                    Returns
                    :   The next value in the file.

                *virtual* int `available`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File9availableEv "Permalink to this definition")
                :   The amount of data left in the file.

                    Returns
                    :   The amount of data until the end of the file, given the current position.

                *virtual* void `flush`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File5flushEv "Permalink to this definition")
                :   Ensures the data is written to the file.

                int `read`(void \**buf*, size\_t *nbyte*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File4readEPv6size_t "Permalink to this definition")
                :   Reads data from the file.

                    Parameters
                    :   * **buf** â The buffer to store the data.
                        * **nbyte** â The amount of data to read.

                    Returns
                    :   The amount of data read.

                bool `seek`(size\_t *pos*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File4seekE6size_t "Permalink to this definition")
                :   Sets the current position of the reader within the file.

                    Parameters
                    :   **pos** â The desired index for reading from the file.

                size\_t `position`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File8positionEv "Permalink to this definition")
                :   Gets the current position of the reader within the file.

                    Returns
                    :   The position of the reader.

                size\_t `size`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File4sizeEv "Permalink to this definition")
                :   Gets the size of the file.

                    Returns
                    :   The size of the file.

                void `close`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File5closeEv "Permalink to this definition")
                :   Closes the file.

                *inline* `operator bool`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4FilecvbEv "Permalink to this definition")
                :   Uses the file object as a bool to determine whether it is open.

                    Returns
                    :   True if the file is open; false otherwise.

                char \*`name`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File4nameEv "Permalink to this definition")
                :   Gets the name of the file.

                    Returns
                    :   The name of the file.

                *inline* bool `isDirectory`(void)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File11isDirectoryEv "Permalink to this definition")
                :   Determines whether the file object is a directory.

                    Returns
                    :   True if the file object is a directory, false otherwise.

                [File](#_CPPv4N10coralmicro7arduino5SDLib4FileE "coralmicro::arduino::SDLib::File") `openNextFile`(uint8\_t *mode* = FILE\_READ)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File12openNextFileE7uint8_t "Permalink to this definition")
                :   Gets the next file or folder if the current file object is a directory

                    Parameters
                    :   **mode** â The mode for opening the file.

                    Returns
                    :   The next file object opened with the given mode if it exists. Otherwise, returns an empty file object.

                void `rewindDirectory`(void)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib4File15rewindDirectoryEv "Permalink to this definition")
                :   Returns to the first file in the directory.

            *class* `SDClass`[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClassE "Permalink to this definition")
            :   *#include <coralmicro\_SD.h>*

                Exposes the Dev Board Microâs internal filesystem, treating it as if it were an SD card.

                You should not initialize this object yourself; instead include `coralmicro_SD.h` and then use the global `SD` instance. `begin()` and `end()` are unnecessary calls, because the filesystem is initialized when the device boots up and cannot be deinitialized. Example code can be found in `sketches/SDFileSystemTest/`.

                Public Functions

                bool `begin`(uint8\_t *csPin* = DONT\_CARE)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass5beginE7uint8_t "Permalink to this definition")
                :   This function is unused and is here to match the Arduino API.

                    Parameters
                    :   **csPin** â Unused and is here to match the Arduino API.

                    Returns
                    :   True if initialization was successful, false otherwise.

                *inline* bool `begin`(uint32\_t *clock*, uint8\_t *csPin*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass5beginE8uint32_t7uint8_t "Permalink to this definition")
                :   This function is unused and is here to match the Arduino API.

                    Parameters
                    :   * **csPin** â Unused and is here to match the Arduino API.
                        * **clock** â Unused and is here to match the Arduino API.

                    Returns
                    :   True if initialization was successful, false otherwise.

                *inline* void `end`()[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass3endEv "Permalink to this definition")
                :   This function is unused and is here to match the Arduino API.

                [File](#_CPPv4N10coralmicro7arduino5SDLib4FileE "coralmicro::arduino::SDLib::File") `open`(*const* char \**filename*, uint8\_t *mode* = FILE\_READ)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass4openEPKc7uint8_t "Permalink to this definition")
                :   Opens the file or directory at the given path

                    Parameters
                    :   * **filename** â The path of the file object.
                        * **mode** â The mode for opening the file object.

                    Returns
                    :   The opened file object.

                *inline* [File](#_CPPv4N10coralmicro7arduino5SDLib4FileE "coralmicro::arduino::SDLib::File") `open`(*const* String &*filename*, uint8\_t *mode* = FILE\_READ)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass4openERK6String7uint8_t "Permalink to this definition")
                :   Opens the file or directory at the given path

                    Parameters
                    :   * **filename** â The path of the file object.
                        * **mode** â The mode for opening the file object.

                    Returns
                    :   The opened file object.

                bool `exists`(*const* char \**filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass6existsEPKc "Permalink to this definition")
                :   Determines whether a file or directory exists at the given path.

                    Parameters
                    :   **filepath** â The path to check for a file.

                    Returns
                    :   True if a file exists at the path; false otherwise.

                *inline* bool `exists`(*const* String &*filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass6existsERK6String "Permalink to this definition")
                :   Determines whether a file or directory exists at the given path.

                    Parameters
                    :   **filepath** â The path to check for a file.

                    Returns
                    :   True if a file exists at the path, false otheriwse.

                bool `mkdir`(*const* char \**filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass5mkdirEPKc "Permalink to this definition")
                :   Creates a directory at the given path.

                    Parameters
                    :   **filepath** â The location to make a directory.

                    Returns
                    :   True if a directory was successfully created, false otherwise.

                *inline* bool `mkdir`(*const* String &*filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass5mkdirERK6String "Permalink to this definition")
                :   Creates a directory at the given path.

                    Parameters
                    :   **filepath** â The location to make a directory.

                    Returns
                    :   True if a directory was successfully created, false otherwise.

                bool `remove`(*const* char \**filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass6removeEPKc "Permalink to this definition")
                :   Removes the file at the given path

                    Parameters
                    :   **filepath** â The location of the file to remove.

                    Returns
                    :   True if a file was successfully removed, false otherwise.

                *inline* bool `remove`(*const* String &*filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass6removeERK6String "Permalink to this definition")
                :   Removes the file at the given path

                    Parameters
                    :   **filepath** â The location of the file to remove.

                    Returns
                    :   True if a file was successfully removed, false otherwise.

                bool `rmdir`(*const* char \**filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass5rmdirEPKc "Permalink to this definition")
                :   Removes the directory at the given path

                    Parameters
                    :   **filepath** â The location of the directory to remove.

                    Returns
                    :   True if a directory was successfully removed, false otherwise.

                *inline* bool `rmdir`(*const* String &*filepath*)[Â¶](#_CPPv4N10coralmicro7arduino5SDLib7SDClass5rmdirERK6String "Permalink to this definition")
                :   Removes the directory at the given path

                    Parameters
                    :   **filepath** â The location of the directory to remove.

                    Returns
                    :   True if a directory was successfully removed, false otherwise.

Typedefs

*typedef* [coralmicro](#_CPPv410coralmicro "coralmicro")::[arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[SDLib](#_CPPv4N10coralmicro7arduino5SDLibE "coralmicro::arduino::SDLib")::[File](#_CPPv4N10coralmicro7arduino5SDLib4FileE "coralmicro::arduino::SDLib::File") `SDFile`[Â¶](#_CPPv46SDFile "Permalink to this definition")

Variables

[coralmicro](#_CPPv410coralmicro "coralmicro")::[arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[SDLib](#_CPPv4N10coralmicro7arduino5SDLibE "coralmicro::arduino::SDLib")::[SDClass](#_CPPv4N10coralmicro7arduino5SDLib7SDClassE "coralmicro::arduino::SDLib::SDClass") `SD`[Â¶](#_CPPv42SD "Permalink to this definition")
:   This is the global `SDClass` instance you should use instead of creating your own instance.

I/O pins & LEDs
---------------

You can interact with GPIO pins (digital or analog input/output pins)
on the Dev Board Micro 12-pin headers with Arduino the same as you would on
other Arduino boards, using Arduino APIs such as
[digitalRead()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/),
[digitalWrite()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/),
[analogRead()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/), and
[analogWrite()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/).
To address these GPIO pins, use the
Arduino pin names shown in figure 1.

Likewise, you can use
[digitalWrite()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/)
to toggle the on-board LEDs and
[digitalRead()](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/)
to listen for User button presses.

![](/static/docs/reference/micro/images/micro-pinout.png)


**Figure 1.** Pinout for the 12-pin headers, LEDs and User button

Once booting is complete, all digital/analog pins are set to a high-Z (floating)
state, except for the I2C pins, which default to high. So
be sure to call [pinMode()](https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/)
before reading or writing values.

Note

All GPIO pins are powered by the 1.8 V power rail, and provide a max current
of approximately 6 mA on most pins.

**Example**:

This code toggles the on-board User LED when you press the on-board User button.

```
#include "Arduino.h"

int led_pin = PIN_LED_USER;
int button_pin = PIN_BTN;
PinStatus val = LOW;

void setup() {
  Serial.begin(115200);
  // Turn on Status LED to show the board is on.
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("Arduino Button LED!");

  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);
}

void loop() {
  val = digitalRead(button_pin);
  digitalWrite(led_pin, val == LOW ? HIGH : LOW);
}
```

For more information about using digital and analog pins, see the
[Arduino GPIO documentation](https://docs.arduino.cc/learn/starting-guide/getting-started-arduino#gpio--pin-management).

I2C
---

You can interact with I2C devices on the Dev Board Micro using the [Arduino Wire
library](https://www.arduino.cc/reference/en/language/functions/communication/wire/).

Only one pair of I2C lines on the 12-pin headers is available in Arduino,
so you do not need to specify the pin names. It is assumed that you are
connected to `D0` and `D3` (shown in figure 1).

**Example**:

This code sends messages to an I2C device thatâs connected to `D0` and `D3`.

```
#include <cstdint>
#include <vector>

#include "Arduino.h"
#include "Wire.h"

namespace {
constexpr int kTargetAddress = 0x42;
constexpr int kTransferSize = 16;
std::vector<uint8_t> buffer(kTransferSize, 0);
}  // namespace

void requestEvent() { Wire.write(buffer.data(), kTransferSize); }

void receiveEvent(int count) {
  for (int i = 0; i < count && i < kTransferSize; ++i) {
    buffer[i] = Wire.read();
    Serial.print(buffer[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  // Turn on Status LED to show the board is on.
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("Arduino I2C Target Example!");

  Wire.begin(kTargetAddress);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {}
```

SPI
---

You can interact with Serial Peripheral Interface (SPI) devices connected to
the SPI pins on the 12-pin headers (see figure 1), using the APIs described
below. The Dev Board Micro must be the controller.

Only one SPI bus available on the 12-pin headers, so you do not
need to specify the pin names.

Note

This SPI is designed to be compatible with the [Arduino SPI library](https://www.arduino.cc/reference/en/language/functions/communication/spi/),
but our API is a little different because the Dev Board Micro does not
support using SPI from interrupts. Thus, instead of `beginTransaction()`,
you must set the [SPISettings](https://www.arduino.cc/reference/en/language/functions/communication/spi/spisettings/)
with [`updateSettings()`](#_CPPv4N10coralmicro7arduino11HardwareSPI14updateSettingsEN7arduino11SPISettingsE "coralmicro::arduino::HardwareSPI::updateSettings") and then
call [`begin()`](#_CPPv4N10coralmicro7arduino11HardwareSPI5beginEv "coralmicro::arduino::HardwareSPI::begin"). Also,
legacy functions `setBitOrder()` and `setClockDivider()` are not
supported because you should instead use
[`updateSettings()`](#_CPPv4N10coralmicro7arduino11HardwareSPI14updateSettingsEN7arduino11SPISettingsE "coralmicro::arduino::HardwareSPI::updateSettings").

To learn more about using SPI with Arduino, read the [Arduino & SPI
documentation](https://docs.arduino.cc/learn/communication/spi).

**Example** (from `sketches/SPI/`):

```
#include "Arduino.h"
#include "SPI.h"

static uint8_t count = 0;

void setup() {
  Serial.begin(115200);
  // Turn on Status LED to show the board is on.
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("Arduino SPI Example!");

  SPI.begin();
}

void loop() {
  Serial.println("Executing a SPI transaction.");
  if (SPI.transfer(count) == count) {
    Serial.println("Transaction success!");
  } else {
    Serial.println("Transaction failed!");
  }
  count++;
  count %= 255;

  delay(1000);
}
```

[[SPI.h source]](https://github.com/google-coral/coralmicro/blob/main/arduino/libraries/SPI/src/SPI.h)

*namespace* `coralmicro`
:   *namespace* `arduino`
    :   *class* `HardwareSPI` : *public* [arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[HardwareSPI](#_CPPv4N10coralmicro7arduino11HardwareSPIE "coralmicro::arduino::HardwareSPI")[Â¶](#_CPPv4N10coralmicro7arduino11HardwareSPIE "Permalink to this definition")
        :   *#include <SPI.h>*

            Allows for communication with SPI devices. SPI devices communicate through the use of the `transfer` functions, which simultaneously exchange data between the two devices, but they cannot be used from within interrupts.

            You should not initialize this object yourself; instead include `SPI.h` and then use the global `SPI` instance. Code samples can be found in `sketches/SPI/` and `sketches/SPITranscation/`.

            Public Functions

            uint8\_t `transfer`(uint8\_t *data*)[Â¶](#_CPPv4N10coralmicro7arduino11HardwareSPI8transferE7uint8_t "Permalink to this definition")
            :   Exchanges one byte of data with SPI transfer.

                Parameters
                :   **data** â The data for the peripheral device.

                Returns
                :   The data received from the peripheral device.

            uint16\_t `transfer16`(uint16\_t *data*)[Â¶](#_CPPv4N10coralmicro7arduino11HardwareSPI10transfer16E8uint16_t "Permalink to this definition")
            :   Exchanges two bytes of data with SPI transfer.

                Parameters
                :   **data** â The data for the peripheral device.

                Returns
                :   The data received from the peripheral device.

            void `transfer`(void \**buf*, size\_t *count*)[Â¶](#_CPPv4N10coralmicro7arduino11HardwareSPI8transferEPv6size_t "Permalink to this definition")
            :   Exchanges an array of data in-place with SPI transfer.

                Parameters
                :   * **buf** â The data for the peripheral device. As data is received, the buffer is overwritten.
                    * **count** â The length of the data in bytes.

            void `updateSettings`([arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::SPISettings *settings*)[Â¶](#_CPPv4N10coralmicro7arduino11HardwareSPI14updateSettingsEN7arduino11SPISettingsE "Permalink to this definition")
            :   Updates the SPI configuration.

                Parameters
                :   **settings** â The desired SPI configuration. See [SPISettings](https://www.arduino.cc/reference/en/language/functions/communication/spi/spisettings/).

            void `begin`()[Â¶](#_CPPv4N10coralmicro7arduino11HardwareSPI5beginEv "Permalink to this definition")
            :   Initializes SPI.

                This function must be called before doing any transfers.

            void `end`()[Â¶](#_CPPv4N10coralmicro7arduino11HardwareSPI3endEv "Permalink to this definition")
            :   De-initializes SPI.

Variables

[coralmicro](#_CPPv410coralmicro "coralmicro")::[arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[HardwareSPI](#_CPPv4N10coralmicro7arduino11HardwareSPIE "coralmicro::arduino::HardwareSPI") `SPI`[Â¶](#_CPPv43SPI "Permalink to this definition")
:   This is the global `HardwareSPI` instance you should use instead of creating your own instance.

Network
-------

### Sockets[Â¶](#sockets "Permalink to this headline")

These APIs define the basic interface for socket connections, whether using
Wi-Fi or Ethernet.

To open a socket as a client or server, you should use
the corresponding typedef aliases, corresponding to the type of network
connection youâre using:
[`WiFiClient`](#_CPPv4N10coralmicro7arduino10WiFiClientE "coralmicro::arduino::WiFiClient")/[`WiFiServer`](#_CPPv4N10coralmicro7arduino10WiFiServerE "coralmicro::arduino::WiFiServer")
or
[`EthernetClient`](#_CPPv4N10coralmicro7arduino14EthernetClientE "coralmicro::arduino::EthernetClient")/[`EthernetServer`](#_CPPv4N10coralmicro7arduino14EthernetServerE "coralmicro::arduino::EthernetServer").

*namespace* `coralmicro`
:   *namespace* `arduino`
    :   *class* `SocketClient` : *public* [arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::Client[Â¶](#_CPPv4N10coralmicro7arduino12SocketClientE "Permalink to this definition")
        :   *#include <SocketClient.h>*

            This is not meant to be instantiated directly. Instead use `arduino::WiFiClient` or `arduino::EthernetClient`.

            Public Functions

            *inline* *virtual* `~SocketClient`()[Â¶](#_CPPv4N10coralmicro7arduino12SocketClientD0Ev "Permalink to this definition")

            `SocketClient`() = default[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient12SocketClientEv "Permalink to this definition")
            :   Initializes a new client socket.

            `SocketClient`(*const* [SocketClient](#_CPPv4N10coralmicro7arduino12SocketClient12SocketClientERK12SocketClient "coralmicro::arduino::SocketClient::SocketClient") &*orig*) = delete[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient12SocketClientERK12SocketClient "Permalink to this definition")

            [SocketClient](#_CPPv4N10coralmicro7arduino12SocketClientE "coralmicro::arduino::SocketClient") &`operator=`(*const* [SocketClient](#_CPPv4N10coralmicro7arduino12SocketClientE "coralmicro::arduino::SocketClient") &*orig*) = delete[Â¶](#_CPPv4N10coralmicro7arduino12SocketClientaSERK12SocketClient "Permalink to this definition")

            *inline* `SocketClient`([SocketClient](#_CPPv4N10coralmicro7arduino12SocketClient12SocketClientERR12SocketClient "coralmicro::arduino::SocketClient::SocketClient") &&*orig*)[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient12SocketClientERR12SocketClient "Permalink to this definition")

            *inline* [SocketClient](#_CPPv4N10coralmicro7arduino12SocketClientE "coralmicro::arduino::SocketClient") &`operator=`([SocketClient](#_CPPv4N10coralmicro7arduino12SocketClientE "coralmicro::arduino::SocketClient") &&*orig*)[Â¶](#_CPPv4N10coralmicro7arduino12SocketClientaSERR12SocketClient "Permalink to this definition")

            int `connect`(IPAddress *ip*, uint16\_t *port*) *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient7connectE9IPAddress8uint16_t "Permalink to this definition")
            :   Start a socket connection with a server.

                Parameters
                :   * **ip** â The serverâs ip address.
                    * **port** â The serverâs port number.

                Returns
                :   1 if successful; -1 otherwise.

            int `connect`(*const* char \**host*, uint16\_t *port*) *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient7connectEPKc8uint16_t "Permalink to this definition")
            :   Starts a socket connection with a server.

                Parameters
                :   * **ip** â The serverâs hostname.
                    * **port** â The serverâs port number.

                Returns
                :   1 if successful; -1 otherwise.

            size\_t `write`(uint8\_t *c*) *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient5writeE7uint8_t "Permalink to this definition")
            :   Writes some data to the socket.

                Parameters
                :   **c** â The data to write.

                Returns
                :   1 if successful; -1 otherwise.

            size\_t `write`(*const* uint8\_t \**buf*, size\_t *size*) *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient5writeEPK7uint8_t6size_t "Permalink to this definition")
            :   Writes an array of data to the socket.

                Parameters
                :   * **array** â The array of data to write.
                    * **size** â The size of the array.

                Returns
                :   The size of the array written if successful; -1 otherwise.

            int `available`() *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient9availableEv "Permalink to this definition")
            :   Checks if the socket has bytes available to read.

                Returns
                :   The number of bytes available to read.

            int `read`() *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient4readEv "Permalink to this definition")
            :   Reads some data from the socket.

                Returns
                :   The data if successful; -1 otherwise.

            int `read`(uint8\_t \**buf*, size\_t *size*) *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient4readEP7uint8_t6size_t "Permalink to this definition")
            :   Reads an array of data from the socket.

                Parameters
                :   * **The** â buffer in which to put the data.
                    * **The** â size of the buffer.

                Returns
                :   The size of the array read if successful; -1 otherwise.

            void `stop`() *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient4stopEv "Permalink to this definition")
            :   Close the socket.

            uint8\_t `connected`() *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClient9connectedEv "Permalink to this definition")
            :   Checks if the socket is connected.

                Returns
                :   True if connected; false otherwise.

            `operator bool`() *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketClientcvbEv "Permalink to this definition")
            :   True if the socket is connected; false otherwise.

*namespace* `coralmicro`
:   *namespace* `arduino`
    :   *class* `SocketServer` : *public* [arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::Server[Â¶](#_CPPv4N10coralmicro7arduino12SocketServerE "Permalink to this definition")
        :   *#include <SocketServer.h>*

            This is not meant to be instantiated directly. Instead use `arduino::WiFiServer` or `arduino::EthernetServer`.

            Public Functions

            *inline* `SocketServer`(int *port*)[Â¶](#_CPPv4N10coralmicro7arduino12SocketServer12SocketServerEi "Permalink to this definition")
            :   Initializes a new server socket.

                Each server socket can accept just one client connection.

                Parameters
                :   **The** â port number to use.

            *virtual* `~SocketServer`()[Â¶](#_CPPv4N10coralmicro7arduino12SocketServerD0Ev "Permalink to this definition")

            void `begin`() *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketServer5beginEv "Permalink to this definition")
            :   Starts the socket.

            size\_t `write`(uint8\_t *c*) *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketServer5writeE7uint8_t "Permalink to this definition")
            :   Writes some data to the socket.

                Parameters
                :   **c** â The data to write.

                Returns
                :   1 if successful; -1 otherwise.

            size\_t `write`(*const* uint8\_t \**buf*, size\_t *size*) *override*[Â¶](#_CPPv4N10coralmicro7arduino12SocketServer5writeEPK7uint8_t6size_t "Permalink to this definition")
            :   Writes an array of data to the socket.

                Parameters
                :   * **array** â The array of data to write.
                    * **size** â The size of the array.

                Returns
                :   The size of the array written if successful; -1 otherwise.

            [SocketClient](#_CPPv4N10coralmicro7arduino12SocketClientE "coralmicro::arduino::SocketClient") `available`()[Â¶](#_CPPv4N10coralmicro7arduino12SocketServer9availableEv "Permalink to this definition")
            :   Waits for and accepts a socket connection from a client. This is a blocking call and returns only when a client has connected to this server socketâs port.

                Note: Only one client can connect at a time.

                Returns
                :   The client object to read and write with the connected client. Before using the returned client, check if the client is connected (check if the object evaluates true or call `arduino::SocketClient::connected()`) and check if it has data to read with `arduino::SocketClient::available()`.

### Wi-Fi[Â¶](#wi-fi "Permalink to this headline")

Note

Requires the Coral Wireless Add-on board.

To use Wi-Fi on the Dev Board Micro:

1. [Connect the Wireless Add-on board](/docs/dev-board-micro/wireless-addon/).
2. Use [`WiFiClass`](#_CPPv4N10coralmicro7arduino9WiFiClassE "coralmicro::arduino::WiFiClass") to connect to a Wi-Fi network.
3. Use [`WiFiClient`](#_CPPv4N10coralmicro7arduino10WiFiClientE "coralmicro::arduino::WiFiClient") to connect to a server,
   or use [`WiFiServer`](#_CPPv4N10coralmicro7arduino10WiFiServerE "coralmicro::arduino::WiFiServer") to host a server on the
   board.

[[WiFi.h source]](https://github.com/google-coral/coralmicro/blob/main/arduino/libraries/WiFi/src/WiFi.h)

*namespace* `coralmicro`
:   *namespace* `arduino`
    :   *class* `WiFiClass`[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClassE "Permalink to this definition")
        :   *#include <WiFi.h>*

            Allows for scanning for and connecting to Wi-Fi networks.

            Connection is initiated with the `WiFiClass::begin()` functions, which connect to a specified access point. Scanning can be executed via ``WiFiClass::scanNetworks()` and the results can be retrieved with the SSID/RSSI/encryptionType methods that accept `networkItem`.

            You should not initialize this object yourself, instead include `WiFi.h` and use the global `arduino::WiFi` instance.

            This API is designed to be code-compatible with projects that use the `WiFi` class from the [Arduino WiFi library](https://www.arduino.cc/reference/en/libraries/wifi/).

            **Example**: This code connects to a Wi-Fi network and prints network details.

            ```
            #include "Arduino.h"
            #include "WiFi.h"

            const char kSsid[] = "great-access-point";
            const char kPsk[] = "letmein";

            void printMacAddress(uint8_t* mac) {
              Serial.print(mac[0], HEX);
              Serial.print(":");
              Serial.print(mac[1], HEX);
              Serial.print(":");
              Serial.print(mac[2], HEX);
              Serial.print(":");
              Serial.print(mac[3], HEX);
              Serial.print(":");
              Serial.print(mac[4], HEX);
              Serial.print(":");
              Serial.print(mac[5], HEX);
            }

            void setup() {
              Serial.begin(115200);
              // Turn on Status LED to show the board is on.
              pinMode(PIN_LED_STATUS, OUTPUT);
              digitalWrite(PIN_LED_STATUS, HIGH);
              Serial.println("Arduino Wi-Fi Connect!");

              int connected = WL_DISCONNECTED;
              if (strlen(kPsk) > 0) {
                connected = WiFi.begin(kSsid, kPsk);
              } else {
                connected = WiFi.begin(kSsid);
              }
              if (connected == WL_CONNECTED) {
                Serial.print("Connected to network ");
                Serial.println(WiFi.SSID());
              } else {
                Serial.println("Failed to connect to network.");
                return;
              }

              uint8_t mac[6];
              WiFi.macAddress(mac);
              uint8_t bssid[6];
              WiFi.BSSID(bssid);
              Serial.print("Our MAC address: ");
              printMacAddress(mac);
              Serial.println();
              Serial.print("Network BSSID: ");
              printMacAddress(bssid);
              Serial.println();

              int32_t rssi = WiFi.RSSI();
              Serial.print("Network RSSI: ");
              Serial.println(rssi);

              IPAddress ip = WiFi.localIP();
              Serial.print("Ip address: ");
              Serial.println(ip);

              WiFi.disconnect();
            }

            void loop() {}
            ```

            Public Functions

            int `begin`(*const* char \**ssid*)[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass5beginEPKc "Permalink to this definition")
            :   Connects to an open Wi-Fi network.

                Parameters
                :   **ssid** â The SSID to connect to.

                Returns
                :   WL\_CONNECTED on success; WL\_CONNECT\_FAILED otherwise.

            int `begin`(*const* char \**ssid*, *const* char \**passphrase*)[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass5beginEPKcPKc "Permalink to this definition")
            :   Connects to a Wi-Fi network using a passphrase.

                Parameters
                :   * **ssid** â The SSID to connect to.
                    * **passphrase** â The passphrase used to connect to the network.

                Returns
                :   WL\_CONNECTED on success; WL\_CONNECT\_FAILED otherwise.

            int `disconnect`()[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass10disconnectEv "Permalink to this definition")
            :   Disconnects from the associated Wi-Fi network.

                Returns
                :   WL\_DISCONNECTED on success.

            char \*`SSID`()[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass4SSIDEv "Permalink to this definition")
            :   Retrieves the SSID of the associated Wi-Fi network.

                Returns
                :   SSID if the device is connected to a network, empty string otherwise.

            char \*`SSID`(uint8\_t *networkItem*)[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass4SSIDE7uint8_t "Permalink to this definition")
            :   Retrieves the SSID of the `networkItem`-th scan result.

                Returns
                :   SSID of the scan result.

            uint8\_t \*`BSSID`(uint8\_t \**bssid*)[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass5BSSIDEP7uint8_t "Permalink to this definition")
            :   Retrieves the BSSID of the access point.

                Parameters
                :   **bssid** â Pointer to memory where BSSID will be stored.

                Returns
                :   Address of BSSID on success; nullptr otherwise.

            int32\_t `RSSI`()[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass4RSSIEv "Permalink to this definition")
            :   Retrieves the RSSI of the associated Wi-Fi network.

                Returns
                :   RSSI if the device is connected to a network, INT\_MIN otherwise.

            int32\_t `RSSI`(uint8\_t *networkItem*)[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass4RSSIE7uint8_t "Permalink to this definition")
            :   Retrieves the RSSI of the `networkItem`-th scan result.

                Returns
                :   RSSI of the scan result.

            uint8\_t `encryptionType`()[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass14encryptionTypeEv "Permalink to this definition")
            :   Retrieves the encryption type of the associated Wi-Fi network.

                Returns
                :   Value from `enum encryption_type` representing the network security.

            uint8\_t `encryptionType`(uint8\_t *networkItem*)[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass14encryptionTypeE7uint8_t "Permalink to this definition")
            :   Retrieves the encryption type of the `networkItem`-th scan result.

                Returns
                :   `encryption_type` representing the network security.

            int8\_t `scanNetworks`()[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass12scanNetworksEv "Permalink to this definition")
            :   Scans for nearby Wi-Fi networks.

                Returns
                :   The number of networks discovered.

            uint8\_t `status`()[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass6statusEv "Permalink to this definition")
            :   Retrieves the status of the Wi-Fi connection.

                Returns
                :   `wl_status_type` representing the connection status.

            uint8\_t \*`macAddress`(uint8\_t \**mac*)[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass10macAddressEP7uint8_t "Permalink to this definition")
            :   Retrieves the MAC address of the Wi-Fi module.

                Parameters
                :   **bssid** â Pointer to memory where the MAC will be stored.

                Returns
                :   Address of MAC on success; nullptr otherwise.

            IPAddress `localIP`()[Â¶](#_CPPv4N10coralmicro7arduino9WiFiClass7localIPEv "Permalink to this definition")
            :   Retrieves the boardâs wifi ip address.

                Returns
                :   The wifi ip address of the board.

        Typedefs

        *using* `WiFiClient` = [SocketClient](#_CPPv4N10coralmicro7arduino12SocketClientE "coralmicro::arduino::SocketClient")[Â¶](#_CPPv4N10coralmicro7arduino10WiFiClientE "Permalink to this definition")
        :   Defines a client-side connection to a server using Wi-Fi.

            This is an alias for the `arduino::SocketClient` class, where all the available functions are defined.

            **Example**:

            This code connects to a Wi-Fi network, connects to a server via hostname and port, then sends an HTTP GET request and prints the response.

            ```
            #include "Arduino.h"
            #include "WiFi.h"

            namespace {
            const char kSsid[] = "great-access-point";
            const char kPsk[] = "letmein";
            coralmicro::arduino::WiFiClient client;
            }  // namespace

            void setup() {
              Serial.begin(115200);
              // Turn on Status LED to show the board is on.
              pinMode(PIN_LED_STATUS, OUTPUT);
              digitalWrite(PIN_LED_STATUS, HIGH);
              Serial.println("Arduino WiFiClient!");

              int connected = WL_DISCONNECTED;
              if (strlen(kPsk) > 0) {
                connected = WiFi.begin(kSsid, kPsk);
              } else {
                connected = WiFi.begin(kSsid);
              }
              if (connected == WL_CONNECTED) {
                Serial.print("Connected to network ");
                Serial.println(WiFi.SSID());
              } else {
                Serial.println("Failed to connect to network.");
                return;
              }

              if (!client.connect("www.example.com", 80)) {
                Serial.println("Connection failed.");
                return;
              }
              Serial.println("Connection successful!");

              const char* kHttpGet = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
              client.write(reinterpret_cast<const uint8_t*>(kHttpGet), strlen(kHttpGet));
            }

            void loop() {
              if (client && client.available()) {
                Serial.write(client.read());
              }
            }
            ```

        *using* `WiFiServer` = [SocketServer](#_CPPv4N10coralmicro7arduino12SocketServerE "coralmicro::arduino::SocketServer")[Â¶](#_CPPv4N10coralmicro7arduino10WiFiServerE "Permalink to this definition")
        :   Defines a server using Wi-Fi.

            This is an alias for the `arduino::SocketServer` class, where all the available functions are defined.

            **Example**:

            This code starts a server on the board and, when a client connects to it, it prints all data read from the client to the board serial console.

            ```
            #include "Arduino.h"
            #include "WiFi.h"

            namespace {
            const char kSsid[] = "great-access-point";
            const char kPsk[] = "letmein";
            coralmicro::arduino::WiFiClient client;
            coralmicro::arduino::WiFiServer server(31337);
            }  // namespace

            void setup() {
              Serial.begin(115200);
              // Turn on Status LED to show the board is on.
              pinMode(PIN_LED_STATUS, OUTPUT);
              digitalWrite(PIN_LED_STATUS, HIGH);
              Serial.println("Arduino WiFiServer!");

              int connected = WL_DISCONNECTED;
              if (strlen(kPsk) > 0) {
                connected = WiFi.begin(kSsid, kPsk);
              } else {
                connected = WiFi.begin(kSsid);
              }
              if (connected == WL_CONNECTED) {
                Serial.print("Connected to network ");
                Serial.println(WiFi.SSID());
              } else {
                Serial.println("Failed to connect to network.");
                return;
              }

              IPAddress ip = WiFi.localIP();

              server.begin();
              Serial.print("Our IP address is ");
              Serial.println(ip);
              Serial.println("Server ready on port 31337");
              // Blocks until a client is connected.
              client = server.available();
            }

            void loop() {
              // If a client is connected and has data available to read,
              // write the data to the serial console.
              if (client && client.available()) {
                Serial.write(client.read());
                Serial.flush();
              }
            }
            ```

Enums

*enum* `encryption_type`[Â¶](#_CPPv415encryption_type "Permalink to this definition")
:   Common encryption types for networks.

    *Values:*

    *enumerator* `ENC_TYPE_WEP`[Â¶](#_CPPv4N15encryption_type12ENC_TYPE_WEPE "Permalink to this definition")

    *enumerator* `ENC_TYPE_TKIP`[Â¶](#_CPPv4N15encryption_type13ENC_TYPE_TKIPE "Permalink to this definition")

    *enumerator* `ENC_TYPE_CCMP`[Â¶](#_CPPv4N15encryption_type13ENC_TYPE_CCMPE "Permalink to this definition")

    *enumerator* `ENC_TYPE_NONE`[Â¶](#_CPPv4N15encryption_type13ENC_TYPE_NONEE "Permalink to this definition")

    *enumerator* `ENC_TYPE_UNKNOWN`[Â¶](#_CPPv4N15encryption_type16ENC_TYPE_UNKNOWNE "Permalink to this definition")

    *enumerator* `ENC_TYPE_AUTO`[Â¶](#_CPPv4N15encryption_type13ENC_TYPE_AUTOE "Permalink to this definition")

*enum* `wl_status_t`[Â¶](#_CPPv411wl_status_t "Permalink to this definition")
:   Status codes for Wi-Fi.

    *Values:*

    *enumerator* `WL_NO_SHIELD`[Â¶](#_CPPv4N11wl_status_t12WL_NO_SHIELDE "Permalink to this definition")

    *enumerator* `WL_IDLE_STATUS`[Â¶](#_CPPv4N11wl_status_t14WL_IDLE_STATUSE "Permalink to this definition")

    *enumerator* `WL_NO_SSID_AVAIL`[Â¶](#_CPPv4N11wl_status_t16WL_NO_SSID_AVAILE "Permalink to this definition")

    *enumerator* `WL_SCAN_COMPLETED`[Â¶](#_CPPv4N11wl_status_t17WL_SCAN_COMPLETEDE "Permalink to this definition")

    *enumerator* `WL_CONNECTED`[Â¶](#_CPPv4N11wl_status_t12WL_CONNECTEDE "Permalink to this definition")

    *enumerator* `WL_CONNECT_FAILED`[Â¶](#_CPPv4N11wl_status_t17WL_CONNECT_FAILEDE "Permalink to this definition")

    *enumerator* `WL_CONNECTION_LOST`[Â¶](#_CPPv4N11wl_status_t18WL_CONNECTION_LOSTE "Permalink to this definition")

    *enumerator* `WL_DISCONNECTED`[Â¶](#_CPPv4N11wl_status_t15WL_DISCONNECTEDE "Permalink to this definition")

Variables

[coralmicro](#_CPPv410coralmicro "coralmicro")::[arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[WiFiClass](#_CPPv4N10coralmicro7arduino9WiFiClassE "coralmicro::arduino::WiFiClass") `WiFi`[Â¶](#_CPPv44WiFi "Permalink to this definition")
:   This is the global `WiFiClass` instance you should use instead of creating your own instance.

### Ethernet[Â¶](#ethernet "Permalink to this headline")

Note

Requires the Coral PoE Add-on board.

To use Ethernet on the Dev Board Micro:

1. [Connect the PoE Add-on board](/docs/dev-board-micro/poe-addon/).
2. Call [`coralmicro::arduino::EthernetClass::begin()`](#_CPPv4N10coralmicro7arduino13EthernetClass5beginEv "coralmicro::arduino::EthernetClass::begin") to enable the
   Ethernet connection.
3. Use [`EthernetClient`](#_CPPv4N10coralmicro7arduino14EthernetClientE "coralmicro::arduino::EthernetClient") to connect to a server,
   or use [`EthernetServer`](#_CPPv4N10coralmicro7arduino14EthernetServerE "coralmicro::arduino::EthernetServer") to host a server on
   the board.

[[Ethernet.h source]](https://github.com/google-coral/coralmicro/blob/main/arduino/libraries/Ethernet/src/Ethernet.h)

*namespace* `coralmicro`
:   *namespace* `arduino`
    :   *class* `EthernetClass`[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClassE "Permalink to this definition")
        :   *#include <Ethernet.h>*

            Allows for connection of the device to a network via Ethernet. At the moment, this interface only supports networks with DHCP. After a connection is established, information about the connection such as IP address, DNS, and gateway can be retrieved. You should not initialize this object yourself, instead include `Ethernet.h` and use the global `Ethernet` instance.

            Public Functions

            int `begin`()[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass5beginEv "Permalink to this definition")
            :   Variant of begin that only uses DHCP.

                Returns
                :   1 if DHCP was successful; 0 otherwise.

            int `begin`(IPAddress *ip*)[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass5beginE9IPAddress "Permalink to this definition")
            :   Initializes an Ethernet connection, using the specified IP address. The DNS server will be the IP address with the final octet as 1. The gateway will be the IP address with the final octet as 1. The subnet mask will be 255.255.255.0.

                Parameters
                :   **ip** â The desired IP address.

                Returns
                :   1 if Ethernet is brought up successfully.

            int `begin`(IPAddress *ip*, IPAddress *dns\_server*)[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass5beginE9IPAddress9IPAddress "Permalink to this definition")
            :   Initializes an Ethernet connection, using the specified IP address and DNS server. The gateway will be the IP address with the final octet as 1. The subnet mask will be 255.255.255.0.

                Parameters
                :   * **ip** â The desired IP address.
                    * **dns\_server** â The IP address of a DNS server.

                Returns
                :   1 if Ethernet is brought up successfully.

            int `begin`(IPAddress *ip*, IPAddress *dns\_server*, IPAddress *gateway*)[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass5beginE9IPAddress9IPAddress9IPAddress "Permalink to this definition")
            :   Initializes an Ethernet connection, using the specified IP address, DNS server, and gateway. The subnet mask will be 255.255.255.0.

                Parameters
                :   * **ip** â The desired IP address.
                    * **dns\_server** â The IP address of a DNS server.
                    * **gateway** â The IP address of the network gateway.

                Returns
                :   1 if Ethernet is brought up successfully.

            int `begin`(IPAddress *ip*, IPAddress *dns\_server*, IPAddress *gateway*, IPAddress *subnet\_mask*)[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass5beginE9IPAddress9IPAddress9IPAddress9IPAddress "Permalink to this definition")
            :   Initializes an Ethernet connection, using the specified IP address, DNS server, gateway, and subnet mask.

                Parameters
                :   * **ip** â The desired IP address.
                    * **dns\_server** â The IP address of a DNS server.
                    * **gateway** â The IP address of the network gateway.
                    * **subnet\_mask** â The subnet mask for the network.

                Returns
                :   1 if Ethernet is brought up successfully.

            IPAddress `localIP`()[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass7localIPEv "Permalink to this definition")
            :   Returns the IP address of the device.

                Returns
                :   The Ethernet IP address of the device.

            IPAddress `subnetMask`()[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass10subnetMaskEv "Permalink to this definition")
            :   Returns the subnet mask of Ethernet connection.

                Returns
                :   The subnet mask of the connection.

            IPAddress `gatewayIP`()[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass9gatewayIPEv "Permalink to this definition")
            :   Returns the gateway of the Ethernet connection.

                Returns
                :   The IP of the network gateway.

            IPAddress `dnsServerIP`()[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass11dnsServerIPEv "Permalink to this definition")
            :   Returns the primary DNS server of the device.

                Returns
                :   The IP of the primary DNS server.

            void `MACAddress`(uint8\_t \**mac*)[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass10MACAddressEP7uint8_t "Permalink to this definition")
            :   Retrieves the MAC address of the Ethernet interface.

                Parameters
                :   **mac** â Pointer to memory where the MAC address will be stored.

            [EthernetLinkStatus](#_CPPv418EthernetLinkStatus "EthernetLinkStatus") `linkStatus`()[Â¶](#_CPPv4N10coralmicro7arduino13EthernetClass10linkStatusEv "Permalink to this definition")
            :   Returns the status of the Ethernet connection.

                Returns
                :   `EthernetLinkStatus` representing the state of the connection.

        Typedefs

        *using* `EthernetClient` = [SocketClient](#_CPPv4N10coralmicro7arduino12SocketClientE "coralmicro::arduino::SocketClient")[Â¶](#_CPPv4N10coralmicro7arduino14EthernetClientE "Permalink to this definition")
        :   Defines a client-side connection to a server using Ethernet.

            This is an alias for the `arduino::SocketClient` class, where all the available functions are defined.

            **Example**:

            This code enables the Ethernet connection, connects to a server via hostname and port, then sends an HTTP GET request and prints the response.

            ```
            #include "Arduino.h"
            #include "Ethernet.h"

            namespace {
            coralmicro::arduino::EthernetClient client;
            }

            void setup() {
              Serial.begin(115200);
              pinMode(PIN_LED_STATUS, OUTPUT);
              digitalWrite(PIN_LED_STATUS, HIGH);
              Serial.println("Arduino EthernetClient!");

              if (!Ethernet.begin()) {
                Serial.println("DHCP failed to get an IP.");
                return;
              }

              if (!client.connect("www.example.com", 80)) {
                Serial.println("Connection failed.");
                return;
              }
              Serial.println("Connection successful!");

              const char* kHttpGet = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
              client.write(reinterpret_cast<const uint8_t*>(kHttpGet), strlen(kHttpGet));
            }

            void loop() {
              if (client && client.available()) {
                Serial.write(client.read());
              }
            }
            ```

        *using* `EthernetServer` = [SocketServer](#_CPPv4N10coralmicro7arduino12SocketServerE "coralmicro::arduino::SocketServer")[Â¶](#_CPPv4N10coralmicro7arduino14EthernetServerE "Permalink to this definition")
        :   Defines a server using Ethernet.

            This is an alias for the `arduino::SocketServer` class, where all the available functions are defined.

            **Example**:

            This code starts a server on the board and, when a client connects to it, it prints all data read from the client to the board serial console.

            ```
            #include "Arduino.h"
            #include "Ethernet.h"

            namespace {
            coralmicro::arduino::EthernetClient client;
            coralmicro::arduino::EthernetServer server(31337);
            }  // namespace

            void setup() {
              Serial.begin(115200);
              pinMode(PIN_LED_STATUS, OUTPUT);
              digitalWrite(PIN_LED_STATUS, HIGH);
              Serial.println("Arduino EthernetServer!");

              if (!Ethernet.begin()) {
                Serial.println("DHCP failed to get an IP.");
                return;
              }

              if (!client.connect("www.example.com", 80)) {
                Serial.println("Connection failed.");
                return;
              }
              Serial.println("Connection successful!");
              IPAddress ip = Ethernet.localIP();

              server.begin();
              Serial.print("Our IP address is ");
              Serial.println(ip);
              Serial.println("Server ready on port 31337");
              // Blocks until a client is connected.
              client = server.available();
            }

            void loop() {
              // If a client is connected and has data available to read,
              // write the data to the serial console.
              if (client && client.available()) {
                Serial.write(client.read());
                Serial.flush();
              }
            }
            ```

Enums

*enum* `EthernetLinkStatus`[Â¶](#_CPPv418EthernetLinkStatus "Permalink to this definition")
:   Status of the Ethernet link.

    *Values:*

    *enumerator* `Unknown`[Â¶](#_CPPv4N18EthernetLinkStatus7UnknownE "Permalink to this definition")

    *enumerator* `LinkON`[Â¶](#_CPPv4N18EthernetLinkStatus6LinkONE "Permalink to this definition")

    *enumerator* `LinkOFF`[Â¶](#_CPPv4N18EthernetLinkStatus7LinkOFFE "Permalink to this definition")

Variables

[coralmicro](#_CPPv410coralmicro "coralmicro")::[arduino](#_CPPv4N10coralmicro7arduinoE "coralmicro::arduino")::[EthernetClass](#_CPPv4N10coralmicro7arduino13EthernetClassE "coralmicro::arduino::EthernetClass") `Ethernet`[Â¶](#_CPPv48Ethernet "Permalink to this definition")
:   This is the global `EthernetClass` instance you should use instead of creating your own instance.

TensorFlow Lite
---------------

To run inference with TensorFlow Lite models in Arduino, youâll use the same
[TensorFlow Lite Micro APIs](../tensorflow/) that are used with FreeRTOS apps.
Of course, the rest of your code can continue using the Arduino language and
other Arduino-style APIs such as [Camera](#camera).

**Example**:

This code performs object detection with TensorFlow Lite using images from
the camera.

```
#include <coralmicro_SD.h>
#include <coralmicro_camera.h>

#include <cstdint>
#include <memory>

#include "Arduino.h"
#include "coral_micro.h"
#include "libs/rpc/rpc_http_server.h"
#include "libs/tensorflow/detection.h"

using namespace coralmicro;
using namespace coralmicro::arduino;

namespace {
bool setup_success{false};
int button_pin = PIN_BTN;
int last_button_state = LOW;
int current_button_state = HIGH;
unsigned long last_debounce_time = 0;
constexpr unsigned long kDebounceDelay = 50;

tflite::MicroMutableOpResolver<3> resolver;
const tflite::Model* model = nullptr;
std::vector<uint8_t> model_data;
std::shared_ptr<coralmicro::EdgeTpuContext> context = nullptr;
std::unique_ptr<tflite::MicroInterpreter> interpreter = nullptr;
TfLiteTensor* input_tensor = nullptr;
int model_height;
int model_width;

constexpr char kModelPath[] =
    "/models/tf2_ssd_mobilenet_v2_coco17_ptq_edgetpu.tflite";
std::vector<tensorflow::Object> results;

constexpr int kTensorArenaSize = 8 * 1024 * 1024;
STATIC_TENSOR_ARENA_IN_SDRAM(tensor_arena, kTensorArenaSize);

FrameBuffer frame_buffer;

bool DetectFromCamera() {
  if (Camera.grab(frame_buffer) != CameraStatus::SUCCESS) {
    return false;
  }
  std::memcpy(tflite::GetTensorData<uint8_t>(input_tensor),
              frame_buffer.getBuffer(), frame_buffer.getBufferSize());
  if (interpreter->Invoke() != kTfLiteOk) {
    return false;
  }
  results = tensorflow::GetDetectionResults(interpreter.get(), 0.6f, 3);
  return true;
}

void DetectRpc(struct jsonrpc_request* r) {
  if (!setup_success) {
    jsonrpc_return_error(
        r, -1, "Inference failed because setup was not successful", nullptr);
    return;
  }
  if (!DetectFromCamera()) {
    jsonrpc_return_error(r, -1, "Failed to run classification from camera.",
                         nullptr);
    return;
  }
  if (!results.empty()) {
    const auto& result = results[0];
    jsonrpc_return_success(
        r,
        "{%Q: %d, %Q: %d, %Q: %V, %Q: {%Q: %d, %Q: %g, %Q: %g, %Q: %g, "
        "%Q: %g, %Q: %g}}",
        "width", model_width, "height", model_height, "base64_data",
        frame_buffer.getBufferSize(), frame_buffer.getBuffer(), "detection",
        "id", result.id, "score", result.score, "xmin", result.bbox.xmin,
        "xmax", result.bbox.xmax, "ymin", result.bbox.ymin, "ymax",
        result.bbox.ymax);
    return;
  }
  jsonrpc_return_success(r, "{%Q: %d, %Q: %d, %Q: %V, %Q: None}", "width",
                         model_width, "height", model_height, "base64_data",
                         frame_buffer.getBufferSize(), frame_buffer.getBuffer(),
                         "detection");
}
}  // namespace

void setup() {
  Serial.begin(115200);
  // Turn on Status LED to show the board is on.
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("Arduino Camera Detection!");

  pinMode(button_pin, INPUT);

  SD.begin();
  Serial.println("Loading Model");

  if (!SD.exists(kModelPath)) {
    Serial.println("Model file not found");
    return;
  }

  SDFile model_file = SD.open(kModelPath);
  uint32_t model_size = model_file.size();
  model_data.resize(model_size);
  if (model_file.read(model_data.data(), model_size) != model_size) {
    Serial.print("Error loading model");
    return;
  }

  model = tflite::GetModel(model_data.data());
  context = coralmicro::EdgeTpuManager::GetSingleton()->OpenDevice();
  if (!context) {
    Serial.println("Failed to get EdgeTpuContext");
    return;
  }
  Serial.println("model and context created");

  tflite::MicroErrorReporter error_reporter;
  resolver.AddDequantize();
  resolver.AddDetectionPostprocess();
  resolver.AddCustom(coralmicro::kCustomOp, coralmicro::RegisterCustomOp());

  interpreter = std::make_unique<tflite::MicroInterpreter>(
      model, resolver, tensor_arena, kTensorArenaSize, &error_reporter);

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("allocate tensors failed");
    return;
  }

  if (!interpreter) {
    Serial.println("Failed to make interpreter");
    return;
  }
  if (interpreter->inputs().size() != 1) {
    Serial.println("Bad inputs size");
    Serial.println(interpreter->inputs().size());
    return;
  }

  input_tensor = interpreter->input_tensor(0);
  model_height = input_tensor->dims->data[1];
  model_width = input_tensor->dims->data[2];

  if (Camera.begin(model_width, model_height, coralmicro::CameraFormat::kRgb,
                   coralmicro::CameraFilterMethod::kBilinear,
                   coralmicro::CameraRotation::k270,
                   true) != CameraStatus::SUCCESS) {
    Serial.println("Failed to start camera");
    return;
  }

  Serial.println("Initializing detection server...");
  jsonrpc_init(nullptr, nullptr);
  jsonrpc_export("detect_from_camera", DetectRpc);
  UseHttpServer(new JsonRpcHttpServer);
  Serial.println("Detection server ready!");

  setup_success = true;
  Serial.println("Initialized");
}
void loop() {
  int reading = digitalRead(button_pin);
  if (reading != last_button_state) {
    last_debounce_time = millis();
  }
  if ((millis() - last_debounce_time) > kDebounceDelay) {
    if (reading != current_button_state) {
      current_button_state = reading;
      if (current_button_state == HIGH) {
        Serial.println("Button triggered, running detection...");
        if (!setup_success) {
          Serial.println("Cannot run because of a problem during setup!");
          return;
        }

        if (!DetectFromCamera()) {
          Serial.println("Failed to run detection");
          return;
        }

        Serial.print("Results count: ");
        Serial.println(results.size());
        for (auto result : results) {
          Serial.print("id: ");
          Serial.print(result.id);
          Serial.print(" score: ");
          Serial.print(result.score);
          Serial.print(" xmin: ");
          Serial.print(result.bbox.xmin);
          Serial.print(" ymin: ");
          Serial.print(result.bbox.ymin);
          Serial.print(" xmax: ");
          Serial.print(result.bbox.xmax);
          Serial.print(" ymax: ");
          Serial.println(result.bbox.ymax);
        }
      }
    }
  }
  last_button_state = reading;
}
```