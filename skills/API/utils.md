Utility APIs
============

Miscellaneous helper APIs.

JPEGs
-----

APIs to create JPEG files from RGB images.

For example, this code shows how to create a JPEG with an image captured from
the camera (from `examples/camera_streaming_http/`):

```
    std::vector<uint8_t> buf(CameraTask::kWidth * CameraTask::kHeight *
                             CameraFormatBpp(CameraFormat::kRgb));
    auto fmt = CameraFrameFormat{
        CameraFormat::kRgb,       CameraFilterMethod::kBilinear,
        CameraRotation::k0,       CameraTask::kWidth,
        CameraTask::kHeight,
        /*preserve_ratio=*/false, buf.data(),
        /*while_balance=*/true};
    if (!CameraTask::GetSingleton()->GetFrame({fmt})) {
      printf("Unable to get frame from camera\r\n");
      return {};
    }

    std::vector<uint8_t> jpeg;
    JpegCompressRgb(buf.data(), fmt.width, fmt.height, /*quality=*/75, &jpeg);
```

[[jpeg.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/libjpeg/jpeg.h)

*namespace* `coralmicro`
:   Functions

    unsigned long `JpegCompressRgb`(unsigned char \**rgb*, int *width*, int *height*, int *quality*, unsigned char \**buf*, unsigned long *size*)[Â¶](#_CPPv4N10coralmicro15JpegCompressRgbEPhiiiPhm "Permalink to this definition")
    :   Converts an RGB image to JPEG format.

        Parameters
        :   * **rgb** â The image in RGB format.
            * **width** â The imageâs width.
            * **height** â The imageâs height.
            * **quality** â The quality of the image after compression (must be within [0-100]).
            * **buf** â The buffer to return the JPEG image data to.
            * **size** â The size allocated for buf.

        Returns
        :   The size of the resulting JPEG buffer.

    [JpegBuffer](#_CPPv4N10coralmicro10JpegBufferE "coralmicro::JpegBuffer") `JpegCompressRgb`(unsigned char \**rgb*, int *width*, int *height*, int *quality*)[Â¶](#_CPPv4N10coralmicro15JpegCompressRgbEPhiii "Permalink to this definition")
    :   Converts an RGB image to JPEG format.

        Parameters
        :   * **rgb** â The image in RGB format.
            * **width** â The imageâs width.
            * **height** â The imageâs height.
            * **quality** â The quality of the image after compression (must be within [0-100]).

        Returns
        :   A JPEG image buffer. Note: You have to deallocate data from [JpegBuffer](#structcoralmicro_1_1JpegBuffer) with `free()`.

    void `JpegCompressRgb`(unsigned char \**rgb*, int *width*, int *height*, int *quality*, std::vector<uint8\_t> \**out*)[Â¶](#_CPPv4N10coralmicro15JpegCompressRgbEPhiiiPNSt6vectorI7uint8_tEE "Permalink to this definition")
    :   Converts an RGB image to JPEG format.

        Parameters
        :   * **rgb** â The image in RGB format.
            * **width** â The imageâs width.
            * **height** â The imageâs height.
            * **quality** â The quality of the image after compression (must be within [0-100]).
            * **out** â the output vector to return the resulting JPEG image to.

    *struct* `JpegBuffer`[Â¶](#_CPPv4N10coralmicro10JpegBufferE "Permalink to this definition")
    :   *#include <jpeg.h>*

        Represents a JPEG image buffer.

        Public Members

        unsigned char \*`data`[Â¶](#_CPPv4N10coralmicro10JpegBuffer4dataE "Permalink to this definition")
        :   The Jpeg image data.

        unsigned long `size`[Â¶](#_CPPv4N10coralmicro10JpegBuffer4sizeE "Permalink to this definition")
        :   The size of the image.

Strings
-------

[[strings.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/strings.h)

*namespace* `coralmicro`
:   Functions

    template<size\_t `N`> *constexpr* size\_t `StrLen`(*const* char (&*str*)[N])[Â¶](#_CPPv4I_6size_tEN10coralmicro6StrLenE6size_tRA1N_Kc "Permalink to this definition")
    :   Gets the length of a const char.

        This function is preferred over `strlen()` for constant strings that can be evaluated at compile time.

        Parameters
        :   **str** â The string to get length.

        Template Parameters
        :   **N** â The string array length determined by the compiler through function template argument deduction.

        Returns
        :   The length of the const char string.

    template<size\_t `N`> bool `StrStartsWith`(*const* char \**s*, *const* char (&*prefix*)[N])[Â¶](#_CPPv4I_6size_tEN10coralmicro13StrStartsWithEbPKcRA1N_Kc "Permalink to this definition")
    :   Checks if a string starts with a prefix.

        Parameters
        :   * **s** â The full string to check for prefix.
            * **prefix** â The prefix to look for at the beginning of the string s.

        Template Parameters
        :   **N** â The length of the string.

        Returns
        :   True if prefix is the prefix of s, else false.

    template<size\_t `N`> bool `StrEndsWith`(*const* std::string &*s*, *const* char (&*suffix*)[N])[Â¶](#_CPPv4I_6size_tEN10coralmicro11StrEndsWithEbRKNSt6stringERA1N_Kc "Permalink to this definition")
    :   Checks if a string ends with a suffix.

        Parameters
        :   * **s** â The full string to check for suffix.
            * **suffix** â The suffix to look for at the end of the string s.

        Template Parameters
        :   **N** â The length of the string.

        Returns
        :   True if suffix is the suffix of s, else false.

    template<typename `C`, typename ...`T`> void `StrAppend`([C](#_CPPv4I0DpEN10coralmicro9StrAppendEvP1CPKcDp1T "coralmicro::StrAppend::C") \**v*, *const* char \**format\_str*, [T](#_CPPv4I0DpEN10coralmicro9StrAppendEvP1CPKcDp1T "coralmicro::StrAppend::T")... *args*)[Â¶](#_CPPv4I0DpEN10coralmicro9StrAppendEvP1CPKcDp1T "Permalink to this definition")
    :   Appends a string to a source string.

        Parameters
        :   * **v** â The source string to append format\_str string to.
            * **format\_str** â The string with format specifier to append to v.
            * **args** â The arguments to the format specifier.

        Template Parameters
        :   **T** â The variadic class template that allows args to be any type.

    std::string `StrToHex`(*const* char \**s*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro8StrToHexEPKc6size_t "Permalink to this definition")
    :   Returns a stringâs hexadecimal representation.

        Parameters
        :   * **s** â The source array of raw characters.
            * **size** â The size of the source array.

        Returns
        :   The hexadecimal representation of the string.

    *inline* std::string `StrToHex`(*const* std::string &*s*)[Â¶](#_CPPv4N10coralmicro8StrToHexERKNSt6stringE "Permalink to this definition")
    :   Returns a stringâs hexadecimal representation.

        Parameters
        :   **s** â The source string.

        Returns
        :   The hexadecimal representation of the string.

Timers
------

[[timer.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/timer.h)

*namespace* `coralmicro`
:   Functions

    void `TimerInit`()[Â¶](#_CPPv4N10coralmicro9TimerInitEv "Permalink to this definition")
    :   Initializes the timer.

        Programs on the M7 do not need to call this because it is automatically called internally. M7 programs can immediately call functions such as `TimerMicros()`.

        Programs on the M4 must call this to intialize the timer before they can use timers. For example:

        ```
        TimerInit();
        auto current_time = TimerMillis();
        ```

    uint64\_t `TimerMicros`()[Â¶](#_CPPv4N10coralmicro11TimerMicrosEv "Permalink to this definition")
    :   Microseconds since boot.

    *inline* uint64\_t `TimerMillis`()[Â¶](#_CPPv4N10coralmicro11TimerMillisEv "Permalink to this definition")
    :   Milliseconds since boot.

    void `TimerSetRtcTime`(uint32\_t *sec*)[Â¶](#_CPPv4N10coralmicro15TimerSetRtcTimeE8uint32_t "Permalink to this definition")

    void `TimerGetRtcTime`(*struct* tm \**time*)[Â¶](#_CPPv4N10coralmicro15TimerGetRtcTimeEP2tm "Permalink to this definition")

Random numbers
--------------

[[random.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/random.h)

*namespace* `coralmicro`
:   Functions

    void `RandomInit`()[Â¶](#_CPPv4N10coralmicro10RandomInitEv "Permalink to this definition")
    :   Initializes hardware random number generator.

        Programs on the M7 do not need to call this because it is automatically called internally. M7 programs can immediately call `RandomGenerate()`.

        Programs on the M4 must call this to intialize the generator before calling `RandomGenerate()`.

    bool `RandomGenerate`(void \**buf*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro14RandomGenerateEPv6size_t "Permalink to this definition")
    :   Generates random byte sequence.

        Parameters
        :   * **buf** â Buffer to write random bytes to.
            * **size** â Size of the buffer.

        Returns
        :   True upon success, false otherwise.

Temperature sensors
-------------------

[[tempsense.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/tempsense.h)

*namespace* `coralmicro`
:   Enums

    *enum class* `TempSensor`[Â¶](#_CPPv4N10coralmicro10TempSensorE "Permalink to this definition")
    :   Enumerates the various temperature sensors.

        *Values:*

        *enumerator* `kCpu`[Â¶](#_CPPv4N10coralmicro10TempSensor4kCpuE "Permalink to this definition")
        :   The CPU temperature sensor.

        *enumerator* `kTpu`[Â¶](#_CPPv4N10coralmicro10TempSensor4kTpuE "Permalink to this definition")
        :   The Edge TPU temperature sensor.

    Functions

    void `TempSensorInit`()[Â¶](#_CPPv4N10coralmicro14TempSensorInitEv "Permalink to this definition")
    :   Initializes the temperature sensors.

        Programs on the M7 do not need to call this because it is automatically called internally. M7 programs can immediately call `TempSensorRead()`.

        Programs on the M4 must call this to intialize the sensors before calling `TempSensorRead()`.

    float `TempSensorRead`([TempSensor](#_CPPv4N10coralmicro10TempSensorE "coralmicro::TempSensor") *sensor*)[Â¶](#_CPPv4N10coralmicro14TempSensorReadE10TempSensor "Permalink to this definition")
    :   Reads the temperature of a sensor.

        Parameters
        :   **sensor** â The sensor to get the temperature.

        Returns
        :   The actual temperature in Celcius or 0.0 on failure.