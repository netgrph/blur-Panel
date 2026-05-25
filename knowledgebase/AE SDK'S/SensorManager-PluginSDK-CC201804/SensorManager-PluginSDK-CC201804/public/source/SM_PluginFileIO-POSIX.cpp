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


#include "SM_PluginFileIO.h"
#include "SM_PluginTypedefs.h"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#if (! SM_MacBuild)
	#error "This is the POSIX implementation of PluginFile_IO for Mac"
#endif


bool PluginFile_IO::Exists ( SM_NAMESPACE::SM_StringPtr filePath )
{
	struct stat info;
	int err = stat ( filePath, &info );
	return (err == 0);
}
bool PluginFile_IO::Create ( const char* filePath , bool append /*=false*/, bool createAlways /*=false*/)
{
	if(!createAlways)
	{
		if ( PluginFile_IO::Exists ( filePath ) ) {
			if ( PluginFile_IO::GetFileMode ( filePath ) == kFMode_IsFile ) return false;
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FileNotExist)
		}
	}
	
	int flags = (O_CREAT | O_EXCL | O_RDWR);
	if(createAlways)
	{
		flags = (O_CREAT | O_TRUNC | O_RDWR);
	}
	if(append)
		flags|= O_APPEND;

	mode_t mode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	int refNum = open ( filePath, flags, mode );	// *** Include O_EXLOCK?
	if ( refNum == -1 )
	{
        THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotCreateFile)
	}
	close ( refNum );
	return true;
	
}
void PluginFile_IO::Delete ( const char* filePath )
{
	int err;
	
	switch ( PluginFile_IO::GetFileMode ( filePath ) ) {
			
		case PluginFile_IO::kFMode_DoesNotExist :
			return;
			
		case PluginFile_IO::kFMode_IsFile :
			err = unlink ( filePath );
			if ( err != 0 ){
                THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotDeleteFile)
			}
			return;
			
		case PluginFile_IO::kFMode_IsFolder :
			err = rmdir ( filePath );
			if ( err != 0 )
			{
				THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotDeleteFolder)
			}
			return;
			
		case PluginFile_IO::kFMode_IsOther :
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotDeleteFile)
			
	}
	
}


// Returns PluginFile_IO::invalidFileRef (0) if the file does not exist, throws for other errors.
PluginFile_IO::FileRef PluginFile_IO::Open ( SM_NAMESPACE::SM_StringPtr filePath, bool readOnly, bool append /*=false*/ )
{
	int flags = (readOnly ? O_RDONLY : O_RDWR);
	if(append)
		flags|=O_APPEND;
	
	int refNum = open ( filePath, flags, ( S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP ) );
	
	if ( refNum == -1 ) {
		
		int osCode = errno;
		
		if ( osCode == EACCES ) {
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FilePermissionError)
		} else {
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotOpenFile)
		}
	}
	
	if ( ! readOnly ) {
		// A root user might be able to open a write-protected file w/o complaint.
		struct stat info;
		if ( fstat ( refNum, &info ) == -1 ){
            THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotOpenFile)
		}
		if ( 0 == (info.st_mode & S_IWUSR) ){
            THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FilePermissionError)
		}
	}
	
	return refNum;
	
}	// PluginFile_IO::Open

void PluginFile_IO::Close ( PluginFile_IO::FileRef refNum )
{
	if ( refNum == PluginFile_IO::invalidFileRef ) return;
	
	int err = close ( refNum );
	if ( err != 0 ) {
         THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotCloseFile)
	}
	
}	// PluginFile_IO::Close

// =================================================================================================
// PluginFile_IO::Seek
// =============

SM_NAMESPACE::SM_Int64 PluginFile_IO::Seek ( PluginFile_IO::FileRef refNum, SM_NAMESPACE::SM_Int64 offset, SeekMode mode )
{
	int posMode;
	switch ( mode ) {
		case kSM_SeekFromStart :
			posMode = SEEK_SET;
			break;
		case kSM_SeekFromCurrent :
			posMode = SEEK_CUR;
			break;
		case kSM_SeekFromEnd :
			posMode = SEEK_END;
			break;
		default :
            THROW_PLUGIN_EXCEPTION_IN_PARSING(SM_NAMESPACE::kSMPlugin_FileSeekFailure, offset, 0)
			break;
	}
	
	SM_NAMESPACE::SM_Int64 newPos = (SM_NAMESPACE::SM_Int64) lseek ( refNum, offset, mode );
	if ( newPos == -1 )
         THROW_PLUGIN_EXCEPTION_IN_PARSING(SM_NAMESPACE::kSMPlugin_FileSeekFailure, offset, 0)
	
	return newPos;
	
}	// PluginFile_IO::Seek

// =================================================================================================
// PluginFile_IO::Read
// =============

#define TwoGB (SM_NAMESPACE::SM_Uns32)(2*1024*1024*1024UL)

SM_NAMESPACE::SM_Uns32 PluginFile_IO::Read ( PluginFile_IO::FileRef refNum, void * buffer, SM_NAMESPACE::SM_Uns32 count )
{
	if ( count >= TwoGB )
        THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FileReadFailure)
	
	size_t bytesRead = read ( refNum, buffer, count );
	if ( bytesRead == -1 )
       THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FileReadFailure)
	
	return static_cast<SM_NAMESPACE::SM_Uns32>( bytesRead );
	
}

// =====================================   Folder operations   =====================================

PluginFile_IO::FileMode PluginFile_IO::GetFileMode ( SM_NAMESPACE::SM_StringPtr path )
{
	struct stat fileInfo;
	
	int err = stat ( path, &fileInfo );
	if ( err != 0 ) return kFMode_DoesNotExist;
	if ( S_ISREG ( fileInfo.st_mode ) ) return kFMode_IsFile;
	if ( S_ISDIR ( fileInfo.st_mode ) ) return kFMode_IsFolder;
	return kFMode_IsOther;
	
}	// PluginFile_IO::GetFileMode

