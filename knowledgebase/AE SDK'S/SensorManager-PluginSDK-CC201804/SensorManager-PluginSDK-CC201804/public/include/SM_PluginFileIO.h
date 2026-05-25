
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

#ifndef __PLUGIN_FILE_IO_H__
#define __PLUGIN_FILE_IO_H__	1

#include <string>
#include "SM_Const.h"

#if SM_WinBuild
	#include <Windows.h>
#elif SM_MacBuild
	#include <CoreServices/CoreServices.h>
	#include <dirent.h>	// Mac uses the POSIX folder functions.
#else
	#error "Unknown host platform."
#endif

namespace PluginFile_IO {

	#if SM_WinBuild
		typedef HANDLE FileRef;
		static const FileRef invalidFileRef = INVALID_HANDLE_VALUE;
		typedef HANDLE FolderRef;
		static const FolderRef invalidFolderRef = INVALID_HANDLE_VALUE;
		#define kDirChar '\\'

	#elif SM_MacBuild
		typedef FSIORefNum FileRef;
		static const FileRef invalidFileRef = -1;
		typedef off_t SM_off_t;
		
		#define kDirChar '/'

	#endif
	
	enum {
		kFMode_DoesNotExist,
		kFMode_IsFile,
		kFMode_IsFolder,
		kFMode_IsOther
	};
	
	typedef SM_NAMESPACE::SM_Uns8 FileMode;	
	
	//! @breief Returns true if the path exists, whether as a file, folder, or anything else. 
	bool Exists ( SM_NAMESPACE::SM_StringPtr filePath );

	//!@brief  Create a file if possible, return true if successful. Return false if the file
	// already exists. 
	bool Create ( SM_NAMESPACE::SM_StringPtr filePath, bool append = false, bool createAlways = false );

	//!@brief Opens the file in readOnly, read-write or append mode.
	FileRef	Open ( SM_NAMESPACE::SM_StringPtr filePath, bool readOnly, bool append = false );

	//!@brief Close a file. Does nothing if the FileRef is noFileRef. 
	void Close ( FileRef file );

	//!@brief Deletes a file or folder. Does nothing if the path does not exist.
	void Delete ( const char* filePath );

	//! @brief Returns an enum telling if a path names a file, folder, other, or nothing.
	FileMode GetFileMode  ( SM_NAMESPACE::SM_StringPtr path );


	enum SeekMode { kSM_SeekFromStart, kSM_SeekFromCurrent, kSM_SeekFromEnd };

	//! @brief Change the I/O position of an open file, returning the new absolute offset.
	SM_NAMESPACE::SM_Int64	Seek     ( FileRef file, SM_NAMESPACE::SM_Int64 offset, SeekMode mode );

	//! @brief Read into a buffer returning the number of bytes read. Requests are limited to less
	// than 2GB in case the host uses an UnsignedInt32 count.
	SM_NAMESPACE::SM_Uns32	Read     ( FileRef file, void* buffer, SM_NAMESPACE::SM_Uns32 count );

};

#endif	// __PLUGIN_FILE_IO_H__
