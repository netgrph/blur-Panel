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
	class IMgjsonDynamicDataNumProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class IMgjsonDynamicDataNumProxy;

#include "IMgjsonDynamicDataNum.h"


namespace SM_PLUGIN
{
#if SM_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4800 )
#endif
	class IMgjsonDynamicDataNumProxy:public virtual IMgjsonDynamicDataNum
	{
	public:
		IMgjsonDynamicDataNumProxy(pIMgjsonDynamicDataNum ptr)
			: mRawPtr(ptr)
		{
			mRawPtr->Acquire();
		}

		~IMgjsonDynamicDataNumProxy() NOTHROW
		{
			mRawPtr->Release();
		}

		pISharedObject_I APICALL GetISharedObject_I() NOTHROW
		{
			return mRawPtr->GetISharedObject_I();
		}
		
		virtual bool APICALL AddSample(const SM_NAMESPACE::SM_TimeValue& inTime, const double& inVal)
		{
			SM_PluginResult error;
			SM_NAMESPACE::SM_Bool result = 0;
			if (inTime.which() == SM_NAMESPACE::kSM_TimeValueSMPTE)
			{
				SM_NAMESPACE::SM_DateTimeSMPTE time = boost::get<SM_NAMESPACE::SM_DateTimeSMPTE>(inTime);
				result =  mRawPtr->addSampleSMPTE(time.hour, time.minute, time.second, time.frame, inVal, &error);
			}
			else if(inTime.which() == SM_NAMESPACE::kSM_TimeValueUTC)
			{
				SM_NAMESPACE::SM_DateTimeUTC time = boost::get<SM_NAMESPACE::SM_DateTimeUTC>(inTime);
				result = mRawPtr->addSampleUTC(time.year, time.month, time.day, time.hour,
					time.minute, time.second, time.tzSign,
					time.tzHour, time.tzMinute, time.secondFraction, inVal, &error);
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
			mRawPtr->setNumberPattern(digitsInteger, digitsDecimal, isSigned , errResultP);
		}
		virtual void APICALL setNumberRange(double minLegal, double maxLegal, double minOccuring, double maxOccuring, void* errResultP) NOTHROW
		{
			mRawPtr->setNumberRange(minLegal, maxLegal, minOccuring, maxOccuring, errResultP);
		}
		virtual SM_NAMESPACE::SM_Bool APICALL addSampleSMPTE(SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec, SM_NAMESPACE::SM_Int32 frame, double val, void* errResultP) NOTHROW
		{
			assert(false);
			return mRawPtr->addSampleSMPTE(hour, min, sec, frame, val, errResultP);
		}
		virtual SM_NAMESPACE::SM_Bool APICALL addSampleUTC(SM_NAMESPACE::SM_Int32 year, SM_NAMESPACE::SM_Int32 month, SM_NAMESPACE::SM_Int32 day,
			SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec,
			SM_NAMESPACE::SM_Int8 tzSign, SM_NAMESPACE::SM_Int32 tzHour,
			SM_NAMESPACE::SM_Int32 tzMinute, SM_NAMESPACE::SM_Int32 secondFraction,
			double val, void* errResultP) NOTHROW
		{
			assert(false);
			return mRawPtr->addSampleUTC(year, month, day,
				hour, min, sec,
				tzSign, tzHour,
				tzMinute, secondFraction,
				val, errResultP);
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
		pIMgjsonDynamicDataNum mRawPtr;
	};

	spIMgjsonDynamicDataNum IMgjsonDynamicDataNum_v1::CreateDynamicDataNum(ConverterRef inConverter, const SM_NAMESPACE::SM_DynamicDataFields& inDynamicDataFields,
		const SM_NAMESPACE::SM_NumberProperties& inNumberProp) 
	{
		if (inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		 pIMgjsonDynamicDataNum ptr;
		 HostCreateDynamicDataNum(inConverter, &ptr);
		 if (!ptr)
			 return spIMgjsonDynamicDataNum();
		 spIMgjsonDynamicDataNum spProxy = spIMgjsonDynamicDataNum();
		 TRY_BLOCK
			spProxy = shared_ptr< IMgjsonDynamicDataNum >(new IMgjsonDynamicDataNumProxy(ptr));
		 CATCH_RELEASE_THROW

		 SM_PluginResult errorInfo;
		 spProxy->setDynamicDataInfo(inDynamicDataFields.mDisplayName.c_str(), inDynamicDataFields.mMatchName.c_str(), inDynamicDataFields.mSampleSetID.c_str(), inDynamicDataFields.mSampleCount, static_cast<SM_NAMESPACE::SM_Uns32>(inDynamicDataFields.mInterpolationType), inDynamicDataFields.mHasExpectedSampleFrequencyB, inDynamicDataFields.mExpectedMaxInterSampleDuration.c_str(), &errorInfo);
		 PROPOGATE_PLUGIN_EXCEPTION(errorInfo);

		 SM_PluginResult errorNumPattern;
		 spProxy->setNumberPattern(inNumberProp.mPattern.mDigitsInteger, inNumberProp.mPattern.mDigitsDecimal, inNumberProp.mPattern.mIsSigned, &errorNumPattern);
		 PROPOGATE_PLUGIN_EXCEPTION(errorNumPattern);

		 SM_PluginResult errorNumRange;
		 spProxy->setNumberRange(inNumberProp.mRange.mLegal.mMinimum, inNumberProp.mRange.mLegal.mMaximum, inNumberProp.mRange.mOccuring.mMinimum, inNumberProp.mRange.mOccuring.mMaximum, &errorNumRange);
		 PROPOGATE_PLUGIN_EXCEPTION(errorNumRange);
		 
		 return spProxy;
	}
#if SM_WinBuild
	#pragma warning( pop )
#endif
}

