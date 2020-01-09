/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_StudyKMDF,
    0x095fb5fc,0x1f42,0x46dd,0x85,0x8a,0x9c,0xd8,0x5b,0x85,0xd8,0x31);
// {095fb5fc-1f42-46dd-858a-9cd85b85d831}
