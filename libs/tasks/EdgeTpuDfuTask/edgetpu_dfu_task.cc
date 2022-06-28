// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "libs/tasks/EdgeTpuTask/edgetpu_task.h"
#include "libs/tasks/EdgeTpuDfuTask/edgetpu_dfu_task.h"
#include "libs/usb/usb_host_task.h"
#include "third_party/nxp/rt1176-sdk/middleware/usb/host/class/usb_host_dfu.h"
#include "third_party/nxp/rt1176-sdk/middleware/usb/host/usb_host_devices.h"
#include "third_party/nxp/rt1176-sdk/middleware/usb/host/usb_host_ehci.h"

#include <algorithm>
#include <cstdio>
#include <functional>

using namespace std::placeholders;

namespace coral::micro {

using namespace edgetpu_dfu;

void EdgeTpuDfuTask::SetNextState(enum dfu_state next_state) {
    Request req;
    req.type = RequestType::NEXT_STATE;
    req.request.next_state.state = next_state;
    SendRequestAsync(req);
}

usb_status_t EdgeTpuDfuTask::USB_DFUHostEvent(usb_host_handle host_handle,
                                              usb_device_handle device_handle,
                                              usb_host_configuration_handle config_handle,
                                              uint32_t event_code) {
    usb_host_configuration_t *configuration_ptr;
    SetHostInstance(reinterpret_cast<usb_host_instance_t*>(host_handle));
    switch (event_code & 0xFFFF) {
        case kUSB_HostEventAttach:
            configuration_ptr = reinterpret_cast<usb_host_configuration_t*>(config_handle);
            for (int i = 0; i < configuration_ptr->interfaceCount; ++i) {
                auto* interface_ptr = &configuration_ptr->interfaceList[i];
                int id = interface_ptr->interfaceDesc->bInterfaceClass;
                if (id != USB_HOST_DFU_CLASS_CODE) {
                    continue;
                }

                id = interface_ptr->interfaceDesc->bInterfaceSubClass;
                if (id == USB_HOST_DFU_SUBCLASS_CODE) {
                    SetDeviceHandle(device_handle);
                    SetInterfaceHandle(interface_ptr);
                    break;
                }
            }
            return (this->device_handle() != nullptr) ? kStatus_USB_Success : kStatus_USB_NotSupported;
        case kUSB_HostEventEnumerationDone:
            // TODO: check if we're already dfuing, if handles are valid.
            SetNextState(DFU_STATE_ATTACHED);
            return kStatus_USB_Success;
        case kUSB_HostEventDetach:
            SetNextState(DFU_STATE_UNATTACHED);
            return kStatus_USB_Success;
        default:
            return kStatus_USB_Success;
    }
}

void EdgeTpuDfuTask::SetInterfaceCallback(void *param,
                                          uint8_t *data,
                                          uint32_t data_length,
                                          usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUSetInterface\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }
    task->SetNextState(DFU_STATE_GET_STATUS);
}

void EdgeTpuDfuTask::GetStatusCallback(void *param,
                                       uint8_t *data,
                                       uint32_t data_length,
                                       usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUGetStatus\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }

    if (task->bytes_transferred() < task->bytes_to_transfer()) {
        task->SetNextState(DFU_STATE_TRANSFER);
    } else {
        task->SetNextState(DFU_STATE_ZERO_LENGTH_TRANSFER);
    }
}

void EdgeTpuDfuTask::TransferCallback(void *param,
                                      uint8_t *data,
                                      uint32_t data_length,
                                      usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUTransfer\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }

    task->SetCurrentBlockNumber(task->current_block_number() + 1);
    task->SetBytesTransferred(task->bytes_transferred() + data_length);
#if 0
    if (task->current_block_number() % 10 == 0 || task->bytes_transferred() == task->bytes_to_transfer()) {
        printf("Transferred %d bytes\r\n", task->bytes_transferred());
    }
#endif
    task->SetNextState(DFU_STATE_GET_STATUS);
}

void EdgeTpuDfuTask::ZeroLengthTransferCallback(void *param,
                                                uint8_t *data,
                                                uint32_t data_length,
                                                usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUZeroLengthTransfer\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }

    task->SetCurrentBlockNumber(0);
    task->SetBytesTransferred(0);
    task->SetNextState(DFU_STATE_READ_BACK);
}

void EdgeTpuDfuTask::ReadBackCallback(void *param,
                                      uint8_t *data,
                                      uint32_t data_length,
                                      usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUReadBack\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }

    task->SetCurrentBlockNumber(task->current_block_number() + 1);
    task->SetBytesTransferred(task->bytes_transferred() + data_length);
#if 0
    if (task->current_block_number() % 10 == 0 || task->bytes_transferred() == task->bytes_to_transfer()) {
        printf("Read back %d bytes\r\n", task->bytes_transferred());
    }
#endif
    task->SetNextState(DFU_STATE_GET_STATUS_READ);
}

void EdgeTpuDfuTask::GetStatusReadCallback(void *param,
                                           uint8_t *data,
                                           uint32_t data_length,
                                           usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUGetStatusRead\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }

    if (task->bytes_transferred() < task->bytes_to_transfer()) {
        task->SetNextState(DFU_STATE_READ_BACK);
    } else {
        if (memcmp(apex_latest_single_ep_bin, task->read_back_data(), apex_latest_single_ep_bin_len) != 0) {
            printf("Read back firmware does not match!\r\n");
            task->SetNextState(DFU_STATE_ERROR);
        } else {
            task->SetNextState(DFU_STATE_DETACH);
        }
        free(task->read_back_data());
        task->SetReadBackData(nullptr);
        task->SetCurrentBlockNumber(0);
        task->SetBytesTransferred(0);
    }
}

void EdgeTpuDfuTask::DetachCallback(void *param,
                                    uint8_t *data,
                                    uint32_t data_length,
                                    usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUDetach\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }
    // task->SetNextState(DFU_STATE_CHECK_STATUS);
    task->SetNextState(DFU_STATE_COMPLETE);
}

void EdgeTpuDfuTask::CheckStatusCallback(void *param,
                                         uint8_t *data,
                                         uint32_t data_length,
                                         usb_status_t status) {
    auto *task = static_cast<EdgeTpuDfuTask*>(param);
    if (status != kStatus_USB_Success) {
        printf("Error in DFUCheckStatus\r\n");
        task->SetNextState(DFU_STATE_ERROR);
        return;
    }
    task->SetNextState(DFU_STATE_COMPLETE);
}

void EdgeTpuDfuTask::TaskInit() {
    coral::micro::UsbHostTask::GetSingleton()->RegisterUSBHostEventCallback(kDfuVid, kDfuPid,
            std::bind(&EdgeTpuDfuTask::USB_DFUHostEvent, this, _1, _2, _3, _4));
}

void EdgeTpuDfuTask::HandleNextState(NextStateRequest& req) {
    usb_status_t ret;
    uint32_t transfer_length;
    enum dfu_state next_state = req.state;
    switch (next_state) {
        case DFU_STATE_UNATTACHED:
            break;
        case DFU_STATE_ATTACHED:
            ret = USB_HostDfuInit(device_handle(), &class_handle_);
            if (ret == kStatus_USB_Success) {
                SetNextState(DFU_STATE_SET_INTERFACE);
            } else {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_SET_INTERFACE:
            ret = USB_HostDfuSetInterface(class_handle(),
                                          interface_handle(),
                                          0,
                                          EdgeTpuDfuTask::SetInterfaceCallback,
                                          this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_GET_STATUS:
            ret = USB_HostDfuGetStatus(class_handle(),
                                       reinterpret_cast<uint8_t*>(&status_),
                                       EdgeTpuDfuTask::GetStatusCallback,
                                       this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_TRANSFER:
            transfer_length = std::min(256U /* get from descriptor */,
                                       apex_latest_single_ep_bin_len - bytes_transferred());
            ret = USB_HostDfuDnload(class_handle(),
                                    current_block_number(),
                                    apex_latest_single_ep_bin + bytes_transferred(),
                                    transfer_length,
                                    EdgeTpuDfuTask::TransferCallback,
                                    this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_ZERO_LENGTH_TRANSFER:
            ret = USB_HostDfuDnload(class_handle(), current_block_number(), nullptr, 0,
                                    EdgeTpuDfuTask::ZeroLengthTransferCallback, this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_READ_BACK:
            if (!read_back_data()) {
                SetReadBackData(static_cast<uint8_t*>(malloc(apex_latest_single_ep_bin_len)));
            }
            transfer_length = std::min(256U /* get from descriptor */,
                                       apex_latest_single_ep_bin_len - bytes_transferred());
            ret = USB_HostDfuUpload(class_handle(), current_block_number(), read_back_data() + bytes_transferred(),
                                    transfer_length, EdgeTpuDfuTask::ReadBackCallback, this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_GET_STATUS_READ:
            ret = USB_HostDfuGetStatus(class_handle(), reinterpret_cast<uint8_t*>(&status_), EdgeTpuDfuTask::GetStatusReadCallback, this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_DETACH:
            ret = USB_HostDfuDetach(class_handle(), 1000 /* ms */, EdgeTpuDfuTask::DetachCallback, this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_CHECK_STATUS:
            ret = USB_HostDfuGetStatus(class_handle(), reinterpret_cast<uint8_t*>(&status_), EdgeTpuDfuTask::CheckStatusCallback, this);
            if (ret != kStatus_USB_Success) {
                SetNextState(DFU_STATE_ERROR);
            }
            break;
        case DFU_STATE_COMPLETE:
            USB_HostEhciResetBus(static_cast<usb_host_ehci_instance_t*>(host_instance()->controllerHandle));
            ret = USB_HostDfuDeinit(device_handle(), class_handle());
            SetClassHandle(nullptr);
            USB_HostTriggerReEnumeration(device_handle());
            break;
        case DFU_STATE_ERROR:
            printf("DFU error\r\n");
            break;
        default:
            printf("Unhandled DFU state %d\r\n", next_state);
            break;
    }
}

void EdgeTpuDfuTask::RequestHandler(Request *req) {
    Response resp;
    resp.type = req->type;
    switch (req->type) {
        case RequestType::NEXT_STATE:
            HandleNextState(req->request.next_state);
            break;
    }

    if (req->callback) {
        req->callback(resp);
    }
}

}  // namespace coral::micro
