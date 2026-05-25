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

#include "SM_HostAPIAccess.h"
#include "SM_DataTypesCommon.h"
#include "IMgjsonRoot.h"
#include "IMgjsonGroup.h"
#include "IMgjsonDynamicDataNum.h"
#include "IMgjsonDynamicDataNumArr.h"
#include "IMgjsonDynamicDataString.h"
#include "IMgjsonStaticDataNum.h"
#include "IMgjsonStaticDataBool.h"
#include "IMgjsonStaticDataNumArr.h"
#include "IMgjsonStaticDataString.h"
namespace SM_PLUGIN
{


	static HostAPIRef sHostAPI = NULL;
	static SM_NAMESPACE::SM_Uns32 sHostAPIVersion = 0;

	bool SetHostAPI(HostAPIRef hostAPI)
	{
		if (hostAPI)
		{
			sHostAPI = hostAPI;
			sHostAPIVersion = hostAPI->mVersion;
			return true;
		}
		return false;
	}

	// ============================================================================

	static HostAPIRef GetHostAPI()
	{
		return sHostAPI;
	}

	// ============================================================================
	

	void HostCreateRoot(ConverterRef inConverter,  SM_NAMESPACE::SM_UTF8FilePath inPath, IMgjsonRoot** ptr, SM_NAMESPACE::SM_Bool* alreadyCreated)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateRootProc(inConverter, inPath, SetWriterErrorObj, &vPtr, alreadyCreated, IMgjsonRoot::GetInterfaceVersion(),&error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonRoot*>(vPtr);
	
	}

	// ============================================================================

	void HostCreateGroup(ConverterRef inConverter, IMgjsonGroup** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateGroupProc(inConverter, &vPtr, inMatchName, inDisplayName, IMgjsonGroup::GetInterfaceVersion(), &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonGroup*>(vPtr);
	}


	void HostCreateDynamicDataNumArr(ConverterRef inConverter, IMgjsonDynamicDataNumArr** ptr)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateDynamicDataNumArrProc(inConverter, &vPtr, IMgjsonDynamicDataNumArr::GetInterfaceVersion(), &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonDynamicDataNumArr*>(vPtr);
	}


	void HostCreateDynamicDataNum(ConverterRef inConverter, IMgjsonDynamicDataNum** ptr)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateDynamicDataNumProc(inConverter, &vPtr, IMgjsonDynamicDataNum::GetInterfaceVersion(), &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonDynamicDataNum*>(vPtr);
	}

	void HostCreateDynamicDataString(ConverterRef inConverter, IMgjsonDynamicDataString** ptr)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateDynamicDataStringProc(inConverter, &vPtr, IMgjsonDynamicDataString::GetInterfaceVersion(),&error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonDynamicDataString*>(vPtr);
	}

	void HostCreateStaticDataNum(ConverterRef inConverter, IMgjsonStaticDataNum** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, double value)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateStaticDataNumProc(inConverter, &vPtr, inMatchName, inDisplayName, value, IMgjsonStaticDataNum::GetInterfaceVersion(),  &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonStaticDataNum*>(vPtr);
	}

	void HostCreateStaticDataNumArr(ConverterRef inConverter, IMgjsonStaticDataNumArr** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateStaticDataNumArrProc(inConverter, &vPtr, inMatchName, inDisplayName, IMgjsonStaticDataNumArr::GetInterfaceVersion(),  &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonStaticDataNumArr*>(vPtr);
	}

	void HostCreateStaticDataString(ConverterRef inConverter, IMgjsonStaticDataString** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_StringPtr value)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateStaticDataStringProc(inConverter, &vPtr, inMatchName, inDisplayName, value, IMgjsonStaticDataString::GetInterfaceVersion(), &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonStaticDataString*>(vPtr);
	}

	void HostCreateStaticDataBool(ConverterRef inConverter, IMgjsonStaticDataBool** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_Bool value)
	{
		SM_PluginResult error;
		void* vPtr;
		GetHostAPI()->mMGJSONWriterAPI->mCreateStaticDataBoolProc(inConverter, &vPtr, inMatchName, inDisplayName, value, IMgjsonStaticDataBool::GetInterfaceVersion(), &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
		*ptr = static_cast<IMgjsonStaticDataBool*>(vPtr);
	}
	// ============================================================================


	void SetPluginErrorObj(void* result, SM_NAMESPACE::eErrorCode errCode,
		SM_NAMESPACE::SM_Uns64 fileOffset,
		SM_NAMESPACE::SM_Uns64 lineNo)
	{
		SM_Assert(result != 0);
		GetHostAPI()->mHandleExceptionAPI->mSetPluginErrorObjProc(result, errCode, fileOffset, lineNo);
	}

	void HostRemoveConverterFromMap(ConverterRef inConverter)
	{
		SM_PluginResult error;
		GetHostAPI()->mHandleMemoryAPI->mRemoveConverterFromMapProc(inConverter, &error);
		PROPOGATE_PLUGIN_EXCEPTION(error)
	}

} //namespace SM_PLUGIN
