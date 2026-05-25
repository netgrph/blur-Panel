#ifndef __CHANGELOG_H__
#define __CHANGELOG_H__

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2017 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================


#define kConverterEngBuild    007
#define kConverterEngString  "007"

#if NDEBUG
#define kConverter_DebugSuffix
#define kConverter_DebugString "        "
#else
#define kConverter_DebugSuffix (debug)
#define kConverter_DebugString " (debug)"
#endif

#endif	/* __CHANGELOG_H__ */
