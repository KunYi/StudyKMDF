/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD StudyKMDFEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP StudyKMDFEvtDriverContextCleanup;

//
//
//
#define IOCTL_WMI_EXECUTE CTL_CODE(0x8751, 0x875, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
EXTERN_C_END
