#ifndef __SM_DATATYPESCOMMON_H__
#define __SM_DATATYPESCOMMON_H__

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

// =================================================================================================
/// \file SM_DataTypesCommon.h
/// \brief class that defines structures to store different values related to sensor manager.
// =================================================================================================

#include "SM_Const.h"
#include <vector>
#include <string>
#include "boost/variant.hpp"
#include <float.h>

#if __cplusplus
extern "C" {
#endif
    
    SM_NAMESPACE_BEGIN
	
	#define NUMOFELEMENTSINRANGE 4
	
	// ================================================================================================

	/// \struct SM_NumberMinMax
	/// \brief Structure to represent number min max.
	///

	struct SM_NumberMinMax {

		/// minimum
		double mMinimum;

		/// maximum
		double mMaximum;

#if __cplusplus
		//default constructor
		SM_NumberMinMax ( ) : mMinimum ( DBL_MAX ) , mMaximum ( -DBL_MAX ) {}

		//Parameterized constructor
		SM_NumberMinMax ( double inMin , double inMax ) :
						  mMinimum ( inMin ) , mMaximum ( inMax ) {}
#endif

	};

	
	// ================================================================================================

	/// \struct SM_NumberRange
	/// \brief Structure to represent number range.
	///

	struct SM_NumberRange {

		/// legal range of a number
		SM_NumberMinMax mLegal;

		/// occuring range of a number
		SM_NumberMinMax mOccuring;

#if __cplusplus
		//default constructor
		SM_NumberRange ( ) : mLegal ( ) , mOccuring ( ) {}

		//Parameterized constructor
		SM_NumberRange ( double* inNumRangeP){
			this->mLegal.mMinimum = *(inNumRangeP);
			this->mLegal.mMaximum = *(++inNumRangeP);
			this->mOccuring.mMinimum = *(++inNumRangeP);
			this->mOccuring.mMaximum = *(++inNumRangeP);
		}
		//Parameterized constructor
		SM_NumberRange ( SM_NumberMinMax inLegal , SM_NumberMinMax inOccuring = SM_NumberMinMax()) :
						 mLegal ( inLegal ) , mOccuring ( inOccuring ) {}
#endif

	};


	// =================================================================================================

	/// \struct SM_NumberPattern
	/// \brief Structure to represent numberPattern
	///

	struct SM_NumberPattern {

		/// number of digits before decimal
		SM_Uns64 mDigitsInteger;

		/// number of digits after decimal
		SM_Uns64 mDigitsDecimal;

		/// sign is present in number
		SM_Bool mIsSigned;

#if __cplusplus
		//default constructor
		SM_NumberPattern ( ) : mDigitsInteger (Max_SM_Uns64) , mDigitsDecimal (Max_SM_Uns64) , mIsSigned( 0 ) {}

		//Parameterized constructor
		SM_NumberPattern ( SM_Uns64 inDigitsInteger , SM_Uns64 inDigitsDecimal , SM_Bool inIsSigned ) :
						   mDigitsInteger ( inDigitsInteger ) , mDigitsDecimal ( inDigitsDecimal ) , mIsSigned ( inIsSigned ) {}
#endif

	};
	
	
	typedef std::vector<SM_NumberRange> SM_NumberArrayRange;
	typedef std::vector<SM_UTF8String> SM_StringArray;
	typedef std::vector<double> SM_NumberArray;

	
    // ================================================================================================
    
    /// \struct SM_NumberProperties
    /// \brief Structure to represent numberStringProperties.
    ///
    
    struct SM_NumberProperties {
        
        /// pattern of number
        SM_NumberPattern mPattern;
        
        /// range of number
        SM_NumberRange mRange;
        
#if __cplusplus
        //default constructor
        SM_NumberProperties ( ) : mPattern ( ) , mRange ( ) {}
        
        //Parameterized constructor
        SM_NumberProperties (SM_NumberRange inRange, SM_NumberPattern inPattern = SM_NumberPattern()) :
									mPattern ( inPattern ) , mRange ( inRange ) {}
#endif
        
    };
	
	
    // ================================================================================================
    
    /// \struct SM_StringProperties
    /// \brief Structure to represent paddedStringProperties.
    ///
    
    struct SM_StringProperties {
        
        /// maximum length of a padded string
        SM_Uns64 mMaxLen;
		
		/// digits required to represent mMaxLen
		SM_Uns64 mMaxLenValDigit;
		
		/// indicates that the string is an event marker
		SM_Bool mEventMarkerB;
		
#if __cplusplus
        //default constructor
        SM_StringProperties ( ) : mMaxLen ( 0 ), mMaxLenValDigit ( 0 ), mEventMarkerB ( 0 ) {}
        
        //Parameterized constructor
        SM_StringProperties (SM_Bool inEventMarkerB, SM_Uns64 inMaxLen = 0, SM_Uns64 inMaxLenValDigit = 0 ) :
									mMaxLen ( inMaxLen ), mMaxLenValDigit ( inMaxLenValDigit), mEventMarkerB(inEventMarkerB)  {}

		//assignment operator
		void operator= (const SM_StringProperties& inStrProp) {
			this->mMaxLen			=	inStrProp.mMaxLen;
			this->mMaxLenValDigit	=	inStrProp.mMaxLenValDigit;
			this->mEventMarkerB		=	inStrProp.mEventMarkerB;
		}
#endif
        
    };
    
    
	// ================================================================================================

	/// \struct SM_NumberArrayProperties
	/// \brief Structure to represent number array properties .
	///

	struct SM_NumberArrayProperties {

		/// pattern of all numbers in the array
		SM_NumberPattern mPattern;

		/// count of numbers in the array
		SM_Uns64 mCount;

		/// name used for elements of the array
		SM_StringArray mDisplayNames;

		/// range of elements of the array
		SM_NumberArrayRange mRanges;

		/// array should be treated as a point or not
		SM_Bool mIsPoint;
		
#if __cplusplus
		//default constructor
		SM_NumberArrayProperties ( ) : mPattern ( ) , mCount(0), mDisplayNames(), mRanges ( ), mIsPoint( 0 ) {}

		//Paramterized constructor
		SM_NumberArrayProperties(SM_Uns64 inCount, SM_Bool inIsPoint = 0, SM_NumberArrayRange inRanges = SM_NumberArrayRange(),
								 SM_StringArray inNames = SM_StringArray(), SM_NumberPattern inPattern = SM_NumberPattern()) :
								 mPattern(inPattern), mCount(inCount), mDisplayNames(inNames), mRanges(inRanges),
								 mIsPoint(inIsPoint) {}

		SM_NumberArrayProperties(SM_NumberPattern inPattern, SM_Uns64 inCount, char** inNamesP, SM_NAMESPACE::SM_Uns64 inNamesSize, double* inRangesP, SM_NAMESPACE::SM_Uns64 inRangeCount, SM_NAMESPACE::SM_Bool inIsPoint){
			
			this->mPattern = inPattern;
			this->mCount = inCount;
			this->mDisplayNames.assign(inNamesP, inNamesP + inNamesSize);
			this->mIsPoint = inIsPoint;
			
			for(SM_NAMESPACE::SM_Index i = 0; i < inRangeCount ; i++){
				this->mRanges.push_back(SM_NAMESPACE::SM_NumberRange(&inRangesP[i * NUMOFELEMENTSINRANGE]));
			}
		}
#endif

	};

	
	// ================================================================================================
	/// \struct SM_Time
	/// \brief Structure to represent time.
	///
	
	struct SM_Time {
		
		/// value
		SM_Int32 mValue;
		
		/// scale
		SM_Uns32 mScale;
		
#if __cplusplus
		//default constructor
		SM_Time ( ) : mValue ( 0 ) , mScale ( 1 ) {}
		
		//Parameterized constructor
		SM_Time ( SM_Int32 inValue , SM_Uns32 inScale ) :
				  mValue ( inValue ) , mScale ( inScale ) {}
#endif
		
	};
	
	
    // ================================================================================================
    
    /// \struct SM_TemporalExtent
    /// \brief Structure to define time range.
    ///
    
    struct SM_TemporalExtent {
        
        /// start time
        SM_Time mStartTime ;
        
        /// end time
        SM_Time mEndTime ;
        
#if __cplusplus
        
        //default constructor
        SM_TemporalExtent() : mStartTime(), mEndTime(){}
        
        //Parameterized constructor
        SM_TemporalExtent( SM_Time inStartTime, SM_Time inEndTime ) :
						  mStartTime ( inStartTime ) , mEndTime ( inEndTime ){}
#endif
        
    };

    
	// ================================================================================================
	
	/// \struct SM_DynamicDataFields
	/// \brief struct to hold dynamic data fields
	
	
	struct SM_DynamicDataFields {
		
		/// property name
		SM_UTF8String mDisplayName;
		
		/// An identifier with ascii alphanumeric characters and no leading numerals, should be unique within the file
		SM_AsciiString mMatchName;
		
		///Id to identify sample sets for this property, should be unique within the file
		SM_UTF8String mSampleSetID;
		
		///Count to represent total number of samples present in the sample set for this property.
		SM_Uns64 mSampleCount;
		
		///Interpolation Type
		eInterpolationType mInterpolationType;
		
		///true means minimum number of samples is expected
		SM_Bool mHasExpectedSampleFrequencyB;
		
		//Temporal Range
		SM_TemporalExtent mTemporalExtent;
		
		//Maximum expected duration allowed between adjacent samples
		SM_UTF8String mExpectedMaxInterSampleDuration;
		
#if __cplusplus
		//default constructor
		SM_DynamicDataFields() : mDisplayName(""), mMatchName(""), mSampleSetID(""),
		mSampleCount(0), mInterpolationType(kSM_InterpolationType_NONE),
		mHasExpectedSampleFrequencyB( 0 ),
		mTemporalExtent(), mExpectedMaxInterSampleDuration("") {}

		//parameterized constructor
		SM_DynamicDataFields(SM_UTF8String inDisplayName, SM_AsciiString inMatchName, SM_UTF8String inSampleSetID,
			 eInterpolationType inInterpolationType,
			SM_Bool inHasExpectedSampleFrequencyB, SM_Uns64 inSampleCount = 0, SM_UTF8String inExpectedMaxInterSampleDuration = "",
			SM_TemporalExtent inTemporalExtent = SM_TemporalExtent()) : mDisplayName(inDisplayName), mMatchName(inMatchName), mSampleSetID(inSampleSetID),
			mSampleCount(inSampleCount), mInterpolationType(inInterpolationType),
			mHasExpectedSampleFrequencyB(inHasExpectedSampleFrequencyB),
			mTemporalExtent(inTemporalExtent), mExpectedMaxInterSampleDuration(inExpectedMaxInterSampleDuration) {}
		
#endif
		
	};

	
	
	// ================================================================================================

	
	typedef boost::variant < SM_UTF8String , SM_Int32 > SM_SyncTime;

	// ================================================================================================
	/// \brief struct to hold sync time
	
	struct SM_SyncTimeValue {
	
		SM_SyncTime mTime;
		
		bool GetFrameNumber(SM_NAMESPACE::SM_Int32& outVal) const;
		bool GetTimeString(SM_UTF8String& outVal) const ;
		
		SM_SyncTimeValue() {
			mTime = 0;
		}

	};
	
	
	// ================================================================================================
	/// \brief struct to hold video sync time
	
	struct SM_VideoSyncTime {
		
		SM_UTF8String mFileName;
		eSyncTimeType mType;
		SM_SyncTimeValue mSyncTime;

#if __cplusplus
		
		SM_VideoSyncTime() : mFileName(""), mType(kSM_SyncTimetype_NONE), mSyncTime(){}
		
		SM_VideoSyncTime(const char* inFileName){
			
			this->mFileName.assign(inFileName);
			this->mType = kSM_SyncTimetype_NONE;
		}
		
		SM_VideoSyncTime(const char* inFileName, SM_NAMESPACE::SM_Int32& inVal){
			
			this->mFileName.assign(inFileName);
			this->mSyncTime.mTime = inVal;
			this->mType = kSM_SyncTime_FRAME;

		}
			
		SM_VideoSyncTime(const char* inFileName,
						 const char* inStrValue, SM_NAMESPACE::SM_StringLen& inSize) {
			
			this->mFileName = inFileName;
			this->mSyncTime.mTime = SM_UTF8String(inStrValue, inSize);
			this->mType = kSM_SyncTime_STRING;
		}
		
#endif
		
	};

	typedef std::vector <SM_VideoSyncTime> SM_VideoSyncTimeList;
    typedef SM_VideoSyncTimeList::iterator SM_VideoSyncTimeIterator;
    typedef SM_VideoSyncTimeList::const_iterator SM_ConstVideoSyncTimeIterator;


SM_NAMESPACE_END
    
#if __cplusplus
} // extern "C"
#endif

#endif /* __SM_DATATYPESCOMMON__ */
