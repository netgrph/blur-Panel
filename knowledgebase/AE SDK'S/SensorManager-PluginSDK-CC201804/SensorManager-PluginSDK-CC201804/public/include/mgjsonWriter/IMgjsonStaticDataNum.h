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

#ifndef __IMgjsonStaticDataNum_H__
#define __IMgjsonStaticDataNum_H__

/**************************************************************************
* @file IMgjsonStaticDataNum.h
* @brief This allows creation of static data node.
*		This class is derived from IMgjsonStaticDataNum.
*		It would contain static data value of type number
***************************************************************************/

#include "SM_PluginTypes.h"
#include "SM_FwdDeclarations.h"
#include "ISharedObject.h"
#include "SM_HostAPIAccess.h"
#include "IMgjsonStaticData.h"

namespace SM_PLUGIN {
	
	class IMgjsonStaticDataNum_v1 : public virtual IMgjsonStaticData_v1,
								 public virtual ISharedObject
	{
	public:

		//!
		//! @brief Creates a StaticData node of type number.
		//! The node must be later added to an MGJSONGroup or MGJSONRoot to be serialized in the MGJSON file.
		//!                           
		//! @param inConverter         this pointer of the converter instance
		//! @param inMatchName		   unique identifier with ascii, alphanumeric chars and no leading numerals
		//! @param inDisplayName	   label used for stream
		//! @param inValue			   value to be serialized as number.
		//! @param inNumberProp        struct containing NumberProperties associated with this node. It is optional. The properties 
		//!                            would be calculated if not provided. Otherwise the provided properties are used and serialized.
		//! 
		//! @return                    A shared pointer to a SM_PLUGIN::IMgjsonStaticDataNum object.
		//!
		//! \attention SM_PluginException is thrown in case
		//!			   - inConverter is NULL
		//!		       - inMatchName is not valid ASCII
		//!		       - One of mDigitsInteger and mDigitsDecimal is provided and other is not. 
		//!		       - One of mMinimum and mMaximum is provided in mOccuring or mLegal and other is not. 
		//! \note      - It is advised to provide the Legal Range. If it is not provided, then it is made equal to the Occuring Range.
		//!
		static spIMgjsonStaticDataNum CreateStaticDataNum (ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName ,
														const SM_NAMESPACE::SM_UTF8String& inDisplayName,
														const double& inValue,
														const SM_NAMESPACE::SM_NumberProperties& inNumberProp = SM_NAMESPACE::SM_NumberProperties());
		static SM_NAMESPACE::SM_Uns32 GetInterfaceVersion() { return 1; }

	protected:

		// protected virtual destructor.
		virtual ~IMgjsonStaticDataNum_v1 ( ) NOTHROW {}
		virtual void APICALL setNumberPattern(SM_NAMESPACE::SM_Uns64 digitsInteger, SM_NAMESPACE::SM_Uns64 digitsDecimal, SM_NAMESPACE::SM_Bool isSigned, void* errResultP) NOTHROW = 0;
		virtual void APICALL setNumberRange(double minLegal, double maxLegal, double minOccuring, double maxOccuring, void* errResultP) NOTHROW = 0;


#ifdef REQUIRED_FRIEND_CLASS
	MAKE_CLASS_FRIEND
#endif
	};
}

#endif /* __IMgjsonStaticDataNum_H__ */
