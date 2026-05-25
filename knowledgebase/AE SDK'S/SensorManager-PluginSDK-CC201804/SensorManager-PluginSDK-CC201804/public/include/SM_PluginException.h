#ifndef __SM_PLUGINEXCEPTION_H__
#define __SM_PLUGINEXCEPTION_H__

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

#include <vector>
#include "SM_Const.h"

namespace SM_PLUGIN
{

	//!@brief Class to handle exception in plugins

	class SM_PluginException  {
	public:

		SM_PluginException(SM_NAMESPACE::eErrorCode errorID,  const SM_NAMESPACE::SM_Uns64& fileOffset = 0,
			const SM_NAMESPACE::SM_Uns64& lineNo = 0) : mErrorID(errorID), mFileOffset(fileOffset), mLineNo(lineNo) {};

		/// Retrieves the numeric code from an SM_PluginException.
		inline SM_NAMESPACE::eErrorCode GetErrorCode() const { return mErrorID; };

		/// Retrieves the file offset, if any
		inline const SM_NAMESPACE::SM_Uns64& GetFileOffset() const { return mFileOffset; }

		/// Retrieves the file line no., if any
		inline const SM_NAMESPACE::SM_Uns64& GetFileLineNo() const { return mLineNo; }

	private:
		/// Error codes. See SM_Const.h.
		SM_NAMESPACE::eErrorCode mErrorID;

		/// fileOffset offset in file where error occurs, can be used during parsing of native file
		SM_NAMESPACE::SM_Uns64 mFileOffset;

		/// line number where error occurs, can be used during parsing of native file.
		SM_NAMESPACE::SM_Uns64 mLineNo;
	};

	//struct to handle exception in plugins. Used in plugin and host communication
	typedef struct SM_PluginResult
	{
		SM_NAMESPACE::SM_Uns32	 size;
		SM_NAMESPACE::eErrorCode errId;
		SM_NAMESPACE::SM_Uns64 fileOffset;
		SM_NAMESPACE::SM_Uns64 lineNo;
#if __cplusplus
		SM_PluginResult() :size(0), errId(SM_NAMESPACE::kSM_None), fileOffset(0), lineNo(0) {}
#endif /**__cplusplus**/

	}SM_PluginResult;

}


#endif /* __SM_PLUGINEXCEPTION_H__ */
