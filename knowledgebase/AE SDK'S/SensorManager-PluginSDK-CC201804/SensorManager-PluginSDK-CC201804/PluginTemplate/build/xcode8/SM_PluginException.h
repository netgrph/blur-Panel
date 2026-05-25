#ifndef __SM_PLUGINEXCEPTION_h__
#define __SM_PLUGINEXCEPTION_h__

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2017 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include <vector>
#include "SM_Const.h"

namespace SM_PLUGIN
{


	class SM_PluginException  {
	public:

		SM_PluginException(SM_NAMESPACE::eErrorCode errorID,  const SM_NAMESPACE::SM_Uns64& fileOffset = 0,
			const SM_NAMESPACE::SM_Uns64& lineNo = 0) : mErrorID(errorID), mFileOffset(fileOffset), mLineNo(lineNo) {};

		
		inline SM_NAMESPACE::eErrorCode GetErrorCode() const { return mErrorID; };

		inline const SM_NAMESPACE::SM_Uns64& GetFileOffset() const { return mFileOffset; }

		inline const SM_NAMESPACE::SM_Uns64& GetFileLineNo() const { return mLineNo; }

	private:
		/// SM Error code. See eErrorCode in SMErrorCodes.h.
		SM_NAMESPACE::eErrorCode mErrorID;
		
		SM_NAMESPACE::SM_Uns64 mFileOffset;
	
		SM_NAMESPACE::SM_Uns64 mLineNo;
	};

	typedef struct SM_PluginResult
	{
		SM_NAMESPACE::eErrorCode errId;
		SM_NAMESPACE::SM_Uns64 fileOffset;
		SM_NAMESPACE::SM_Uns64 lineNo;
#if __cplusplus
		SM_PluginResult() :errId(SM_NAMESPACE::kSM_None), fileOffset(0), lineNo(0) {}
#endif /**__cplusplus**/

	}SM_PluginResult;

}


#endif /* __SM_PLUGINEXCEPTION_h__ */
