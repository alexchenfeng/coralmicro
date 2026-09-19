Audio APIs
==========

The Dev Board Micro has one on-board PDM microphone from which you can capture
audio using the APIs on this page.

All interactions with the microphone are handled by
[`AudioDriver`](#_CPPv4N10coralmicro11AudioDriverE "coralmicro::AudioDriver") and you can use that class to get direct
memory access to the incoming audio stream. However, we recommend you instead
use [`AudioReader`](#_CPPv4I0EN10coralmicro11AudioReaderE "coralmicro::AudioReader") or [`AudioService`](#_CPPv4N10coralmicro12AudioServiceE "coralmicro::AudioService")
to get audio samples from the microphone. These APIs
provide wrappers around the [`AudioDriver`](#_CPPv4N10coralmicro11AudioDriverE "coralmicro::AudioDriver") to simplify
the code required to properly manage the audio buffer:

* [`AudioReader`](#_CPPv4I0EN10coralmicro11AudioReaderE "coralmicro::AudioReader") provides on-demand audio samples. That is,
  whenever you want to get the latest audio data, call
  [`FillBuffer()`](#_CPPv4N10coralmicro11AudioReader10FillBufferEv "coralmicro::AudioReader::FillBuffer") and then read the samples
  copied to the buffer.
* [`AudioService`](#_CPPv4N10coralmicro12AudioServiceE "coralmicro::AudioService") provides continuous audio samples with
  a callback function. So your task will receive regular callbacks with new
  audio samples whenever the internal buffer fills up.

To use either one, create an instance of [`AudioDriver`](#_CPPv4N10coralmicro11AudioDriverE "coralmicro::AudioDriver")
(the constructor needs to know the buffer size, which you define
with [`AudioDriverBuffers`](#_CPPv4I_6size_t_6size_tEN10coralmicro18AudioDriverBuffersE "coralmicro::AudioDriverBuffers")), and then pass it to the
[`AudioReader`](#_CPPv4I0EN10coralmicro11AudioReaderE "coralmicro::AudioReader") or [`AudioService`](#_CPPv4N10coralmicro12AudioServiceE "coralmicro::AudioService")
constructor. These constructors also need to know some other audio
configurations (such as audio sample rate), which you can specify with
[`AudioDriverConfig`](#_CPPv4N10coralmicro17AudioDriverConfigE "coralmicro::AudioDriverConfig"). Then youâre ready to start
reading audio samples. See below for more details.

Note

These audio APIs are currently not compatible with M4 programs.

Audio reader
------------

The audio reader allows you to read audio samples from the Dev Board Microâs
microphone on-demand, by calling
[`FillBuffer()`](#_CPPv4N10coralmicro11AudioReader10FillBufferEv "coralmicro::AudioReader::FillBuffer") whenever you want to fetch
new audio samples.

This is in contrast to the [audio service](#audio-service), which instead
continuously delivers you new audio samples in a callback function.

template<typename `T`> *class* `coralmicro::``AudioReader`[Â¶](#_CPPv4I0EN10coralmicro11AudioReaderE "Permalink to this definition")
:   Provides a mechanism to read audio samples from the on-board microphone on-demand.

    `AudioReader` manages an internal ring buffer that copies audio samples from the `AudioDriver` you provide to the constructor. You can read samples from the ring buffer at any time by calling `FillBuffer()`. This moves the samples into a regular buffer provided by `Buffer()`, which you can then read for audio processing. Be sure you call `FillBuffer()` fast enough to remove the samples from the ring buffer and make room for new incoming samples. If you donât, the ring buffer will overflow (incrementing `OverflowCount()`) and youâll miss audio data (the ring buffer continues to write so you always get the latest audio).

    The microphone remains powered as long as the `AudioReader` is in scope; it powers off as soon as the `AudioReader` is destroyed.

    For example, this code shows how to set up an `AudioReader` and copy audio samples into the buffer and then read it:

    ```
    namespace {
    AudioDriverBuffers</*NumDmaBuffers=*/4, /*DmaBufferSize=*/6 * 1024>
        g_audio_buffers;
    AudioDriver g_audio_driver(g_audio_buffers);
    }  // namespace

    const AudioDriverConfig config{audio::SampleRate::k16000_Hz,
                                   /*num_dma_buffers=*/4,
                                   /*dma_buffer_size_ms=*/30};
    AudioReader reader(&g_audio_driver, config);

    auto& buffer = reader.Buffer();
    while (true) {
        auto size = reader.FillBuffer();
        ProcessBuffer(buffer.data(), size);
    }
    ```

    For a complete example, see `examples/audio_streaming/`.

    Public Functions

    `AudioReader`([AudioDriver](#_CPPv4N10coralmicro11AudioDriverE "coralmicro::AudioDriver") \**driver*, *const* [AudioDriverConfig](#_CPPv4N10coralmicro17AudioDriverConfigE "coralmicro::AudioDriverConfig") &*config*)[Â¶](#_CPPv4N10coralmicro11AudioReader11AudioReaderEP11AudioDriverRK17AudioDriverConfig "Permalink to this definition")
    :   Constructor.

        Activates the microphone by calling `Enable()` on the given `AudioDriver`. Although the mic is then active, you must call `FillBuffer()` to capture audio into the buffer provided by `Buffer()`.

        Parameters
        :   * **driver** â An audio driver to manage the microphone.
            * **config** â A configuration for audio samples.

    `~AudioReader`()[Â¶](#_CPPv4N10coralmicro11AudioReaderD0Ev "Permalink to this definition")
    :   Destructor. Calls `Disable()` on the `AudioDriver` given to the constructor.

    *inline* *const* std::vector<int32\_t> &`Buffer`() *const*[Â¶](#_CPPv4NK10coralmicro11AudioReader6BufferEv "Permalink to this definition")
    :   Gets the audio buffer thatâs populated with samples when you call `FillBuffer()`.

        Returns
        :   The buffer where audio samples are or will be stored.

    size\_t `FillBuffer`()[Â¶](#_CPPv4N10coralmicro11AudioReader10FillBufferEv "Permalink to this definition")
    :   Fills the audio buffer (provided by `Buffer()`) with audio samples from the microphone.

        This copies audio samples from the internal ring buffer into the buffer provided by `Buffer()` so you can safely process them. This will fetch as many samples as possible, and if you fail to call it fast enough, the internal ring buffer will overflow and increment `OverflowCount()`.

        The samples match the sample rate and size you specify with `AudioDriverConfig` and pass to the `AudioReader` constructor.

        Returns
        :   The number of samples written to the buffer. Youâll need this number so you can read the correct amount from the buffer.

    *inline* int `Drop`(int *min\_count*)[Â¶](#_CPPv4N10coralmicro11AudioReader4DropEi "Permalink to this definition")
    :   Discards microphone samples.

        You should call this before you begin collecting samples in order to avoid audio distortion that may occur when the microphone first starts.

        Parameters
        :   **min\_count** â Minimum number of samples to drop.

        Returns
        :   Number of samples dropped.

    *inline* int `OverflowCount`() *const*[Â¶](#_CPPv4NK10coralmicro11AudioReader13OverflowCountEv "Permalink to this definition")
    :   Gets the number of times that samples from the mic were lost, because you did not read samples fast enough with `FillBuffer()`.

        Returns
        :   The number of times that `FillBuffer()` did not receive the length of samples requested (the ring buffer overflowed and samples were lost).

    *inline* int `UnderflowCount`() *const*[Â¶](#_CPPv4NK10coralmicro11AudioReader14UnderflowCountEv "Permalink to this definition")
    :   Gets the number of times the buffer was not filled when reading from the internal ring buffer.

        Returns
        :   The number of times that `FillBuffer()` was called but the buffer received less than `AudioDriverConfig::dma_buffer_size_samples()`.

Audio service
-------------

The audio service allows you to continuously receive new audio samples from a
separate FreeRTOS task that fetches audio from the Dev Board Microâs
microphone and delivers them to you with one or more callback functions that
you specify with [`AddCallback()`](#_CPPv4N10coralmicro12AudioService11AddCallbackEPv8Callback "coralmicro::AudioService::AddCallback").

You can process the audio samples as your callback receives them or save
copies of the audio samples in an instance of
[`LatestSamples`](#_CPPv4N10coralmicro13LatestSamplesE "coralmicro::LatestSamples") so you can process them later.

This is in contrast to the [audio reader](#audio-reader), which instead
provides audio samples only when you request them.

*class* `coralmicro::``AudioService`[Â¶](#_CPPv4N10coralmicro12AudioServiceE "Permalink to this definition")
:   Provides a mechanism for one or more clients to continuously receive audio samples from the on-board microphone with a callback function.

    This creates a separate FreeRTOS task thatâs dedicated to fetching audio samples from the microphone and passing reference to those audio samples to one or more callbacks that you specify with `AddCallback()`. `AudioService` copies audio samples from the `AudioDriver` stream buffer into its own buffer (actually managed by an internal `AudioReader`) and then sends a reference to this buffer to each callback.

    If you donât want to immediately process the audio samples inside your callback, you can copy the audio samples with `LatestSamples` and then another task outside the callback can read the audio from `LatestSamples`.

    The microphone remains powered as long as there is at least one callback for an `AudioService` client. Otherwise, the microphone is powered off as soon as the `AudioService` is destroyed or all callbacks are removed with `RemoveCallback()`.

    For example, the basic setup for `AudioService` looks like this:

    ```
    namespace {
    AudioDriverBuffers</*NumDmaBuffers=*/4, /*DmaBufferSize=*/6 * 1024>
        g_audio_buffers;
    AudioDriver g_audio_driver(g_audio_buffers);
    }  // namespace

    const AudioDriverConfig config{audio::SampleRate::k16000_Hz,
                                   /*num_dma_buffers=*/4,
                                   /*dma_buffer_size_ms=*/30};
    AudioService service(&g_audio_driver, config);

    auto id = service.AddCallback(...);
    service.RemoveCallback(id);
    ```

    For a complete example, see `examples/yamnet/`.

    Public Types

    *using* `Callback` = bool (\*)(void \*ctx, *const* int32\_t \*samples, size\_t num\_samples)[Â¶](#_CPPv4N10coralmicro12AudioService8CallbackE "Permalink to this definition")
    :   The function type that receives new audio samples as a callback, which must be given to `AddCallback()`.

        Parameters
        :   * **ctx** â Extra parameters, defined with `AddCallback()`.
            * **samples** â A pointer to the buffer.
            * **num\_samples** â The number of audio samples in the buffer.

        Returns
        :   True if the callback should be continued to be called, false otherwise.

    Public Functions

    `AudioService`([AudioDriver](#_CPPv4N10coralmicro11AudioDriverE "coralmicro::AudioDriver") \**driver*, *const* [AudioDriverConfig](#_CPPv4N10coralmicro17AudioDriverConfigE "coralmicro::AudioDriverConfig") &*config*, int *task\_priority*, int *drop\_first\_samples\_ms*)[Â¶](#_CPPv4N10coralmicro12AudioService12AudioServiceEP11AudioDriverRK17AudioDriverConfigii "Permalink to this definition")
    :   Constructor.

        Parameters
        :   * **driver** â An audio driver to manage the microphone.
            * **config** â A configuration for audio samples.
            * **task\_priority** â Priority for internal FreeRTOS task that dispatches audio samples to registered callbacks.
            * **drop\_first\_samples\_ms** â Amount, in milliseconds, of audio to drop at the start of recording.

    int `AddCallback`(void \**ctx*, [Callback](#_CPPv4N10coralmicro12AudioService8CallbackE "coralmicro::AudioService::Callback") *fn*)[Â¶](#_CPPv4N10coralmicro12AudioService11AddCallbackEPv8Callback "Permalink to this definition")
    :   Adds a callback function to receive audio samples.

        You can add as many callbacks as you want. Each one is identified by a unique id, which you must use if you want to remove the callback with `RemoveCallback()`.

        Parameters
        :   * **ctx** â Extra parameters to pass through to the callback function.
            * **fn** â The function to receive audio samples.

        Returns
        :   A unique id for the callback function.

    bool `RemoveCallback`(int *id*)[Â¶](#_CPPv4N10coralmicro12AudioService14RemoveCallbackEi "Permalink to this definition")
    :   Removes a callback function.

        Parameters
        :   **id** â The id of the callback function to remove.

        Returns
        :   True if successfully removed, false otherwise.

    *inline* *const* [AudioDriverConfig](#_CPPv4N10coralmicro17AudioDriverConfigE "coralmicro::AudioDriverConfig") &`Config`() *const*[Â¶](#_CPPv4NK10coralmicro12AudioService6ConfigEv "Permalink to this definition")
    :   Gets the audio driver configuration.

        Returns
        :   The audio driver configuration.

*class* `coralmicro::``LatestSamples`[Â¶](#_CPPv4N10coralmicro13LatestSamplesE "Permalink to this definition")
:   Provides a structure in which you can copy incoming audio samples and read them later. This is designed for use with `AudioService` so that your callback function can continuously receive new audio samples and copy them into a `LatestSamples` object. This allows another task in your program to read the copied samples instead of trying to process the samples as they arrive in the callback.

    Hereâs an example that saves the latest 1000 ms of audio samples from an `AudioService` callback into `LatestSamples`:

    ```
    AudioService* service = ...

    LatestSamples latest(audio::MsToSamples(service->sample_rate(), 1000));
    service->AddCallback(
        &latest, +[](void* ctx, const int32_t* samples, size_t num_samples) {
            static_cast<LatestSamples*>(ctx)->Append(samples, num_samples);
            return true;
        });
    ```

    Then you can directly read the latest `num_samples` saved in `LatestSamples` and apply a function to them by calling `AccessLatestSamples()` (samples received by the function start at `start_index`):

    ```
    latest.AccessLatestSamples([](const std::vector<int32_t>& samples,
                                  size_t start_index) {
        1st: [samples.begin() + start_index, samples.end())
        2nd: [samples.begin(),               samples.begin() + start_index)
    });
    ```

    Or you can get a copy of the latest samples by calling `CopyLatestSamples()`:

    ```
    auto last_second = latest.CopyLatestSamples();
    ```

    For a complete example, see `examples/yamnet/`.

    Public Functions

    *explicit* `LatestSamples`(size\_t *num\_samples*)[Â¶](#_CPPv4N10coralmicro13LatestSamples13LatestSamplesE6size_t "Permalink to this definition")
    :   Constructor.

        Parameters
        :   **num\_samples** â Fixed number of samples that can be saved.

    *inline* size\_t `NumSamples`() *const*[Â¶](#_CPPv4NK10coralmicro13LatestSamples10NumSamplesEv "Permalink to this definition")
    :   Gets the number of samples currently saved.

        Returns
        :   The number of available samples.

    *inline* void `Append`(*const* int32\_t \**samples*, size\_t *num\_samples*)[Â¶](#_CPPv4N10coralmicro13LatestSamples6AppendEPK7int32_t6size_t "Permalink to this definition")
    :   Adds new audio samples to the collection.

        New samples are appended to the collection at the index position where this function left off after the previous append.

        You can read these samples without a copy using â[AccessLatestSamples()](#classcoralmicro_1_1LatestSamples_1a3a6f37f03c13bba53bc2df866e4e5bb1)â. Or get them with a copy using `CopyLatestSamples()`.

        Parameters
        :   * **samples** â A pointer to the buffer position from which you want to begin adding samples.
            * **num\_samples** â The number of audio samples to add from the buffer.

    template<typename `F`> *inline* void `AccessLatestSamples`([F](#_CPPv4I0ENK10coralmicro13LatestSamples19AccessLatestSamplesEv1F "coralmicro::LatestSamples::AccessLatestSamples::F") *f*) *const*[Â¶](#_CPPv4I0ENK10coralmicro13LatestSamples19AccessLatestSamplesEv1F "Permalink to this definition")
    :   Gets the latest samples without a copy and applies a function to them.

        Parameters
        :   **f** â A function to apply to samples. The function receives a reference to the samples as an `int32_t` array and the start index as `size_t`. See the example above, in the `LatestSamples` introduction.

    *inline* std::vector<int32\_t> `CopyLatestSamples`() *const*[Â¶](#_CPPv4NK10coralmicro13LatestSamples17CopyLatestSamplesEv "Permalink to this definition")
    :   Gets a copy of the latest samples.

        This ensures that the samples copied out are actually in chronological order, rather than being a raw copy of the internal array (which can have newer samples at the beginning of the array due to the index position wrapping around after multiple calls to `Append()`).

        Returns
        :   A chronological copy of the latest samples.

Audio driver & configuration
----------------------------

These APIs define the microphone driver and audio configuration to
get audio samples from the Dev Board Microâs microphone.

Although you can receive audio samples directly from AudioDriver, itâs easier
to instead use [audio reader](#audio-reader) or
[audio service](#audio-service).

[[audio\_driver.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/audio/audio_driver.h)

*namespace* `coralmicro`
:   *class* `AudioDriver`[Â¶](#_CPPv4N10coralmicro11AudioDriverE "Permalink to this definition")
    :   *#include <audio\_driver.h>*

        Provides low-level access to the boardâs microphone with audio provided by a callback function. The callback is called from an interrupt service routine (ISR) context and receives audio samples using direct memory access (DMA).

        An instance of this class is required for `AudioReader` and `AudioService`, but you do not need to call `Enable()` and `Disable()` when using those APIs.

        So unless youâre building a custom audio service to manage the `AudioDriver` lifecycle, you only need to instantiate the `AudioDriver` and then pass it to either `AudioReader` or `AudioService`.

        For example usage, see `AudioService`.

        Public Functions

        template<size\_t `NumDmaBuffers`, size\_t `CombinedDmaBufferSize`> *inline* *explicit* `AudioDriver`([AudioDriverBuffers](#_CPPv4I_6size_t_6size_tEN10coralmicro18AudioDriverBuffersE "coralmicro::AudioDriverBuffers")<[NumDmaBuffers](#_CPPv4I_6size_t_6size_tEN10coralmicro11AudioDriver11AudioDriverER18AudioDriverBuffersI13NumDmaBuffers21CombinedDmaBufferSizeE "coralmicro::AudioDriver::AudioDriver::NumDmaBuffers"), [CombinedDmaBufferSize](#_CPPv4I_6size_t_6size_tEN10coralmicro11AudioDriver11AudioDriverER18AudioDriverBuffersI13NumDmaBuffers21CombinedDmaBufferSizeE "coralmicro::AudioDriver::AudioDriver::CombinedDmaBufferSize")> &*buffers*)[Â¶](#_CPPv4I_6size_t_6size_tEN10coralmicro11AudioDriver11AudioDriverER18AudioDriverBuffersI13NumDmaBuffers21CombinedDmaBufferSizeE "Permalink to this definition")
        :   Constructor.

            Parameters
            :   **buffers** â Defines the bufferâs total memory capacity.

        bool `Enable`(*const* [AudioDriverConfig](#_CPPv4N10coralmicro17AudioDriverConfigE "coralmicro::AudioDriverConfig") &*config*, void \**ctx*, [Callback](#_CPPv4N10coralmicro11AudioDriver8CallbackE "coralmicro::AudioDriver::Callback") *fn*)[Â¶](#_CPPv4N10coralmicro11AudioDriver6EnableERK17AudioDriverConfigPv8Callback "Permalink to this definition")
        :   Enables the microphone and specifies a callback to receive audio samples.

            This turns on the microphone and starts audio sampling, but it is called for you when using `AudioReader` or `AudioService`.

            Parameters
            :   * **config** â Driver configuration such as the sample rate and sample size. Used to check if there is space for the specific `AudioDriver`.
                * **ctx** â Extra parameters to pass into the callback.
                * **fn** â Callback that receives the audio samples.

            Returns
            :   True if the microphone successfully starts, false otherwise.

        void `Disable`()[Â¶](#_CPPv4N10coralmicro11AudioDriver7DisableEv "Permalink to this definition")
        :   Stops processing of new audio data and turns off microphone.

        Public Types

        *using* `Callback` = void (\*)(void \*ctx, *const* int32\_t \*dma\_buffer, size\_t dma\_buffer\_size)[Â¶](#_CPPv4N10coralmicro11AudioDriver8CallbackE "Permalink to this definition")
        :   Callback function type to receive audio samples. Called directly by the ISR.

            Parameters
            :   * **ctx** â Extra parameters for the callback function.
                * **dma\_buffer** â A pointer to the buffer.
                * **dma\_buffer\_size** â The number of audio samples in the buffer.

    template<size\_t `NumDmaBuffers`, size\_t `CombinedDmaBufferSize`> *struct* `AudioDriverBuffers`[Â¶](#_CPPv4I_6size_t_6size_tEN10coralmicro18AudioDriverBuffersE "Permalink to this definition")
    :   *#include <audio\_driver.h>*

        Tracks the total space allocated for `AudioDriver`.

        Public Static Attributes

        *static* *constexpr* size\_t `kNumDmaBuffers` = [NumDmaBuffers](#_CPPv4I_6size_t_6size_tEN10coralmicro18AudioDriverBuffersE "coralmicro::AudioDriverBuffers::NumDmaBuffers")[Â¶](#_CPPv4N10coralmicro18AudioDriverBuffers14kNumDmaBuffersE "Permalink to this definition")
        :   Total number of DMA buffers allocated.

        *static* *constexpr* size\_t `kCombinedDmaBufferSize` = [CombinedDmaBufferSize](#_CPPv4I_6size_t_6size_tEN10coralmicro18AudioDriverBuffersE "coralmicro::AudioDriverBuffers::CombinedDmaBufferSize")[Â¶](#_CPPv4N10coralmicro18AudioDriverBuffers22kCombinedDmaBufferSizeE "Permalink to this definition")
        :   Total space of all for DMA buffers allocated.

        Public Static Functions

        *static* *inline* bool `CanHandle`(*const* [AudioDriverConfig](#_CPPv4N10coralmicro17AudioDriverConfigE "coralmicro::AudioDriverConfig") &*config*)[Â¶](#_CPPv4N10coralmicro18AudioDriverBuffers9CanHandleERK17AudioDriverConfig "Permalink to this definition")
        :   Checks if the allocated space can handle a specific `AudioDriverConfig`.

            Parameters
            :   **config** â The config to verify.

            Returns
            :   bool True if we have enough space to allocate the config, false otherwise.

    *struct* `AudioDriverConfig`[Â¶](#_CPPv4N10coralmicro17AudioDriverConfigE "Permalink to this definition")
    :   *#include <audio\_driver.h>*

        Audio driver configuration parameters.

        This is required to instantiate `AudioReader` and `AudioService`.

        Public Members

        [AudioSampleRate](#_CPPv4N10coralmicro15AudioSampleRateE "coralmicro::AudioSampleRate") `sample_rate`[Â¶](#_CPPv4N10coralmicro17AudioDriverConfig11sample_rateE "Permalink to this definition")
        :   Sample rate to be used.

        size\_t `num_dma_buffers`[Â¶](#_CPPv4N10coralmicro17AudioDriverConfig15num_dma_buffersE "Permalink to this definition")
        :   Number of DMA buffers to use.

        size\_t `dma_buffer_size_ms`[Â¶](#_CPPv4N10coralmicro17AudioDriverConfig18dma_buffer_size_msE "Permalink to this definition")
        :   Length in milliseconds of audio data to store in each DMA buffer.

        Public Functions

        *inline* `AudioDriverConfig`([AudioSampleRate](#_CPPv4N10coralmicro15AudioSampleRateE "coralmicro::AudioSampleRate") *sample\_rate*, size\_t *dma\_buffers*, size\_t *dma\_buffer\_ms*)[Â¶](#_CPPv4N10coralmicro17AudioDriverConfig17AudioDriverConfigE15AudioSampleRate6size_t6size_t "Permalink to this definition")
        :   Constructs an [AudioDriverConfig](#structcoralmicro_1_1AudioDriverConfig).

            Parameters
            :   * **sample\_rate** â The sample rate.
                * **dma\_buffer** â The number of dma buffers to use.
                * **dma\_buffer\_ms** â length in milliseconds of audio data to store in each buffer.

        *inline* size\_t `dma_buffer_size_samples`() *const*[Â¶](#_CPPv4NK10coralmicro17AudioDriverConfig23dma_buffer_size_samplesEv "Permalink to this definition")
        :   Gets the DMA buffer size in audio samples according to the specified sample rate for the dma\_buffersize\_ms used in the config.

            Returns
            :   The DMA buffer size in audio samples according to the specified sample rate.

    Functions

    std::optional<[AudioSampleRate](#_CPPv4N10coralmicro15AudioSampleRateE "coralmicro::AudioSampleRate")> `CheckSampleRate`(int *sample\_rate\_hz*)[Â¶](#_CPPv4N10coralmicro15CheckSampleRateEi "Permalink to this definition")
    :   Converts a given sample rate to its corresponding AudioSampleRate.

        Parameters
        :   **sample\_rate\_hz** â An int to convert to an AudioSampleRate.

        Returns
        :   The corresponding sample rate from AudioSampleRate, Otherwise returns a std::nullopt.

    *inline* int `MsToSamples`([AudioSampleRate](#_CPPv4N10coralmicro15AudioSampleRateE "coralmicro::AudioSampleRate") *sample\_rate*, int *ms*)[Â¶](#_CPPv4N10coralmicro11MsToSamplesE15AudioSampleRatei "Permalink to this definition")
    :   Converts time duration in ms to the number of samples according to specified sample rate.

        Parameters
        :   * **sample\_rate** â Conversion rate for 1 second of audio to samples.
            * **ms** â Amount of time in milliseconds to convert.

        Returns
        :   Number of samples by sample\_rate in a timespan of ms milliseconds.

    Enums

    *enum class* `AudioSampleRate` : int32\_t[Â¶](#_CPPv4N10coralmicro15AudioSampleRateE "Permalink to this definition")
    :   Audio sample rates that can be handled by the audio driver.

        *Values:*

        *enumerator* `k16000_Hz`[Â¶](#_CPPv4N10coralmicro15AudioSampleRate9k16000_HzE "Permalink to this definition")
        :   16000 Hz sample rate.

        *enumerator* `k48000_Hz`[Â¶](#_CPPv4N10coralmicro15AudioSampleRate9k48000_HzE "Permalink to this definition")
        :   48000 Hz sample rate.