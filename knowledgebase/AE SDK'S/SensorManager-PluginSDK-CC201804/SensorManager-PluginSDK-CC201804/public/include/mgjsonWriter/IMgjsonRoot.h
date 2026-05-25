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

#ifndef __IMgjsonRoot_H__
#define __IMgjsonRoot_H__

/**************************************************************************
* @file IMgjsonRoot.h
* @brief This allows creation of root node. 
*		Root would contain the first level static data, dynamic data and groups and the file level information. 
*		Its creation must be the first call among writer api's.
*		The commit must be the last call once all the static data, dynamic data 
*		with all the dynamic samples and groups has been added.
***************************************************************************/

#include "IMgjsonGroup.h"
#include "SM_DataTypesCommon.h"
#include "SM_PluginTypes.h"
#include "ISharedObject.h"

namespace SM_PLUGIN {


	class IMgjsonRoot_v1: public virtual IMgjsonGroup_v1,
                        public virtual ISharedObject
	{
        
    public:

		//!
		//! @brief Creates a Root node.
		//! It must be the first and mandatory call among writer api's. Root is singleton for this converter instance
		//! For one converter instance, mgjson path and DynamicFileInfo cannot be changed
		//!                           
		//! @param inConverter         this pointer of the converter instance
		//! @param inMGJSONpath		   path of the output mgjson file
		//! @param inDynamicFileInfo   struct containing Dynamic file info associated with this mgjson file. It is an optional property ,
		//!							   in case SM_DynamicFileInfo is not NULL, then SM_TimeInfo is mandatory. SM_VideoSyncTimeList is optional.
		//!							   If dynamic data has to be added then this must be provided.
		//! 
		//! @return                    A shared pointer to a SM_PLUGIN::IMgjsonRoot object.
		//! \attention SM_PluginException is thrown in case
		//!			   - inConverter is NULL
		//!			   - inMGJSONpath is NULL or empty
		//!
		//! \note	   - In case SM_PluginException is thrown , then file is cleared and the root
		//!				 is also set to an empty root and only the properties passed are preserved.				
		//!
       static spIMgjsonRoot CreateRoot (ConverterRef inConverter, SM_NAMESPACE::SM_UTF8FilePath inMGJSONpath, 
										const SM_NAMESPACE::SM_DynamicFileInfo* inDynamicFileInfo = NULL );

		//!
		//! @brief Serializes the MGJSON file.  
		//! This must be the last and mandatory call once all the static data, dynamic data with all the dynamic samples and groups has been added.
	    //! Any changes done to the root after this API call, would not take any effect.
		//! This should be called only once.
		//!                           
		//! @return    A bool value; true in case the commit is successful. False in case the node was already committed.
		//! 
		//! \attention SM_PluginException is thrown in case
		//!		       - There is a failure in serialization of file.
		//! \note      - In case SM_PluginException is thrown, then file is cleared and the root is also set to an empty root and
	    //!				 only the properties passed in \c CreateRoot() are preserved
		//!
		virtual bool APICALL Commit ( )  = 0;
	
		//!
		//! @brief Sets the value of creator filed in mgjson.
		//! @param inVal			value of creator
		
		virtual void APICALL SetCreator ( const SM_NAMESPACE::SM_UTF8String& inVal ) = 0;
                            
		//!
		//! @brief Returns the actual raw pointer from the shared pointer.
		//! @return the actual raw pointer from the shared pointer.
		//!
        virtual pIMgjsonRoot APICALL GetActualIMgjsonRoot ( ) NOTHROW = 0;
        pcIMgjsonRoot GetActualIMgjsonRoot ( ) const NOTHROW {
			return const_cast< IMgjsonRoot * >(this)->GetActualIMgjsonRoot ( );
		}
	
	static SM_NAMESPACE::SM_Uns32	GetInterfaceVersion(){ return 1; }
	
	protected:

		// protected virtual destructor.
		virtual ~IMgjsonRoot_v1 ( ) NOTHROW {}
	
	   virtual SM_NAMESPACE::SM_Bool APICALL commit ( void* errResultP ) NOTHROW = 0;
	   virtual void APICALL setCreator ( SM_NAMESPACE::SM_StringPtr val, void * errResultP ) NOTHROW = 0;

	   virtual void APICALL setSMPTETimeInfo(SM_NAMESPACE::SM_Bool dropFrame,SM_NAMESPACE::SM_Uns32 frameRateValue,
                                      SM_NAMESPACE::SM_Uns32 frameRateScale, void * errResultP) NOTHROW = 0;
	   virtual void APICALL setUTCTimeInfo(SM_NAMESPACE::SM_Uns32 precisionLength, SM_NAMESPACE::SM_Bool isGMT, void * errResultP) NOTHROW = 0;
	   virtual void APICALL addSyncTimeFrameNum(SM_NAMESPACE::SM_StringPtr fileName, SM_NAMESPACE::SM_Int32 frameNumber, void * errResultP) NOTHROW = 0;
	   virtual void APICALL addSyncTimeStr(SM_NAMESPACE::SM_StringPtr fileName, SM_NAMESPACE::SM_StringPtr timeString,
                                        SM_NAMESPACE::SM_StringLen timeStringLen, void * errResultP) NOTHROW = 0;
	   virtual void APICALL writeVersionToFile(void * errResultP) NOTHROW = 0;

#ifdef REQUIRED_FRIEND_CLASS
	MAKE_CLASS_FRIEND
#endif
};

} //SM_PLUGIN

#endif /* __IMgjsonRoot_H__ */
