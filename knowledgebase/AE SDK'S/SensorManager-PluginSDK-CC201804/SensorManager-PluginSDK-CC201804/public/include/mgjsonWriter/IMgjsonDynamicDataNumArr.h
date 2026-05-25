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

#ifndef __IMgjsonDynamicDataNumArr_H__
#define __IMgjsonDynamicDataNumArr_H__

/**************************************************************************
* @file IMgjsonDynamicDataNumArr.h
* @brief This class is derived from IMgjsonDynamicData.
* It would contain dynamic data info and sample values of type numberStringArray
***************************************************************************/

#include "IMgjsonDynamicData.h"
#include "SM_PluginTypes.h"
#include "ISharedObject.h"
#include "SM_FwdDeclarations.h"

namespace SM_PLUGIN {

	class IMgjsonDynamicDataNumArr_v1 :public virtual IMgjsonDynamicData_v1,
									public virtual ISharedObject
	{

	public:
		//!
		//! @brief Creates a DynamicData node of type numberStringArray. The maximum number of elements allowed in an number array is 3.
		//! The node must be later added to an MGJSONGroup or MGJSONRoot to be serialized in the MGJSON file.
		//!                           
		//! @param inConverter         this pointer of the converter instance
		//! @param inDynamicDataFields struct containing Dynamic Data Fields associated with this node. mDisplayName,  
		//!                            mMatchName, mSampleSetID, mInterpolationType and mHasExpectedSampleFrequencyB are mandatory.
		//!                            mSampleCount, mExpectedMaxInterSampleDuration are optional. If mSampleCount is provided with
		//!                            default value of 0, then the calculated count of valid added samples is serialized. 
		//!                            Otherwise the provided value is serialized.
		//!                            mTemporalExtent is also optional and not required. It is not serialized in MGJSON.
		//! @param inNumberArrayProp   struct containing NumberArrayProperties associated with this node. mCount is mandatory.
		//!                            All other properties are optional. The optional properties would be
		//!                            calculated if not provided. Otherwise the provided properties are used and serialized.
		//! 
		//! @return                    A shared pointer to a SM_PLUGIN::IMgjsonDynamicDataNumArr object.
		//! \attention SM_PluginException is thrown in case
		//!			   - inConverter is NULL
		//!            - inDynamicFileInfo is not provided in \c CreateRoot() signifying that dynamic data would not be present.
		//!            - \c CreateRoot() with this inConverter is not called before this call. 
		//!		       - mMatchName is not valid ASCII
		//!            - mInterpolationType is not a valid value
		//!            - if mCount is not provided and is left to default value of 0 or is greater than 3.
		//!		       - One of mDigitsInteger and mDigitsDecimal is provided and other is not. 
		//!            - if mRanges is not empty and the size differs from mCount.
		//!            - in case mRanges is not empty and both mMinimum and mMaximum of mOccuring are not provided
		//!              for all the elements.
		//!            - in case mRanges is not empty and both mMinimum and mMaximum of mLegal are not provided
		//!              for all the elements.
		//!            - if mDisplayNames is not empty and the size differs from mCount.
		//!		     
		//! \note      - It is advised to provide the Legal Range. If it is not provided, then it is made equal to the Occuring Range. 
		//!            - It is advised to provide the value of isPoint otherwise it is assumed to be false. 
		//!
		static spIMgjsonDynamicDataNumArr CreateDynamicDataNumArr(ConverterRef inConverter, const SM_NAMESPACE::SM_DynamicDataFields& inDynamicDataFields,
																	const SM_NAMESPACE::SM_NumberArrayProperties& inNumberArrayProp) ;

		//!
		//! @brief Adds the time-value pair to the DynamicData node
		//! After adding all the sample to the DynamicData node, Commit() API must be called in order to serialize the samples to the MGJSON file.
		//!                           
		//! @param inTime              One of TimeUTC or TimeSMPTE structures. The time format must be same as that given in
		//!                            inDynamicFileInfo in \c CreateRoot().
		//! @param inVal               Value to be serialized as numberArrayString corresponding to the timestamp. 
		//!                           
		//! @return                    A bool value; true in case the sample is added successfully. False is returned in case
		//!                            -the time format or properties of time such as precisionLength etc differs from the one 
		//!                             set in inDynamicFileInfo in MGJSONRoot
		//!                            -the size of inVal does not match mCount.
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
		virtual bool APICALL AddSample(const SM_NAMESPACE::SM_TimeValue& inTime, const std::vector<double>& inVal)  = 0;

		static SM_NAMESPACE::SM_Uns32	GetInterfaceVersion(){ return 1; }

	protected:

		// protected virtual destructor.
		virtual ~IMgjsonDynamicDataNumArr_v1() NOTHROW {}

		virtual void APICALL setNumberPattern(SM_NAMESPACE::SM_Uns64 digitsInteger, SM_NAMESPACE::SM_Uns64 digitsDecimal, SM_NAMESPACE::SM_Bool isSigned, void* errResultP) NOTHROW = 0;
		virtual void APICALL setNumberRange(double minLegal, double maxLegal, double minOccuring, double maxOccuring,  void* errResultP) NOTHROW = 0;
		virtual void APICALL setArrCount(SM_NAMESPACE::SM_Uns64 count, void * errResultP) NOTHROW = 0;
		virtual void APICALL setIsPoint(SM_NAMESPACE::SM_Bool isPoint, void * errResultP) NOTHROW = 0;
		virtual void APICALL setDisplayName(SM_NAMESPACE::SM_StringPtr displayName, void * errResultP) NOTHROW = 0;

		virtual SM_NAMESPACE::SM_Bool APICALL addTimeSMPTE(SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec,
															SM_NAMESPACE::SM_Int32 frame, void* errResultP) NOTHROW = 0;
		virtual SM_NAMESPACE::SM_Bool APICALL addTimeUTC(SM_NAMESPACE::SM_Int32 year, SM_NAMESPACE::SM_Int32 month, SM_NAMESPACE::SM_Int32 day,
														 SM_NAMESPACE::SM_Int32 hour, SM_NAMESPACE::SM_Int32 min, SM_NAMESPACE::SM_Int32 sec,
														 SM_NAMESPACE::SM_Int8 tzSign, SM_NAMESPACE::SM_Int32 tzHour,
														 SM_NAMESPACE::SM_Int32 tzMinute, SM_NAMESPACE::SM_Int32 secondFraction, void* errResultP) NOTHROW = 0;
		virtual SM_NAMESPACE::SM_Bool APICALL addElement(double val, void* errResultP) NOTHROW = 0;

#ifdef REQUIRED_FRIEND_CLASS
		MAKE_CLASS_FRIEND
#endif

	};

}/* __IMgjsonDynamicDataNumArr_H__*/

#endif
