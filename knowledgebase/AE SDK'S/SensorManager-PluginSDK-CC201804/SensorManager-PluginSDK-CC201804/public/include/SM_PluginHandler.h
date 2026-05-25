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


/**************************************************************************
* @file SM_PluginHandler.h
* @brief 
*
*************************************************************************** 
*/

#ifndef __SM_PLUGINHANDLER_H__
#define __SM_PLUGINHANDLER_H__ 

#include "SM_Const.h"

// versioning
#define SM_PLUGIN_VERSION 1

namespace SM_PLUGIN
{

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__GNUC__)
#define DllExport		__attribute__((visibility("default")))
#else
#define DllExport		__declspec( dllexport )
#endif

struct PluginAPI;
struct HostAPI;

typedef PluginAPI*				PluginAPIRef;
typedef HostAPI*				HostAPIRef;
typedef void*					SessionRef;
typedef SessionRef				ConverterRef;



typedef void(*SetWriterErrorObjProc)(void*, SM_NAMESPACE::eErrorCode, SM_NAMESPACE::SM_Uns64, SM_NAMESPACE::SM_Uns64);
	
void SetWriterErrorObj(void* result, SM_NAMESPACE::eErrorCode errCode, SM_NAMESPACE::SM_Uns64 fileOffset, SM_NAMESPACE::SM_Uns64 lineNo);
	

	typedef void (*TerminatePluginProc)( void* error );

	typedef void (*InitializeSessionProc)(SM_NAMESPACE::SM_StringPtr uid, SM_NAMESPACE::SM_StringPtr ext,
		SM_NAMESPACE::SM_StringPtr filePath, SM_NAMESPACE::SM_StringPtr mgJSONPath, SessionRef* session,
		void* error);

	typedef void (*TerminateSessionProc)(SessionRef session, void* error);

	typedef void (*CheckSessionFileFormatProc)(SM_NAMESPACE::SM_StringPtr uid, SM_NAMESPACE::SM_StringPtr filePath, SM_NAMESPACE::SM_Bool* result, void* error);

	
	typedef void (*ConvertToMGJSONProc)(SessionRef session, SM_NAMESPACE::SM_Bool* result, void* error);

	

struct PluginAPI
{
	/** 
	 *  Size of the PluginAPI structure.
	 */
	SM_NAMESPACE::SM_Uns32						mSize;

	/** 
	 *  Version number of the plugin.
	 */
	SM_NAMESPACE::SM_Uns32						mVersion;

	TerminatePluginProc				mTerminatePluginProc;
	InitializeSessionProc			mInitializeSessionProc;
	TerminateSessionProc			mTerminateSessionProc;
	CheckSessionFileFormatProc		mCheckFileFormatProc;
	ConvertToMGJSONProc				mConvertToMGJSONProc;

};

DllExport void InitializePlugin(SM_NAMESPACE::SM_StringPtr moduleID, HostAPIRef hostAPI, PluginAPIRef pluginAPI, void* error);
typedef void (*InitializePluginProc)(SM_NAMESPACE::SM_StringPtr moduleID, HostAPIRef hostAPI, PluginAPIRef pluginAPI, void* error);

#ifdef __cplusplus
}
#endif

} //namespace SM_PLUGIN

#endif // __SM_PLUGINHANDLER_H__

