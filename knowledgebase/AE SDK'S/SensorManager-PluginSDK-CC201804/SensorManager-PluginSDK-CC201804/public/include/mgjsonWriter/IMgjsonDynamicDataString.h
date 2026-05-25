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

#ifndef __IMgjsonDynamicDataString_H__
#define __IMgjsonDynamicDataString_H__

/**************************************************************************
* @file IMgjsonDynamicDataString.h
* @brief This class is derived from IMgjsonDynamicData.
* It would contain dynamic data info and sample values of type paddedString
***************************************************************************/

#include "IMgjsonDynamicData.h"
#include "SM_PluginTypes.h"
#include "ISharedObject.h"
#include "SM_FwdDeclarations.h"

namespace SM_PLUGIN {

	class IMgjsonDynamicDataString_v1 : public virtual IMgjsonDynamicData_v1,
								  	 public virtual ISharedObject 
	{

	public:

		//!
		//! @brief Creates a DynamicData node of type paddedString.
		//! The node must be later added to an MGJSONGroup or MGJSONRoot to be serialized in the MGJSON file.
		//!                           
		//! @param inConverter         this pointer of the converter instance
		//! @param inDynamicDataFields struct containing Dynamic Data Fields associated with this node. mDisplayName,
		//!                            mMatchName, mSampleSetID,  mInterpolationType and mHasExpectedSampleFrequencyB are mandatory.
		//!                            mSampleCount, mExpectedMaxInterSampleDuration are optional. If mSampleCount is provided with 
		//!                            default value of 0, then the calculated count of valid added samples is serialized. Otherwise the
		//!                            provided value is serialized.
		//!                            mTemporalExtent is also optional and not required. It is not serialized in MGJSON.
		//! @param inStringProp        struct containing StringProperties associated with this node. It is optional. The properties 
		//!                            would be calculated if not provided. Otherwise the provided properties are used and serialized.
		//! 
		//! @return                    A shared pointer to a SM_PLUGIN::IMgjsonDynamicDataString object.
		//! \attention SM_PluginException is thrown in case
		//!			   - inConverter is NULL
		//!            - inDynamicFileInfo is not provided in \c CreateRoot() signifying that dynamic data would not be present.
		//!            - \c CreateRoot() with this inConverter is not called before this call. 
		//!		       - mMatchName is not valid ASCII
		//!            - mInterpolationType is not a valid value
		//!		       - One of mMaxLen and mMaxLenValDigit is provided and other is not. 
		//! \note      - It is advised to provide the value for mEventMarkerB else it is assumed to be false.
		//!
		static spIMgjsonDynamicDataString CreateDynamicDataString(ConverterRef inConverter, const SM_NAMESPACE::SM_DynamicDataFields& inDynamicDataFields,
																	const SM_NAMESPACE::SM_StringProperties& inStringProp = SM_NAMESPACE::SM_StringProperties());
	

		//!
		//! @brief Adds the time-value pair to the DynamicData node
		//! After adding all the sample to the DynamicData node, Commit() API must be called in order to serialize the samples to the MGJSON file.
		//!                           
		//! @param inTime              One of TimeUTC or TimeSMPTE structures. The time format must be same as that given in inDynamicFileInfo 
		//!                            in \c CreateRoot().
		//! @param inVal               Value to be serialized as paddedString corresponding to the timestamp. 
		//!                           
		//! @return                    A bool value; true in case the sample is added successfully. False is returned in case
		//!                            the time format or properties of time such as precisionLength etc differs from the one set 
		//!                            in inDynamicFileInfo in MGJSONRoot 
		//!                            In case false is returned, sample is not added.
		//!
		//! \note      In case SM_PluginException is thrown, DynamicData node is marked as dirty and is invalidated. 
		//!            No operation should be performed on a dirty node, neither should it be added in MGJSONRoot or MGJSONGroup.
		//!            In case a dirty node is already added, then it would not be serailzed on the Commit() call of IMGJSONRoot.
		//! \attention SM_PluginException is thrown in case
		//!            - There is a failure in calculation of properties.
		//!		       - There is a failure in storing/serialization of samples
		//!			   - The node is already dirty
		//!
		virtual bool APICALL AddSample(const SM_NAMESPACE::SM_TimeValue& inTime, const SM_NAMESPACE::SM_UTF8String& inVal) = 0;

		static SM_NAMESPACE::SM_Uns32 GetInterfaceVersion() { return 1; }

	protected:

		// protected virtual destructor.
		virtual ~IMgjsonDynamicDataString_v1() NOTHROW {}

		virtual void APICALL setStringProperties(SM_NAMESPACE::SM_Uns64 inMaxLen, SM_NAMESPACE::SM_Uns64 inMaxLenValDigit,
														SM_NAMESPACE::SM_Bool inEventMarkerB, void* errResultP) NOTHROW = 0;
		virtual SM_NAMESPACE::SM_Bool APICALL addSampleSMPTE(SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec,
															SM_NAMESPACE::SM_Int32 frame, SM_NAMESPACE::SM_StringPtr val, void* errResultP) NOTHROW = 0;
		virtual SM_NAMESPACE::SM_Bool APICALL addSampleUTC(SM_NAMESPACE::SM_Int32 year, SM_NAMESPACE::SM_Int32 month, SM_NAMESPACE::SM_Int32 day,
															SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec,
															SM_NAMESPACE::SM_Int8 tzSign, SM_NAMESPACE::SM_Int32 tzHour,
															SM_NAMESPACE::SM_Int32 tzMinute, SM_NAMESPACE::SM_Int32 secondFraction,
															SM_NAMESPACE::SM_StringPtr val, void* errResultP) NOTHROW = 0;

#ifdef REQUIRED_FRIEND_CLASS
		MAKE_CLASS_FRIEND
#endif

	};

}

#endif /* __IMgjsonDynamicDataString_H__*/
