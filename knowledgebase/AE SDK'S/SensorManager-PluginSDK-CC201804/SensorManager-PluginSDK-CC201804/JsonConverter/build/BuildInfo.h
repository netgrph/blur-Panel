#ifndef __BUILDINFO_H__
#define __BUILDINFO_H__

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2017 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

/*
 // =================================================================================================
 // This file provides build numbers that are set automatically by the external build system. This is
 // in contrast to the manual version numbers of SM_ChangeLog.h.
 //
 // The value of BUILDDATE is set to the date of the build, in some reasonable format.
 // The value of BUILDVERSION is set to some number that is meaningful to the build system, such
 // as a Perforce changelist number.
 // =================================================================================================
 */

#define kConverter_Copyright Copyright 2017, Adobe Systems Incorporated. All Rights Reserved.
#define kConverter_CopyrightStr "Copyright 2017, Adobe Systems Incorporated. All Rights Reserved."

// Windows: Stringification convention
#define MAKESTR2(a) #a
#define MAKESTR(a) MAKESTR2(a)

// MAC: Symbols needed for Info.plist; ugly multistage construction because
// they need to be symbols, not strings, no quotes, no whitespace
#define MACRO_PASTE2(a, b, c) a##b##c
#define MACRO_PASTE(a, b, c) MACRO_PASTE2(a, b, c)

#define BUILDID 1

#define BUILDDATE 2018/01/18-00:00:00
#define BUILDBRANCH 1
#define BUILDCHANGELIST 000000
// No RE_REBUILD for now, as not part of SRM2.

#define BUILDVERSION_MAC MACRO_PASTE(BUILDBRANCH, ., BUILDCHANGELIST)

// This mapping is just to aviod changing all the old variables in the code
#define BUILDDATESTR MAKESTR(BUILDDATE)
#define BUILDBRANCHSTR MAKESTR(BUILDBRANCH)
#define BUILDVERSIONSTR MAKESTR(BUILDVERSION_MAC)
#define BUILDIDSTR MAKESTR(BUILDID)

#endif /*__BUILDINFO_H__*/
