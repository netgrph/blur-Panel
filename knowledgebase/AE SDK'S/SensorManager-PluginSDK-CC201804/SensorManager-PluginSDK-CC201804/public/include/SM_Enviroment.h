#ifndef __SM_ENVIROMENT_H__
#define __SM_ENVIROMENT_H__

/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2018 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/


// =================================================================================================
// SM_Enviroment.h - Build environment flags for the Sensor Manager.
// ================================================================
//
// This header is just C preprocessor macro definitions to set up the Sensor Manager build environment.
// It must be the first #include in any chain since it might affect things in other #includes.
//
// =================================================================================================

// =================================================================================================
// Determine the Platform
// ======================

// One of MAC_ENV, WIN_ENV must be defined by the client.

#if defined ( MAC_ENV )

#if defined ( WIN_ENV )
#error "SM environment error - must define only one of MAC_ENV or WIN_ENV"
#endif

#define SM_MacBuild  1
#define SM_WinBuild  0

#elif defined ( WIN_ENV )

#if defined ( MAC_ENV )
#error "SM environment error - must define only one of MAC_ENV or WIN_ENV"
#endif

#define SM_MacBuild  0
#define SM_WinBuild  1

#else

#error "SM environment error - must define one of MAC_ENV or WIN_ENV"

#endif

// =================================================================================================
// Common Macros
// =============

#if defined ( DEBUG )
#if defined ( NDEBUG )
#error "SM environment error - both DEBUG and NDEBUG are defined"
#endif
#define SM_DebugBuild 1
#endif

#if defined ( NDEBUG )
#define SM_DebugBuild 0
#endif

#ifndef SM_DebugBuild
#define SM_DebugBuild 0
#endif

#if SM_DebugBuild
#include <stdio.h>  // The assert macro needs printf.
#endif

#ifndef SM_64
#if _WIN64 || defined(_LP64)
#define SM_64 1
#else
#define SM_64 0
#endif
#endif

// =================================================================================================
// Macintosh Specific Settings
// ===========================
#if (SM_MacBuild)
#define SM_DLL_IMPORT __attribute__((visibility("default")))
#define SM_DLL_EXPORT __attribute__((visibility("default")))
#define SM_DLL_INTERNAL __attribute__((visibility("hidden")))
#define APICALL
#endif

// =================================================================================================
// Windows Specific Settings
// =========================
#if (SM_WinBuild)
#define SM_DLL_IMPORT
#define SM_DLL_EXPORT
#define SM_DLL_INTERNAL
#define APICALL __stdcall
#endif


// =================================================================================================

#if (SM_DynamicBuild)
#define SM_PUBLIC SM_DLL_EXPORT
#define SM_INTERNAL SM_DLL_INTERNAL
#elif (SM_StaticBuild)
#define SM_PUBLIC
#define SM_INTERNAL
#else
#define SM_PUBLIC SM_DLL_IMPORT
#define SM_INTERNAL SM_DLL_INTERNAL
#endif



#endif /* __SM_ENVIROMENT_H__ */
