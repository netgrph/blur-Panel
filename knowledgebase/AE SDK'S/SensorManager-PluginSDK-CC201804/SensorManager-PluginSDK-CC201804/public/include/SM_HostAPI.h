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
* @file SM_HostAPI.h
*
*  Function prototypes for the functions that are provided by the host (the SensorManager Library)
*  and can be used by a plugin implementation.
*/

#ifndef __SM_HOSTAPI_H__
#define __SM_HOSTAPI_H__	

#include "SM_PluginHandler.h"
#include "SM_PluginTypes.h"   
#include "SM_PluginTypedefs.h"

#define SM_HOST_API_VERSION 1

namespace SM_PLUGIN
{

#ifdef __cplusplus
	extern "C" {
#endif


		struct MGJSONWriter_API;
		struct HandleException_API;
		struct HandleMemory_API;
		
		/** @struct HostAPI
		*  @brief This is a Host API structure.
		*
		*/
		struct HostAPI
		{
			/**
			*  Size of the structure.
			*/
			SM_NAMESPACE::SM_Uns32				mSize;

			/**
			*  Version number of the API.
			*/
			SM_NAMESPACE::SM_Uns32				mVersion;

			/**
			*  Pointer to a structure which contains factory functions to access APIs of Writer Interfaces.
			*/
			MGJSONWriter_API*				mMGJSONWriterAPI;

			/**
			*  Pointer to a structure which contains error callback functions.
			*/
			HandleException_API*			mHandleExceptionAPI;

			/**
			*  Pointer to a structure which contains functions required for memory clean up.
			*/
			HandleMemory_API*				mHandleMemoryAPI;
		};


		struct MGJSONWriter_API
		{
		
			SM_NAMESPACE::SM_Uns32		mSize;

			typedef void (*CreateRootProc)(ConverterRef inConverter, SM_NAMESPACE::SM_UTF8FilePath inPath, SetWriterErrorObjProc inProc, void** ptr, SM_NAMESPACE::SM_Bool* alreadyCreated, SM_NAMESPACE::SM_Uns32 version,void* error);
			CreateRootProc		mCreateRootProc;
		
			typedef void (*CreateGroupProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateGroupProc		mCreateGroupProc;

			
			typedef void (*CreateDynamicDataNumArrProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateDynamicDataNumArrProc		mCreateDynamicDataNumArrProc;


			typedef void (*CreateDynamicDataNumProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateDynamicDataNumProc		mCreateDynamicDataNumProc;


			typedef void (*CreateDynamicDataStringProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateDynamicDataStringProc		mCreateDynamicDataStringProc;

			
			typedef void(*CreateStaticDataNumProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, double value, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateStaticDataNumProc		mCreateStaticDataNumProc;


			typedef void(*CreateStaticDataNumArrProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateStaticDataNumArrProc		mCreateStaticDataNumArrProc;

	
			typedef void(*CreateStaticDataStringProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_StringPtr value, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateStaticDataStringProc		mCreateStaticDataStringProc;

		
			typedef void(*CreateStaticDataBoolProc)(ConverterRef inConverter, void** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_Bool value, SM_NAMESPACE::SM_Uns32 version, void* error);
			CreateStaticDataBoolProc		mCreateStaticDataBoolProc;
		};

		struct HandleException_API
		{
			SM_NAMESPACE::SM_Uns32		mSize;

			typedef void(*SetPluginErrorObjProc)(void*, SM_NAMESPACE::eErrorCode, SM_NAMESPACE::SM_Uns64, SM_NAMESPACE::SM_Uns64);
			SetPluginErrorObjProc mSetPluginErrorObjProc;
		};

		struct HandleMemory_API
		{
			SM_NAMESPACE::SM_Uns32		mSize;

			typedef void(*RemoveConverterFromMapProc)(ConverterRef converter, void* error);
			RemoveConverterFromMapProc mRemoveConverterFromMapProc;
		};
#ifdef __cplusplus
	}
#endif

} //namespace SM_PLUGIN
#endif // __SM_HOSTAPI_H__
