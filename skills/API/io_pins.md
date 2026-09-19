I/O Pins APIs
=============

The Dev Board Micro provides access to several digital pins on the
two 12-pin headers, including GPIO, PWM, I2C, and SPI. This page describes all
the coralmicro APIs available for these pins, plus APIs for the on-board LEDs.

Note

The Dev Board Micro does not include header pins. For development, we
suggest you solder header pins to the board with pins facing down
(the direction is important to attach to breadboards and for compatibility
with Coral cases). If you plan to put the board into a case, be sure your
header pins are long enough to be accessible through the case.

![](/static/docs/reference/micro/images/micro-pinout.png)


**Figure 1.** Pinout for the 12-pin headers, LEDs and User button

Note

All pins are powered by the 1.8 V power rail, and provide a max current of
approximately 6 mA on most pins.

Warning

When handling any of these pins, be cautious
to avoid electrostatic discharge or contact with conductive materials
(metals). Failure to properly handle the board can result in a short circuit,
electric shock, serious injury, death, fire, or damage to your board and other
property.

GPIO
----

Almost all digital pins on the 12-pin headers can be used for GPIO
(exceptions are the UART TX/RX and DAC pins).

To use a GPIO, specify the GPIO pin name (indicated in figure 1), the
direction, and pull-up or pull-down with
[`coralmicro::GpioSetMode()`](#_CPPv4N10coralmicro11GpioSetModeE4Gpio8GpioMode "coralmicro::GpioSetMode"). Then set the value or get the value with
[`coralmicro::GpioSet()`](#_CPPv4N10coralmicro7GpioSetE4Gpiob "coralmicro::GpioSet") and [`coralmicro::GpioGet()`](#_CPPv4N10coralmicro7GpioGetE4Gpio "coralmicro::GpioGet").

For example:

```
extern "C" void app_main(void* param) {
  coralmicro::GpioSetMode(coralmicro::Gpio::kAA, coralmicro::GpioMode::kOutput);
  bool on = true;
  while (true) {
    on = !on;
    coralmicro::GpioSet(coralmicro::Gpio::kAA, on);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
```

[[gpio.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/gpio.h)

*namespace* `coralmicro`
:   Typedefs

    *using* `GpioCallback` = std::function<void()>[Â¶](#_CPPv4N10coralmicro12GpioCallbackE "Permalink to this definition")
    :   The function type required by `GpioConfigureInterrupt()`.

    Enums

    *enum* `Gpio`[Â¶](#_CPPv4N10coralmicro4GpioE "Permalink to this definition")
    :   Pre-configured GPIO pins.

        *Values:*

        *enumerator* `kStatusLed`[Â¶](#_CPPv4N10coralmicro4Gpio10kStatusLedE "Permalink to this definition")
        :   On-board Status LED (orange). Instead use `LedSet()`.

        *enumerator* `kUserLed`[Â¶](#_CPPv4N10coralmicro4Gpio8kUserLedE "Permalink to this definition")
        :   On-board User LED (green). Instead use `LedSet()`.

        *enumerator* `kEdgeTpuPgood`[Â¶](#_CPPv4N10coralmicro4Gpio13kEdgeTpuPgoodE "Permalink to this definition")
        :   Power-good signal from TPU. Active high.

        *enumerator* `kEdgeTpuReset`[Â¶](#_CPPv4N10coralmicro4Gpio13kEdgeTpuResetE "Permalink to this definition")
        :   Reset input to TPU. Active low.

        *enumerator* `kEdgeTpuPmic`[Â¶](#_CPPv4N10coralmicro4Gpio12kEdgeTpuPmicE "Permalink to this definition")
        :   Power-enable input to TPU. Active high.

        *enumerator* `kBtRegOn`[Â¶](#_CPPv4N10coralmicro4Gpio8kBtRegOnE "Permalink to this definition")
        :   Enables power regulator on CYW43455 (via B2B connector)

        *enumerator* `kUserButton`[Â¶](#_CPPv4N10coralmicro4Gpio11kUserButtonE "Permalink to this definition")
        :   On-board User button. Active low.

        *enumerator* `kCameraTrigger`[Â¶](#_CPPv4N10coralmicro4Gpio14kCameraTriggerE "Permalink to this definition")
        :   Trigger GPIO for single-shot camera capture. Not to be used by apps; instead see `CameraTask::Trigger()`.

        *enumerator* `kCameraInt`[Â¶](#_CPPv4N10coralmicro4Gpio10kCameraIntE "Permalink to this definition")
        :   Input from the camera to indicate motion was detected.

        *enumerator* `kAntennaSelect`[Â¶](#_CPPv4N10coralmicro4Gpio14kAntennaSelectE "Permalink to this definition")
        :   Selects between on-board antenna and antenna connector (for Wi-Fi and BT). Low for internal, high for external.

        *enumerator* `kBtHostWake`[Â¶](#_CPPv4N10coralmicro4Gpio11kBtHostWakeE "Permalink to this definition")
        :   Input from Bluetooth to wake the host, if it was sleeping.

        *enumerator* `kBtDevWake`[Â¶](#_CPPv4N10coralmicro4Gpio10kBtDevWakeE "Permalink to this definition")
        :   Output to Bluetooth, to wake the Bluetooth module from sleep.

        *enumerator* `kEthPhyRst`[Â¶](#_CPPv4N10coralmicro4Gpio10kEthPhyRstE "Permalink to this definition")
        :   Reset signal to the Ethernet PHY. Active low.

        *enumerator* `kCameraPrivacyOverride`[Â¶](#_CPPv4N10coralmicro4Gpio22kCameraPrivacyOverrideE "Permalink to this definition")
        :   Override for the Camera LED. Low to disable the LED.

        *enumerator* `kCryptoRst`[Â¶](#_CPPv4N10coralmicro4Gpio10kCryptoRstE "Permalink to this definition")
        :   Reset signal to the A71CH. Active low.

        *enumerator* `kLpuart1SwitchEnable`[Â¶](#_CPPv4N10coralmicro4Gpio20kLpuart1SwitchEnableE "Permalink to this definition")
        :   Enable signal for the switch that blocks LPUART1 during boot. Set high to allow the LPUART1 signal to pass through.

        *enumerator* `kSpiCs`[Â¶](#_CPPv4N10coralmicro4Gpio6kSpiCsE "Permalink to this definition")
        :   SPI6\_CS (GPIO\_LPSR\_09); right header (J10); pin 5.

        *enumerator* `kSpiSck`[Â¶](#_CPPv4N10coralmicro4Gpio7kSpiSckE "Permalink to this definition")
        :   SPI6\_SCK (GPIO\_LPSR\_10); right header (J10); pin 6.

        *enumerator* `kSpiSdo`[Â¶](#_CPPv4N10coralmicro4Gpio7kSpiSdoE "Permalink to this definition")
        :   SPI6\_SDO (GPIO\_LPSR\_11); right header (J10); pin 7.

        *enumerator* `kSpiSdi`[Â¶](#_CPPv4N10coralmicro4Gpio7kSpiSdiE "Permalink to this definition")
        :   SPI6\_SDI (GPIO\_LPSR\_12); right header (J10); pin 8.

        *enumerator* `kSda6`[Â¶](#_CPPv4N10coralmicro4Gpio5kSda6E "Permalink to this definition")
        :   I2C6\_SDA (GPIO\_LPSR\_06); right header (J10); pin 10.

        *enumerator* `kScl1`[Â¶](#_CPPv4N10coralmicro4Gpio5kScl1E "Permalink to this definition")
        :   I2C1\_SCL (GPIO\_AD\_32); right header (J10); pin 11.

        *enumerator* `kSda1`[Â¶](#_CPPv4N10coralmicro4Gpio5kSda1E "Permalink to this definition")
        :   I2C1\_SDA (GPIO\_AD\_33); right header (J10); pin 12.

        *enumerator* `kAA`[Â¶](#_CPPv4N10coralmicro4Gpio3kAAE "Permalink to this definition")
        :   ADC1\_CH0A (GPIO\_AD\_06); left header (J9), pin 3.

        *enumerator* `kAB`[Â¶](#_CPPv4N10coralmicro4Gpio3kABE "Permalink to this definition")
        :   ADC1\_CH0B (GPIO\_AD\_07); left header (J9), pin 4.

        *enumerator* `kUartCts`[Â¶](#_CPPv4N10coralmicro4Gpio8kUartCtsE "Permalink to this definition")
        :   UART6\_CTS (GPIO\_EMC\_B2\_00); left header (J9), pin 7.

        *enumerator* `kUartRts`[Â¶](#_CPPv4N10coralmicro4Gpio8kUartRtsE "Permalink to this definition")
        :   UART6\_RTS (GPIO\_EMC\_B2\_01); left header (J9), pin 8.

        *enumerator* `kPwm1`[Â¶](#_CPPv4N10coralmicro4Gpio5kPwm1E "Permalink to this definition")
        :   PWM\_B (GPIO\_AD\_01); left header (J9), pin 9.

        *enumerator* `kPwm0`[Â¶](#_CPPv4N10coralmicro4Gpio5kPwm0E "Permalink to this definition")
        :   PWM\_A (GPIO\_AD\_00); left header (J9), pin 10.

        *enumerator* `kScl6`[Â¶](#_CPPv4N10coralmicro4Gpio5kScl6E "Permalink to this definition")
        :   I2C6\_SCL (GPIO\_LPSR\_07); left header (J9), pin 11.

        *enumerator* `kCount`[Â¶](#_CPPv4N10coralmicro4Gpio6kCountE "Permalink to this definition")
        :   Number of pre-configured GPIOs.

    *enum* `GpioInterruptMode`[Â¶](#_CPPv4N10coralmicro17GpioInterruptModeE "Permalink to this definition")
    :   Interrupt modes for use with `GpioConfigureInterrupt()`.

        *Values:*

        *enumerator* `kIntModeNone`[Â¶](#_CPPv4N10coralmicro17GpioInterruptMode12kIntModeNoneE "Permalink to this definition")
        :   Disables GPIO interrupt.

        *enumerator* `kIntModeLow`[Â¶](#_CPPv4N10coralmicro17GpioInterruptMode11kIntModeLowE "Permalink to this definition")
        :   Interrupt when line is low.

        *enumerator* `kIntModeHigh`[Â¶](#_CPPv4N10coralmicro17GpioInterruptMode12kIntModeHighE "Permalink to this definition")
        :   Interrupt when line is high.

        *enumerator* `kIntModeRising`[Â¶](#_CPPv4N10coralmicro17GpioInterruptMode14kIntModeRisingE "Permalink to this definition")
        :   Interrupt when line is rising.

        *enumerator* `kIntModeFalling`[Â¶](#_CPPv4N10coralmicro17GpioInterruptMode15kIntModeFallingE "Permalink to this definition")
        :   Interrupt when line is falling.

        *enumerator* `kIntModeChanging`[Â¶](#_CPPv4N10coralmicro17GpioInterruptMode16kIntModeChangingE "Permalink to this definition")
        :   Interrupt when line is either rising or falling.

        *enumerator* `kIntModeCount`[Â¶](#_CPPv4N10coralmicro17GpioInterruptMode13kIntModeCountE "Permalink to this definition")
        :   Number of interrupt modes.

    *enum class* `GpioMode`[Â¶](#_CPPv4N10coralmicro8GpioModeE "Permalink to this definition")
    :   GPIO modes for `GpioSetMode()`.

        *Values:*

        *enumerator* `kInput`[Â¶](#_CPPv4N10coralmicro8GpioMode6kInputE "Permalink to this definition")

        *enumerator* `kOutput`[Â¶](#_CPPv4N10coralmicro8GpioMode7kOutputE "Permalink to this definition")

        *enumerator* `kInputPullUp`[Â¶](#_CPPv4N10coralmicro8GpioMode12kInputPullUpE "Permalink to this definition")

        *enumerator* `kInputPullDown`[Â¶](#_CPPv4N10coralmicro8GpioMode14kInputPullDownE "Permalink to this definition")

    Functions

    void `GpioSet`([Gpio](#_CPPv4N10coralmicro4GpioE "coralmicro::Gpio") *gpio*, bool *enable*)[Â¶](#_CPPv4N10coralmicro7GpioSetE4Gpiob "Permalink to this definition")
    :   Sets the output value of a GPIO.

        Parameters
        :   * **gpio** â Pin to configure. Only pins in the `Gpio` enumeration can be configured with this module. To use a GPIO that is not covered by this module, use the functions in `third_party/nxp/rt1176-sdk/devices/MIMXRT1176/drivers/fsl_gpio.h`
            * **enable** â Whether to set the pin to high or low.

    bool `GpioGet`([Gpio](#_CPPv4N10coralmicro4GpioE "coralmicro::Gpio") *gpio*)[Â¶](#_CPPv4N10coralmicro7GpioGetE4Gpio "Permalink to this definition")
    :   Gets the input value of a GPIO.

        Parameters
        :   **gpio** â Pin to read.

        Returns
        :   Boolean representing high or low state of the pin.

    void `GpioSetMode`([Gpio](#_CPPv4N10coralmicro4GpioE "coralmicro::Gpio") *gpio*, [GpioMode](#_CPPv4N10coralmicro8GpioModeE "coralmicro::GpioMode") *mode*)[Â¶](#_CPPv4N10coralmicro11GpioSetModeE4Gpio8GpioMode "Permalink to this definition")
    :   Sets the mode of a GPIO.

        Parameters
        :   * **gpio** â Pin to configure.
            * **mode** â Mode to configure the gpio as.

    void `GpioConfigureInterrupt`([Gpio](#_CPPv4N10coralmicro4GpioE "coralmicro::Gpio") *gpio*, [GpioInterruptMode](#_CPPv4N10coralmicro17GpioInterruptModeE "coralmicro::GpioInterruptMode") *mode*, [GpioCallback](#_CPPv4N10coralmicro12GpioCallbackE "coralmicro::GpioCallback") *cb*)[Â¶](#_CPPv4N10coralmicro22GpioConfigureInterruptE4Gpio17GpioInterruptMode12GpioCallback "Permalink to this definition")
    :   Sets the interrupt mode and callback for a GPIO.

        Parameters
        :   * **gpio** â Pin to configure.
            * **mode** â The style of interrupt to sense.
            * **cb** â Callback function that will be invoked when the interrupt is raised. This is called from interrupt context, so it should not do much work.

    void `GpioConfigureInterrupt`([Gpio](#_CPPv4N10coralmicro4GpioE "coralmicro::Gpio") *gpio*, [GpioInterruptMode](#_CPPv4N10coralmicro17GpioInterruptModeE "coralmicro::GpioInterruptMode") *mode*, [GpioCallback](#_CPPv4N10coralmicro12GpioCallbackE "coralmicro::GpioCallback") *cb*, uint64\_t *debounce\_interval\_us*)[Â¶](#_CPPv4N10coralmicro22GpioConfigureInterruptE4Gpio17GpioInterruptMode12GpioCallback8uint64_t "Permalink to this definition")
    :   Sets the interrupt mode, callback, and debounce interval for a GPIO.

        **Example** (from `examples/button_led/`):

        ```
        [[noreturn]] void Main() {
          printf("Button LED Example!\r\n");
          // Turn on Status LED to show the board is on.
          LedSet(Led::kStatus, true);

          printf("Press the User button.\r\n");

          // Register callback for the user button.
          GpioConfigureInterrupt(
              Gpio::kUserButton, GpioInterruptMode::kIntModeFalling,
              [handle = xTaskGetCurrentTaskHandle()]() { xTaskResumeFromISR(handle); },
              /*debounce_interval_us=*/50 * 1e3);
          bool on = false;
          while (true) {
            vTaskSuspend(nullptr);
            on = !on;
            LedSet(Led::kUser, on);
          }
        }
        ```

        Parameters
        :   * **gpio** â Pin to configure.
            * **mode** â The style of interrupt to sense.
            * **cb** â Callback function that will be invoked when the interrupt is raised. This is called from interrupt context, so it should not do much work.
            * **debounce\_interval\_us** â Minimum interval in microseconds between repeated invocations of `cb`. Useful for cases where the GPIO line could toggle back and forth more frequently than expected, such as a mechanical button.

PWM
---

There are two pins pre-configured for pulse-width modulation (PWM)
on the left header (pins 9 and 10).

To use a PWM pin, you must first call
[`coralmicro::PwmInit()`](#_CPPv4N10coralmicro7PwmInitEv "coralmicro::PwmInit"). Then specify the PWM settings in an instance of
[`coralmicro::PwmPinConfig`](#_CPPv4N10coralmicro12PwmPinConfigE "coralmicro::PwmPinConfig") and pass it to
[`coralmicro::PwmEnable()`](#_CPPv4N10coralmicro9PwmEnableERKNSt6vectorI12PwmPinConfigEE "coralmicro::PwmEnable").

**Example** (from examples/pwm/):

```
[[noreturn]] void Main() {
  printf("PWM Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  PwmInit();
  PwmPinConfig pin_a_config{.duty_cycle = 20,
                            .frequency = 1000,
                            .pin_setting = PwmPinSettingFor(PwmPin::k10)};
  PwmPinConfig pin_b_config{.duty_cycle = 80,
                            .frequency = 1000,
                            .pin_setting = PwmPinSettingFor(PwmPin::k9)};
  std::vector<PwmPinConfig> configs = {pin_a_config, pin_b_config};
  while (true) {
    PwmEnable(configs);
    vTaskDelay(pdMS_TO_TICKS(1000));
    PwmDisable(configs);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
```

[[pwm.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/pwm.h)

*namespace* `coralmicro`
:   Enums

    *enum class* `PwmPin`[Â¶](#_CPPv4N10coralmicro6PwmPinE "Permalink to this definition")
    :   The board currently has 2 pwm pins exposed: PWM\_A (pin 10 on the left-side header) PWM\_B (pin 9 on the left-side header). Note: These pins output a max of 1.8V

        *Values:*

        *enumerator* `k9`[Â¶](#_CPPv4N10coralmicro6PwmPin2k9E "Permalink to this definition")

        *enumerator* `k10`[Â¶](#_CPPv4N10coralmicro6PwmPin3k10E "Permalink to this definition")

    Functions

    [PwmPinSetting](#_CPPv4N10coralmicro13PwmPinSettingE "coralmicro::PwmPinSetting") `PwmPinSettingFor`([PwmPin](#_CPPv4N10coralmicro6PwmPinE "coralmicro::PwmPin") *pin*)[Â¶](#_CPPv4N10coralmicro16PwmPinSettingForE6PwmPin "Permalink to this definition")
    :   Gets the HW settings for a pwm pin.

        Parameters
        :   **pin** â The pin to get settings for.

        Returns
        :   A setting for the pin or a std::nullopt.

    void `PwmInit`()[Â¶](#_CPPv4N10coralmicro7PwmInitEv "Permalink to this definition")
    :   Initializes the PWM module.

    void `PwmEnable`(*const* std::vector<[PwmPinConfig](#_CPPv4N10coralmicro12PwmPinConfigE "coralmicro::PwmPinConfig")> &*pin\_configs*)[Â¶](#_CPPv4N10coralmicro9PwmEnableERKNSt6vectorI12PwmPinConfigEE "Permalink to this definition")
    :   Enables a PWM sub\_module with some pins configs.

        Parameters
        :   **pin\_configs** â The list of pin configurations to enable.

    void `PwmDisable`(*const* std::vector<[PwmPinConfig](#_CPPv4N10coralmicro12PwmPinConfigE "coralmicro::PwmPinConfig")> &*pin\_configs*)[Â¶](#_CPPv4N10coralmicro10PwmDisableERKNSt6vectorI12PwmPinConfigEE "Permalink to this definition")
    :   Disables a PWM sub\_module.

        Parameters
        :   **pin\_configs** â The list of pin configurations to disable.

    *struct* `PwmPinConfig`[Â¶](#_CPPv4N10coralmicro12PwmPinConfigE "Permalink to this definition")
    :   *#include <pwm.h>*

        Represents the configuration for a single PWM pin.

        Public Members

        int `duty_cycle`[Â¶](#_CPPv4N10coralmicro12PwmPinConfig10duty_cycleE "Permalink to this definition")
        :   The duty cycle (from 0-100) of the PWM waveform.

        uint32\_t `frequency`[Â¶](#_CPPv4N10coralmicro12PwmPinConfig9frequencyE "Permalink to this definition")
        :   Frequency in hz.

        [PwmPinSetting](#_CPPv4N10coralmicro13PwmPinSettingE "coralmicro::PwmPinSetting") `pin_setting`[Â¶](#_CPPv4N10coralmicro12PwmPinConfig11pin_settingE "Permalink to this definition")
        :   The HW setting of the pin to start duty cycle.

    *struct* `PwmPinSetting`[Â¶](#_CPPv4N10coralmicro13PwmPinSettingE "Permalink to this definition")
    :   *#include <pwm.h>*

        Represents a PWM Pinâs HW setting.

        Public Members

        PWM\_Type \*`base`[Â¶](#_CPPv4N10coralmicro13PwmPinSetting4baseE "Permalink to this definition")
        :   Pointer to the base register of the PWM module.

        pwm\_submodule\_t `sub_module`[Â¶](#_CPPv4N10coralmicro13PwmPinSetting10sub_moduleE "Permalink to this definition")
        :   The PWM submodule of this pin.

        pwm\_channels\_t `pwm_channel`[Â¶](#_CPPv4N10coralmicro13PwmPinSetting11pwm_channelE "Permalink to this definition")
        :   The channel of this pin.

ADC / DAC
---------

There are two analog-to-digital converter (ADC) pins pre-configured on the left
header (pins 3 and 4) and one digital-to-analog converter (DAC) pin on the right
header (pin 9).

To use ADC in either single-ended or differential mode, you must first call
[`coralmicro::AdcInit()`](#_CPPv4N10coralmicro7AdcInitEv "coralmicro::AdcInit") and
[`coralmicro::AdcCreateConfig()`](#_CPPv4N10coralmicro15AdcCreateConfigER9AdcConfigi7AdcSideb "coralmicro::AdcCreateConfig"). Then you can read input with
[`coralmicro::AdcRead()`](#_CPPv4N10coralmicro7AdcReadERK9AdcConfig "coralmicro::AdcRead").

For DAC, first call [`coralmicro::DacInit()`](#_CPPv4N10coralmicro7DacInitEv "coralmicro::DacInit") and
[`coralmicro::DacEnable()`](#_CPPv4N10coralmicro9DacEnableEb "coralmicro::DacEnable"). Then you can write output with
[`coralmicro::DacWrite()`](#_CPPv4N10coralmicro8DacWriteE8uint16_t "coralmicro::DacWrite").

**Example** (from examples/analog/):

```
[[noreturn]] void Main() {
  printf("Analog Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  AdcInit();
  DacInit();
  AdcConfig config{};
  AdcCreateConfig(config,
                  /*channel=*/0,
                  /*primary_side=*/AdcSide::kB,
                  /*differential=*/false);

  // Set the DAC to 0V before we enable it initially.
  DacWrite(0);
  DacEnable(true);
  while (true) {
    uint16_t val = AdcRead(config);
    DacWrite(val);
    printf("ADC val: %u\r\n", val);
  }
}
```

[[analog.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/analog.h)

*namespace* `coralmicro`
:   Enums

    *enum class* `AdcSide`[Â¶](#_CPPv4N10coralmicro7AdcSideE "Permalink to this definition")
    :   Choices for the primary side of an ADC.

        *Values:*

        *enumerator* `kA`[Â¶](#_CPPv4N10coralmicro7AdcSide2kAE "Permalink to this definition")
        :   ADC1\_CH0A (GPIO\_AD\_06); left header, pin 3.

        *enumerator* `kB`[Â¶](#_CPPv4N10coralmicro7AdcSide2kBE "Permalink to this definition")
        :   ADC1\_CH0B (GPIO\_AD\_07); left header, pin 4.

    Functions

    void `AdcInit`()[Â¶](#_CPPv4N10coralmicro7AdcInitEv "Permalink to this definition")
    :   Initializes ADC device.

    void `AdcCreateConfig`([AdcConfig](#_CPPv4N10coralmicro9AdcConfigE "coralmicro::AdcConfig") &*config*, int *channel*, [AdcSide](#_CPPv4N10coralmicro7AdcSideE "coralmicro::AdcSide") *primary\_side*, bool *differential*)[Â¶](#_CPPv4N10coralmicro15AdcCreateConfigER9AdcConfigi7AdcSideb "Permalink to this definition")
    :   Populates an `ADCConfig` struct based on the given parameters.

        Parameters
        :   * **config** â Configuration struct to populate.
            * **channel** â The ADC channel to use (must be less than the max number of channels: `kAdc1ChannelCount`).
            * **primary\_side** â In single ended mode, this is the pin thatâs connected. In differential mode, this is the pin to use as the primary side.
            * **differential** â Whether or not to run the ADC in differential mode.

    uint16\_t `AdcRead`(*const* [AdcConfig](#_CPPv4N10coralmicro9AdcConfigE "coralmicro::AdcConfig") &*config*)[Â¶](#_CPPv4N10coralmicro7AdcReadERK9AdcConfig "Permalink to this definition")
    :   Reads voltage values from an ADC.

        Parameters
        :   **config** â ADC configuration to use.

        Returns
        :   Digitized value of the voltage that the ADC is sensing. The ADC has 12 bits of precision, so the maximum value returned is 4095.

    void `DacInit`()[Â¶](#_CPPv4N10coralmicro7DacInitEv "Permalink to this definition")
    :   Initializes DAC device.

        Parameters
        :   **device** â DAC to initialize.

    void `DacEnable`(bool *enable*)[Â¶](#_CPPv4N10coralmicro9DacEnableEb "Permalink to this definition")
    :   Sets the state of the DAC. It is recommended that you set a voltage with `DacWrite` prior to enabling the DAC.

        Parameters
        :   **enable** â True enables the DAC; false disables it.

    void `DacWrite`(uint16\_t *value*)[Â¶](#_CPPv4N10coralmicro8DacWriteE8uint16_t "Permalink to this definition")
    :   Writes voltage values to the DAC.

        For example code, see `examples/analog/`.

        Parameters
        :   **value** â The voltage value to output. The DAC has 12-bit resolution, so the allowed values are 0 to 4095. The maximum output voltage of the DAC is 1.8V.

    Variables

    *constexpr* int `kAdc1ChannelCount` = 6[Â¶](#_CPPv4N10coralmicro17kAdc1ChannelCountE "Permalink to this definition")
    :   Available channels on ADC1.

    *struct* `AdcConfig`[Â¶](#_CPPv4N10coralmicro9AdcConfigE "Permalink to this definition")
    :   *#include <analog.h>*

        Represents the configuration of an ADC. Each ADC has a 12-bit resolution with 1.8V reference voltage.

        Public Members

        ADC\_Type \*`device`[Â¶](#_CPPv4N10coralmicro9AdcConfig6deviceE "Permalink to this definition")
        :   Pointer to the base register of the ADC.

        lpadc\_conv\_command\_config\_t `conv_config`[Â¶](#_CPPv4N10coralmicro9AdcConfig11conv_configE "Permalink to this definition")
        :   Configuration for ADC conversion.

        lpadc\_conv\_trigger\_config\_t `trigger_config`[Â¶](#_CPPv4N10coralmicro9AdcConfig14trigger_configE "Permalink to this definition")
        :   Configuration for ADC triggers.

I2C
---

You can use the board as either the device controller or target, using
either of two I2C lines on the 12-pin headers:

* I2C1 (I2c::kI2c1)

  > + SDA is pin 10 on the right side
  > + SCL is pin 11 on the left side
* I2C6 (I2c::kI2c6)

  > + SDA is pin 12 on the right side
  > + SCL is pin 11 on the right side

**Example** (from examples/i2c/controller.cc):

```
void Main() {
  printf("i2c Controller Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  auto config = I2cGetDefaultConfig(coralmicro::I2c::kI2c1);
  I2cInitController(config);

  std::string serial = GetSerialNumber();
  constexpr int kTargetAddress = 0x42;
  int kTransferSize = serial.length();

  printf("Writing our serial number to the remote device...\r\n");
  CHECK(I2cControllerWrite(config, kTargetAddress,
                           reinterpret_cast<uint8_t*>(serial.data()),
                           kTransferSize));
  auto buffer = std::vector<uint8_t>(kTransferSize, 0);

  printf("Reading back our serial number from the remote device...\r\n");
  CHECK(
      I2cControllerRead(config, kTargetAddress, buffer.data(), kTransferSize));
  CHECK(memcmp(buffer.data(), serial.data(), kTransferSize) == 0);
  printf("Readback of data from target device matches written data!\r\n");
}
```

[[i2c.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/i2c.h)

*namespace* `coralmicro`
:   Typedefs

    *typedef* lpi2c\_slave\_config\_t `lpi2c_target_config_t`[Â¶](#_CPPv4N10coralmicro21lpi2c_target_config_tE "Permalink to this definition")

    *typedef* lpi2c\_slave\_transfer\_t `lpi2c_target_transfer_t`[Â¶](#_CPPv4N10coralmicro23lpi2c_target_transfer_tE "Permalink to this definition")

    *typedef* lpi2c\_slave\_handle\_t `lpi2c_target_handle_t`[Â¶](#_CPPv4N10coralmicro21lpi2c_target_handle_tE "Permalink to this definition")

    *using* `TargetCallback` = std::function<void([I2cConfig](#_CPPv4N10coralmicro9I2cConfigE "coralmicro::I2cConfig")\*, [lpi2c\_target\_transfer\_t](#_CPPv4N10coralmicro23lpi2c_target_transfer_tE "coralmicro::lpi2c_target_transfer_t")\*)>[Â¶](#_CPPv4N10coralmicro14TargetCallbackE "Permalink to this definition")

    Enums

    *enum class* `I2c`[Â¶](#_CPPv4N10coralmicro3I2cE "Permalink to this definition")
    :   I2C buses available on the header pins.

        *Values:*

        *enumerator* `kI2c1`[Â¶](#_CPPv4N10coralmicro3I2c5kI2c1E "Permalink to this definition")
        :   I2C1\_SCL (J10, pin 11) and I2C1\_SDA (J10, pin 12)

        *enumerator* `kI2c6`[Â¶](#_CPPv4N10coralmicro3I2c5kI2c6E "Permalink to this definition")
        :   I2C6\_SCL (J9, pin 11) and I2C6\_SDA (J10, pin 10)

    Functions

    [I2cConfig](#_CPPv4N10coralmicro9I2cConfigE "coralmicro::I2cConfig") `I2cGetDefaultConfig`([I2c](#_CPPv4N10coralmicro3I2cE "coralmicro::I2c") *bus*)[Â¶](#_CPPv4N10coralmicro19I2cGetDefaultConfigE3I2c "Permalink to this definition")
    :   Gets the default configuration for an I2C bus that is available on the header.

        Parameters
        :   **bus** â `I2c` of the desired bus.

        Returns
        :   Configuration for using `bus`.

    bool `I2cInitController`([I2cConfig](#_CPPv4N10coralmicro9I2cConfigE "coralmicro::I2cConfig") &*config*)[Â¶](#_CPPv4N10coralmicro17I2cInitControllerER9I2cConfig "Permalink to this definition")
    :   Initializes a bus in âcontrollerâ mode using the given config.

        Parameters
        :   **config** â `I2cConfig` to initialize the hardware with.

    bool `I2cInitTarget`([I2cConfig](#_CPPv4N10coralmicro9I2cConfigE "coralmicro::I2cConfig") &*config*, uint8\_t *address*, [TargetCallback](#_CPPv4N10coralmicro14TargetCallbackE "coralmicro::TargetCallback") *callback*)[Â¶](#_CPPv4N10coralmicro13I2cInitTargetER9I2cConfig7uint8_t14TargetCallback "Permalink to this definition")
    :   Initializes a bus in âtargetâ mode using the given config.

        Parameters
        :   * **config** â `I2cConfig` to initialize the hardware with.
            * **address** â Address to listen for on the bus.
            * **callback** â `TargetCallback` method to provide the response data.

    bool `I2cControllerRead`([I2cConfig](#_CPPv4N10coralmicro9I2cConfigE "coralmicro::I2cConfig") &*config*, uint8\_t *address*, uint8\_t \**buffer*, size\_t *count*)[Â¶](#_CPPv4N10coralmicro17I2cControllerReadER9I2cConfig7uint8_tP7uint8_t6size_t "Permalink to this definition")
    :   Reads data from the configured bus, in `controller` mode.

        Parameters
        :   * **config** â `I2cConfig` for the configured bus.
            * **address** â Address of the target device we wish to read from.
            * **buffer** â Pointer to a buffer that will contain the receieved data.
            * **count** â Number of bytes to read.

    bool `I2cControllerWrite`([I2cConfig](#_CPPv4N10coralmicro9I2cConfigE "coralmicro::I2cConfig") &*config*, uint8\_t *address*, uint8\_t \**buffer*, size\_t *count*)[Â¶](#_CPPv4N10coralmicro18I2cControllerWriteER9I2cConfig7uint8_tP7uint8_t6size_t "Permalink to this definition")
    :   Writes data on the configured bus, in `controller` mode.

        Parameters
        :   * **config** â `I2cConfig` for the configured bus.
            * **address** â Address of the target device we wish to write to.
            * **buffer** â Pointer to a buffer that contains the data.
            * **count** â Number of bytes to write.

    *struct* `I2cConfig`[Â¶](#_CPPv4N10coralmicro9I2cConfigE "Permalink to this definition")
    :   *#include <i2c.h>*

        Configuration for an I2C interface.

        Public Members

        LPI2C\_Type \*`base`[Â¶](#_CPPv4N10coralmicro9I2cConfig4baseE "Permalink to this definition")
        :   Base pointer to the I2C peripheral.

        int `interrupt`[Â¶](#_CPPv4N10coralmicro9I2cConfig9interruptE "Permalink to this definition")
        :   Interrupt number for the peripheral.

        std::array<uint32\_t, 5> `sda`[Â¶](#_CPPv4N10coralmicro9I2cConfig3sdaE "Permalink to this definition")
        :   IOMUXC configuration for the SDA pin.

        std::array<uint32\_t, 5> `scl`[Â¶](#_CPPv4N10coralmicro9I2cConfig3sclE "Permalink to this definition")
        :   IOMUXC configuration for the SCL pin.

        bool `controller`[Â¶](#_CPPv4N10coralmicro9I2cConfig10controllerE "Permalink to this definition")
        :   True if the interface will be the controller; false if target.

        lpi2c\_rtos\_handle\_t `controller_handle`[Â¶](#_CPPv4N10coralmicro9I2cConfig17controller_handleE "Permalink to this definition")
        :   Handle for LPI2C RTOS driver.

        lpi2c\_master\_config\_t `controller_config`[Â¶](#_CPPv4N10coralmicro9I2cConfig17controller_configE "Permalink to this definition")
        :   Configuration for LPI2C in controller mode.

        [lpi2c\_target\_config\_t](#_CPPv4N10coralmicro21lpi2c_target_config_tE "coralmicro::lpi2c_target_config_t") `target_config`[Â¶](#_CPPv4N10coralmicro9I2cConfig13target_configE "Permalink to this definition")
        :   Configuration for LPI2C in target mode.

        [lpi2c\_target\_handle\_t](#_CPPv4N10coralmicro21lpi2c_target_handle_tE "coralmicro::lpi2c_target_handle_t") `target_handle`[Â¶](#_CPPv4N10coralmicro9I2cConfig13target_handleE "Permalink to this definition")
        :   Handle for LPI2C driver in target mode.

        std::function<void([I2cConfig](#_CPPv4N10coralmicro9I2cConfigE "coralmicro::I2cConfig")\*, [lpi2c\_target\_transfer\_t](#_CPPv4N10coralmicro23lpi2c_target_transfer_tE "coralmicro::lpi2c_target_transfer_t")\*)> `target_callback`[Â¶](#_CPPv4N10coralmicro9I2cConfig15target_callbackE "Permalink to this definition")
        :   Callback function for sending data in target mode. Note: this will be called in interrupt context, so this canât take too long to execute.

        uint32\_t `clk_freq`[Â¶](#_CPPv4N10coralmicro9I2cConfig8clk_freqE "Permalink to this definition")
        :   Root clock frequency of the I2C module.

SPI
---

There is one serial peripheral interface (SPI) bus pre-configured on the
right header:

* Pin 5 is chip select
* Pin 6 is clock
* Pin 7 is data out
* Pin 8 is data in

**Example** (from examples/spi/):

```
void Main() {
  printf("SPI Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  constexpr int kTransferBytes = 256;
  std::array<uint8_t, kTransferBytes> tx_data{};
  std::array<uint8_t, kTransferBytes> rx_data{};
  for (int i = 0; i < kTransferBytes; ++i) {
    tx_data[i] = i;
    rx_data[i] = 0;
  }

  SpiConfig config{};
  SpiGetDefaultConfig(&config);
  CHECK(SpiInit(config));
  CHECK(SpiTransfer(config, tx_data.data(), rx_data.data(), kTransferBytes));
  printf("Executing a SPI transaction.\r\n");
  for (int i = 0; i < kTransferBytes; ++i) {
    CHECK(tx_data[i] == rx_data[i]);
  }
  printf("Transaction success!\r\n");
}
```

[[spi.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/spi.h)

*namespace* `coralmicro`
:   Functions

    void `SpiGetDefaultConfig`([SpiConfig](#_CPPv4N10coralmicro9SpiConfigE "coralmicro::SpiConfig") \**config*)[Â¶](#_CPPv4N10coralmicro19SpiGetDefaultConfigEP9SpiConfig "Permalink to this definition")
    :   Gets the default configuration for using SPI on the device header.

        Parameters
        :   **config** â `SpiConfig` to populate with default values.

    bool `SpiInit`([SpiConfig](#_CPPv4N10coralmicro9SpiConfigE "coralmicro::SpiConfig") &*config*)[Â¶](#_CPPv4N10coralmicro7SpiInitER9SpiConfig "Permalink to this definition")
    :   Initializes SPI with a given configuration.

        Parameters
        :   **config** â `SpiConfig` to initialize hardware with.

        Returns
        :   True on success; false otherwise.

    bool `SpiTransfer`([SpiConfig](#_CPPv4N10coralmicro9SpiConfigE "coralmicro::SpiConfig") &*config*, uint8\_t \**tx\_data*, uint8\_t \**rx\_data*, size\_t *size*)[Â¶](#_CPPv4N10coralmicro11SpiTransferER9SpiConfigP7uint8_tP7uint8_t6size_t "Permalink to this definition")
    :   Executes a transfer over SPI with the given configuration.

        Parameters
        :   * **config** â `SpiConfig` to use to execute the transaction.
            * **tx\_data** â Pointer to data that will be sent.
            * **rx\_data** â Pointer to a buffer to contain received data (may be NULL).
            * **size** â Number of bytes to transfer.

        Returns
        :   True on success; false otherwise.

    *struct* `SpiConfig`[Â¶](#_CPPv4N10coralmicro9SpiConfigE "Permalink to this definition")
    :   *#include <spi.h>*

        Configuration for a SPI interface.

        Public Members

        LPSPI\_Type \*`base`[Â¶](#_CPPv4N10coralmicro9SpiConfig4baseE "Permalink to this definition")
        :   Base pointer to the SPI peripheral.

        int `interrupt`[Â¶](#_CPPv4N10coralmicro9SpiConfig9interruptE "Permalink to this definition")
        :   Interrupt number for the peripheral.

        std::array<uint32\_t, 5> `cs`[Â¶](#_CPPv4N10coralmicro9SpiConfig2csE "Permalink to this definition")
        :   IOMUXC configuration for CS pin.

        std::array<uint32\_t, 5> `out`[Â¶](#_CPPv4N10coralmicro9SpiConfig3outE "Permalink to this definition")
        :   IOMUXC configuration for SDO pin.

        std::array<uint32\_t, 5> `in`[Â¶](#_CPPv4N10coralmicro9SpiConfig2inE "Permalink to this definition")
        :   IOMUXC configuration for SDI pin.

        std::array<uint32\_t, 5> `clk`[Â¶](#_CPPv4N10coralmicro9SpiConfig3clkE "Permalink to this definition")
        :   IOMUXC configuration for SCK pin.

        lpspi\_rtos\_handle\_t `handle`[Â¶](#_CPPv4N10coralmicro9SpiConfig6handleE "Permalink to this definition")
        :   Handle for LPSPI RTOS driver.

        lpspi\_master\_config\_t `config`[Â¶](#_CPPv4N10coralmicro9SpiConfig6configE "Permalink to this definition")
        :   Config for LPSPI module.

        uint32\_t `clk_freq`[Â¶](#_CPPv4N10coralmicro9SpiConfig8clk_freqE "Permalink to this definition")
        :   Output clock frequency.

LEDs
----

These APIs allow you to control the LEDs built into the Dev Board Micro,
indicated in [figure 1](#figure1).

To control other LEDs attached to GPIO pins, you
must instead use the [GPIO APIs](#gpio), but beware that a GPIO pin
alone is not strong enough to drive an LED.

Note

The camera LED is not available with this API because itâs intended to give
people awareness that images are being captured by an image sensor for
storage, processing, and/or transmission. We strongly recommend this LED
behavior remain unchanged and always be visible to users.

**Example** (from examples/blink\_led/):

```
[[noreturn]] void Main() {
  printf("Blink LED Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  bool on = true;
  while (true) {
    on = !on;
    LedSet(Led::kUser, on);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
```

[[led.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/base/led.h)

*namespace* `coralmicro`
:   Enums

    *enum class* `Led`[Â¶](#_CPPv4N10coralmicro3LedE "Permalink to this definition")
    :   Available board LEDs.

        *Values:*

        *enumerator* `kStatus`[Â¶](#_CPPv4N10coralmicro3Led7kStatusE "Permalink to this definition")
        :   Orange status LED.

        *enumerator* `kUser`[Â¶](#_CPPv4N10coralmicro3Led5kUserE "Permalink to this definition")
        :   Green user-programmable LED.

        *enumerator* `kTpu`[Â¶](#_CPPv4N10coralmicro3Led4kTpuE "Permalink to this definition")
        :   White Edge TPU LED. The Edge TPU must be powered to use this.

    Functions

    bool `LedSet`([Led](#_CPPv4N10coralmicro3LedE "coralmicro::Led") *led*, bool *enable*)[Â¶](#_CPPv4N10coralmicro6LedSetE3Ledb "Permalink to this definition")
    :   Turns an LED on or off.

        Parameters
        :   * **led** â The LED to enable/disable.
            * **enable** â True turns the LED on, false turns it off.

        Returns
        :   True upon success, false otherwise.

    bool `LedSetBrightness`([Led](#_CPPv4N10coralmicro3LedE "coralmicro::Led") *led*, int *brightness*)[Â¶](#_CPPv4N10coralmicro16LedSetBrightnessE3Ledi "Permalink to this definition")
    :   Turns an LED on or off with brightness setting.

        Parameters
        :   * **led** â The LED to enable/disable.
            * **brightness** â The LED brightness, from 0 to 100.

        Returns
        :   True upon success, false otherwise.

    Variables

    *constexpr* int `kLedFullyOn` = 100[Â¶](#_CPPv4N10coralmicro11kLedFullyOnE "Permalink to this definition")
    :   Fully-on brightness for `Set(led, enable, brightness)`

    *constexpr* int `kLedFullyOff` = 0[Â¶](#_CPPv4N10coralmicro12kLedFullyOffE "Permalink to this definition")
    :   Fully-off brightness for `Set(led, enable, brightness)`