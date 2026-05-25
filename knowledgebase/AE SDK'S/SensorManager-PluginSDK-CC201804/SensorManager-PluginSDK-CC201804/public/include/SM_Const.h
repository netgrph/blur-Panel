#ifndef __SM_CONST__
#define __SM_CONST__

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

#include "SM_Enviroment.h"
#include <stddef.h>
#include <string>
#include <vector>

#if SM_DebugBuild
#include <cassert>
#endif

#if SM_MacBuild || SM_WinBuild
#define SUPPORT_SHARED_POINTERS_IN_STD 1
#else
#define SUPPORT_SHARED_POINTERS_IN_STD 0
#endif

#if SUPPORT_SHARED_POINTERS_IN_STD
#include <memory>
#include <functional>
using std::shared_ptr;
using std::enable_shared_from_this;
using std::mem_fn;
#endif

#if SM_MacBuild 	// ! No stdint.h on Windows and some UNIXes.
#include <stdint.h>
#endif

#if __cplusplus
extern "C" {
#endif
    
#ifndef SM_NAMESPACE
#define SM_NAMESPACE ns_sensor
#endif
#ifndef SM_NAMESPACE_BEGIN
#define SM_NAMESPACE_BEGIN namespace SM_NAMESPACE {
#endif
#ifndef SM_NAMESPACE_END
#define SM_NAMESPACE_END }
#endif

    // =================================================================================================
    /// \file SM_Const.h
    /// \brief Common C/C++ types and constants for the Sensor Manager.
    // =================================================================================================
    
    // =================================================================================================
    // Basic types and constants
    // =========================
    
SM_NAMESPACE_BEGIN
    
#if SM_MacBuild
    
    typedef int8_t   SM_Int8;
    typedef int16_t  SM_Int16;
    typedef int32_t  SM_Int32;
    typedef int64_t  SM_Int64;
    
    typedef uint8_t  SM_Uns8;
    typedef uint16_t SM_Uns16;
    typedef uint32_t SM_Uns32;
    typedef uint64_t SM_Uns64;
    
#elif SM_WinBuild
    
    typedef signed char SM_Int8;
    typedef signed short SM_Int16;
    typedef signed long SM_Int32;
    typedef signed long long SM_Int64;
    
    typedef unsigned char SM_Uns8;
    typedef unsigned short SM_Uns16;
    typedef unsigned long SM_Uns32;
    typedef unsigned long long SM_Uns64;
#else
    
#error "SM environment error - must define one of SM_MacBuild or SM_WinBuild"
    
#endif
	
    
    #define ROOT_INDEX -1
	#define ROOT_MATCHNAME "dataOutline"

    typedef SM_Uns8 SM_Bool;
    
    // =================================================================================================
    
    /// \name General scalar types and constants
    /// @{
    
    /// \typedef SM_StringPtr
    /// \brief The type for input string parameters. A <tt>const char *</tt>, a null-terminated UTF-8
    /// string.
    
    /// \typedef SM_StringLen
    /// \brief The type for string length parameters. A 32-bit unsigned integer, as big as will be
    /// practically needed.
    
    /// \typedef SM_Index
    /// \brief The type for offsets and indices. A 32-bit signed integer. It is signed to allow -1 for
    /// loop termination.

    
    typedef const char *  SM_StringPtr;  // Points to a null terminated UTF-8 string.
	typedef std::string	SM_FileExtension;
#if SM_64
    typedef SM_Uns64	SM_STLSize;
	typedef SM_Uns64	SM_StringLen;
#else
	typedef SM_Uns32	SM_STLSize;
	typedef SM_Uns32	SM_StringLen;
#endif /*SM_64*/

    typedef SM_Int32      SM_Index;			// Signed, sometimes -1 is handy.
	typedef SM_StringPtr  SM_UTF8FilePath;
	typedef std::string   SM_UTF8String ;	// Indicates utf 8 encoded buffer
	typedef std::string   SM_AsciiString;	// Indicates ascii encoded buffer.
	typedef std::vector<SM_UTF8String>		SM_StringVec;

    static const SM_Uns32 kMaxEnumValue			( (SM_Uns32) 0xFFFFFFFF );
	static const SM_Uns64 Max_SM_Uns64			((SM_Uns64)0xFFFFFFFFFFFFFFFFLL);
	/// @}

    //!
    //! @brief Indicates various types of error codes within General Domain.
    //!
    typedef enum {
        //! Indicates no error
        kSM_None = 0,
        
        //! Indicates that parameters passed to function are not as expected.
        kSM_ParametersNotAsExpected = 1,
		
		//! Indicates that invalid input file path
		kSM_InvalidFilePath = 2,
		
		//!Indicates that input file path larger than the supported file path length.
		kSM_FilePathTooLarge = 3,
		
		//! Indicates that file not present at the given path.
		kSM_FileNotExist = 4,
		
		//!Indicates that the input file is corrupt.
		kSM_CorruptInputFile = 5,
		
		//!Indicates that file cannot be opened for processing.
		kSM_CannotOpenFile = 6,

		//!Indicates that file cannot be created
		kSM_CannotCreateFile = 7,

		//!Indicates that file cannot be closed
		kSM_CannotCloseFile = 8,

		//!Indicates that file cannot be deleted
		kSM_CannotDeleteFile = 9,

		//!Indicates file seek failure
		kSM_FileSeekFailure = 10,

		//!Indicates file read failure
		kSM_FileReadFailure = 11,

		//!Indicates that file path cannot be converted
		kSM_CannotConvertFilePath = 12,

		//!Indicates that file has permission issues
		kSM_FilePermissionError = 13,

		//!Indicates that temp file directory not present
		kSM_TempDirectoryFailure = 14,

		//!Indicates the temp file failure
		kSM_TempFileFailure = 15,

		//!Indicates that the folder cannot be opened
		kSM_CannotOpenFolder = 16,

		//!Indicates that the folder cannot be closed
		kSM_CannotCloseFolder = 17,
		
		//!Indicates that the folder cannot be deleted
		kSM_CannotDeleteFolder = 18,
		
        //! Indicates that some assertion has failed.
        kSM_AssertionFailure = 19,
        
        //! Indicates logic failure.
        kSM_LogicalError = 20,
        
        //! Indicates an internal failure.
        kSM_InternalFailure = 21,
        
        //! Indicates an external failure.
        kSM_ExternalFailure = 22,
        
        //! Indicates an unknown failure.
        kSM_UnknownFailure = 23,
        
        //! Indicates that functionality is not yet implemented.
        kSM_NotImplemented = 24,
        
        //!Indicates that provided format is not supported by sensor manager.
        kSM_FormatNotSupported = 25,
        
		//!Indicates that no file opens for the given handler.
		kSM_NoFileOpened = 26,
		
		//!Indicates that file handler is not created by Sensor manager instance.
		kSM_InvalidFileHandler = 27,
		
        //!Indicates that required field not present in file.
        kSM_MissingRequiredField = 28,
		
		//!Indicates that top level mandatory properties missing.
		kSM_MissingTopLevelProperties = 29,
		
		//!Indicates that property not defined at top level as per MGJSON schema version.
		kSM_InvalidTopLevelProperty = 30,
		
		//!Indicates that the property is present before its dependend property as per MGJSON schema.
		kSM_DependentPropertyNotFound = 31,
        
		//!Indicates error in parsing mgjson file.
		kSM_MGJSONParsingError = 32,
		
		//! Indicates that invalid SM_DataPath.
		kSM_InvalidSMDataPath = 33,
        
        //!Indicates that memory is not available
        kSM_MemoryNotAvailable = 34,
		
        //!Indicates that no dynamic node with input sample id is present in data outline.
		kSM_SampleIdNotpresent = 35,
		
		//!Indicates that dynamic info in outline for sample set is invalid.
		kSM_InvalidSampleSetInfo = 36,
		
		//!Indicates multiple properties with same sample id is present.
        kSM_DuplicateSampleIdPresent = 37,
        
        //!//!Indicates that no dynamic sample data present for the given sample id.
        kSM_SamplesNotPresent = 38,
        
        //!Indicates invalid sample set data.
        kSM_InvalidSampleSetData = 39,
        
		//!Indicates same match name is present for multiple properties
		kSM_DuplicateMatchNamePresent = 40,

		//!Indicates frame rate is not supported
		kSM_FrameRateNotSupported = 41,

		//!Indicates match name is invalid
		kSM_InvalidMatchName = 42,
		
		//!Indicates MGJSON schema voilation, property value type not matching with schema defined type.
		kSM_InvalidPropertyType = 43,
		
		//!Indicates MGJSON schema voilation, property value not matching with schema defined value.
		kSM_InvalidPropertyValue = 44,
		
		//!Indicates that path to data node is invalid.
		kSM_InvalidDataNodePath = 45,
		
		//! Indicates "utcInfo" is present and "useTimeCodeB" is set to true
		//! or "timeCodeInfo" is present and "useTimeCodeB" is set to false.
		
		kSM_IncorrectTimeInfo = 46,
		
		//! Indicate array of zero items.
		kSM_EmptyArrayItem = 47,
		
		//!Indicates invalid value i.e. value not belong to defined set of values
		kSM_InvalidValue = 48,
		
		//!Indicates no dynamic data node present for sample set.
		kSM_SampleIdDynamicNodeNotPresent = 49,
        
        //!Indicates that invalid additional members found in MGJSON object.
        kSM_InvalidMemberCount = 50,
		
		//!Indicates that the number represented through number string is invalid.
		kSM_InvalidNumberInNumberString = 51,
		
		//!Indicates that API is not compatible with open MGJSON file version.
		kSM_IncompatibleAPI = 52,
		
		//!Indicates invalid value type.
		kSM_InvalidValueType = 53,
		
		//!Indicates that file is empty
		kSM_EmptyFile = 54,

		/************************ PLUGIN ERROR CODES ************************/

		//! Indicates that parameters passed to function are not as expected.
		kSMPlugin_ParametersNotAsExpected = 1001,

		//! Indicates that file not present at the given path.
		kSMPlugin_FileNotExist = 1004,

		//!Indicates that file cannot be opened for processing.
		kSMPlugin_CannotOpenFile = 1006,

		//!Indicates that file cannot be created
		kSMPlugin_CannotCreateFile = 1007,

		//!Indicates that file cannot be closed
		kSMPlugin_CannotCloseFile = 1008,

		//!Indicates that file cannot be deleted
		kSMPlugin_CannotDeleteFile = 1009,

		//!Indicates file seek failure
		kSMPlugin_FileSeekFailure = 1010,

		//!Indicates file read failure
		kSMPlugin_FileReadFailure = 1011,

		//!Indicates that file path cannot be converted
		kSMPlugin_CannotConvertFilePath = 1012,

		//!Indicates that file has permission issues
		kSMPlugin_FilePermissionError = 1013,

		//!Indicates that the folder cannot be deleted
		kSMPlugin_CannotDeleteFolder = 1018,
		
		//! Indicates an internal failure.
		kSMPlugin_InternalFailure = 1021,

		//! Indicates an unknown failure.
		kSMPlugin_UnknownFailure = 1023,

		//!Indicates that memory is not available
		kSMPlugin_MemoryNotAvailable = 1034,

		/**************Only Plugin***************/
		//!Indicates plugin initialization failed.
		kSMPlugin_InitializeFailed = 1500,
		
		//!Indicates plugin identifier mismatch.
		kSMPlugin_IdentifierMismatch = 1501,
				
		//!Indicates plugin resource parsing failure.
		kSMPlugin_ResourceParsingFailure = 1503,
		
		//!Indicates plugin manifest schema voilation.
		kSMPlugin_ManifestSchemaViolation = 1504,
		
		//!Indicates plugin invalid mgjson filepath.
		kSMPlugin_InvalidMGJSONFilePath = 1505,

		//!Indicates conversion to mgjson failed .
		kSMPlugin_ConversionFailed = 1506,

		//!Indicates plugin termination failed.
		kSMPlugin_TerminationFailed = 1507,
		 
   		/************************ WRITER ERROR CODES ************************/

		//! Indicates that no valid data node present in root/group.
		kSMWriter_NoValidDataNodePresent = 2000,

		//!Indicates invalid dynamic data passed in writer apis.
		kSMWriter_InvalidDynamicData = 2001,
		
		//!Indicates invalid static data passed in writer apis.
		kSMWriter_InvalidStaticData = 2002,
		
		//!Indicates invalid group passed in writer apis.
		kSMWriter_InvalidGroup = 2003,

		//!Indicates mgjson file is invalidated.
		kSMWriter_MGJSONFileInvalidated = 2004,

		//!Indicates invalid root.
		kSMWriter_InvalidRoot = 2005,

		//!Indicates invalid number pattern property.
		kSMWriter_InvalidNumberPatternProperty = 2006,

		//!Indicates invalid occuring range property.
		kSMWriter_InvalidOccuringRangeProperty = 2007,

		//!Indicates invalid legal range property.
		kSMWriter_InvalidLegalRangeProperty = 2008,

		//!Indicates invalid time type.
		kSMWriter_InvalidTimeType = 2009,

		//!Indicates invalid sync time type.
		kSMWriter_InvalidSyncTimeType = 2010,
		
		//!Indicates invalid display names.
		kSMWriter_InvalidDisplayNames = 2011,
		
		//!Indicates invalid ranges property.
		kSMWriter_InvalidRangesProperty = 2012,
		
		//!Indicates invalid padded string property.
		kSMWriter_InvalidPaddedStringProperty = 2013,
		
		//!Indicates invalid sample value .
		kSMWriter_InvalidSampleValue = 2014,
		
		//!Indicates invalid sync time info .
		kSMWriter_InvalidSyncTimeInfo = 2015,

		//!Indicates writer api internal failure.
		kSMWriter_InternalFailure = 2016,
		
		//!Indicates invalid count in NumberArrayProperties.
		kSMWriter_InvalidNumberArrayCount = 2017,
		
		//!Indicates invalid count of NumberArrayRange.
		kSMWriter_InvalidNumberArrayRangeCount = 2018,
		
		//!Indicates invalid count of DisplayNames
		kSMWriter_InvalidNumberArrayDisplayNameCount = 2019,
		
		//!Indicates invalid child type.
		kSMWriter_InvalidChildType = 2020,

		//! Indicates that parameters passed to function are not as expected.
		kSMWriter_NoDataNodePresent = 2021,

		//!Indicates match name is invalid
		kSMWriter_InvalidMatchName = 2022,

		//!Indicates MGJSON schema voilation, property value type not matching with schema defined type.
		kSMWriter_InvalidPropertyType = 2023,

		//! Indicates "utcInfo" is present and "useTimeCodeB" is set to true
		//! or "timeCodeInfo" is present and "useTimeCodeB" is set to false.

		kSMWriter_IncorrectTimeInfo = 2024,

		//!Indicates invalid value i.e. value not belong to defined set of values
		kSMWriter_InvalidValue = 2025,

		//!Indicates invalid value type.
		kSMWriter_InvalidValueType = 2026,

		//! Indicates that parameters passed to function are not as expected.
		kSMWriter_ParametersNotAsExpected = 2027,
		
		//! Indicates that Dynamic data should not be present since DynamicSamplesPresentB is false.
		kSMWriter_DynamicDataNotExpected = 2028,
		 
		//! Indicates that plugin version is higher than the SM Libaray version.
		kSMWriter_InterfaceHigherVersion = 2029,
		
		//! Indicates that the requested interface version is not available.
		kSMWriter_InterfaceVersionNotAvailable = 2030,
		
		/***kSMClientPlugins_ErrorCode = 3000****/
		/// please add new errors after 3000

		//! Maximum value this enum can hold, should be treated as invalid value.
        kSM_MaxValue = kMaxEnumValue
    } eErrorCode;
    
    
    ///@brief Interpolation type for use with SM.
    typedef enum {
        
        //! Indicates no interpolation type
        kSM_InterpolationType_NONE = 0,
		
        //!Indicates linear type interpolation
        kSM_InterpolationType_LINEAR,
        
        //!Indicates hold type interpolation
        kSM_InterpolationType_HOLD
        
    } eInterpolationType;
    
    ///@brief Mgjson schema data value type
    typedef enum {
        
        //!Indicates number value type
        kSM_ValueType_NUMBER = 0,
        
        //!Indicates string value type
        kSM_ValueType_STRING,
        
        //!Indicates number array value type --see SM_DataTypes.h
		//!limited to max of 3 elements
        kSM_ValueType_NUMBER_ARRAY,
		
		//!Indicates JSON boolean value type
		kSM_ValueType_BOOLEAN,

		//!Indicates invalid value
		kSM_ValueType_INVALID
        
    } eDataValueType;
	
	
    /// @brief Group Type
    typedef enum{
        
        //!Indicates Data Group
        kSM_ChildType_GROUP = 0,
        
        //!Indicates Dynamic Group
        kSM_ChildType_DYNAMIC_DATA,
        
        //!Indicates Static Group
        kSM_ChildType_STATIC_DATA,
        
        //!Indicates invalid entry
        kSM_ChildType_INVALID
    } eGroupChildType;

	typedef enum {

		//!Indicates Number properties.
		kSM_NumberProperties = 0,

		//!Indicates Number array properties.
		kSM_NumberArrayProperties,

		//!Indicates Mgjson String properties.
		kSM_StringProperties

	}eTypeProperty;
	
	////@brief sync time type
	typedef enum {
		
		//!Indicates time type string
		kSM_SyncTime_STRING = 0 ,
		
		//!Indicates time type frame
		kSM_SyncTime_FRAME ,
		
		//!Indicates invalid time type
		kSM_SyncTimetype_NONE
	}eSyncTimeType;
	
	
	//typedef for localization string.

	/** English US */
	static SM_StringPtr SMEnglishUSCode = "en_US";

	/** English UK */
	static SM_StringPtr SMEnglishUKCode = "en_GB";

	/** Japanese */
	static SM_StringPtr SMJapaneseCode = "ja_JP";

	/** Korean */
	static SM_StringPtr SMKoreanCode = "ko_KR";

	/** Chinese Traditional */
	static SM_StringPtr SMTradChineseCode = "zh_TW";

	/** Chinese Simplified */
	static SM_StringPtr SMSimpChineseCode = "zh_CN";

	/** German */
	static SM_StringPtr SMGermanCode = "de_DE";

	/** French France */
	static SM_StringPtr SMFrenchCode = "fr_FR";

	/** French Canada */
	static SM_StringPtr SMCanadian_FrenchCode = "fr_CA";

	/** Italian */
	static SM_StringPtr SMItalianCode = "it_IT";

	/** Spanish Traditional */
	static SM_StringPtr SMSpanishCode = "es_ES";

	/** Spanish Mexico */
	static SM_StringPtr SMSpanishMexicoCode = "es_MX";

	/** Dutch */
	static SM_StringPtr SMDutchCode = "nl_NL";

	/** Portuguese */
	static SM_StringPtr SMBrazilianCode = "pt_BR";

	/** Swedish */
	static SM_StringPtr SMSwedishCode = "sv_SE";

	/** Danish */
	static SM_StringPtr SMDanishCode = "da_DK";

	/** Norwegian */
	static SM_StringPtr SMNorwegianCode = "nb_NO";

	/** Finnish */
	static SM_StringPtr SMFinnishCode = "fi_FI";

	/** Czech */
	static SM_StringPtr SMCzechCode = "cs_CZ";

	/** Russian */
	static SM_StringPtr SMRussianCode = "ru_RU";

	/** Turkish */
	static SM_StringPtr SMTurkishCode = "tr_TR";

	/** Polish */
	static SM_StringPtr SMPolishCode = "pl_PL";

	/** Hungarian */
	static SM_StringPtr SMHungarianCode = "hu_HU";

	/** Ukrainian */
	static SM_StringPtr SMUkrainianCode = "uk_UA";

	SM_NAMESPACE_END
	
#if __cplusplus
} // extern "C"
#endif

#define NOTHROW noexcept

#if ! SM_DebugBuild
#define SM_Assert(c)	((void) 0)
#else
#define SM_Assert(c)	assert ( c )
#endif


const SM_NAMESPACE::SM_Uns8 kSM_Bool_False = 0;

#define ConvertSM_BoolToBool(a) (a) != kSM_Bool_False
#define ConvertBoolToSM_Bool(a) (a) ? !kSM_Bool_False : kSM_Bool_False

#endif /*__SM_CONST__*/

