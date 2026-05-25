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
	class IMgjsonDynamicDataNumArrProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class IMgjsonDynamicDataNumArrProxy;

#include "IMgjsonDynamicDataNumArr.h"


namespace SM_PLUGIN
{
#if SM_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4800 )
#endif
	class IMgjsonDynamicDataNumArrProxy :public virtual IMgjsonDynamicDataNumArr
	{
	public:
		IMgjsonDynamicDataNumArrProxy(pIMgjsonDynamicDataNumArr ptr)
			: mRawPtr(ptr)
		{
			mRawPtr->Acquire();
		}

		~IMgjsonDynamicDataNumArrProxy() NOTHROW
		{
			mRawPtr->Release();
		}

		pISharedObject_I APICALL GetISharedObject_I() NOTHROW
		{
			return mRawPtr->GetISharedObject_I();
		}

		virtual bool APICALL AddSample(const SM_NAMESPACE::SM_TimeValue& inTime, const std::vector<double>& inVal)
		{
			std::vector<double>::const_iterator it = inVal.begin();
			for (; it != inVal.end(); it++)
			{
				SM_PluginResult error;
				mRawPtr->addElement(*it, &error);
				PROPOGATE_PLUGIN_EXCEPTION(error)
			}

			SM_PluginResult error;
			SM_NAMESPACE::SM_Bool result = 0;
			if (inTime.which() == SM_NAMESPACE::kSM_TimeValueSMPTE )
			{
				SM_NAMESPACE::SM_DateTimeSMPTE time = boost::get<SM_NAMESPACE::SM_DateTimeSMPTE>(inTime);
				result = mRawPtr->addTimeSMPTE(time.hour, time.minute, time.second, time.frame, &error);
			}
			else if (inTime.which() == SM_NAMESPACE::kSM_TimeValueUTC )
			{
				SM_NAMESPACE::SM_DateTimeUTC time = boost::get<SM_NAMESPACE::SM_DateTimeUTC>(inTime);
				result = mRawPtr->addTimeUTC(time.year, time.month, time.day, time.hour,
												time.minute, time.second, time.tzSign,
												time.tzHour, time.tzMinute, time.secondFraction, &error);
			}
			else
			{
				THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidTimeType)
			}
			PROPOGATE_PLUGIN_EXCEPTION(error)
			return ConvertSM_BoolToBool(result);
		}

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

		virtual void APICALL setDisplayName(SM_NAMESPACE::SM_StringPtr displayName, void* errResultP) NOTHROW
		{
			mRawPtr->setDisplayName(displayName, errResultP);
		}

		virtual SM_NAMESPACE::SM_Bool APICALL addElement(double val, void* errResultP) NOTHROW
		{
			assert(false);
			return mRawPtr->addElement(val, errResultP);
		}

		virtual SM_NAMESPACE::SM_Bool APICALL addTimeSMPTE(SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec, SM_NAMESPACE::SM_Int32 frame, void* errResultP) NOTHROW
		{
			assert(false);
			return mRawPtr->addTimeSMPTE(hour, min, sec, frame, errResultP);
		}

		virtual SM_NAMESPACE::SM_Bool APICALL addTimeUTC(SM_NAMESPACE::SM_Int32 year, SM_NAMESPACE::SM_Int32 month, SM_NAMESPACE::SM_Int32 day,
			SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec,
			SM_NAMESPACE::SM_Int8 tzSign, SM_NAMESPACE::SM_Int32 tzHour,
			SM_NAMESPACE::SM_Int32 tzMinute, SM_NAMESPACE::SM_Int32 secondFraction,
			 void* errResultP) NOTHROW
		{
			assert(false);
			return mRawPtr->addTimeUTC(year, month, day,
				hour, min, sec,
				tzSign, tzHour,
				tzMinute, secondFraction, errResultP);
		}

		virtual SM_NAMESPACE::eDataValueType APICALL GetType() const NOTHROW
		{
			SM_NAMESPACE::SM_Uns32 type = mRawPtr->GetType();
			return static_cast<SM_NAMESPACE::eDataValueType>(type);

		}

		virtual void APICALL SetSampleCount(SM_NAMESPACE::SM_Uns64 inSampleCount) NOTHROW
		{
			mRawPtr->SetSampleCount(inSampleCount);
		}

		virtual SM_NAMESPACE::SM_Uns64 APICALL GetSampleCount() const NOTHROW
		{
			return mRawPtr->GetSampleCount();
		}
		
		virtual bool APICALL Commit()  {
			SM_PluginResult error;
			SM_NAMESPACE::SM_Bool committed = mRawPtr->commit(&error);
			PROPOGATE_PLUGIN_EXCEPTION(error)
			return ConvertSM_BoolToBool(committed);
		}

		virtual void APICALL setDynamicDataInfo(SM_NAMESPACE::SM_StringPtr displayName, SM_NAMESPACE::SM_StringPtr matchName, SM_NAMESPACE::SM_StringPtr sampleSetID, SM_NAMESPACE::SM_Uns64 sampleCount, SM_NAMESPACE::SM_Uns32 interpolationType, SM_NAMESPACE::SM_Bool hasExpectedSampleFrequencyB, SM_NAMESPACE::SM_StringPtr expectedMaxInterSampleDuration, void* errResultP) NOTHROW {
			mRawPtr->setDynamicDataInfo(displayName, matchName, sampleSetID, sampleCount, interpolationType, hasExpectedSampleFrequencyB, expectedMaxInterSampleDuration, errResultP);
		}

		virtual void APICALL Acquire() const NOTHROW
		{
			assert(false);
		}
		virtual void APICALL Release() const NOTHROW
		{
			assert(false);
		}

		pIMgjsonDynamicData APICALL GetActualIMgjsonDynamicData() NOTHROW { return mRawPtr; }

		virtual  SM_NAMESPACE::SM_Bool APICALL commit(void* errResultP) NOTHROW
		{
			assert(false);
			return mRawPtr->commit(errResultP);
		}

	private:
		pIMgjsonDynamicDataNumArr mRawPtr;
	};

	spIMgjsonDynamicDataNumArr IMgjsonDynamicDataNumArr_v1::CreateDynamicDataNumArr(ConverterRef inConverter, const SM_NAMESPACE::SM_DynamicDataFields& inDynamicDataFields,
		const SM_NAMESPACE::SM_NumberArrayProperties& inNumberArrProp)
	{
		if (inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		pIMgjsonDynamicDataNumArr ptr;
		HostCreateDynamicDataNumArr(inConverter, &ptr);
		if (!ptr)
			return spIMgjsonDynamicDataNumArr();

		spIMgjsonDynamicDataNumArr spProxy = spIMgjsonDynamicDataNumArr();
		
		TRY_BLOCK
			spProxy = std::shared_ptr< IMgjsonDynamicDataNumArr >(new IMgjsonDynamicDataNumArrProxy(ptr));
		CATCH_RELEASE_THROW



		SM_PluginResult errorInfo;
		spProxy->setDynamicDataInfo(inDynamicDataFields.mDisplayName.c_str(), inDynamicDataFields.mMatchName.c_str(), inDynamicDataFields.mSampleSetID.c_str(), inDynamicDataFields.mSampleCount, static_cast<SM_NAMESPACE::SM_Uns32>(inDynamicDataFields.mInterpolationType), inDynamicDataFields.mHasExpectedSampleFrequencyB, inDynamicDataFields.mExpectedMaxInterSampleDuration.c_str(), &errorInfo);
		PROPOGATE_PLUGIN_EXCEPTION(errorInfo);

		if (inNumberArrProp.mCount == 0 || inNumberArrProp.mCount > 3)
		{
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidNumberArrayCount)
		}

		SM_PluginResult errorArrCnt;
		spProxy->setArrCount(inNumberArrProp.mCount, &errorArrCnt);
		PROPOGATE_PLUGIN_EXCEPTION(errorArrCnt)

		SM_PluginResult errorArrPoint;
		spProxy->setIsPoint(inNumberArrProp.mIsPoint, &errorArrPoint);
		PROPOGATE_PLUGIN_EXCEPTION(errorArrPoint)

		SM_PluginResult errorNumPattern;
		spProxy->setNumberPattern(inNumberArrProp.mPattern.mDigitsInteger, inNumberArrProp.mPattern.mDigitsDecimal, inNumberArrProp.mPattern.mIsSigned, &errorNumPattern);
		PROPOGATE_PLUGIN_EXCEPTION(errorNumPattern)

		
		if (!inNumberArrProp.mRanges.empty() && inNumberArrProp.mRanges.size() != inNumberArrProp.mCount)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidNumberArrayRangeCount)
		
		SM_NAMESPACE::SM_NumberArrayRange vecRange = inNumberArrProp.mRanges;
		if (inNumberArrProp.mRanges.empty())
		{
			vecRange.assign(inNumberArrProp.mCount, SM_NAMESPACE::SM_NumberRange());
		}

		SM_NAMESPACE::SM_NumberArrayRange::const_iterator it = vecRange.begin();
		for (; it != vecRange.end(); it++)
		{
			SM_PluginResult errorNumRange;
			spProxy->setNumberRange((*it).mLegal.mMinimum, (*it).mLegal.mMaximum, (*it).mOccuring.mMinimum, (*it).mOccuring.mMaximum, &errorNumRange);
			PROPOGATE_PLUGIN_EXCEPTION(errorNumRange)
		}

		if (!inNumberArrProp.mDisplayNames.empty() && inNumberArrProp.mDisplayNames.size() != inNumberArrProp.mCount)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidNumberArrayDisplayNameCount)

		SM_NAMESPACE::SM_StringArray::const_iterator iter = inNumberArrProp.mDisplayNames.begin();
		for (; iter != inNumberArrProp.mDisplayNames.end(); iter++)
		{
			SM_PluginResult errorArrDispNames;
			spProxy->setDisplayName((*iter).c_str(), &errorArrDispNames);
			PROPOGATE_PLUGIN_EXCEPTION(errorArrDispNames)
		}
		
		return spProxy;
	}
#if SM_WinBuild
	#pragma warning( pop )
#endif
}
