Camera APIs
===========

The Dev Board Micro includes an on-board camera module with
324 x 324 px resolution. All camera control is handled through the
[`CameraTask`](#_CPPv4N10coralmicro10CameraTaskE "coralmicro::CameraTask") singleton. You can capture images
one-at-a-time or receive a stream of images. The camera also offers
hardware-based motion detection.

To get started, you must acquire the `CameraTask` object with
[`GetSingleton()`](#_CPPv4N10coralmicro10CameraTask12GetSingletonEv "coralmicro::CameraTask::GetSingleton").
Then power on the camera with [`SetPower()`](#_CPPv4N10coralmicro10CameraTask8SetPowerEb "coralmicro::CameraTask::SetPower")
and specify the camera mode (trigger or streaming mode) with
[`Enable()`](#_CPPv4N10coralmicro10CameraTask6EnableE10CameraMode "coralmicro::CameraTask::Enable").

If you enable trigger mode, the camera captures a single image and saves it to
the cameraâs memory when you call [`Trigger()`](#_CPPv4N10coralmicro10CameraTask7TriggerEv "coralmicro::CameraTask::Trigger").

If you enable streaming mode, the camera continuously captures new images and
saves them to an internal buffer.

In either mode, you must manually fetch the latest image by calling
[`GetFrame()`](#_CPPv4N10coralmicro10CameraTask8GetFrameERKNSt6vectorI17CameraFrameFormatEE "coralmicro::CameraTask::GetFrame"),
which requires you to specify the image format you want with
[`CameraFrameFormat`](#_CPPv4N10coralmicro17CameraFrameFormatE "coralmicro::CameraFrameFormat"). This object also specifies
your own buffer where you want to save the processed image. You can specify
multiple formats for each frame, which is useful if you want one format to use
as input for your ML model and another format to use for display.

Note

In streaming mode, your perceived framerate is only as fast as your main
loop, because [`GetFrame()`](#_CPPv4N10coralmicro10CameraTask8GetFrameERKNSt6vectorI17CameraFrameFormatEE "coralmicro::CameraTask::GetFrame") returns only
the most recent frame captured. And if your loop is faster
than the camera, it will be blocked by
[`GetFrame()`](#_CPPv4N10coralmicro10CameraTask8GetFrameERKNSt6vectorI17CameraFrameFormatEE "coralmicro::CameraTask::GetFrame") until a new frame is available.

**Photo capture example:**

The following code enables the camera with trigger mode so it
will capture an image when you press the User button on the Dev Board Micro.
The image is saved internally in raw format until the app fetches it with
[`GetFrame()`](#_CPPv4N10coralmicro10CameraTask8GetFrameERKNSt6vectorI17CameraFrameFormatEE "coralmicro::CameraTask::GetFrame"), which is called by the
`GetCapturedImage()` RPC function.

This example is available in `coralmicro/examples/camera_triggered/`, which
also provides a Python client to fetch images over RPC.

```
namespace coralmicro {
namespace {
void GetCapturedImage(struct jsonrpc_request* request) {
  int width;
  int height;

  // The width and height are specified by the RPC client.
  if (!JsonRpcGetIntegerParam(request, "width", &width)) return;
  if (!JsonRpcGetIntegerParam(request, "height", &height)) return;

  auto format = CameraFormat::kRgb;
  std::vector<uint8_t> image(width * height * CameraFormatBpp(format));
  CameraFrameFormat fmt{format,
                        CameraFilterMethod::kBilinear,
                        CameraRotation::k270,
                        width,
                        height,
                        /*preserve_ratio=*/false,
                        /*buffer=*/image.data(),
                        /*white_balance=*/true};
  if (!CameraTask::GetSingleton()->GetFrame({fmt})) {
    jsonrpc_return_error(request, -1, "Failed to get image from camera.",
                         nullptr);
    return;
  }

  jsonrpc_return_success(request, "{%Q: %d, %Q: %d, %Q: %V}", "width", width,
                         "height", height, "base64_data", image.size(),
                         image.data());
}

[[noreturn]] void Main() {
  printf("Camera Triggered Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  // Starting Camera in triggered mode.
  CameraTask::GetSingleton()->SetPower(true);
  CameraTask::GetSingleton()->Enable(CameraMode::kTrigger);

  // Set up an RPC server that serves the latest image.
  jsonrpc_export("get_captured_image", GetCapturedImage);
  UseHttpServer(new JsonRpcHttpServer);

  // Register callback for the user button.
  printf("Press the user button to take a picture.\r\n");
  GpioConfigureInterrupt(
      Gpio::kUserButton, GpioInterruptMode::kIntModeFalling,
      [handle = xTaskGetCurrentTaskHandle()]() { xTaskResumeFromISR(handle); },
      /*debounce_interval_us=*/50 * 1e3);
  while (true) {
    vTaskSuspend(nullptr);
    CameraTask::GetSingleton()->Trigger();
    printf("Picture taken\r\n");
  }
}
}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
}
```

**Motion detection example:**

This example turns on the User LED whenever motion is detected, passing the
callback function a timer that turns the LED back off after a 200ms delay.

```
namespace coralmicro {
namespace {
void Main() {
  printf("Camera Motion Detection example\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  TimerHandle_t motion_detection_timer = xTimerCreate(
      "motion_detection_timer", pdMS_TO_TICKS(200), pdFALSE, nullptr,
      [](TimerHandle_t timer) { LedSet(Led::kUser, false); });
  CHECK(motion_detection_timer);

  // Enable Power, configure motion detection, and enable streaming.
  CameraTask::GetSingleton()->SetPower(true);

  CameraMotionDetectionConfig config{};
  CameraTask::GetSingleton()->GetMotionDetectionConfigDefault(config);
  config.cb = [](void* param) {
    auto timer = reinterpret_cast<TimerHandle_t>(param);
    CHECK(timer);
    printf("Motion detected\r\n");
    LedSet(Led::kUser, true);
    if (xTimerIsTimerActive(timer) == pdTRUE) {
      xTimerReset(timer, portMAX_DELAY);
    } else {
      xTimerStart(timer, portMAX_DELAY);
    }
  };
  config.cb_param = motion_detection_timer;

  CameraTask::GetSingleton()->SetMotionDetectionConfig(config);
  CameraTask::GetSingleton()->Enable(CameraMode::kStreaming);

  vTaskSuspend(nullptr);
}
}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[camera.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/camera/camera.h)

*namespace* `coralmicro`
:   *struct* `CameraFrameFormat`[Â¶](#_CPPv4N10coralmicro17CameraFrameFormatE "Permalink to this definition")
    :   *#include <camera.h>*

        Specifies your image buffer location and any image processing you want to perform when fetching images with `CameraTask::GetFrame()`.

        Public Members

        [CameraFormat](#_CPPv4N10coralmicro12CameraFormatE "coralmicro::CameraFormat") `fmt`[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat3fmtE "Permalink to this definition")
        :   Image format such as RGB or raw.

        [CameraFilterMethod](#_CPPv4N10coralmicro18CameraFilterMethodE "coralmicro::CameraFilterMethod") `filter` = [CameraFilterMethod](#_CPPv4N10coralmicro18CameraFilterMethodE "coralmicro::CameraFilterMethod")::[kBilinear](#_CPPv4N10coralmicro18CameraFilterMethod9kBilinearE "coralmicro::CameraFilterMethod::kBilinear")[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat6filterE "Permalink to this definition")
        :   Filter method such as bilinear (default) or nearest-neighbor.

        [CameraRotation](#_CPPv4N10coralmicro14CameraRotationE "coralmicro::CameraRotation") `rotation` = [CameraRotation](#_CPPv4N10coralmicro14CameraRotationE "coralmicro::CameraRotation")::[k270](#_CPPv4N10coralmicro14CameraRotation4k270E "coralmicro::CameraRotation::k270")[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat8rotationE "Permalink to this definition")
        :   Image rotation in 90-degree increments. Default is 270 degree which corresponds to the device held vertically with USB port facing down.

        int `width`[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat5widthE "Permalink to this definition")
        :   Image width. (Native size is `CameraTask::kWidth`.)

        int `height`[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat6heightE "Permalink to this definition")
        :   Image height. (Native size is `CameraTask::kHeight`.)

        bool `preserve_ratio`[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat14preserve_ratioE "Permalink to this definition")
        :   If using non-native width/height, set this true to maintain the native aspect ratio, false to crop the image.

        uint8\_t \*`buffer`[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat6bufferE "Permalink to this definition")
        :   Location to store the image.

        bool `white_balance` = true[Â¶](#_CPPv4N10coralmicro17CameraFrameFormat13white_balanceE "Permalink to this definition")
        :   Set true to perform auto whitebalancing (default), false to disable it.

    *struct* `CameraMotionDetectionConfig`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfigE "Permalink to this definition")
    :   *#include <camera.h>*

        Specifies the configuration for motion detection (performed in camera).

        You can pre-fill this configuration with defaults by passing an instance to `CameraTask::GetMotionDetectionConfigDefault()`. Then specify your callback function (the `cb` variable) and pass this config to `CameraTask::SetMotionDetectionConfig()`. You must also enable camera streaming mode with `CameraTask::Enable()`.

        The default configuration detects any movement in the image frame, but you can specify a smaller detection region by defining a bounding box with the `x0`, `y0`, `x1`, and `y1` variables.

        Public Members

        CameraMotionDetectionCallback `cb`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfig2cbE "Permalink to this definition")
        :   The callback function to call when the camera detects motion. The default config is `nullptr`.

        void \*`cb_param`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfig8cb_paramE "Permalink to this definition")
        :   Optional parameters to pass to the callback function. The default config is `nullptr`.

        bool `enable`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfig6enableE "Permalink to this definition")
        :   Set true to enable motion detection; false to disable it. The default config is true.

        size\_t `x0`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfig2x0E "Permalink to this definition")
        :   The detection zoneâs left-most pixel (index position). The default config is `0`.

        size\_t `y0`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfig2y0E "Permalink to this definition")
        :   The detection zoneâs top-most pixel (index position). The default config is `0`.

        size\_t `x1`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfig2x1E "Permalink to this definition")
        :   The detection zoneâs right-most pixel (index position). The default config is `CameraTask::kWidth - 1` (`323`).

        size\_t `y1`[Â¶](#_CPPv4N10coralmicro27CameraMotionDetectionConfig2y1E "Permalink to this definition")
        :   The detection zoneâs left-most pixel (index position). The default config is `CameraTask::kHeight - 1` (`323`).

    *class* `CameraTask` : *public* [coralmicro](../arduino/#_CPPv410coralmicro "coralmicro")::QueueTask<camera::Request, camera::Response, kCameraTaskName, configMINIMAL\_STACK\_SIZE \* 10, kCameraTaskPriority, 4>[Â¶](#_CPPv4N10coralmicro10CameraTaskE "Permalink to this definition")
    :   *#include <camera.h>*

        Provides access to the Dev Board Micro camera.

        You can access the shared camera object with `CameraTask::GetSingleton()`.

        Public Functions

        void `Init`(lpi2c\_rtos\_handle\_t \**i2c\_handle*)[Â¶](#_CPPv4N10coralmicro10CameraTask4InitEP19lpi2c_rtos_handle_t "Permalink to this definition")
        :   Initializes the camera.

            Programs on the M7 do not need to call this because it is automatically called internally. M7 programs can immediately turn on the camera with `SetPower()`.

            Programs on the M4 must call this to intialize the camera before they can turn on the camera. For example:

            ```
            CameraTask::GetSingleton()->Init(I2C5Handle());
            CameraTask::GetSingleton()->SetPower(true);
            ```

            Parameters
            :   **i2c\_handle** â The camera I2C handle: `I2C5Handle()`.

        bool `Enable`([CameraMode](#_CPPv4N10coralmicro10CameraModeE "coralmicro::CameraMode") *mode*)[Â¶](#_CPPv4N10coralmicro10CameraTask6EnableE10CameraMode "Permalink to this definition")
        :   Enables the camera to begin capture. You must call `SetPower()` before this.

            Parameters
            :   **mode** â The operating mode (either `kStreaming` or `kTrigger`).

            Returns
            :   True if camera is enabled, false otherwise.

        void `Disable`()[Â¶](#_CPPv4N10coralmicro10CameraTask7DisableEv "Permalink to this definition")
        :   Sets the camera into a low-power state, using appoximately 200 Î¼W (compared to approximately 4 mW when streaming). The camera configuration is sustained so it can quickly start again with `Enable()`.

        bool `GetFrame`(*const* std::vector<[CameraFrameFormat](#_CPPv4N10coralmicro17CameraFrameFormatE "coralmicro::CameraFrameFormat")> &*fmts*)[Â¶](#_CPPv4N10coralmicro10CameraTask8GetFrameERKNSt6vectorI17CameraFrameFormatEE "Permalink to this definition")
        :   Gets one frame from the camera buffer and processes it into one or more formats.

            Parameters
            :   **fmts** â A list of image formats you want to receive.

            Returns
            :   True if image processing succeeds, false otherwise.

            Note

            This blocks until a new frame is available from the camera. However, if trigger mode, it returns false if the camera has not been trigged (via `CameraTask::Trigger`) since the last time `CameraTask::GetFrame` was called.

        bool `SetPower`(bool *enable*)[Â¶](#_CPPv4N10coralmicro10CameraTask8SetPowerEb "Permalink to this definition")
        :   Turns the camera power on and off. You must call this before `Enable()`.

            Parameters
            :   **enable** â True to turn the camera on, false to turn it off.

            Returns
            :   True if the action was successful, false otherwise.

        void `SetTestPattern`([CameraTestPattern](#_CPPv4N10coralmicro17CameraTestPatternE "coralmicro::CameraTestPattern") *pattern*)[Â¶](#_CPPv4N10coralmicro10CameraTask14SetTestPatternE17CameraTestPattern "Permalink to this definition")
        :   Enables a camera test pattern instead of using actual sensor data.

            Parameters
            :   **pattern** â The test pattern to use.

        void `Trigger`()[Â¶](#_CPPv4N10coralmicro10CameraTask7TriggerEv "Permalink to this definition")
        :   Triggers image capture when the camera is enabled with `CameraMode::kTrigger`.

            The raw image is held in the camera module memory and you must then fetch it with `GetFrame()`.

        void `DiscardFrames`(int *count*)[Â¶](#_CPPv4N10coralmicro10CameraTask13DiscardFramesEi "Permalink to this definition")
        :   Purges the image sensor data one frame at a time.

            This essentially captures images without saving any of the data, which allows the sensor to calibrate exposure and rid the sensor of any image artifacts that sometimes occur upon initialization.

            Parameters
            :   **The** â number of frames to capture and immediately discard. To allow auto exposure to calibrate, try discarding 100 frames before you begin using images with `GetFrame()`.

        void `GetMotionDetectionConfigDefault`([CameraMotionDetectionConfig](#_CPPv4N10coralmicro27CameraMotionDetectionConfigE "coralmicro::CameraMotionDetectionConfig") &*config*)[Â¶](#_CPPv4N10coralmicro10CameraTask31GetMotionDetectionConfigDefaultER27CameraMotionDetectionConfig "Permalink to this definition")
        :   Gets the default configuration for motion detection.

            Parameters
            :   **config** â The `CameraMotionDetectionConfig` struct to fill with default values.

        void `SetMotionDetectionConfig`(*const* [CameraMotionDetectionConfig](#_CPPv4N10coralmicro27CameraMotionDetectionConfigE "coralmicro::CameraMotionDetectionConfig") &*config*)[Â¶](#_CPPv4N10coralmicro10CameraTask24SetMotionDetectionConfigERK27CameraMotionDetectionConfig "Permalink to this definition")
        :   Sets the configuration for hardware motion detection.

            Note: You must enable camera streaming mode with `CameraTask::Enable()`.

            Parameters
            :   **config** â `CameraMotionDetectionConfig` to apply to the camera.

        Public Static Attributes

        *static* *constexpr* size\_t `kWidth` = 324[Â¶](#_CPPv4N10coralmicro10CameraTask6kWidthE "Permalink to this definition")
        :   Native image pixel width.

        *static* *constexpr* size\_t `kHeight` = 324[Â¶](#_CPPv4N10coralmicro10CameraTask7kHeightE "Permalink to this definition")
        :   Native image pixel height.

        Public Static Functions

        *static* *inline* [CameraTask](#_CPPv4N10coralmicro10CameraTaskE "coralmicro::CameraTask") \*`GetSingleton`()[Â¶](#_CPPv4N10coralmicro10CameraTask12GetSingletonEv "Permalink to this definition")
        :   Gets the `CameraTask` singleton.

            You must use this to acquire the shared `CameraTask` object.

    Functions

    int `CameraFormatBpp`([CameraFormat](#_CPPv4N10coralmicro12CameraFormatE "coralmicro::CameraFormat") *fmt*)[Â¶](#_CPPv4N10coralmicro15CameraFormatBppE12CameraFormat "Permalink to this definition")
    :   Gets the bytes-per-pixel (the number of color channels) used by the given image format.

        Parameters
        :   **The** â image format (from `CameraFormat`).

        Returns
        :   The number of bytes per pixel.

    Enums

    *enum class* `CameraMode` : uint8\_t[Â¶](#_CPPv4N10coralmicro10CameraModeE "Permalink to this definition")
    :   The camera operating mode for `CameraTask::Enable()`.

        *Values:*

        *enumerator* `kStreaming`[Â¶](#_CPPv4N10coralmicro10CameraMode10kStreamingE "Permalink to this definition")
        :   Streaming mode. The camera continuously captures and pushes raw images to an internal image buffer. You can then fetch images one at a time in your preferred format with `CameraTask::GetFrame()`.

        *enumerator* `kTrigger`[Â¶](#_CPPv4N10coralmicro10CameraMode8kTriggerE "Permalink to this definition")
        :   Trigger mode. The camera captures one image at a time when you call `CameraTask::Trigger()`. You can then fetch each image and process it into your preferred format with `CameraTask::GetFrame()`.

    *enum class* `CameraTestPattern` : uint8\_t[Â¶](#_CPPv4N10coralmicro17CameraTestPatternE "Permalink to this definition")
    :   Test patterns to use with `CameraTask::SetTestPattern()`

        *Values:*

        *enumerator* `kNone`[Â¶](#_CPPv4N10coralmicro17CameraTestPattern5kNoneE "Permalink to this definition")

        *enumerator* `kColorBar`[Â¶](#_CPPv4N10coralmicro17CameraTestPattern9kColorBarE "Permalink to this definition")

        *enumerator* `kWalkingOnes`[Â¶](#_CPPv4N10coralmicro17CameraTestPattern12kWalkingOnesE "Permalink to this definition")

    *enum class* `CameraFormat`[Â¶](#_CPPv4N10coralmicro12CameraFormatE "Permalink to this definition")
    :   Image format options, used with `CameraFrameFormat`.

        *Values:*

        *enumerator* `kRgb`[Â¶](#_CPPv4N10coralmicro12CameraFormat4kRgbE "Permalink to this definition")
        :   RGB image.

        *enumerator* `kY8`[Â¶](#_CPPv4N10coralmicro12CameraFormat3kY8E "Permalink to this definition")
        :   Y8 (grayscale) image.

        *enumerator* `kRaw`[Â¶](#_CPPv4N10coralmicro12CameraFormat4kRawE "Permalink to this definition")
        :   Raw bayer image.

    *enum class* `CameraFilterMethod`[Â¶](#_CPPv4N10coralmicro18CameraFilterMethodE "Permalink to this definition")
    :   Image resampling method (when resizing the image).

        *Values:*

        *enumerator* `kBilinear`[Â¶](#_CPPv4N10coralmicro18CameraFilterMethod9kBilinearE "Permalink to this definition")

        *enumerator* `kNearestNeighbor`[Â¶](#_CPPv4N10coralmicro18CameraFilterMethod16kNearestNeighborE "Permalink to this definition")

    *enum class* `CameraRotation`[Â¶](#_CPPv4N10coralmicro14CameraRotationE "Permalink to this definition")
    :   Clockwise image rotations.

        *Values:*

        *enumerator* `k0`[Â¶](#_CPPv4N10coralmicro14CameraRotation2k0E "Permalink to this definition")
        :   The natural orientation for the camera module.

        *enumerator* `k90`[Â¶](#_CPPv4N10coralmicro14CameraRotation3k90E "Permalink to this definition")
        :   Rotated 90-degrees clockwise. Upside down, relative to the boardâs âCoralâ label.

        *enumerator* `k180`[Â¶](#_CPPv4N10coralmicro14CameraRotation4k180E "Permalink to this definition")
        :   Rotated 180-degrees clockwise.

        *enumerator* `k270`[Â¶](#_CPPv4N10coralmicro14CameraRotation4k270E "Permalink to this definition")
        :   Rotated 270-degrees clockwise. Right-side up, relative to the boardâs âCoralâ label.
