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

#ifndef __IMgjsonStaticDataString_H__
#define __IMgjsonStaticDataString_H__

/**************************************************************************
* @file IMgjsonStaticDataString.h
* @brief This class is derived from IMgjsonStaticData.
*		It would contain static data value of type string
***************************************************************************/

#include "SM_PluginTypes.h"
#include "SM_FwdDeclarations.h"
#include "ISharedObject.h"
#include "SM_HostAPIAccess.h"
#include "IMgjsonStaticData.h"

namespace SM_PLUGIN {
	
	class IMgjsonStaticDataString_v1 : public virtual IMgjsonStaticData_v1,
									public virtual ISharedObject
	{
	public:

		//!
		//! @brief Creates a StaticData node of type string.
		//! The node must be later added to an MGJSONGroup or MGJSONRoot to be serialized in the MGJSON file.
		//!                           
		//! @param inConverter         this pointer of the converter instance
		//! @param inMatchName		   unique identifier with ascii, alphanumeric chars and no leading numerals
		//! @param inDisplayName	   label used for stream
		//! @param inValue			   value to be serialized as string.
		//! @param inStringProp        struct containing StringProperties associated with this node. It is optional. The properties 
		//!                            would be calculated if not provided. Otherwise the provided properties are used and serialized.
		//! 
		//! @return                    A shared pointer to a SM_PLUGIN::IMgjsonStaticDataString object.
		//!
		//! \attention SM_PluginException is thrown in case
		//!			   - inConverter is NULL
		//!		       - inMatchName is not valid ASCII
		//!		       - One of mMaxLen and mMaxLenValDigit is provided and other is not. 
		//!
		static spIMgjsonStaticDataString CreateStaticDataString (ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName ,
														const SM_NAMESPACE::SM_UTF8String& inDisplayName,
														const SM_NAMESPACE::SM_UTF8String& inValue,
														const SM_NAMESPACE::SM_StringProperties& inStringProp = SM_NAMESPACE::SM_StringProperties());

		static SM_NAMESPACE::SM_Uns32 GetInterfaceVersion() { return 1; }

	protected:

		// protected virtual destructor.
		virtual ~IMgjsonStaticDataString_v1 ( ) NOTHROW {}
		virtual void APICALL setStringProperties(SM_NAMESPACE::SM_Uns64 inMaxLen, SM_NAMESPACE::SM_Uns64 inMaxLenValDigit,
												 SM_NAMESPACE::SM_Bool inEventMarkerB, void* errResultP) NOTHROW = 0;


#ifdef REQUIRED_FRIEND_CLASS
	MAKE_CLASS_FRIEND
#endif
	};
}

#endif /* __IMgjsonStaticDataString_H__ */
