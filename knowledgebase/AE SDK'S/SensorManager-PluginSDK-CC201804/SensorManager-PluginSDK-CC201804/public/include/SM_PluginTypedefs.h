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

#ifndef __SM_PLUGIN_TYPEDEFS_H__
#define __SM_PLUGIN_TYPEDEFS_H__

#include "SM_PluginException.h"

namespace SM_PLUGIN {

#define THROW_PLUGIN_EXCEPTION(errId)													\
	throw SM_PLUGIN::SM_PluginException(errId);

#define THROW_PLUGIN_EXCEPTION_IN_PARSING(errId, offset, lineNo)						\
	throw SM_PLUGIN::SM_PluginException(errId, offset, lineNo);


#define TRY_BLOCK	 \
try{				

#define TRANSLATE_EXCEPTION_TO_PLUGIN_EXCEPTION														\
}																									\
		catch(SM_PLUGIN::SM_PluginException& exp) { throw exp;}												\
		catch(std::bad_alloc& ) { THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_MemoryNotAvailable); } \
		catch(...) { THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_UnknownFailure); }

#define TRANSLATE_EXCEPTION_TO_PLUGIN_RESULT																	\
	}catch(SM_PLUGIN::SM_PluginException& exp)	{																			\
		SetPluginErrorObj(error,exp.GetErrorCode(),																\
								exp.GetFileOffset(),															\
								exp.GetFileLineNo());}															\
	catch(std::bad_alloc& ) {																					\
			SetPluginErrorObj(error,SM_NAMESPACE::kSMPlugin_MemoryNotAvailable,										\
								0,0);}																			\
	catch(...) {																								\
			SetPluginErrorObj(error,SM_NAMESPACE::kSMPlugin_UnknownFailure,											\
												0,0);}


#define PROPOGATE_PLUGIN_EXCEPTION(result) if ( result.errId != SM_NAMESPACE::kSM_None ) throw SM_PLUGIN::SM_PluginException ( result.errId, result.fileOffset, result.lineNo );

#define CATCH_RELEASE_THROW			\
		}catch(...){				\
		ptr->Release();				\
		throw;	}

#define JOIN_CLASSNAME_WITH_VERSION_NUMBER_INT(x,y) x ## _v ## y
#define JOIN_CLASSNAME_WITH_VERSION_NUMBER(x,y)  JOIN_CLASSNAME_WITH_VERSION_NUMBER_INT(x,y)
#define LATEST_CLASS(classNameWithoutVersionNumber, versionNumber) JOIN_CLASSNAME_WITH_VERSION_NUMBER(classNameWithoutVersionNumber, versionNumber)
	
}//SM_PLUGIN

#endif //__SM_PLUGIN_TYPEDEFS_H__
