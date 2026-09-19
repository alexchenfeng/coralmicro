USB APIs
========

Note

USB is not available within M4 programs.

USB device
----------

[[usb\_device\_task.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/usb/usb_device_task.h)

*namespace* `coralmicro`
:   *class* `UsbDeviceTask`[Â¶](#_CPPv4N10coralmicro13UsbDeviceTaskE "Permalink to this definition")
    :   *#include <usb\_device\_task.h>*

        Operates the Dev Board Micro as a USB device.

        Public Functions

        `UsbDeviceTask`()[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask13UsbDeviceTaskEv "Permalink to this definition")

        `UsbDeviceTask`(*const* [UsbDeviceTask](#_CPPv4N10coralmicro13UsbDeviceTask13UsbDeviceTaskERK13UsbDeviceTask "coralmicro::UsbDeviceTask::UsbDeviceTask")&) = delete[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask13UsbDeviceTaskERK13UsbDeviceTask "Permalink to this definition")

        [UsbDeviceTask](#_CPPv4N10coralmicro13UsbDeviceTaskE "coralmicro::UsbDeviceTask") &`operator=`(*const* [UsbDeviceTask](#_CPPv4N10coralmicro13UsbDeviceTaskE "coralmicro::UsbDeviceTask")&) = delete[Â¶](#_CPPv4N10coralmicro13UsbDeviceTaskaSERK13UsbDeviceTask "Permalink to this definition")

        void `AddDevice`(*const* usb\_device\_class\_config\_struct\_t &*config*, [UsbSetHandleCallback](#_CPPv4N10coralmicro13UsbDeviceTask20UsbSetHandleCallbackE "coralmicro::UsbDeviceTask::UsbSetHandleCallback") *sh\_cb*, [UsbHandleEventCallback](#_CPPv4N10coralmicro13UsbDeviceTask22UsbHandleEventCallbackE "coralmicro::UsbDeviceTask::UsbHandleEventCallback") *he\_cb*, *const* void \**descriptor\_data*, size\_t *descriptor\_data\_size*)[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask9AddDeviceERK32usb_device_class_config_struct_t20UsbSetHandleCallback22UsbHandleEventCallbackPKv6size_t "Permalink to this definition")

        void `UsbDeviceTaskFn`()[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask15UsbDeviceTaskFnEv "Permalink to this definition")

        *inline* uint8\_t `next_descriptor_value`()[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask21next_descriptor_valueEv "Permalink to this definition")

        *inline* uint8\_t `next_interface_value`()[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask20next_interface_valueEv "Permalink to this definition")

        *inline* usb\_device\_handle `device_handle`() *const*[Â¶](#_CPPv4NK10coralmicro13UsbDeviceTask13device_handleEv "Permalink to this definition")

        Public Static Functions

        *static* *inline* [UsbDeviceTask](#_CPPv4N10coralmicro13UsbDeviceTaskE "coralmicro::UsbDeviceTask") \*`GetSingleton`()[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask12GetSingletonEv "Permalink to this definition")

        Public Types

        *using* `UsbSetHandleCallback` = std::function<void(class\_handle\_t)>[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask20UsbSetHandleCallbackE "Permalink to this definition")

        *using* `UsbHandleEventCallback` = std::function<bool(uint32\_t, void\*)>[Â¶](#_CPPv4N10coralmicro13UsbDeviceTask22UsbHandleEventCallbackE "Permalink to this definition")

USB host
--------

[[usb\_host\_task.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/usb/usb_host_task.h)

*namespace* `coralmicro`
:   *class* `UsbHostTask`[Â¶](#_CPPv4N10coralmicro11UsbHostTaskE "Permalink to this definition")
    :   *#include <usb\_host\_task.h>*

        Operates the Dev Board Micro as a USB host.

        Public Functions

        `UsbHostTask`()[Â¶](#_CPPv4N10coralmicro11UsbHostTask11UsbHostTaskEv "Permalink to this definition")

        `UsbHostTask`(*const* [UsbHostTask](#_CPPv4N10coralmicro11UsbHostTask11UsbHostTaskERK11UsbHostTask "coralmicro::UsbHostTask::UsbHostTask")&) = delete[Â¶](#_CPPv4N10coralmicro11UsbHostTask11UsbHostTaskERK11UsbHostTask "Permalink to this definition")

        [UsbHostTask](#_CPPv4N10coralmicro11UsbHostTaskE "coralmicro::UsbHostTask") &`operator=`(*const* [UsbHostTask](#_CPPv4N10coralmicro11UsbHostTaskE "coralmicro::UsbHostTask")&) = delete[Â¶](#_CPPv4N10coralmicro11UsbHostTaskaSERK11UsbHostTask "Permalink to this definition")

        void `RegisterUsbHostEventCallback`(uint32\_t *vid*, uint32\_t *pid*, [UsbHostEventCallback](#_CPPv4N10coralmicro11UsbHostTask20UsbHostEventCallbackE "coralmicro::UsbHostTask::UsbHostEventCallback") *fn*)[Â¶](#_CPPv4N10coralmicro11UsbHostTask28RegisterUsbHostEventCallbackE8uint32_t8uint32_t20UsbHostEventCallback "Permalink to this definition")

        usb\_status\_t `HostEvent`(usb\_device\_handle *device\_handle*, usb\_host\_configuration\_handle *config\_handle*, uint32\_t *event\_code*)[Â¶](#_CPPv4N10coralmicro11UsbHostTask9HostEventE17usb_device_handle29usb_host_configuration_handle8uint32_t "Permalink to this definition")

        *inline* usb\_host\_handle `host_handle`() *const*[Â¶](#_CPPv4NK10coralmicro11UsbHostTask11host_handleEv "Permalink to this definition")

        Public Static Functions

        *static* *inline* [UsbHostTask](#_CPPv4N10coralmicro11UsbHostTaskE "coralmicro::UsbHostTask") \*`GetSingleton`()[Â¶](#_CPPv4N10coralmicro11UsbHostTask12GetSingletonEv "Permalink to this definition")

        Public Types

        *using* `UsbHostEventCallback` = std::function<usb\_status\_t(usb\_host\_handle, usb\_device\_handle, usb\_host\_configuration\_handle, uint32\_t)>[Â¶](#_CPPv4N10coralmicro11UsbHostTask20UsbHostEventCallbackE "Permalink to this definition")

USB host communication (CDC-ACM)
--------------------------------

[[cdc\_acm.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/cdc_acm/cdc_acm.h)

*namespace* `coralmicro`
:   *class* `CdcAcm`[Â¶](#_CPPv4N10coralmicro6CdcAcmE "Permalink to this definition")
    :   *#include <cdc\_acm.h>*

        Public Functions

        `CdcAcm`() = default[Â¶](#_CPPv4N10coralmicro6CdcAcm6CdcAcmEv "Permalink to this definition")

        `CdcAcm`(*const* [CdcAcm](#_CPPv4N10coralmicro6CdcAcm6CdcAcmERK6CdcAcm "coralmicro::CdcAcm::CdcAcm")&) = delete[Â¶](#_CPPv4N10coralmicro6CdcAcm6CdcAcmERK6CdcAcm "Permalink to this definition")

        [CdcAcm](#_CPPv4N10coralmicro6CdcAcmE "coralmicro::CdcAcm") &`operator=`(*const* [CdcAcm](#_CPPv4N10coralmicro6CdcAcmE "coralmicro::CdcAcm")&) = delete[Â¶](#_CPPv4N10coralmicro6CdcAcmaSERK6CdcAcm "Permalink to this definition")

        void `Init`(uint8\_t *interrupt\_in\_ep*, uint8\_t *bulk\_in\_ep*, uint8\_t *bulk\_out\_ep*, uint8\_t *comm\_iface*, uint8\_t *data\_iface*, [RxHandler](#_CPPv4N10coralmicro6CdcAcm9RxHandlerE "coralmicro::CdcAcm::RxHandler") *rx\_handler*)[Â¶](#_CPPv4N10coralmicro6CdcAcm4InitE7uint8_t7uint8_t7uint8_t7uint8_t7uint8_t9RxHandler "Permalink to this definition")

        *inline* *const* usb\_device\_class\_config\_struct\_t &`config_data`() *const*[Â¶](#_CPPv4NK10coralmicro6CdcAcm11config_dataEv "Permalink to this definition")

        *inline* *const* void \*`descriptor_data`() *const*[Â¶](#_CPPv4NK10coralmicro6CdcAcm15descriptor_dataEv "Permalink to this definition")

        *inline* size\_t `descriptor_data_size`() *const*[Â¶](#_CPPv4NK10coralmicro6CdcAcm20descriptor_data_sizeEv "Permalink to this definition")

        *inline* void `SetClassHandle`(class\_handle\_t *class\_handle*)[Â¶](#_CPPv4N10coralmicro6CdcAcm14SetClassHandleE14class_handle_t "Permalink to this definition")
        :   TODO(atv): Make me private.

        bool `HandleEvent`(uint32\_t *event*, void \**param*)[Â¶](#_CPPv4N10coralmicro6CdcAcm11HandleEventE8uint32_tPv "Permalink to this definition")

        bool `Transmit`(*const* uint8\_t \**buffer*, *const* size\_t *length*)[Â¶](#_CPPv4N10coralmicro6CdcAcm8TransmitEPK7uint8_tK6size_t "Permalink to this definition")

        Public Types

        *using* `RxHandler` = std::function<void(*const* uint8\_t\*, *const* uint32\_t)>[Â¶](#_CPPv4N10coralmicro6CdcAcm9RxHandlerE "Permalink to this definition")

*struct* `_usb_host_cdc_line_coding_struct`[Â¶](#_CPPv432_usb_host_cdc_line_coding_struct "Permalink to this definition")
:   *#include <cdc\_acm.h>*

    Public Members

    uint32\_t `dwDTERate`[Â¶](#_CPPv4N32_usb_host_cdc_line_coding_struct9dwDTERateE "Permalink to this definition")
    :   Data terminal rate, in bits per second

    uint8\_t `bCharFormat`[Â¶](#_CPPv4N32_usb_host_cdc_line_coding_struct11bCharFormatE "Permalink to this definition")
    :   Stop bits

    uint8\_t `bParityType`[Â¶](#_CPPv4N32_usb_host_cdc_line_coding_struct11bParityTypeE "Permalink to this definition")
    :   Parity

    uint8\_t `bDataBits`[Â¶](#_CPPv4N32_usb_host_cdc_line_coding_struct9bDataBitsE "Permalink to this definition")
    :   Data bits (5, 6, 7, 8 or 16).

Functions

`struct _usb_host_cdc_line_coding_struct __attribute__ ((packed)) usb_host_cdc_line_coding_struct_t`

USB Ethernet emulation (CDC-EEM)
--------------------------------

[[cdc\_eem.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/cdc_eem/cdc_eem.h)

*namespace* `coralmicro`
:   *class* `CdcEem`[Â¶](#_CPPv4N10coralmicro6CdcEemE "Permalink to this definition")
    :   *#include <cdc\_eem.h>*

        Public Functions

        `CdcEem`() = default[Â¶](#_CPPv4N10coralmicro6CdcEem6CdcEemEv "Permalink to this definition")

        `CdcEem`(*const* [CdcEem](#_CPPv4N10coralmicro6CdcEem6CdcEemERK6CdcEem "coralmicro::CdcEem::CdcEem")&) = delete[Â¶](#_CPPv4N10coralmicro6CdcEem6CdcEemERK6CdcEem "Permalink to this definition")

        [CdcEem](#_CPPv4N10coralmicro6CdcEemE "coralmicro::CdcEem") &`operator=`(*const* [CdcEem](#_CPPv4N10coralmicro6CdcEemE "coralmicro::CdcEem")&) = delete[Â¶](#_CPPv4N10coralmicro6CdcEemaSERK6CdcEem "Permalink to this definition")

        void `Init`(uint8\_t *bulk\_in\_ep*, uint8\_t *bulk\_out\_ep*, uint8\_t *data\_iface*)[Â¶](#_CPPv4N10coralmicro6CdcEem4InitE7uint8_t7uint8_t7uint8_t "Permalink to this definition")

        *inline* *const* usb\_device\_class\_config\_struct\_t &`config_data`() *const*[Â¶](#_CPPv4NK10coralmicro6CdcEem11config_dataEv "Permalink to this definition")

        *inline* *const* void \*`descriptor_data`() *const*[Â¶](#_CPPv4NK10coralmicro6CdcEem15descriptor_dataEv "Permalink to this definition")

        *inline* size\_t `descriptor_data_size`() *const*[Â¶](#_CPPv4NK10coralmicro6CdcEem20descriptor_data_sizeEv "Permalink to this definition")

        *inline* void `SetClassHandle`(class\_handle\_t *class\_handle*)[Â¶](#_CPPv4N10coralmicro6CdcEem14SetClassHandleE14class_handle_t "Permalink to this definition")

        bool `HandleEvent`(uint32\_t *event*, void \**param*)[Â¶](#_CPPv4N10coralmicro6CdcEem11HandleEventE8uint32_tPv "Permalink to this definition")
