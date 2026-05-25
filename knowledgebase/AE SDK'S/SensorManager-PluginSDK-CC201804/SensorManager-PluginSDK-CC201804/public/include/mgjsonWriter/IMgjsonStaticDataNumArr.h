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

#ifndef __IMgjsonStaticDataNumArr_H__
#define __IMgjsonStaticDataNumArr_H__

/**************************************************************************
* @file IMgjsonStaticDataNumArr.h
* @brief This allows creation of static data node.
*		This class is derived from IMgjsonStaticDataNumArr.
*		It would contain static data value of type number array
***************************************************************************/

#include "SM_PluginTypes.h"
#include "SM_FwdDeclarations.h"
#include "ISharedObject.h"
#include "SM_HostAPIAccess.h"
#include "IMgjsonStaticData.h"

namespace SM_PLUGIN {
	
	class IMgjsonStaticDataNumArr_v1 : public virtual IMgjsonStaticData_v1,
									public virtual ISharedObject 
	{
	public:

		//!
		//! @brief Creates a StaticData node of type number array.
		//! The node must be later added to an MGJSONGroup or MGJSONRoot to be serialized in the MGJSON file.
		//!                           
		//! @param inConverter         this pointer of the converter instance
		//! @param inMatchName		   unique identifier with ascii, alphanumeric chars and no leading numerals
		//! @param inDisplayName	   label used for stream
		//! @param inValue			   value to be serialized as number array.
		//! @param inNumberArrayProp   struct containing NumberArrayProperties associated with this node.
		//!                            All other properties are optional. The optional properties would be
		//!                            calculated if not provided. Otherwise the provided properties are used and serialized.
		//! 
		//! @return                    A shared pointer to a SM_PLUGIN::IMgjsonStaticDataNumArr object.
		//!
		//! \attention SM_PluginException is thrown in case
		//!			   - inConverter is NULL
		//!		       - inMatchName is not valid ASCII
		//!            - if mCount is provided and is greater than 3 or does not match with size of inValue.
		//!			   - if inValue size is greater than 3
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
		static spIMgjsonStaticDataNumArr CreateStaticDataNumArr (ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName ,
														const SM_NAMESPACE::SM_UTF8String& inDisplayName,
														const std::vector<double>& inValue,
														const SM_NAMESPACE::SM_NumberArrayProperties& inNumberArrayProp = SM_NAMESPACE::SM_NumberArrayProperties());

		static SM_NAMESPACE::SM_Uns32 GetInterfaceVersion() { return 1; }

	protected:

		// protected virtual destructor.
		virtual ~IMgjsonStaticDataNumArr_v1( ) NOTHROW {}
 	    virtual void APICALL addElement ( double value, void* errResultP ) NOTHROW = 0;
		virtual void APICALL addDisplayName(SM_NAMESPACE::SM_StringPtr displayName, void * errResultP) NOTHROW = 0;
		virtual void APICALL setNumberPattern(SM_NAMESPACE::SM_Uns64 digitsInteger, SM_NAMESPACE::SM_Uns64 digitsDecimal, SM_NAMESPACE::SM_Bool isSigned, void* errResultP) NOTHROW = 0;
		virtual void APICALL setNumberRange(double minLegal, double maxLegal, double minOccuring, double maxOccuring, void* errResultP) NOTHROW = 0;
		virtual void APICALL setArrCount(SM_NAMESPACE::SM_Uns64 count, void * errResultP) NOTHROW = 0;
		virtual void APICALL setIsPoint(SM_NAMESPACE::SM_Bool isPoint, void * errResultP) NOTHROW = 0;

#ifdef REQUIRED_FRIEND_CLASS
	MAKE_CLASS_FRIEND
#endif
	};
}

#endif /* __IMgjsonStaticDataNumArr_H__ */
