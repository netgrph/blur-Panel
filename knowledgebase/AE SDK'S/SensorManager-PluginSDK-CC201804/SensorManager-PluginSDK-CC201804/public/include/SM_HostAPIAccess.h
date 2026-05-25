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


#ifndef __SM_HOSTAPI_ACCESS_H__
#define __SM_HOSTAPI_ACCESS_H__

#include "SM_HostAPI.h"
#include "SM_FwdDeclarations.h"


namespace SM_PLUGIN
{

	bool SetHostAPI(HostAPIRef hostAPI);

	void HostCreateRoot(ConverterRef inConverter, SM_NAMESPACE::SM_UTF8FilePath inPath, IMgjsonRoot** ptr, SM_NAMESPACE::SM_Bool* alreadyCreated);

	void HostCreateGroup(ConverterRef inConverter, IMgjsonGroup** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName);

	void HostCreateDynamicDataNumArr(ConverterRef inConverter, IMgjsonDynamicDataNumArr** ptr);

	void HostCreateDynamicDataNum(ConverterRef inConverter, IMgjsonDynamicDataNum** ptr);

	void HostCreateDynamicDataString(ConverterRef inConverter, IMgjsonDynamicDataString** ptr);

	void HostCreateStaticDataNum(ConverterRef inConverter, IMgjsonStaticDataNum** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, double value);

	void HostCreateStaticDataNumArr(ConverterRef inConverter, IMgjsonStaticDataNumArr** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName);


	void HostCreateStaticDataString(ConverterRef inConverter, IMgjsonStaticDataString** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_StringPtr value);

	void HostCreateStaticDataBool(ConverterRef inConverter, IMgjsonStaticDataBool** ptr, SM_NAMESPACE::SM_StringPtr inMatchName, SM_NAMESPACE::SM_StringPtr inDisplayName, SM_NAMESPACE::SM_Bool value);

	void HostRemoveConverterFromMap(ConverterRef inConverter);
	
	void SetPluginErrorObj(void* result, SM_NAMESPACE::eErrorCode errCode, SM_NAMESPACE::SM_Uns64 fileOffset, SM_NAMESPACE::SM_Uns64 lineNo);

} //namespace SM_PLUGIN

#endif  // __SM_HOSTAPI_ACCESS_H__

