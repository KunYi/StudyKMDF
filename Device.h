/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include "public.h"

EXTERN_C_START

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
    ULONG PrivateDeviceData;  // just a placeholder
    PVOID pWMIObject;         //
    WCHAR wmiInstanceName[200];

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// definition WMIACPI method guid
// ref.
// https://github.com/microsoft/Windows-driver-samples/blob/master/wmi/wmiacpi/device.asl#L55
// {ABBC0F6d-8EA1-11d1-00A0-C90629100000}
//
DEFINE_GUID(DEMO_WMIACPI_GUID, 0xABBC0F6d, 0x8EA1, 0x11d1, 0x00, 0xa0, 0xC9,
            0x06, 0x29, 0x10, 0x00, 0x00);

// Function to initialize the device and its callbacks
//
NTSTATUS
StudyKMDFCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

EXTERN_C_END
