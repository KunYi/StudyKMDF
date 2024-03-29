/*++

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "device.tmh"
#include <ntstrsafe.h.>
#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, StudyKMDFCreateDevice)
#endif

NTSTATUS
StudyKMDFCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (NT_SUCCESS(status)) {
        //
        // Get a pointer to the device context structure that we just associated
        // with the device object. We define this structure in the device.h
        // header file. DeviceGetContext is an inline function generated by
        // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
        // This function will do the type checking and return the device context.
        // If you pass a wrong object handle it will return NULL and assert if
        // run under framework verifier mode.
        //
        deviceContext = DeviceGetContext(device);

        //
        // Initialize the context.
        //
        deviceContext->PrivateDeviceData = 0;
        deviceContext->pWMIObject = NULL;
        RtlZeroMemory(deviceContext->wmiInstanceName,
                      sizeof(deviceContext->wmiInstanceName));
        //
        // Create a device interface so that applications can find and talk
        // to us.
        //
        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_StudyKMDF,
            NULL // ReferenceString
            );

        if (NT_SUCCESS(status)) {
            //
            // Initialize the I/O Package and any Queues
            //
            status = StudyKMDFQueueInitialize(device);
        }

        GUID sampleWMIACPI = DEMO_WMIACPI_GUID;

        status = IoWMIOpenBlock(&sampleWMIACPI, WMIGUID_EXECUTE,
                                &deviceContext->pWMIObject);

        if (!NT_SUCCESS(status)) {
          KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
                     "Failed, execute IoWMIOpenBlock(), return %d\n", status));
          return status;
        }

        BYTE buff[2048];
        ULONG buffSize = sizeof(buff);
        status = IoWMIQueryAllData(deviceContext->pWMIObject, &buffSize, buff);
        if (!NT_SUCCESS(status)) {
          KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
                     "Failed, execute IoWMIQueryAllData(), return %d\n",
                     status));
          return status;
        }
        // store instance
        WNODE_ALL_DATA *pWNode = (WNODE_ALL_DATA *)buff;
        ULONG offset = *((PULONG)(buff + pWNode->OffsetInstanceNameOffsets));
        LPCWCHAR pStr = (PWCHAR)(buff + offset + sizeof(USHORT)); // due to the string is COUNTED_STRINGS
        RtlStringCbCopyW(deviceContext->wmiInstanceName,
                         sizeof(deviceContext->wmiInstanceName), pStr);
    }

    return status;
}
