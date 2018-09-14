// This file was @generated with LibOVRPlatform/codegen/main. Do not modify it!

#ifndef OVR_REQUESTS_LANGUAGEPACK_H
#define OVR_REQUESTS_LANGUAGEPACK_H

#include "OVR_Types.h"
#include "OVR_Platform_Defs.h"


/// Returns currently installed and selected language pack for an app in the
/// view of the `asset_details`. Use `language` field to extract neeeded
/// language info. A particular language can be download and installed by a
/// user from the Oculus app on the application page.
///
/// A message with type ::ovrMessage_LanguagePack_GetCurrent will be generated in response.
///
/// First call ::ovr_Message_IsError() to check if an error occurred.
///
/// If no error occurred, the message will contain a payload of type ::ovrAssetDetailsHandle.
/// Extract the payload from the message handle with ::ovr_Message_GetAssetDetails().
OVRP_PUBLIC_FUNCTION(ovrRequest) ovr_LanguagePack_GetCurrent();

#endif
