#ifndef __VERSIONINFO_H__
#define __VERSIONINFO_H__

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2017 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#define CONVERTER_API_VERSION_MAJOR  1
#define CONVERTER_API_VERSION_MINOR  0

#define CONVERTER_API_VERSION MACRO_PASTE(CONVERTER_API_VERSION_MAJOR, ., CONVERTER_API_VERSION_MINOR)
#define CONVERTER_API_VERSION_STRING MAKESTR(CONVERTER_API_VERSION)

// =================================================================================================

#endif /* __VERSIONINFO_H__ */
