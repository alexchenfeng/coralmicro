TensorFlow Lite Micro APIs
==========================

The Coral Dev Board Micro allows you to run two types of TensorFlow models:
TensorFlow Lite Micro models that run on entirely the microcontroller (MCU) and
TensorFlow Lite models that are compiled for acceleration on the Coral Edge TPU.
Although you can run TensorFlow Lite Micro models on either MCU core
(M4 or M7), currently, you must execute Edge TPU models from the M7.

Note

If you have experience with TensorFlow Lite on other platforms (including
other Coral boards/accelerators), a lot of the code to run inference on the
Dev Board Micro should be familiar, but the APIs are actually different
for microcontrollers, so your code is not 100% portable.

To run any TensorFlow Lite model on the Dev Board Micro, you must use the
TensorFlow interpreter provided by
[TensorFlow Lite for Microcontrollers](https://www.tensorflow.org/lite/microcontrollers)
(TFLM): [`tflite::MicroInterpreter`](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter").
If youâre running a model on the Edge TPU, the only difference compared to
running a model on the MCU is that you
need to specify the Edge TPU custom op when you instantiate the
[`tflite::MicroInterpreter`](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter")
(and your model must be [compiled for the Edge TPU](/docs/edgetpu/models-intro/)).
The following steps describe the basic procedures to run inference on the
Dev Board Micro using either type of model.

First, you need to perform some setup:

1. If using on the Edge TPU, power on the Edge TPU with
   [`OpenDevice()`](#_CPPv4N10coralmicro14EdgeTpuManager10OpenDeviceE15PerformanceMode "coralmicro::EdgeTpuManager::OpenDevice"):

   ```
   auto tpu_context = EdgeTpuManager::GetSingleton()->OpenDevice();
   if (!tpu_context) {
     printf("ERROR: Failed to get EdgeTpu context\r\n");
   }
   ```
2. Load your `.tflite` model from a file into a byte array
   with [`LfsReadFile()`](../filesystem/#_CPPv4N10coralmicro11LfsReadFileEPKcPNSt6vectorI7uint8_tEE "coralmicro::LfsReadFile"):

   ```
   constexpr char kModelPath[] =
       "/models/tf2_ssd_mobilenet_v2_coco17_ptq_edgetpu.tflite";
   std::vector<uint8_t> model;
   if (!LfsReadFile(kModelPath, &model)) {
     printf("ERROR: Failed to load %s\r\n", kModelPath);
   }
   ```

   **Note:** Some micro ML apps instead load their model from a C array thatâs
   compiled with the app, which is also an option, but thatâs intended for
   microcontrollers without a filesystem (Dev Board Micro has a
   littlefs filesystem).
3. Specify each of the TensorFlow ops required by your model with a
   [`MicroMutableOpResolver`](#_CPPv4I_jEN6tflite22MicroMutableOpResolverE "tflite::MicroMutableOpResolver"). When using a model compiled for
   the Edge TPU, you must include the [`kCustomOp`](#_CPPv4N10coralmicro9kCustomOpE "coralmicro::kCustomOp") with
   [`AddCustom()`](#_CPPv4N6tflite22MicroMutableOpResolver9AddCustomEPKcP18TfLiteRegistration "tflite::MicroMutableOpResolver::AddCustom"). For example:

   ```
   tflite::MicroMutableOpResolver<3> resolver;
   resolver.AddDequantize();
   resolver.AddDetectionPostprocess();
   resolver.AddCustom(kCustomOp, RegisterCustomOp());
   ```
4. Specify the memory arena required for your modelâs input, output, and
   intermediate tensors. To ensure 16-bit alignment (required by TFLM) and
   avoid running out of heap space, you should use either the
   [`STATIC_TENSOR_ARENA_IN_SDRAM`](#c.STATIC_TENSOR_ARENA_IN_SDRAM "STATIC_TENSOR_ARENA_IN_SDRAM") or
   [`STATIC_TENSOR_ARENA_IN_OCRAM`](#c.STATIC_TENSOR_ARENA_IN_OCRAM "STATIC_TENSOR_ARENA_IN_OCRAM") macro to allocate your
   tensor arena:

   ```
   constexpr int kTensorArenaSize = 8 * 1024 * 1024;
   STATIC_TENSOR_ARENA_IN_SDRAM(tensor_arena, kTensorArenaSize);
   ```

   Selecting the best arena size depends on the model and requires some
   trial-and-error: Just start with a small number like 1024 and run it;
   TFLM will throw an error at runtime and tell you the size you actually need.
5. Instantiate a [`MicroInterpreter`](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter"), passing it your model,
   op resolver, tensor arena, and a [`MicroErrorReporter`](#_CPPv4N6tflite18MicroErrorReporterE "tflite::MicroErrorReporter"):

   ```
   tflite::MicroErrorReporter error_reporter;
   tflite::MicroInterpreter interpreter(tflite::GetModel(model.data()),
                                        resolver, tensor_arena,
                                        kTensorArenaSize, &error_reporter);
   ```
6. Allocate all model tensors with
   [`AllocateTensors()`](#_CPPv4N6tflite16MicroInterpreter15AllocateTensorsEv "tflite::MicroInterpreter::AllocateTensors"):

   ```
   if (interpreter.AllocateTensors() != kTfLiteOk) {
     printf("ERROR: AllocateTensors() failed\r\n");
   }
   ```

Now youâre ready to run each inference as follows:

1. Get the allocated input tensor with
   [`input_tensor()`](#_CPPv4N6tflite16MicroInterpreter12input_tensorE6size_t "tflite::MicroInterpreter::input_tensor") and fill it with your
   input data. For example, if youâre
   using the Dev Board Micro camera, you can simply set the input tensor as the
   `buffer` for your [`CameraFrameFormat`](../camera/#_CPPv4N10coralmicro17CameraFrameFormatE "coralmicro::CameraFrameFormat") (see
   `examples/detect_faces/`). Or you can copy
   your input data using [`GetTensorData()`](#_CPPv4I0EN6tflite5micro13GetTensorDataEP1TP16TfLiteEvalTensor "tflite::micro::GetTensorData") and
   `std::memcpy` like this:

   ```
   auto* input_tensor = interpreter.input_tensor(0);
   std::memcpy(tflite::GetTensorData<uint8_t>(input_tensor), image.data(),
               image.size());
   ```
2. Execute the model with [`Invoke()`](#_CPPv4N6tflite16MicroInterpreter6InvokeEv "tflite::MicroInterpreter::Invoke"):

   ```
   if (interpreter.Invoke() != kTfLiteOk) {
     printf("ERROR: Invoke() failed\r\n");
   }
   ```
3. Similar to writing the input tensor, you can then read the output tensor with
   [`GetTensorData()`](#_CPPv4I0EN6tflite5micro13GetTensorDataEP1TP16TfLiteEvalTensor "tflite::micro::GetTensorData") by passing
   it [`output_tensor()`](#_CPPv4N6tflite16MicroInterpreter13output_tensorE6size_t "tflite::MicroInterpreter::output_tensor").

   However, instead of processing this output data yourself, you can use the
   APIs below that correspond to the type of model youâre running. For example,
   if youâre running an object detection model, instead of reading the output
   tensor directly, call [`GetDetectionResults()`](#_CPPv4N10coralmicro10tensorflow19GetDetectionResultsEPKfPKfPKf6size_tf6size_t "coralmicro::tensorflow::GetDetectionResults")
   and pass it your [`MicroInterpreter`](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter"). This function returns
   an [`Object`](#_CPPv4N10coralmicro10tensorflow6ObjectE "coralmicro::tensorflow::Object") for each detected object, which
   specifies the detected objectâs label id, prediction score, and
   bounding-box coordinates:

   ```
   auto results = tensorflow::GetDetectionResults(&interpreter, 0.6, 3);
   printf("%s\r\n", tensorflow::FormatDetectionOutput(results).c_str());
   ```

See the following documentation for more code examples, each of which is
included from the coralmicro examples, which you can browse at
[coralmicro/examples/](https://github.com/google-coral/coralmicro/blob/main/examples).

Also check out the [TensorFlow Lite for Microcontrollers documentation](https://www.tensorflow.org/lite/microcontrollers).

TFLM interpreter
----------------

This is just a small set of APIs from TensorFlow Lite for Microcontrollers
(TFLM) that represent the core APIs you need to run inference on the Dev Board
Micro. You can see the rest of the TFLM APIs in
`coralmicro/third_party/tflite-micro/`.

Note

The version of TFLM included in coralmicro is not continuously updated,
so some APIs might be different from the latest version of [TFLM on GitHub](https://github.com/tensorflow/tflite-micro).

For usage examples, see the following sections, such as for
[image classification](#image-classification).

[[micro\_interpreter.h source]](https://github.com/tensorflow/tflite-micro/tree/24c08505dfd2a97b343220bd1c4006f881061ea6/tensorflow/lite/micro/micro_interpreter.h)

*class* `tflite::``MicroInterpreter`[Â¶](#_CPPv4N6tflite16MicroInterpreterE "Permalink to this definition")
:   Encapsulates a pre-trained model and drives the model inference.

    Note

    This class is not thread-safe. The client is responsible for ensuring serialized interaction to avoid data races and undefined behavior.

    Public Functions

    `MicroInterpreter`(*const* Model \**model*, *const* MicroOpResolver &*op\_resolver*, uint8\_t \**tensor\_arena*, size\_t *tensor\_arena\_size*, ErrorReporter \**error\_reporter*, MicroResourceVariables \**resource\_variables* = nullptr, MicroProfiler \**profiler* = nullptr)[Â¶](#_CPPv4N6tflite16MicroInterpreter16MicroInterpreterEPK5ModelRK15MicroOpResolverP7uint8_t6size_tP13ErrorReporterP22MicroResourceVariablesP13MicroProfiler "Permalink to this definition")
    :   Constructor. Creates an instance with an allocated tensor arena.

        The lifetime of the model, op resolver, tensor arena, error reporter, resource variables, and profiler must be at least as long as that of the interpreter object, since the interpreter may need to access them at any time. This means that you should usually create them with the same scope as each other, for example having them all allocated on the stack as local variables through a top-level function. The interpreter doesnât do any deallocation of any of the pointed-to objects, ownership remains with the caller.

        Parameters
        :   * **model** â A trained TensorFlow Lite model.
            * **op\_resolver** â The op resolver that contains all ops used by the model. This is usually an instance of `tflite::MicroMutableOpResolver`.
            * **tensor\_arena** â The allocated memory for all intermediate tensor data.
            * **tensor\_arena\_size** â The size of `tensor_arena`.
            * **error\_reporter** â Object to use for error reports.
            * **resource\_variables** â Handles assign/read ops for resource variables. See [Resource variable docs](https://github.com/tensorflow/tflite-micro/blob/main/tensorflow/lite/micro/docs/resource_variables.md).
            * **profiler** â Handles profiling for op kernels and TFLM routines. See [Profiling docs](https://github.com/tensorflow/tflite-micro/blob/main/tensorflow/lite/micro/docs/profiling.md).

    `MicroInterpreter`(*const* Model \**model*, *const* MicroOpResolver &*op\_resolver*, MicroAllocator \**allocator*, ErrorReporter \**error\_reporter*, MicroResourceVariables \**resource\_variables* = nullptr, MicroProfiler \**profiler* = nullptr)[Â¶](#_CPPv4N6tflite16MicroInterpreter16MicroInterpreterEPK5ModelRK15MicroOpResolverP14MicroAllocatorP13ErrorReporterP22MicroResourceVariablesP13MicroProfiler "Permalink to this definition")
    :   Constructor. Creates an instance using an existing MicroAllocator instance.

        This constructor should be used when creating an allocator that needs to have allocation handled in more than one interpreter or for recording allocations inside the interpreter. The lifetime of the allocator must be as long as that of the interpreter object.

        Parameters
        :   * **model** â A trained TensorFlow Lite model.
            * **op\_resolver** â The op resolver that contains all ops used by the model. This is usually an instance of `tflite::MicroMutableOpResolver`.
            * **allocator** â The object that allocates all intermediate tensor data.
            * **error\_reporter** â Object to use for error reports.
            * **resource\_variables** â Handles assign/read ops for resource variables. See [Resource variable docs](https://github.com/tensorflow/tflite-micro/blob/main/tensorflow/lite/micro/docs/resource_variables.md).
            * **profiler** â Handles profiling for op kernels and TFLM routines. See [Profiling docs](https://github.com/tensorflow/tflite-micro/blob/main/tensorflow/lite/micro/docs/profiling.md).

    TfLiteStatus `AllocateTensors`()[Â¶](#_CPPv4N6tflite16MicroInterpreter15AllocateTensorsEv "Permalink to this definition")
    :   Allocates all the modelâs necessary input, output and intermediate tensors.

        This will redim dependent tensors using the input tensor dimensionality as given. This is relatively expensive. This must be called after the interpreter has been created and before running inference (and accessing tensor buffers), and must be called again if (and only if) an input tensor is resized.

        Returns
        :   Atatus of success or failure. Will fail if any of the ops in the model (other than those which were rewritten by delegates, if any) are not supported by the Interpreterâs OpResolver.

    TfLiteStatus `Invoke`()[Â¶](#_CPPv4N6tflite16MicroInterpreter6InvokeEv "Permalink to this definition")
    :   Invokes the model to run inference using allocated input tensors.

        In order to support partial graph runs for strided models, this can return values other than kTfLiteOk and kTfLiteError.

    TfLiteStatus `SetMicroExternalContext`(void \**external\_context\_payload*)[Â¶](#_CPPv4N6tflite16MicroInterpreter23SetMicroExternalContextEPv "Permalink to this definition")
    :   This is the recommended API for an application to pass an external payload pointer as an external context to kernels. The life time of the payload pointer should be at least as long as this interpreter. TFLM supports only one external context.

    TfLiteTensor \*`input`(size\_t *index*)[Â¶](#_CPPv4N6tflite16MicroInterpreter5inputE6size_t "Permalink to this definition")
    :   Gets a mutable pointer to an input tensor.

        Parameters
        :   **index** â The index position of the input tensor. Must be between 0 and `inputs_size()`.

        Returns
        :   The input tensor.

    *inline* size\_t `inputs_size`() *const*[Â¶](#_CPPv4NK6tflite16MicroInterpreter11inputs_sizeEv "Permalink to this definition")
    :   Gets the size of the input tensors.

    *inline* *const* flatbuffers::Vector<int32\_t> &`inputs`() *const*[Â¶](#_CPPv4NK6tflite16MicroInterpreter6inputsEv "Permalink to this definition")
    :   Gets a read-only list of all inputs.

    *inline* TfLiteTensor \*`input_tensor`(size\_t *index*)[Â¶](#_CPPv4N6tflite16MicroInterpreter12input_tensorE6size_t "Permalink to this definition")
    :   Same as `input()`.

    template<class `T`> *inline* [T](#_CPPv4I0EN6tflite16MicroInterpreter18typed_input_tensorEP1Ti "tflite::MicroInterpreter::typed_input_tensor::T") \*`typed_input_tensor`(int *tensor\_index*)[Â¶](#_CPPv4I0EN6tflite16MicroInterpreter18typed_input_tensorEP1Ti "Permalink to this definition")
    :   Gets a mutable pointer into the data of a given input tensor.

        The given index must be between 0 and `inputs_size()`.

    TfLiteTensor \*`output`(size\_t *index*)[Â¶](#_CPPv4N6tflite16MicroInterpreter6outputE6size_t "Permalink to this definition")
    :   Gets a mutable pointer to an output tensor.

        Parameters
        :   **index** â The index position of the output tensor. Must be between 0 and `outputs_size()`.

        Returns
        :   The output tensor.

    *inline* size\_t `outputs_size`() *const*[Â¶](#_CPPv4NK6tflite16MicroInterpreter12outputs_sizeEv "Permalink to this definition")
    :   Gets the size of the output tensors.

    *inline* *const* flatbuffers::Vector<int32\_t> &`outputs`() *const*[Â¶](#_CPPv4NK6tflite16MicroInterpreter7outputsEv "Permalink to this definition")
    :   Gets a read-only list of all outputs.

    *inline* TfLiteTensor \*`output_tensor`(size\_t *index*)[Â¶](#_CPPv4N6tflite16MicroInterpreter13output_tensorE6size_t "Permalink to this definition")
    :   Same as `output()`.

    template<class `T`> *inline* [T](#_CPPv4I0EN6tflite16MicroInterpreter19typed_output_tensorEP1Ti "tflite::MicroInterpreter::typed_output_tensor::T") \*`typed_output_tensor`(int *tensor\_index*)[Â¶](#_CPPv4I0EN6tflite16MicroInterpreter19typed_output_tensorEP1Ti "Permalink to this definition")
    :   Gets a mutable pointer into the data of a given output tensor.

        The given index must be between 0 and `outputs_size()`.

    TfLiteStatus `ResetVariableTensors`()[Â¶](#_CPPv4N6tflite16MicroInterpreter20ResetVariableTensorsEv "Permalink to this definition")
    :   Reset all variable tensors to the default value.

    TfLiteStatus `PrepareNodeAndRegistrationDataFromFlatbuffer`()[Â¶](#_CPPv4N6tflite16MicroInterpreter44PrepareNodeAndRegistrationDataFromFlatbufferEv "Permalink to this definition")
    :   Populates node and registration pointers representing the inference graph of the model from values inside the flatbuffer (loaded from the TfLiteModel instance). Persistent data (e.g. operator data) is allocated from the arena.

    *inline* size\_t `arena_used_bytes`() *const*[Â¶](#_CPPv4NK6tflite16MicroInterpreter16arena_used_bytesEv "Permalink to this definition")
    :   For debugging only. Returns the actual used arena in bytes. This method gives the optimal arena size. Itâs only available after `AllocateTensors` has been called. Note that normally `tensor_arena` requires 16 bytes alignment to fully utilize the space. If itâs not the case, the optimial arena size would be [arena\_used\_bytes()](#classtflite_1_1MicroInterpreter_1a783784985875a7dc2be50a22b805c06f) + 16.

[[schema\_generated.h source]](https://github.com/tensorflow/tflite-micro/tree/24c08505dfd2a97b343220bd1c4006f881061ea6/tensorflow/lite/schema/schema_generated.h)

*inline* *const* tflite::Model \*`tflite::``GetModel`(*const* void \**buf*)[Â¶](#_CPPv4N6tflite8GetModelEPKv "Permalink to this definition")
:   Creates a `Model` object with the given model data, which you need for the `tflite::MicroInterpreter` constructor.

    Parameters
    :   **buf** â The model data, either loaded from a C array or from a `.tflite` file.

    Returns
    :   The model object to use with the `tflite::MicroInterpreter` constructor.

[[micro\_mutable\_op\_resolver.h source]](https://github.com/tensorflow/tflite-micro/tree/24c08505dfd2a97b343220bd1c4006f881061ea6/tensorflow/lite/micro/micro_mutable_op_resolver.h)

template<unsigned int `tOpCount`> *class* `tflite::``MicroMutableOpResolver` : *public* tflite::MicroOpResolver[Â¶](#_CPPv4I_jEN6tflite22MicroMutableOpResolverE "Permalink to this definition")
:   Maps ops in the loaded model to executable functions on the device.

    You must use this object to specify each of the ops required by your model (using the various `Add...` functions), and then pass this to the `tflite::MicroInterpreter` constructor.

    Public Functions

    *inline* TfLiteStatus `AddCustom`(*const* char \**name*, TfLiteRegistration \**registration*)[Â¶](#_CPPv4N6tflite22MicroMutableOpResolver9AddCustomEPKcP18TfLiteRegistration "Permalink to this definition")
    :   Registers a Custom Operator with the MicroOpResolver.

        Only the first call for a given name will be successful. That is, if this function is called again for a previously added Custom Operator, the MicroOpResolver will be unchanged and this function will return kTfLiteError.

        Parameters
        :   * **name** â Name of the custom op.
            * **registration** â Handler for the custom op.

        Returns
        :   `kTfLiteOk` if successful; `kTfLiteError` otherwise.

Note

The [`tflite::MicroMutableOpResolver`](#_CPPv4I_jEN6tflite22MicroMutableOpResolverE "tflite::MicroMutableOpResolver") has a long list of `Add...`
functions to specify the ops that you need for your model. To see them
all, refer to the [micro\_mutable\_op\_resolver.h source code](https://github.com/google-coral/coralmicro/blob/main/third_party/tflite-micro/tensorflow/lite/micro/micro_mutable_op_resolver.h).

[[micro\_error\_reporter.h source]](https://github.com/tensorflow/tflite-micro/tree/24c08505dfd2a97b343220bd1c4006f881061ea6/tensorflow/lite/micro/micro_error_reporter.h)

*class* `tflite::``MicroErrorReporter` : *public* ErrorReporter[Â¶](#_CPPv4N6tflite18MicroErrorReporterE "Permalink to this definition")
:   Reports errors for `MicroInterpreter`.

    Public Functions

    *inline* `~MicroErrorReporter`() *override*[Â¶](#_CPPv4N6tflite18MicroErrorReporterD0Ev "Permalink to this definition")

    int `Report`(*const* char \**format*, va\_list *args*) *override*[Â¶](#_CPPv4N6tflite18MicroErrorReporter6ReportEPKc7va_list "Permalink to this definition")

[[kernel\_util.h source]](https://github.com/tensorflow/tflite-micro/tree/24c08505dfd2a97b343220bd1c4006f881061ea6/tensorflow/lite/micro/kernels/kernel_util.h)

template<typename `T`> [T](#_CPPv4I0EN6tflite5micro13GetTensorDataEP1TP16TfLiteEvalTensor "tflite::micro::GetTensorData::T") \*`tflite::micro::``GetTensorData`(TfLiteEvalTensor \**tensor*)[Â¶](#_CPPv4I0EN6tflite5micro13GetTensorDataEP1TP16TfLiteEvalTensor "Permalink to this definition")
:   Gets the mutable data for a specified tensor.

    Parameters
    :   **tensor** â The tensor to read/write.

    Returns
    :   A pointer to the tensor data.

Edge TPU runtime
----------------

Note

The Edge TPU is not available within M4 programs.

These APIs provide access to the Edge TPU on the Dev Board Micro.
Anytime you want to use the Edge TPU for acceleration with
[`MicroInterpreter`](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter"), you need to do two things:

1. Start the Edge TPU with [`OpenDevice()`](#_CPPv4N10coralmicro14EdgeTpuManager10OpenDeviceE15PerformanceMode "coralmicro::EdgeTpuManager::OpenDevice").
2. Register the Edge TPU custom op with your interpreter by passing
   [`kCustomOp`](#_CPPv4N10coralmicro9kCustomOpE "coralmicro::kCustomOp") and
   [`RegisterCustomOp()`](#_CPPv4N10coralmicro16RegisterCustomOpEv "coralmicro::RegisterCustomOp")
   to [`tflite::MicroMutableOpResolver::AddCustom()`](#_CPPv4N6tflite22MicroMutableOpResolver9AddCustomEPKcP18TfLiteRegistration "tflite::MicroMutableOpResolver::AddCustom").

**Example** (from examples/classify\_images\_file/):

```
  auto tpu_context = EdgeTpuManager::GetSingleton()->OpenDevice();
  if (!tpu_context) {
    printf("ERROR: Failed to get EdgeTpu context\r\n");
    return;
  }

  tflite::MicroErrorReporter error_reporter;
  tflite::MicroMutableOpResolver<1> resolver;
  resolver.AddCustom(kCustomOp, RegisterCustomOp());

  tflite::MicroInterpreter interpreter(tflite::GetModel(model.data()), resolver,
                                       tensor_arena, kTensorArenaSize,
                                       &error_reporter);
```

Note

Unlike the libcoral C++ API, when using this coralmicro C++ API, you
do not need to pass the `EdgeTpuContext` to the
`tflite::MicroInterpreter`, but the context must be opened and the custom
op must be registered before you create an interpreter. (This is different
because libcoral is based on TensorFlow Lite and coralmicro is based on
TensorFlow Lite for Microcontrollers.)

[[edgetpu\_manager.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tpu/edgetpu_manager.h)

*namespace* `coralmicro`
:   *class* `EdgeTpuContext`[Â¶](#_CPPv4N10coralmicro14EdgeTpuContextE "Permalink to this definition")
    :   *#include <edgetpu\_manager.h>*

        This class is a representation of the Edge TPU device, so there is one shared `EdgeTpuContext` used by all model interpreters.

        Instances of this should be allocated with `EdgeTpuManager::OpenDevice()`.

        The `EdgeTpuContext` can be shared among multiple software components, and the life of this object is directly tied to the Edge TPU power, so the Edge TPU powers down after the last `EdgeTpuContext` reference leaves scope.

        The lifetime of the `EdgeTpuContext` must be longer than all associated `tflite::MicroInterpreter` instances.

    *class* `EdgeTpuManager`[Â¶](#_CPPv4N10coralmicro14EdgeTpuManagerE "Permalink to this definition")
    :   *#include <edgetpu\_manager.h>*

        Singleton Edge TPU manager for allocating new instances of `EdgeTpuContext`.

        Public Functions

        std::shared\_ptr<[EdgeTpuContext](#_CPPv4N10coralmicro14EdgeTpuContextE "coralmicro::EdgeTpuContext")> `OpenDevice`(PerformanceMode *mode* = PerformanceMode::kHigh)[Â¶](#_CPPv4N10coralmicro14EdgeTpuManager10OpenDeviceE15PerformanceMode "Permalink to this definition")
        :   Gets the default Edge TPU device (and starts it if necessary).

            The Edge TPU device (represented by `EdgeTpuContext`) can be shared among multiple software components, and the `EdgeTpuManager` is a singleton object, so you should always call this function like this:

            ```
            auto tpu_context = EdgeTpuManager::GetSingleton()->OpenDevice();
            ```

            Parameters
            :   **mode** â The `PerformanceMode` to use for the Edge TPU. Options are: `kMax` (500Mhz), `kHigh` (250Mhz), `kMedium` (125Mhz), or `kLow` (63Mhz). If omitted, the default is `kHigh`. **Caution**: If you set the performance mode to `kMax`, it can increase the Edge TPU inferencing speed, but it can also make the Edge TPU module hotter, which might cause burns if touched.

            Returns
            :   A shared pointer to Edge TPU device. The shared\_ptr can point to nullptr in case of error.

        std::optional<float> `GetTemperature`()[Â¶](#_CPPv4N10coralmicro14EdgeTpuManager14GetTemperatureEv "Permalink to this definition")
        :   Gets the current Edge TPU junction temperature.

            Returns
            :   The temperature in Celcius, or `std::nullopt` if `EdgeTpuContext` is empty.

        Public Static Functions

        *static* *inline* [EdgeTpuManager](#_CPPv4N10coralmicro14EdgeTpuManagerE "coralmicro::EdgeTpuManager") \*`GetSingleton`()[Â¶](#_CPPv4N10coralmicro14EdgeTpuManager12GetSingletonEv "Permalink to this definition")
        :   Gets a pointer to the `EdgeTpuManager` singleton object.

[[edgetpu\_op.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tpu/edgetpu_op.h)

*namespace* `coralmicro`
:   Functions

    TfLiteRegistration \*`RegisterCustomOp`()[Â¶](#_CPPv4N10coralmicro16RegisterCustomOpEv "Permalink to this definition")
    :   Returns pointer to an instance of `tflite::TfLiteRegistration` to handle Edge TPU custom ops. Pass this to `tflite::MicroMutableOpResolver::AddCustom()`.

    Variables

    *constexpr* char `kCustomOp`[] = "edgetpu-custom-op"[Â¶](#_CPPv4N10coralmicro9kCustomOpE "Permalink to this definition")
    :   Edge TPU custom op. Pass this to `tflite::MicroMutableOpResolver::AddCustom()`.

Image classification
--------------------

These APIs simplify the pre- and post-processing for image classification models.

**Example** (from examples/classify\_images\_file/):

```
namespace coralmicro {
namespace {
constexpr char kModelPath[] =
    "/models/mobilenet_v1_1.0_224_quant_edgetpu.tflite";
constexpr char kImagePath[] = "/examples/classify_images_file/cat_224x224.rgb";
constexpr int kTensorArenaSize = 1024 * 1024;
STATIC_TENSOR_ARENA_IN_SDRAM(tensor_arena, kTensorArenaSize);

void Main() {
  printf("Classify Image Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  std::vector<uint8_t> model;
  if (!LfsReadFile(kModelPath, &model)) {
    printf("ERROR: Failed to load %s\r\n", kModelPath);
    return;
  }

  // [start-sphinx-snippet:edgetpu]
  auto tpu_context = EdgeTpuManager::GetSingleton()->OpenDevice();
  if (!tpu_context) {
    printf("ERROR: Failed to get EdgeTpu context\r\n");
    return;
  }

  tflite::MicroErrorReporter error_reporter;
  tflite::MicroMutableOpResolver<1> resolver;
  resolver.AddCustom(kCustomOp, RegisterCustomOp());

  tflite::MicroInterpreter interpreter(tflite::GetModel(model.data()), resolver,
                                       tensor_arena, kTensorArenaSize,
                                       &error_reporter);
  // [end-sphinx-snippet:edgetpu]
  if (interpreter.AllocateTensors() != kTfLiteOk) {
    printf("ERROR: AllocateTensors() failed\r\n");
    return;
  }

  if (interpreter.inputs().size() != 1) {
    printf("ERROR: Model must have only one input tensor\r\n");
    return;
  }

  auto* input_tensor = interpreter.input_tensor(0);
  if (!LfsReadFile(kImagePath, tflite::GetTensorData<uint8_t>(input_tensor),
                   input_tensor->bytes)) {
    printf("ERROR: Failed to load %s\r\n", kImagePath);
    return;
  }

  if (interpreter.Invoke() != kTfLiteOk) {
    printf("ERROR: Invoke() failed\r\n");
    return;
  }

  auto results = tensorflow::GetClassificationResults(&interpreter, 0.0f, 3);
  for (auto& result : results)
    printf("Label ID: %d Score: %f\r\n", result.id, result.score);
}
}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[classification.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tensorflow/classification.h)

*namespace* `coralmicro`
:   *namespace* `tensorflow`[Â¶](#_CPPv4N10coralmicro10tensorflowE "Permalink to this definition")
    :   *struct* `Class`[Â¶](#_CPPv4N10coralmicro10tensorflow5ClassE "Permalink to this definition")
        :   *#include <classification.h>*

            Represents a classification result.

            Public Members

            int `id`[Â¶](#_CPPv4N10coralmicro10tensorflow5Class2idE "Permalink to this definition")
            :   The class label id.

            float `score`[Â¶](#_CPPv4N10coralmicro10tensorflow5Class5scoreE "Permalink to this definition")
            :   The prediction score.

        Functions

        std::string `FormatClassificationOutput`(*const* std::vector<[tensorflow](#_CPPv4N10coralmicro10tensorflowE "coralmicro::tensorflow")::[Class](#_CPPv4N10coralmicro10tensorflow5ClassE "coralmicro::tensorflow::Class")> &*classes*)[Â¶](#_CPPv4N10coralmicro10tensorflow26FormatClassificationOutputERKNSt6vectorIN10tensorflow5ClassEEE "Permalink to this definition")
        :   Format the Classification outputs into a string.

            Parameters
            :   **classes** â All the classification class predictions, as returned by `GetClassificationResults()`.

            Returns
            :   a string with all predictions in a line-delimited list with ids and scores for each classification.

        std::vector<[Class](#_CPPv4N10coralmicro10tensorflow5ClassE "coralmicro::tensorflow::Class")> `GetClassificationResults`(*const* float \**scores*, ssize\_t *scores\_count*, float *threshold* = -std::numeric\_limits<float>::infinity(), size\_t *top\_k* = std::numeric\_limits<size\_t>::max())[Â¶](#_CPPv4N10coralmicro10tensorflow24GetClassificationResultsEPKf7ssize_tf6size_t "Permalink to this definition")
        :   Converts a classification output tensor into a list of ordered classes.

            Parameters
            :   * **scores** â The dequantized output tensor.
                * **scores\_count** â The number of scores in the output (the size of the output tensor).
                * **threshold** â The score threshold for results. All returned results have a score greater-than-or-equal-to this value.
                * **top\_k** â The maximum number of predictions to return.

            Returns
            :   The top\_k [Class](#structcoralmicro_1_1tensorflow_1_1Class) predictions (id, score), ordered by score (first element has the highest score).

        std::vector<[Class](#_CPPv4N10coralmicro10tensorflow5ClassE "coralmicro::tensorflow::Class")> `GetClassificationResults`(tflite::[MicroInterpreter](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter") \**interpreter*, float *threshold* = -std::numeric\_limits<float>::infinity(), size\_t *top\_k* = std::numeric\_limits<size\_t>::max())[Â¶](#_CPPv4N10coralmicro10tensorflow24GetClassificationResultsEPN6tflite16MicroInterpreterEf6size_t "Permalink to this definition")
        :   Gets results from a classification model as a list of ordered classes.

            Parameters
            :   * **interpreter** â The already-invoked interpreter for your classification model.
                * **threshold** â The score threshold for results. All returned results have a score greater-than-or-equal-to this value.
                * **top\_k** â The maximum number of predictions to return.

            Returns
            :   The top\_k [Class](#structcoralmicro_1_1tensorflow_1_1Class) predictions (id, score), ordered by score (first element has the highest score).

        bool `ClassificationInputNeedsPreprocessing`(*const* TfLiteTensor &*input\_tensor*)[Â¶](#_CPPv4N10coralmicro10tensorflow37ClassificationInputNeedsPreprocessingERK12TfLiteTensor "Permalink to this definition")
        :   Checks whether an input tensor needs pre-processing for classification.

            Parameters
            :   **intput\_tensor** â The tensor intended as input for a classification model.

            Returns
            :   True if the input tensor requires normalization AND quantization (you should run [ClassificationPreprocess()](#namespacecoralmicro_1_1tensorflow_1a307d48bfcd921cee4454619218d181bc)); false otherwise.

        bool `ClassificationPreprocess`(TfLiteTensor \**input\_tensor*)[Â¶](#_CPPv4N10coralmicro10tensorflow24ClassificationPreprocessEP12TfLiteTensor "Permalink to this definition")
        :   Performs normalization and quantization pre-processing on the given tensor.

            Parameters
            :   **input\_tensor** â The tensor you want to pre-process for a clasification model.

            Returns
            :   True upon success; false if the tensor type is the wrong format.

Object detection
----------------

These APIs simplify the post-processing for object detection models.

**Example** (from examples/detect\_objects\_file/):

```
namespace coralmicro {
namespace {
constexpr char kModelPath[] =
    "/models/tf2_ssd_mobilenet_v2_coco17_ptq_edgetpu.tflite";
constexpr char kImagePath[] = "/examples/detect_objects_file/cat_300x300.rgb";
constexpr int kTensorArenaSize = 8 * 1024 * 1024;
STATIC_TENSOR_ARENA_IN_SDRAM(tensor_arena, kTensorArenaSize);

void Main() {
  printf("Detect Image Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  std::vector<uint8_t> model;
  if (!LfsReadFile(kModelPath, &model)) {
    printf("ERROR: Failed to load %s\r\n", kModelPath);
    return;
  }

  auto tpu_context = EdgeTpuManager::GetSingleton()->OpenDevice();
  if (!tpu_context) {
    printf("ERROR: Failed to get EdgeTpu context\r\n");
    return;
  }

  tflite::MicroErrorReporter error_reporter;
  tflite::MicroMutableOpResolver<3> resolver;
  resolver.AddDequantize();
  resolver.AddDetectionPostprocess();
  resolver.AddCustom(kCustomOp, RegisterCustomOp());

  tflite::MicroInterpreter interpreter(tflite::GetModel(model.data()), resolver,
                                       tensor_arena, kTensorArenaSize,
                                       &error_reporter);
  if (interpreter.AllocateTensors() != kTfLiteOk) {
    printf("ERROR: AllocateTensors() failed\r\n");
    return;
  }

  if (interpreter.inputs().size() != 1) {
    printf("ERROR: Model must have only one input tensor\r\n");
    return;
  }

  auto* input_tensor = interpreter.input_tensor(0);
  if (!LfsReadFile(kImagePath, tflite::GetTensorData<uint8_t>(input_tensor),
                   input_tensor->bytes)) {
    printf("ERROR: Failed to load %s\r\n", kImagePath);
    return;
  }

  if (interpreter.Invoke() != kTfLiteOk) {
    printf("ERROR: Invoke() failed\r\n");
    return;
  }

  auto results = tensorflow::GetDetectionResults(&interpreter, 0.6, 3);
  printf("%s\r\n", tensorflow::FormatDetectionOutput(results).c_str());
}
}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[detection.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tensorflow/detection.h)

*namespace* `coralmicro`
:   *namespace* `tensorflow`
    :   template<typename `T`> *struct* `BBox`[Â¶](#_CPPv4I0EN10coralmicro10tensorflow4BBoxE "Permalink to this definition")
        :   *#include <detection.h>*

            Represents the bounding box of a detected object.

            Public Members

            [T](#_CPPv4I0EN10coralmicro10tensorflow4BBoxE "coralmicro::tensorflow::BBox::T") `ymin`[Â¶](#_CPPv4N10coralmicro10tensorflow4BBox4yminE "Permalink to this definition")
            :   The box y-minimum (top-most) point.

            [T](#_CPPv4I0EN10coralmicro10tensorflow4BBoxE "coralmicro::tensorflow::BBox::T") `xmin`[Â¶](#_CPPv4N10coralmicro10tensorflow4BBox4xminE "Permalink to this definition")
            :   The box x-minimum (left-most) point.

            [T](#_CPPv4I0EN10coralmicro10tensorflow4BBoxE "coralmicro::tensorflow::BBox::T") `ymax`[Â¶](#_CPPv4N10coralmicro10tensorflow4BBox4ymaxE "Permalink to this definition")
            :   The box y-maximum (bottom-most) point.

            [T](#_CPPv4I0EN10coralmicro10tensorflow4BBoxE "coralmicro::tensorflow::BBox::T") `xmax`[Â¶](#_CPPv4N10coralmicro10tensorflow4BBox4xmaxE "Permalink to this definition")
            :   The box x-maximum (right-most) point.

        *struct* `Object`[Â¶](#_CPPv4N10coralmicro10tensorflow6ObjectE "Permalink to this definition")
        :   *#include <detection.h>*

            Represents a detected object.

            Public Members

            int `id`[Â¶](#_CPPv4N10coralmicro10tensorflow6Object2idE "Permalink to this definition")
            :   The class label id.

            float `score`[Â¶](#_CPPv4N10coralmicro10tensorflow6Object5scoreE "Permalink to this definition")
            :   The prediction score.

            [BBox](#_CPPv4I0EN10coralmicro10tensorflow4BBoxE "coralmicro::tensorflow::BBox")<float> `bbox`[Â¶](#_CPPv4N10coralmicro10tensorflow6Object4bboxE "Permalink to this definition")
            :   The bounding-box (ymin,xmin,ymax,xmax).

        Functions

        std::string `FormatDetectionOutput`(*const* std::vector<[Object](#_CPPv4N10coralmicro10tensorflow6ObjectE "coralmicro::tensorflow::Object")> &*objects*)[Â¶](#_CPPv4N10coralmicro10tensorflow21FormatDetectionOutputERKNSt6vectorI6ObjectEE "Permalink to this definition")
        :   Formats the detection outputs into a string.

            Parameters
            :   **object** â A vector with all the objects in an object detection output.

            Returns
            :   A description of all detected objects.

        std::vector<[Object](#_CPPv4N10coralmicro10tensorflow6ObjectE "coralmicro::tensorflow::Object")> `GetDetectionResults`(*const* float \**bboxes*, *const* float \**ids*, *const* float \**scores*, size\_t *count*, float *threshold* = -std::numeric\_limits<float>::infinity(), size\_t *top\_k* = std::numeric\_limits<size\_t>::max())[Â¶](#_CPPv4N10coralmicro10tensorflow19GetDetectionResultsEPKfPKfPKf6size_tf6size_t "Permalink to this definition")
        :   Converts detection output tensors into a vector of Objects.

            Parameters
            :   * **bboxes** â The output tensor for all detected bounding boxes in box-corner encoding, for example: (ymin1,xmin1,ymax1,xmax1,ymin2,xmin2,â¦).
                * **ids** â The output tensor for all label IDs.
                * **scores** â The output tensor for all scores.
                * **count** â The number of detected objects (all tensors defined above have valid data for this number of objects).
                * **threshold** â The score threshold for results. All returned results have a score greater-than-or-equal-to this value.
                * **top\_k** â The maximum number of predictions to return.

            Returns
            :   The top\_k object predictions (id, score, [BBox](#structcoralmicro_1_1tensorflow_1_1BBox)), ordered by score (first element has the highest score).

        std::vector<[Object](#_CPPv4N10coralmicro10tensorflow6ObjectE "coralmicro::tensorflow::Object")> `GetDetectionResults`(tflite::[MicroInterpreter](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter") \**interpreter*, float *threshold* = -std::numeric\_limits<float>::infinity(), size\_t *top\_k* = std::numeric\_limits<size\_t>::max())[Â¶](#_CPPv4N10coralmicro10tensorflow19GetDetectionResultsEPN6tflite16MicroInterpreterEf6size_t "Permalink to this definition")
        :   Gets results from a detection model as a vector of Objects.

            Parameters
            :   * **interpreter** â The already-invoked interpreter for your detection model.
                * **threshold** â The score threshold for results. All returned results have a score greater-than-or-equal-to this value.
                * **top\_k** â The maximum number of predictions to return.

            Returns
            :   The top\_k object predictions (id, score, [BBox](#structcoralmicro_1_1tensorflow_1_1BBox)), ordered by score (first element has the highest score).

Pose estimation
---------------

These APIs not only simplify the post-processing for pose estimation with
PoseNet, but also optimize execution of the post-processing layers on the MCU
with a custom op (because the post-processing ops are not compatible with the
Edge TPU).

So when running PoseNet, in addition to specifying the
[`kCustomOp`](#_CPPv4N10coralmicro9kCustomOpE "coralmicro::kCustomOp") for the
Edge TPU, you should also register the [`kPosenetDecoderOp`](#_CPPv4N10coralmicro17kPosenetDecoderOpE "coralmicro::kPosenetDecoderOp")
provided here.

**Example** (from examples/detect\_poses/):

```
namespace coralmicro {
namespace {

constexpr int kModelArenaSize = 1 * 1024 * 1024;
constexpr int kExtraArenaSize = 1 * 1024 * 1024;
constexpr int kTensorArenaSize = kModelArenaSize + kExtraArenaSize;
STATIC_TENSOR_ARENA_IN_SDRAM(tensor_arena, kTensorArenaSize);
constexpr char kModelPath[] =
    "/models/posenet_mobilenet_v1_075_324_324_16_quant_decoder_edgetpu.tflite";
constexpr char kTestInputPath[] = "/models/posenet_test_input_324.bin";

void Main() {
  printf("Posenet Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  tflite::MicroErrorReporter error_reporter;
  TF_LITE_REPORT_ERROR(&error_reporter, "Posenet!");
  // Turn on the TPU and get it's context.
  auto tpu_context =
      EdgeTpuManager::GetSingleton()->OpenDevice(PerformanceMode::kMax);
  if (!tpu_context) {
    printf("ERROR: Failed to get EdgeTpu context\r\n");
    vTaskSuspend(nullptr);
  }
  // Reads the model and checks version.
  std::vector<uint8_t> posenet_tflite;
  if (!LfsReadFile(kModelPath, &posenet_tflite)) {
    TF_LITE_REPORT_ERROR(&error_reporter, "Failed to load model!");
    vTaskSuspend(nullptr);
  }
  auto* model = tflite::GetModel(posenet_tflite.data());
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(&error_reporter,
                         "Model schema version is %d, supported is %d",
                         model->version(), TFLITE_SCHEMA_VERSION);
    vTaskSuspend(nullptr);
  }
  // Creates a micro interpreter.
  tflite::MicroMutableOpResolver<2> resolver;
  resolver.AddCustom(kCustomOp, RegisterCustomOp());
  resolver.AddCustom(kPosenetDecoderOp, RegisterPosenetDecoderOp());
  auto interpreter = tflite::MicroInterpreter{
      model, resolver, tensor_arena, kTensorArenaSize, &error_reporter};
  if (interpreter.AllocateTensors() != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(&error_reporter, "AllocateTensors failed.");
    vTaskSuspend(nullptr);
  }
  auto* posenet_input = interpreter.input(0);
  // Runs posenet on a test image.
  printf("Getting outputs for posenet test input\r\n");
  std::vector<uint8_t> posenet_test_input_bin;
  if (!LfsReadFile(kTestInputPath, &posenet_test_input_bin)) {
    TF_LITE_REPORT_ERROR(&error_reporter, "Failed to load test input!");
    vTaskSuspend(nullptr);
  }
  if (posenet_input->bytes != posenet_test_input_bin.size()) {
    TF_LITE_REPORT_ERROR(&error_reporter,
                         "Input tensor length doesn't match canned input\r\n");
    vTaskSuspend(nullptr);
  }
  memcpy(tflite::GetTensorData<uint8_t>(posenet_input),
         posenet_test_input_bin.data(), posenet_test_input_bin.size());
  if (interpreter.Invoke() != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(&error_reporter, "Invoke failed.");
    vTaskSuspend(nullptr);
  }
  auto test_image_output =
      tensorflow::GetPosenetOutput(&interpreter, /*threshold=*/0.5);
  printf("%s\r\n", tensorflow::FormatPosenetOutput(test_image_output).c_str());
  // Starts the camera for live poses.
  CameraTask::GetSingleton()->SetPower(true);
  CameraTask::GetSingleton()->Enable(CameraMode::kStreaming);
  printf("Starting live posenet\r\n");
  auto model_height = posenet_input->dims->data[1];
  auto model_width = posenet_input->dims->data[2];
  for (;;) {
    CameraFrameFormat fmt{
        /*fmt=*/CameraFormat::kRgb,
        /*filter=*/CameraFilterMethod::kBilinear,
        /*rotation=*/CameraRotation::k270,
        /*width=*/model_width,
        /*height=*/model_height,
        /*preserve_ratio=*/false,
        /*buffer=*/tflite::GetTensorData<uint8_t>(posenet_input)};
    if (!CameraTask::GetSingleton()->GetFrame({fmt})) {
      TF_LITE_REPORT_ERROR(&error_reporter, "Failed to get image from camera.");
      break;
    }
    if (interpreter.Invoke() != kTfLiteOk) {
      TF_LITE_REPORT_ERROR(&error_reporter, "Invoke failed.");
      break;
    }
    auto output = tensorflow::GetPosenetOutput(&interpreter,
                                               /*threshold=*/0.5);
    printf("%s\r\n", tensorflow::FormatPosenetOutput(output).c_str());
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  CameraTask::GetSingleton()->SetPower(false);
}

}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[posenet\_decoder\_op.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tensorflow/posenet_decoder_op.h)

*namespace* `coralmicro`
:   Functions

    TfLiteRegistration \*`RegisterPosenetDecoderOp`()[Â¶](#_CPPv4N10coralmicro24RegisterPosenetDecoderOpEv "Permalink to this definition")
    :   Returns pointer to an instance of `tflite::TfLiteRegistration` to handle the custom op for post-processing PoseNet output tensors on the MCU. Pass this to `tflite::MicroMutableOpResolver::AddCustom()`.

    Variables

    *constexpr* char `kPosenetDecoderOp`[] = "PosenetDecoderOp"[Â¶](#_CPPv4N10coralmicro17kPosenetDecoderOpE "Permalink to this definition")
    :   PoseNet custom op name. Pass this to `tflite::MicroMutableOpResolver::AddCustom()`.

[[posenet.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tensorflow/posenet.h)

*namespace* `coralmicro`
:   *namespace* `tensorflow`
    :   Functions

        std::string `FormatPosenetOutput`(*const* std::vector<[Pose](#_CPPv4N10coralmicro10tensorflow4PoseE "coralmicro::tensorflow::Pose")> &*poses*)[Â¶](#_CPPv4N10coralmicro10tensorflow19FormatPosenetOutputERKNSt6vectorI4PoseEE "Permalink to this definition")
        :   Formats all the PoseNet output into a string.

            Parameters
            :   **poses** â A vector contains all the poses in a posenet output.

            Returns
            :   A string showing the posenetâs output.

        std::vector<[Pose](#_CPPv4N10coralmicro10tensorflow4PoseE "coralmicro::tensorflow::Pose")> `GetPosenetOutput`(tflite::[MicroInterpreter](#_CPPv4N6tflite16MicroInterpreterE "tflite::MicroInterpreter") \**interpreter*, float *threshold* = -std::numeric\_limits<float>::infinity())[Â¶](#_CPPv4N10coralmicro10tensorflow16GetPosenetOutputEPN6tflite16MicroInterpreterEf "Permalink to this definition")
        :   Gets the results from a PoseNet model in the form of a vector of poses.

            After you invoke the interpreter, pass it to this function to get structured pose results.

            Parameters
            :   * **interpreter** â The already-invoked interpreter for your PoseNet model.
                * **threshold** â The overall pose score threshold for results.

            Returns
            :   All detected poses with an overall score greater-than-or-equal-to the threshold.

        Variables

        *constexpr* int `kKeypoints` = 17[Â¶](#_CPPv4N10coralmicro10tensorflow10kKeypointsE "Permalink to this definition")
        :   Number of keypoints in each pose.

        *const* char \**const* `KeypointTypes`[] = {"NOSE", "LEFT\_EYE", "RIGHT\_EYE", "LEFT\_EAR", "RIGHT\_EAR", "LEFT\_SHOULDER", "RIGHT\_SHOULDER", "LEFT\_ELBOW", "RIGHT\_ELBOW", "LEFT\_WRIST", "RIGHT\_WRIST", "LEFT\_HIP", "RIGHT\_HIP", "LEFT\_KNEE", "RIGHT\_KNEE", "LEFT\_ANKLE", "RIGHT\_ANKLE",}[Â¶](#_CPPv4N10coralmicro10tensorflow13KeypointTypesE "Permalink to this definition")
        :   A map of keypoint index to the keypoint name.

        *struct* `Keypoint`[Â¶](#_CPPv4N10coralmicro10tensorflow8KeypointE "Permalink to this definition")
        :   *#include <posenet.h>*

            The location and score of a pose keypoint.

            Public Members

            float `x`[Â¶](#_CPPv4N10coralmicro10tensorflow8Keypoint1xE "Permalink to this definition")
            :   The keypointâs x position, relative to the image size (0 to 1.0).

            float `y`[Â¶](#_CPPv4N10coralmicro10tensorflow8Keypoint1yE "Permalink to this definition")
            :   The keypointâs y position, relative to the image size (0 to 1.0).

            float `score`[Â¶](#_CPPv4N10coralmicro10tensorflow8Keypoint5scoreE "Permalink to this definition")
            :   The keypointâs prediction score (0 to 1.0).

        *struct* `Pose`[Â¶](#_CPPv4N10coralmicro10tensorflow4PoseE "Permalink to this definition")
        :   *#include <posenet.h>*

            Represents an individual pose.

            Public Members

            float `score`[Â¶](#_CPPv4N10coralmicro10tensorflow4Pose5scoreE "Permalink to this definition")
            :   The poseâs overall prediction score.

            [Keypoint](#_CPPv4N10coralmicro10tensorflow8KeypointE "coralmicro::tensorflow::Keypoint") `keypoints`[`kKeypoints`][Â¶](#_CPPv4N10coralmicro10tensorflow4Pose9keypointsE "Permalink to this definition")
            :   An array of keypoints in this pose.

Audio Classification
--------------------

The following APIs assist with running audio classification models on the
Dev Board Micro, either on CPU or Edge TPU. For supported models, see the
[audio classification models](/models/audio-classification/).

For an example, see `examples/classify_speech/`.

[[audio\_models.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tensorflow/audio_models.h)

*namespace* `coralmicro`
:   *namespace* `tensorflow`
    :   Enums

        *enum* `AudioModel`[Â¶](#_CPPv4N10coralmicro10tensorflow10AudioModelE "Permalink to this definition")
        :   Supported models.

            *Values:*

            *enumerator* `kYAMNet`[Â¶](#_CPPv4N10coralmicro10tensorflow10AudioModel7kYAMNetE "Permalink to this definition")
            :   YamNet without the frontend.

            *enumerator* `kKeywordDetector`[Â¶](#_CPPv4N10coralmicro10tensorflow10AudioModel16kKeywordDetectorE "Permalink to this definition")
            :   Keyword detector (or âKeyword Spotterâ).

        Functions

        template<bool `tForTpu`> auto `SetupYamNetResolver`()[Â¶](#_CPPv4I_bEN10coralmicro10tensorflow19SetupYamNetResolverEDav "Permalink to this definition")
        :   Sets up the MicroMutableOpResolver with ops required for YamNet.

            Template Parameters
            :   **tForTpu** â If true the Resolver will be setup for TPU else CPU.

            Returns
            :   A [tflite::MicroMutableOpResolver](#classtflite_1_1MicroMutableOpResolver) that is prepared for the YamNet model.

        bool `PrepareAudioFrontEnd`(FrontendState \**frontend\_state*, [AudioModel](#_CPPv4N10coralmicro10tensorflow10AudioModelE "coralmicro::tensorflow::AudioModel") *model\_type*)[Â¶](#_CPPv4N10coralmicro10tensorflow20PrepareAudioFrontEndEP13FrontendState10AudioModel "Permalink to this definition")
        :   Prepares the input preprocess engine for TensorFlow to converts raw audio data to spectrogram. This function must be called before `PreprocessAudioInput()` is called.

            Parameters
            :   * **frontend\_state** â The FrontendState struct to populate.
                * **model\_type** â The type of audio model.

            Returns
            :   true on `FrontendPopulateState()` success, else false.

        void `YamNetPreprocessInput`(*const* int16\_t \**audio\_data*, TfLiteTensor \**input\_tensor*, FrontendState \**frontend\_state*)[Â¶](#_CPPv4N10coralmicro10tensorflow21YamNetPreprocessInputEPK7int16_tP12TfLiteTensorP13FrontendState "Permalink to this definition")
        :   Performs input preprocessing to convert raw input to spectrogram.

            Parameters
            :   * **audio\_data** â An array of signed int16 audio data.
                * **input\_tensor** â The tensor where the preprocessed spectrogram data is stored.
                * **frontend\_state** â The populated frontend state that you want to preprocess the input tensor, must not be nullptr.

        void `KeywordDetectorPreprocessInput`(*const* int16\_t \**audio\_data*, TfLiteTensor \**input\_tensor*, FrontendState \**frontend\_state*)[Â¶](#_CPPv4N10coralmicro10tensorflow30KeywordDetectorPreprocessInputEPK7int16_tP12TfLiteTensorP13FrontendState "Permalink to this definition")
        :   Performs input preprocessing to convert raw audio input to spectrogram.

            Parameters
            :   * **audio\_data** â An array of signed int16 audio data.
                * **input\_tensor** â The tensor you want to pre-process for a TensorFlow model, must not be nullptr.
                * **frontend\_state** â The populated frontend state that you want to preprocess the input tensor, must not be nullptr.

        Variables

        *constexpr* int `kYamnetSampleRate` = 16000[Â¶](#_CPPv4N10coralmicro10tensorflow17kYamnetSampleRateE "Permalink to this definition")

        *constexpr* int `kYamnetSampleRateMs` = [kYamnetSampleRate](#_CPPv4N10coralmicro10tensorflow17kYamnetSampleRateE "coralmicro::tensorflow::kYamnetSampleRate") / 1000[Â¶](#_CPPv4N10coralmicro10tensorflow19kYamnetSampleRateMsE "Permalink to this definition")

        *constexpr* int `kYamnetDurationMs` = 975[Â¶](#_CPPv4N10coralmicro10tensorflow17kYamnetDurationMsE "Permalink to this definition")

        *constexpr* int `kYamnetAudioSize` = [kYamnetSampleRate](#_CPPv4N10coralmicro10tensorflow17kYamnetSampleRateE "coralmicro::tensorflow::kYamnetSampleRate") \* [kYamnetDurationMs](#_CPPv4N10coralmicro10tensorflow17kYamnetDurationMsE "coralmicro::tensorflow::kYamnetDurationMs") / 1000[Â¶](#_CPPv4N10coralmicro10tensorflow16kYamnetAudioSizeE "Permalink to this definition")

        *constexpr* int `kYamnetFeatureSliceSize` = 64[Â¶](#_CPPv4N10coralmicro10tensorflow23kYamnetFeatureSliceSizeE "Permalink to this definition")

        *constexpr* int `kYamnetFeatureSliceCount` = 96[Â¶](#_CPPv4N10coralmicro10tensorflow24kYamnetFeatureSliceCountE "Permalink to this definition")

        *constexpr* int `kYamnetFeatureElementCount` = ([kYamnetFeatureSliceSize](#_CPPv4N10coralmicro10tensorflow23kYamnetFeatureSliceSizeE "coralmicro::tensorflow::kYamnetFeatureSliceSize") \* [kYamnetFeatureSliceCount](#_CPPv4N10coralmicro10tensorflow24kYamnetFeatureSliceCountE "coralmicro::tensorflow::kYamnetFeatureSliceCount"))[Â¶](#_CPPv4N10coralmicro10tensorflow26kYamnetFeatureElementCountE "Permalink to this definition")

        *constexpr* int `kYamnetFeatureSliceStrideMs` = 10[Â¶](#_CPPv4N10coralmicro10tensorflow27kYamnetFeatureSliceStrideMsE "Permalink to this definition")

        *constexpr* int `kYamnetFeatureSliceDurationMs` = 25[Â¶](#_CPPv4N10coralmicro10tensorflow29kYamnetFeatureSliceDurationMsE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorSampleRate` = 16000[Â¶](#_CPPv4N10coralmicro10tensorflow26kKeywordDetectorSampleRateE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorSampleRateMs` = [kKeywordDetectorSampleRate](#_CPPv4N10coralmicro10tensorflow26kKeywordDetectorSampleRateE "coralmicro::tensorflow::kKeywordDetectorSampleRate") / 1000[Â¶](#_CPPv4N10coralmicro10tensorflow28kKeywordDetectorSampleRateMsE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorDurationMs` = 2000[Â¶](#_CPPv4N10coralmicro10tensorflow26kKeywordDetectorDurationMsE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorAudioSize` = [kKeywordDetectorSampleRate](#_CPPv4N10coralmicro10tensorflow26kKeywordDetectorSampleRateE "coralmicro::tensorflow::kKeywordDetectorSampleRate") \* [kKeywordDetectorDurationMs](#_CPPv4N10coralmicro10tensorflow26kKeywordDetectorDurationMsE "coralmicro::tensorflow::kKeywordDetectorDurationMs") / 1000[Â¶](#_CPPv4N10coralmicro10tensorflow25kKeywordDetectorAudioSizeE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorFeatureSliceSize` = 32[Â¶](#_CPPv4N10coralmicro10tensorflow32kKeywordDetectorFeatureSliceSizeE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorFeatureSliceCount` = 198[Â¶](#_CPPv4N10coralmicro10tensorflow33kKeywordDetectorFeatureSliceCountE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorFeatureElementCount` = ([kKeywordDetectorFeatureSliceSize](#_CPPv4N10coralmicro10tensorflow32kKeywordDetectorFeatureSliceSizeE "coralmicro::tensorflow::kKeywordDetectorFeatureSliceSize") \* [kKeywordDetectorFeatureSliceCount](#_CPPv4N10coralmicro10tensorflow33kKeywordDetectorFeatureSliceCountE "coralmicro::tensorflow::kKeywordDetectorFeatureSliceCount"))[Â¶](#_CPPv4N10coralmicro10tensorflow35kKeywordDetectorFeatureElementCountE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorFeatureSliceStrideMs` = 10[Â¶](#_CPPv4N10coralmicro10tensorflow36kKeywordDetectorFeatureSliceStrideMsE "Permalink to this definition")

        *constexpr* int `kKeywordDetectorFeatureSliceDurationMs` = 25[Â¶](#_CPPv4N10coralmicro10tensorflow38kKeywordDetectorFeatureSliceDurationMsE "Permalink to this definition")

Utilities
---------

The following functions help with some common tasks during inferencing, such as
manipulate images and tensors.

[[utils.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/tensorflow/utils.h)

*namespace* `coralmicro`
:   *namespace* `tensorflow`
    :   *struct* `ImageDims`[Â¶](#_CPPv4N10coralmicro10tensorflow9ImageDimsE "Permalink to this definition")
        :   *#include <utils.h>*

            Represents the dimensions of an image.

            Public Members

            int `height`[Â¶](#_CPPv4N10coralmicro10tensorflow9ImageDims6heightE "Permalink to this definition")
            :   Pixel height.

            int `width`[Â¶](#_CPPv4N10coralmicro10tensorflow9ImageDims5widthE "Permalink to this definition")
            :   Pixel width.

            int `depth`[Â¶](#_CPPv4N10coralmicro10tensorflow9ImageDims5depthE "Permalink to this definition")
            :   Channel depth.

        Functions

        *inline* bool `operator==`(*const* [ImageDims](#_CPPv4N10coralmicro10tensorflow9ImageDimsE "coralmicro::tensorflow::ImageDims") &*a*, *const* [ImageDims](#_CPPv4N10coralmicro10tensorflow9ImageDimsE "coralmicro::tensorflow::ImageDims") &*b*)[Â¶](#_CPPv4N10coralmicro10tensorfloweqERK9ImageDimsRK9ImageDims "Permalink to this definition")
        :   Operator == to compares 2 [ImageDims](#structcoralmicro_1_1tensorflow_1_1ImageDims) object.

        *inline* int `ImageSize`(*const* [ImageDims](#_CPPv4N10coralmicro10tensorflow9ImageDimsE "coralmicro::tensorflow::ImageDims") &*dims*)[Â¶](#_CPPv4N10coralmicro10tensorflow9ImageSizeERK9ImageDims "Permalink to this definition")
        :   Gets an [ImageDims](#structcoralmicro_1_1tensorflow_1_1ImageDims)âs size.

        bool `ResizeImage`(*const* [ImageDims](#_CPPv4N10coralmicro10tensorflow9ImageDimsE "coralmicro::tensorflow::ImageDims") &*in\_dims*, *const* uint8\_t \**uin*, *const* [ImageDims](#_CPPv4N10coralmicro10tensorflow9ImageDimsE "coralmicro::tensorflow::ImageDims") &*out\_dims*, uint8\_t \**uout*)[Â¶](#_CPPv4N10coralmicro10tensorflow11ResizeImageERK9ImageDimsPK7uint8_tRK9ImageDimsP7uint8_t "Permalink to this definition")
        :   Resizes a bitmap image.

            Parameters
            :   * **in\_dims** â The current dimensions for image `uin`.
                * **uin** â The input image location.
                * **out\_dims** â The desired dimensions for image `uout`.
                * **uout** â The output image location.

        *inline* int `TensorSize`(TfLiteTensor \**tensor*)[Â¶](#_CPPv4N10coralmicro10tensorflow10TensorSizeEP12TfLiteTensor "Permalink to this definition")
        :   Gets the size of a tensor.

            Parameters
            :   **tensor** â The tensor to get the size.

            Returns
            :   The size of the tensor.

        template<typename `I`, typename `O`> void `Dequantize`(int *tensor\_size*, [I](#_CPPv4I00EN10coralmicro10tensorflow10DequantizeEviP1IP1Off "coralmicro::tensorflow::Dequantize::I") \**tensor\_data*, [O](#_CPPv4I00EN10coralmicro10tensorflow10DequantizeEviP1IP1Off "coralmicro::tensorflow::Dequantize::O") \**dequant\_data*, float *scale*, float *zero\_point*)[Â¶](#_CPPv4I00EN10coralmicro10tensorflow10DequantizeEviP1IP1Off "Permalink to this definition")
        :   Dequantizes data.

            Parameters
            :   * **tensor\_size** â The tensorâs size.
                * **tensor\_data** â The tensorâs data.
                * **dequant\_data** â The buffer to return the dequantized data to.
                * **scale** â The scale of the input tensor.
                * **zero\_point** â The zero point of the input tensor.

            Template Parameters
            :   * **I** â The data type of tensor\_data.
                * **O** â The desired data type of the dequantized data. Note: You should instead use `DequantizeTensor()`.

        template<typename `T`> std::vector<[T](#_CPPv4I0EN10coralmicro10tensorflow16DequantizeTensorENSt6vectorI1TEEP12TfLiteTensor "coralmicro::tensorflow::DequantizeTensor::T")> `DequantizeTensor`(TfLiteTensor \**tensor*)[Â¶](#_CPPv4I0EN10coralmicro10tensorflow16DequantizeTensorENSt6vectorI1TEEP12TfLiteTensor "Permalink to this definition")
        :   Dequantizes a tensor.

            Parameters
            :   **tensor** â The tensor to dequantize.

            Template Parameters
            :   **T** â The desired output type of the dequantized data. When using a model adapter API such as `GetClassificationResults()`, this dequantization is done for you.

            Returns
            :   A vector of quantized data.

Defines

`STATIC_TENSOR_ARENA_IN_SDRAM`(*name*, *size*)[Â¶](#c.STATIC_TENSOR_ARENA_IN_SDRAM "Permalink to this definition")
:   Allocates a uint8\_t tensor arena statically in the Dev Board Micro SDRAM (max of 64 MB). This is slightly slower than OCRAM due to off-chip I/O overhead costs.

    Parameters
    :   * **name** â The variable name for this allocation.
        * **size** â The byte size to allocate. This macro automatically aligns the size to 16 bits.

`STATIC_TENSOR_ARENA_IN_OCRAM`(*name*, *size*)[Â¶](#c.STATIC_TENSOR_ARENA_IN_OCRAM "Permalink to this definition")
:   Allocates a uint8\_t tensor arena statically in the RT1176 on-chip RAM (max of 1.25 MB).

    Parameters
    :   * **name** â The variable name for this allocation.
        * **size** â The byte size to allocate. This macro automatically aligns the size to 16 bits.
