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

#ifndef __IMgjsonGroup_H__
#define __IMgjsonGroup_H__

/**************************************************************************
* @file IMgjsonGroup.h
* @brief This allows creation of group.
*		Group can contain static data, dynamic data with sample information
*		and groups itself.
***************************************************************************/

#include "IMgjsonDynamicData.h"
#include "IMgjsonStaticData.h"
#include "ISharedObject.h"


namespace SM_PLUGIN {
    
    class IMgjsonGroup_v1 : public virtual ISharedObject
	{
    
	public:
 
		//!
		//! @brief Creates a Group node .
		//! The node must be later added to an MGJSONGroup or MGJSONRoot to be serialized in the MGJSON file.
		//!                           
		//! @param inConverter         this pointer of the converter instance
		//! @param inMatchName		   unique identifier with ascii, alphanumeric chars and no leading numerals
		//! @param inDisplayName	   label used for stream
		//! 
		//! @return                    A shared pointer to a SM_PLUGIN::IMgjsonGroup object.
		//!
		//! \attention SM_PluginException is thrown in case
		//!			   - inConverter is NULL
		//!		       - inMatchName is not valid ASCII
		//!
        static spIMgjsonGroup CreateGroup (ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName ,
                                           const SM_NAMESPACE::SM_UTF8String& inDisplayName );
		
		//!
		//! @brief Adds the dynamic data node to MGJSONGroup or MGJSONRoot  
		//! @param inDynamic		  shared pointer to a SM_PLUGIN::IMgjsonDynamicData object
		//!
		//! \attention SM_PluginException is thrown in case
		//!            - inDynamic is empty. 
		//!		       - Dirty node is added.
		//!
        virtual void APICALL AddDynamicData ( const spIMgjsonDynamicData& inDynamic ) = 0;
		
		//!
		//! @brief Adds the static data node to MGJSONGroup or MGJSONRoot  
		//! @param inStatic         shared pointer to a SM_PLUGIN::IMgjsonStaticData object
		//!
		//! \attention SM_PluginException is thrown in case inStatic is empty. 
		//!
		virtual void APICALL AddStaticData ( const spIMgjsonStaticData& inStatic )  = 0;

		//!
		//! @brief Adds the group node to MGJSONGroup or MGJSONRoot  
		//! @param inGroup         shared pointer to a SM_PLUGIN::IMgjsonGroup object
		//!                       
		//! \attention SM_PluginException is thrown in case inGroup is empty. 
		//!
		virtual void APICALL AddGroup ( const spIMgjsonGroup& inGroup ) = 0;
        
		//!
		//! @brief Returns the actual raw pointer from the shared pointer.
		//! @return the actual raw pointer from the shared pointer.
		//!       
		virtual pIMgjsonGroup APICALL GetActualIMgjsonGroup ( ) NOTHROW = 0;
        pcIMgjsonGroup GetActualIMgjsonGroup ( ) const NOTHROW {
			 return const_cast< IMgjsonGroup * >(this)->GetActualIMgjsonGroup ( );
        }

		static SM_NAMESPACE::SM_Uns32 GetInterfaceVersion() { return 1; }

    protected:

		// protected virtual destructor.
		virtual ~IMgjsonGroup_v1 ( ) NOTHROW {}

	    virtual void APICALL addDynamicData ( pIMgjsonDynamicData_base inDynamicP , void* errResultP ) NOTHROW = 0;
		virtual void APICALL addStaticData ( pIMgjsonStaticData_base inStaticP , void* errResultP ) NOTHROW = 0;
		virtual void APICALL addGroup ( pIMgjsonGroup_base inGroupP , void* errResultP ) NOTHROW = 0;

#ifdef REQUIRED_FRIEND_CLASS
	MAKE_CLASS_FRIEND
#endif
};

	class IMgjsonGroupProxy : public virtual IMgjsonGroup {
	private:
		pIMgjsonGroup mRawPtr;

	public:
		IMgjsonGroupProxy(pIMgjsonGroup ptr) : mRawPtr(ptr) {
			mRawPtr->Acquire();
		}
		~IMgjsonGroupProxy() NOTHROW {
			mRawPtr->Release();
		}

		static spIMgjsonGroup CreateGroup(const SM_NAMESPACE::SM_AsciiString& inMatchName,
			const SM_NAMESPACE::SM_UTF8String& inDisplayName);
		virtual void APICALL AddDynamicData(const spIMgjsonDynamicData& inDynamic);
		virtual void APICALL AddStaticData(const spIMgjsonStaticData& inStatic);
		virtual void APICALL AddGroup(const spIMgjsonGroup& inGroup);

		virtual pIMgjsonGroup APICALL GetActualIMgjsonGroup() NOTHROW;
		void APICALL Acquire() const NOTHROW;
		void APICALL Release() const NOTHROW;
		pISharedObject_I APICALL GetISharedObject_I() NOTHROW;

	protected:
		virtual void APICALL addDynamicData(pIMgjsonDynamicData inDynamicP, void* errResultP) NOTHROW;
		virtual void APICALL addStaticData(pIMgjsonStaticData inStaticP, void* errResultP) NOTHROW;
		virtual void APICALL addGroup(pIMgjsonGroup inGroupP, void* errResultP) NOTHROW;

	};

}
#endif /* __IMgjsonGroup_H__ */
