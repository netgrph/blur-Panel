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


#include "SM_PluginHandler.h"
#include "SM_PluginBase.h"
#include "SM_HostAPIAccess.h"
#include "SM_PluginRegistry.h"
#include "SM_PluginTypedefs.h"

namespace SM_PLUGIN
{

	static void Static_TerminatePlugin ( void* error ) {
        
		TRY_BLOCK
			bool ret = SM_PluginRegistry::terminate ( );
		if (ret == false)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_TerminationFailed);
		TRANSLATE_EXCEPTION_TO_PLUGIN_RESULT
	}

	// ============================================================================

	static void Static_InitializeSession ( SM_NAMESPACE::SM_StringPtr uid , SM_NAMESPACE::SM_StringPtr ext ,
		SM_NAMESPACE::SM_StringPtr filePath , SM_NAMESPACE::SM_StringPtr mgJSONPath , SessionRef* session ,
		void* error ) {

		TRY_BLOCK
			*session = SM_PluginRegistry::create ( uid , ext , filePath , mgJSONPath ); 

		TRANSLATE_EXCEPTION_TO_PLUGIN_RESULT
	}

	// ============================================================================

	static void Static_TerminateSession ( SessionRef session , void* error ) {

		SM_PluginBase* thiz = ( SM_PluginBase* ) session;
		TRY_BLOCK
			HostRemoveConverterFromMap(session);
			delete thiz;
		TRANSLATE_EXCEPTION_TO_PLUGIN_RESULT
	}

	// ============================================================================

	static void Static_CheckSessionFileFormat ( SM_NAMESPACE::SM_StringPtr uid , SM_NAMESPACE::SM_StringPtr filePath , SM_NAMESPACE::SM_Bool* result , void* error ) {

		TRY_BLOCK
			*result = static_cast<SM_NAMESPACE::SM_Bool>(SM_PluginRegistry::checkFileFormat ( uid , filePath ));
		TRANSLATE_EXCEPTION_TO_PLUGIN_RESULT
	}

	// ============================================================================

	static void Static_ConvertToMGJSON ( SessionRef session , SM_NAMESPACE::SM_Bool* result , void* error ) {

		SM_PluginBase* thiz = ( SM_PluginBase* ) session;
		TRY_BLOCK
			if( thiz ) {
				*result = static_cast<SM_NAMESPACE::SM_Bool>(thiz->convertToMGJSON ( ));
			}
			else {
				*result = 0;
			}
		TRANSLATE_EXCEPTION_TO_PLUGIN_RESULT
	}

	// ============================================================================

	void SetWriterErrorObj ( void* result , SM_NAMESPACE::eErrorCode errCode ,
		SM_NAMESPACE::SM_Uns64 fileOffset ,
		SM_NAMESPACE::SM_Uns64 lineNo ) {

        SM_Assert(result != 0);
        
		SM_PluginResult* clientObj = static_cast<SM_PluginResult*>(result);
		clientObj->errId = errCode;
		clientObj->fileOffset = fileOffset;
		clientObj->lineNo = lineNo;
	}

	// ============================================================================

	void InitializePlugin ( SM_NAMESPACE::SM_StringPtr moduleID , HostAPIRef hostAPI , PluginAPIRef pluginAPI , void* error )
	{

		TRY_BLOCK

			if( hostAPI == NULL )
			{
				THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ParametersNotAsExpected);
			}

		if( SetHostAPI ( hostAPI ) )
		{
			if( !pluginAPI || moduleID == NULL )
			{
				THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ParametersNotAsExpected);
			}

			bool identical = (0 == memcmp(GetPluginIdentifier(), moduleID, std::min(strlen(GetPluginIdentifier()), strlen(moduleID))));
			if( !identical )
			{
				THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_IdentifierMismatch);
			}

			RegisterConverters ( ); // Register all converters

			bool initialized = SM_PluginRegistry::initialize ( ); // Initialize all the registered converters 

			if( initialized )
			{

				pluginAPI->mVersion = SM_PLUGIN_VERSION;

				pluginAPI->mTerminatePluginProc = Static_TerminatePlugin;
				pluginAPI->mInitializeSessionProc = Static_InitializeSession;
				pluginAPI->mTerminateSessionProc = Static_TerminateSession;

				pluginAPI->mCheckFileFormatProc = Static_CheckSessionFileFormat;
				pluginAPI->mConvertToMGJSONProc = Static_ConvertToMGJSON;
			}
			else
			{
				THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_InitializeFailed);
			}

		}
		else
		{
            THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_InitializeFailed);
		}
		TRANSLATE_EXCEPTION_TO_PLUGIN_RESULT

	}

} //namespace SM_PLUGIN
