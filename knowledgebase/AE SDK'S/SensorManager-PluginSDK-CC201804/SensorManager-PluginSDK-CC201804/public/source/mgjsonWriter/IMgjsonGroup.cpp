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

namespace SM_PLUGIN {
	class IMgjsonGroupProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class SM_PLUGIN::IMgjsonGroupProxy;

#include "IMgjsonGroup.h"

namespace SM_PLUGIN {

    pIMgjsonGroup APICALL IMgjsonGroupProxy::GetActualIMgjsonGroup ( ) NOTHROW { return mRawPtr; }

    void APICALL IMgjsonGroupProxy::addDynamicData ( pIMgjsonDynamicData_base inDynamicP , void* errResultP ) NOTHROW {
			assert ( false );
			mRawPtr->addDynamicData ( inDynamicP , errResultP );
		}

    void APICALL IMgjsonGroupProxy::addStaticData ( pIMgjsonStaticData_base inStaticP , void* errResultP ) NOTHROW {
			assert ( false );
			mRawPtr->addStaticData ( inStaticP , errResultP );
		}

    void APICALL IMgjsonGroupProxy::addGroup ( pIMgjsonGroup_base inGroupP , void* errResultP ) NOTHROW {
			assert ( false );
		    mRawPtr->addGroup ( inGroupP , errResultP );
		}
		
    void  IMgjsonGroupProxy::AddDynamicData ( const spIMgjsonDynamicData& inDynamic ) {
			SM_PluginResult error;
			mRawPtr->addDynamicData ( inDynamic ? inDynamic->GetActualIMgjsonDynamicData ( ): NULL, &error );
			PROPOGATE_PLUGIN_EXCEPTION(error);
		}

    void  IMgjsonGroupProxy::AddStaticData ( const spIMgjsonStaticData& inStatic ) {
			SM_PluginResult error;
			mRawPtr->addStaticData ( inStatic ? inStatic->GetActualIMgjsonStaticData ( ) : NULL , &error );
			PROPOGATE_PLUGIN_EXCEPTION(error);
		}

    void  IMgjsonGroupProxy::AddGroup ( const spIMgjsonGroup& inGroup ) {
			SM_PluginResult error;
			mRawPtr->addGroup (inGroup ? inGroup->GetActualIMgjsonGroup ( ) : NULL, &error );
			PROPOGATE_PLUGIN_EXCEPTION(error);
		}
	
    void  IMgjsonGroupProxy::Acquire ( ) const NOTHROW {
			assert ( false );

		}
		
    void  IMgjsonGroupProxy::Release ( ) const NOTHROW {
			assert ( false );

		}

    pISharedObject_I APICALL IMgjsonGroupProxy::GetISharedObject_I ( ) NOTHROW {
			 return mRawPtr->GetISharedObject_I ( );

		}

	spIMgjsonGroup IMgjsonGroup_v1::CreateGroup (ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName ,
								 const SM_NAMESPACE::SM_UTF8String& inDisplayName ) {
		if (inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		pIMgjsonGroup ptr;
		HostCreateGroup ( inConverter, &ptr , inMatchName.c_str ( ) , inDisplayName.c_str ( ) );
		if( !ptr )
			return spIMgjsonGroup ( );
		
		spIMgjsonGroup spProxy = spIMgjsonGroup();
		TRY_BLOCK
			spProxy = shared_ptr< IMgjsonGroup > ( new IMgjsonGroupProxy ( ptr ) );
		CATCH_RELEASE_THROW
		return spProxy;
	}

}
