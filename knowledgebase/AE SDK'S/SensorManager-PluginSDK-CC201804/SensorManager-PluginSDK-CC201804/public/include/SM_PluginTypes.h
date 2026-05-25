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

#ifndef __SM_PLUGINTYPES_H__
#define __SM_PLUGINTYPES_H__

#include "boost/variant.hpp"
#include "SM_Const.h"
#include "SM_DataTypesCommon.h"
#include "SM_PluginHandler.h"

#include <vector>

SM_NAMESPACE_BEGIN

	// ================================================================================================
	/// \struct SM_DateTimeUTC
	/// \brief struct to hold date and time in UTC format
	///

	typedef struct SM_DateTimeUTC {

		/// year
		SM_NAMESPACE::SM_Int32 year;

		/// month in the range 1..12.
		SM_NAMESPACE::SM_Int32 month;

		/// day of the month in the range 1..31.
		SM_NAMESPACE::SM_Int32 day;

		/// hour in the range 0..23.
		SM_NAMESPACE::SM_Int32 hour;

		/// minute in the range 0..59.
		SM_NAMESPACE::SM_Int32 minute;

		/// second in the range 0..59.
		SM_NAMESPACE::SM_Int32 second;

		/// "sign" of the time zone, +/-/Z .
		SM_NAMESPACE::SM_Int8 tzSign;

		/// time zone hour in the range 0..23.
		SM_NAMESPACE::SM_Int32 tzHour;

		/// time zone minute in the range 0..59.
		SM_NAMESPACE::SM_Int32 tzMinute;

		/// fraction within a second.
		SM_NAMESPACE::SM_Int32 secondFraction;

#if __cplusplus

		SM_DateTimeUTC() : year(0), month(0), day(0), hour(0), minute(0), second(0),
			 tzSign(0), tzHour(0), tzMinute(0), secondFraction(0) {}
		//parameterized constructor
		SM_DateTimeUTC(SM_NAMESPACE::SM_Int32 inYear, SM_NAMESPACE::SM_Int32 inMonth, SM_NAMESPACE::SM_Int32 inDay, SM_NAMESPACE::SM_Int32 inHour,
			SM_NAMESPACE::SM_Int32 inMinute, SM_NAMESPACE::SM_Int32 inSec, SM_NAMESPACE::SM_Int8 inTzSign, SM_NAMESPACE::SM_Int32 inTzHour,
			SM_NAMESPACE::SM_Int32 inTzMin, SM_NAMESPACE::SM_Int32 inSecFrac) :year(inYear), month(inMonth),
			day(inDay), hour(inHour), minute(inMinute), second(inSec), tzSign(inTzSign), tzHour(inTzHour),
			tzMinute(inTzMin), secondFraction(inSecFrac) {}
#endif

	} TimeUTC;

	// ================================================================================================
	/// \struct SM_DateTimeSMPTE
	/// \brief struct to hold date and time in SMPTE
	///


	typedef struct SM_DateTimeSMPTE {

		/// hour in the range 0..23.
		SM_NAMESPACE::SM_Int32 hour;

		/// minute in the range 0..59.
		SM_NAMESPACE::SM_Int32 minute;

		/// second in the range 0..59.
		SM_NAMESPACE::SM_Int32 second;

		/// frames
		SM_NAMESPACE::SM_Int32 frame;

#if __cplusplus
		SM_DateTimeSMPTE() :hour(0), minute(0), second(0), frame(0) {}
		SM_DateTimeSMPTE(SM_NAMESPACE::SM_Int32 inHour, SM_NAMESPACE::SM_Int32 inMin, SM_NAMESPACE::SM_Int32 inSec, SM_NAMESPACE::SM_Int32 inFrame)
			:hour(inHour), minute(inMin), second(inSec), frame(inFrame) {}
#endif
	} TimeSMPTE;

	typedef boost::variant < TimeSMPTE, TimeUTC > SM_TimeValue;

	// ================================================================================================
	/// \struct SM_UTCInfo
	/// \brief Structure to represent information about UTC time.
	///

	struct SM_UTCInfo {

		/// represents the precision of the fractional part of a second in time string.
		SM_NAMESPACE::SM_Uns16 mUTCPrecision;

		/// represents whether timezone info is present in file or not.
		SM_NAMESPACE::SM_Bool mIsGMT;

#if __cplusplus

		//default constructor
		SM_UTCInfo() : mUTCPrecision(0), mIsGMT(false) {}

		//Parameterized constructor
		SM_UTCInfo(SM_NAMESPACE::SM_Uns16 inUTCPrecision, SM_NAMESPACE::SM_Bool inIsGMT) :
			mUTCPrecision(inUTCPrecision), mIsGMT(inIsGMT) {}

#endif
	};

	// ================================================================================================
	/// \struct SM_TimeCodeInfo
	/// \brief Structure to represent information about SMPTE timecode.
	///
	struct SM_TimeCodeInfo {
		/// rate
		SM_NAMESPACE::SM_Time mFrameRate;
		/// drop/non-drop
		SM_NAMESPACE::SM_Bool mIsDrop;

#if __cplusplus
        
        //default constructor
        SM_TimeCodeInfo() : mFrameRate(), mIsDrop(false) {}
        
        //Parameterized constructor
        SM_TimeCodeInfo(SM_NAMESPACE::SM_Time inFrameRate, SM_NAMESPACE::SM_Bool inIsDrop) :
        mFrameRate(inFrameRate), mIsDrop(inIsDrop) {}
        
#endif
	};

	typedef boost::variant < SM_UTCInfo, SM_TimeCodeInfo > SM_TimeInfo;
    
    typedef enum
    {
		
		kSM_UTCInfo = 0,
        
		kSM_TimeCodeInfo = 1,
        
        //!Indicates invalid value
        kSM_TimeInfo_INVALID
    }eTimeInfo;

    // ================================================================================================
    /// \struct SM_DynamicFileInfo
    /// \brief Structure to represent information about dynamic data info at file level.
    /// This structure is required only if mgjson contains dynamic data.

    struct SM_DynamicFileInfo {
		
		//! Mandatory
        SM_TimeInfo mTimeInfo;
		//! Optional
        SM_NAMESPACE::SM_VideoSyncTimeList mVideoSyncTimeList;
#if __cplusplus
        
        /// default constructor
        SM_DynamicFileInfo (SM_TimeInfo inTimeInfo = SM_TimeInfo(), SM_NAMESPACE::SM_VideoSyncTimeList inVideoSyncTimeList = SM_NAMESPACE::SM_VideoSyncTimeList()): mTimeInfo(inTimeInfo), mVideoSyncTimeList(inVideoSyncTimeList) {}

#endif
        
    };

	typedef enum
	{
		kSM_TimeValueSMPTE = 0,
	
		kSM_TimeValueUTC
	
	}eTimeValueFormat;


SM_NAMESPACE_END

#endif /* __SM_PLUGINTYPES_H__ */
