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

namespace SM_PLUGIN {
	class IMgjsonStaticDataNumArrProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class SM_PLUGIN::IMgjsonStaticDataNumArrProxy;

#include "IMgjsonStaticDataNumArr.h"

namespace SM_PLUGIN {

	class IMgjsonStaticDataNumArrProxy
		: public virtual IMgjsonStaticDataNumArr
	{
	private:
		pIMgjsonStaticDataNumArr mRawPtr;

	public:
		IMgjsonStaticDataNumArrProxy(pIMgjsonStaticDataNumArr ptr)
			: mRawPtr(ptr) {

			mRawPtr->Acquire();
		}

		~IMgjsonStaticDataNumArrProxy() NOTHROW {

			mRawPtr->Release();

		}

		void  Acquire() const NOTHROW {
			assert(false);

		}

		void  Release() const NOTHROW {
			assert(false);

		}

		pISharedObject_I APICALL GetISharedObject_I() NOTHROW {
			return mRawPtr->GetISharedObject_I();
		}

		

		SM_NAMESPACE::eDataValueType APICALL GetType() const NOTHROW
		{

			SM_NAMESPACE::SM_Uns32 type = mRawPtr->GetType();
			return static_cast<SM_NAMESPACE::eDataValueType>( type );
		}


		pIMgjsonStaticData APICALL GetActualIMgjsonStaticData() NOTHROW { return mRawPtr; }

		virtual void APICALL setNumberPattern(SM_NAMESPACE::SM_Uns64 digitsInteger, SM_NAMESPACE::SM_Uns64 digitsDecimal, SM_NAMESPACE::SM_Bool isSigned, void* errResultP) NOTHROW
		{
			mRawPtr->setNumberPattern(digitsInteger, digitsDecimal, isSigned, errResultP);
		}
		virtual void APICALL setNumberRange(double minLegal, double maxLegal, double minOccuring, double maxOccuring, void* errResultP) NOTHROW
		{
			mRawPtr->setNumberRange(minLegal, maxLegal, minOccuring, maxOccuring, errResultP);
		}

		virtual void APICALL setArrCount(SM_NAMESPACE::SM_Uns64 count, void* errResultP) NOTHROW
		{
			mRawPtr->setArrCount(count, errResultP);
		}
		virtual void APICALL setIsPoint(SM_NAMESPACE::SM_Bool isPoint, void* errResultP) NOTHROW
		{
			mRawPtr->setIsPoint(isPoint, errResultP);
		}
		virtual void APICALL addDisplayName(SM_NAMESPACE::SM_StringPtr displayName, void* errResultP) NOTHROW
		{
			mRawPtr->addDisplayName(displayName, errResultP);
		}

		virtual void APICALL addElement(double val, void* errResultP) NOTHROW
		{
			mRawPtr->addElement(val, errResultP);
		}
	};

	spIMgjsonStaticDataNumArr IMgjsonStaticDataNumArr_v1::CreateStaticDataNumArr(ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName,
																				const SM_NAMESPACE::SM_UTF8String& inDisplayName,
																				const std::vector<double>& inValue,
																				const SM_NAMESPACE::SM_NumberArrayProperties& inNumberArrProp) {
		if (inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		pIMgjsonStaticDataNumArr ptr;
		HostCreateStaticDataNumArr(inConverter, &ptr, inMatchName.c_str(), inDisplayName.c_str());
		if(!ptr)
			return spIMgjsonStaticDataNumArr();

		spIMgjsonStaticDataNumArr  spProxy = spIMgjsonStaticDataNumArr();
		TRY_BLOCK
		spProxy = shared_ptr< IMgjsonStaticDataNumArr >(new IMgjsonStaticDataNumArrProxy(ptr));
		CATCH_RELEASE_THROW

		SM_PluginResult error;

		if (inNumberArrProp.mCount != 0)
		{
			if(inNumberArrProp.mCount > 3 || inNumberArrProp.mCount != inValue.size())
				THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidNumberArrayCount)
		}

		if( inValue.size() > 3 ){
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSM_InvalidValue);
		}

		if( inNumberArrProp.mDisplayNames.size() > 0 && inNumberArrProp.mDisplayNames.size() != inValue.size()){
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidDisplayNames)
		}
		if(inNumberArrProp.mRanges.size() > 0 && inNumberArrProp.mRanges.size() != inValue.size()){
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidRangesProperty)
		}
		
		std::vector<double>::const_iterator itr = inValue.begin();
		for (; itr != inValue.end(); itr++)
		{
			SM_PluginResult error;
			spProxy->addElement(*itr, &error);
			PROPOGATE_PLUGIN_EXCEPTION(error);

		}
		
		SM_NAMESPACE::SM_NumberArrayRange vecRange = inNumberArrProp.mRanges;
		if(inNumberArrProp.mRanges.empty())
		{
			vecRange.assign(inValue.size(), SM_NAMESPACE::SM_NumberRange());
		}

		SM_NAMESPACE::SM_NumberArrayRange::const_iterator it = vecRange.begin();
		for(; it != vecRange.end(); it++)
		{
			SM_PluginResult errorNumRange;
			spProxy->setNumberRange(( *it ).mLegal.mMinimum, ( *it ).mLegal.mMaximum, ( *it ).mOccuring.mMinimum, ( *it ).mOccuring.mMaximum, &errorNumRange);
			PROPOGATE_PLUGIN_EXCEPTION(errorNumRange);
		}

		SM_NAMESPACE::SM_StringArray::const_iterator iter = inNumberArrProp.mDisplayNames.begin();
		for (; iter != inNumberArrProp.mDisplayNames.end(); iter++)
		{
			SM_PluginResult errorArrDispNames;
			spProxy->addDisplayName((*iter).c_str(), &errorArrDispNames);
			PROPOGATE_PLUGIN_EXCEPTION(errorArrDispNames)
		}

		SM_PluginResult errorArrPoint;
		spProxy->setIsPoint(inNumberArrProp.mIsPoint, &errorArrPoint);
		PROPOGATE_PLUGIN_EXCEPTION(errorArrPoint);

		SM_PluginResult errorNumPattern;
		spProxy->setNumberPattern(inNumberArrProp.mPattern.mDigitsInteger, inNumberArrProp.mPattern.mDigitsDecimal, inNumberArrProp.mPattern.mIsSigned, &errorNumPattern);
		PROPOGATE_PLUGIN_EXCEPTION(errorNumPattern);

		//setArrCount should be the last call from this function to denote that all values have been passed
		SM_PluginResult errorArrCnt;
		spProxy->setArrCount(inValue.size(), &errorArrCnt);
		PROPOGATE_PLUGIN_EXCEPTION(errorArrCnt);

		return spProxy;

	}
}
