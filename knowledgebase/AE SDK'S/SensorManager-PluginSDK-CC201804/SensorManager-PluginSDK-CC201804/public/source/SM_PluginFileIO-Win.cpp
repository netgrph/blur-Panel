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

#if SM_WinBuild
	#pragma warning ( disable : 4800 )	// forcing value to bool 'true' or 'false' (performance warning)
#endif

// =================================================================================================
// PluginFile_IO implementations for Windows
// ===================================

#if ! SM_WinBuild
	#error "This is the Windows implementation of PluginFile_IO."
#endif

static bool IsLongPath ( const std::string& path );
static bool IsNetworkPath ( const std::string& path );
static bool IsRelativePath ( const std::string& path );
static bool GetWidePath ( const char* path, std::string & widePath );
static std::string & CorrectSlashes ( std::string & path );

static bool Exists ( const std::string & widePath );
static PluginFile_IO::FileRef Open ( const std::string & widePath, bool readOnly, bool append = false );
static PluginFile_IO::FileMode GetFileMode ( const std::string & widePath );


bool PluginFile_IO::Exists ( const char* filePath )
{
	std::string wideName;
	if ( GetWidePath(filePath, wideName) ) {
		return ::Exists(wideName);
	}
	return false;
}

// =================================================================================================
// PluginFile_IO::Create
// ===============

bool PluginFile_IO::Create ( const char* filePath, bool append  /*=false*/, bool createAlways /*=false*/ )
{
	std::string wideName;
	if (!GetWidePath(filePath, wideName) || wideName.length() == 0)
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotConvertFilePath)

	if (!createAlways)
	{
		if (::Exists(wideName)) {
			if (::GetFileMode(wideName) == kFMode_IsFile) return false;
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FileNotExist)
		}
	}

	PluginFile_IO::FileRef fileHandle;
	DWORD access = GENERIC_READ;
	if (append)
		access |= FILE_APPEND_DATA;
	else
		access |= GENERIC_WRITE;

	fileHandle = CreateFileW ( (LPCWSTR)wideName.data(), access, 0, 0, CREATE_ALWAYS,
 							   (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS), 0 );
	if( fileHandle == INVALID_HANDLE_VALUE ) 
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotCreateFile)

	CloseHandle ( fileHandle );
	return true;
}	// PluginFile_IO::Create

// =================================================================================================
// PluginFile_IO::Open
// =============
//
// Returns PluginFile_IO::invalidFileRef (0) if the file does not exist, throws for other errors.

PluginFile_IO::FileRef PluginFile_IO::Open ( const char* filePath, bool readOnly, bool append /*=false*/ )
{
	std::string wideName;
	if( !GetWidePath ( filePath, wideName ) || wideName.length ( ) == 0 ) 
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotConvertFilePath)

	return ::Open( wideName, readOnly, append );

}	// PluginFile_IO::Open

// =================================================================================================
// PluginFile_IO::Close
// ==============

void PluginFile_IO::Close ( PluginFile_IO::FileRef fileHandle )
{
	if ( fileHandle == PluginFile_IO::invalidFileRef ) return;

	BOOL ok = CloseHandle ( fileHandle );
	if( !ok ) 
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotCloseFile)

}	// PluginFile_IO::Close

// =================================================================================================
// PluginFile_IO::Delete
// ===============

void PluginFile_IO::Delete ( const char* filePath )
{
	std::string wideName;
	if( !GetWidePath ( filePath , wideName ) || wideName.length ( ) == 0 ) 
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotConvertFilePath)
	
	if ( !::Exists ( wideName ) ) return;

	BOOL ok = DeleteFileW ( (LPCWSTR)wideName.data() );
	if ( ! ok ) {
		DWORD errCode = GetLastError();
		if ( errCode != ERROR_FILE_NOT_FOUND ) {
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotDeleteFile)
		}
	}

}	// PluginFile_IO::Delete

// =================================================================================================
// PluginFile_IO::Seek
// =============

SM_NAMESPACE::SM_Int64 PluginFile_IO::Seek ( PluginFile_IO::FileRef fileHandle, SM_NAMESPACE::SM_Int64 offset, SeekMode mode )
{
	DWORD method;
	switch ( mode ) {
		case kSM_SeekFromStart :
			method = FILE_BEGIN;
			break;
		case kSM_SeekFromCurrent :
			method = FILE_CURRENT;
			break;
		case kSM_SeekFromEnd :
			method = FILE_END;
			break;
		default :
			THROW_PLUGIN_EXCEPTION_IN_PARSING(SM_NAMESPACE::kSMPlugin_FileSeekFailure,offset,0)
			break;
	}

	LARGE_INTEGER seekOffset, newPos;
	seekOffset.QuadPart = offset;

	BOOL ok = SetFilePointerEx ( fileHandle, seekOffset, &newPos, method );
	if( !ok ) 
		THROW_PLUGIN_EXCEPTION_IN_PARSING(SM_NAMESPACE::kSMPlugin_FileSeekFailure, offset, 0)

	return newPos.QuadPart;

}	// PluginFile_IO::Seek

// =================================================================================================
// PluginFile_IO::Read
// =============

#define TwoGB (SM_NAMESPACE::SM_Uns32)(2*1024*1024*1024UL)

SM_NAMESPACE::SM_Uns32 PluginFile_IO::Read ( PluginFile_IO::FileRef fileHandle, void * buffer, SM_NAMESPACE::SM_Uns32 count )
{
	if( count >= TwoGB ) 
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FileReadFailure)

	DWORD bytesRead;
	BOOL ok = ReadFile ( fileHandle, buffer, count, &bytesRead, 0 );
	if( !ok ) 
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FileReadFailure)

	return bytesRead;

}	// PluginFile_IO::Read


static DWORD kOtherAttrs = (FILE_ATTRIBUTE_DEVICE);

PluginFile_IO::FileMode PluginFile_IO::GetFileMode ( const char * path )
{
	std::string utf16;	// GetFileAttributes wants native UTF-16.
	if( !GetWidePath ( path , utf16 ) ) 
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotConvertFilePath)
	
	return ::GetFileMode( utf16 );
}	// PluginFile_IO::GetFileMode

// =================================================================================================
// IsLongPath
// =====================

bool IsLongPath ( const std::string& path ) {
	if ( path.find ( "\\\\?\\" ) == 0 ) return true;
	return false;
}

// =================================================================================================
// IsNetworkPath
// =====================

bool IsNetworkPath ( const std::string& path ) {
	if ( path.find ( "\\\\" ) == 0 ) return true;
	return false;
}

// =================================================================================================
// IsRelativePath
// =====================

bool IsRelativePath ( const std::string& path ) {
	if ( path.length() > 2) {
		char driveLetter = path[0];
		if ( ( driveLetter >= 'a' && driveLetter <= 'z' ) || ( driveLetter >= 'A' && driveLetter <= 'Z' ) ) {
			if (path[1] == ':' && path[2] == '\\' ) {
				if ( path.find(".\\") == std::string::npos )
					return false;
			}
		}
	}
	return true;
}

// =================================================================================================
// GetWidePath
// =====================

bool GetWidePath( const char* path, std::string & widePath ) {
	std::string utfPath ( path );
	CorrectSlashes ( utfPath);

	if ( !IsLongPath ( utfPath ) ) {
		if ( IsNetworkPath ( utfPath ) ) {
			utfPath = "\\\\?\\UNC\\" + utfPath.substr ( 2 );
		} else if ( IsRelativePath ( utfPath ) ) {
			//don't do anything
		} else { // absolute path
			utfPath = "\\\\?\\" + utfPath;
		}
	}

	widePath.clear();
	const size_t maxLen =  MultiByteToWideChar ( CP_UTF8, 0, utfPath.c_str(), -1, (LPWSTR)0, (int)0 );

	widePath.reserve ( maxLen * sizeof (WCHAR) );
	widePath.assign ( maxLen * sizeof (WCHAR) , '\0' );
	int wideLen = MultiByteToWideChar ( CP_UTF8, 0, utfPath.c_str(), -1, (LPWSTR)widePath.data(), (int)maxLen );
	if ( wideLen == 0 ) return false;
	return true;
}

// =================================================================================================
// CorrectSlashes
// =====================

std::string & CorrectSlashes ( std::string & path ) {
	size_t idx = 0;

	while( (idx = path.find_first_of('/',idx)) != std::string::npos )
		path.replace( idx, 1, "\\" );
	return path;
}

bool Exists ( const std::string & widePath )
{
	DWORD attrs = INVALID_FILE_ATTRIBUTES;
	attrs = GetFileAttributesW ( (LPCWSTR)widePath.data() );
	return ( attrs != INVALID_FILE_ATTRIBUTES);
}


PluginFile_IO::FileMode GetFileMode ( const std::string & widePath )
{
	// ! A shortcut is seen as a file, we would need extra code to recognize it and find the target.
	DWORD fileAttrs = INVALID_FILE_ATTRIBUTES;

	fileAttrs =  GetFileAttributesW ( (LPCWSTR) widePath.c_str() );
	if ( fileAttrs == INVALID_FILE_ATTRIBUTES ) return PluginFile_IO::kFMode_DoesNotExist;	// ! Any failure turns into does-not-exist.

	if ( fileAttrs & FILE_ATTRIBUTE_DIRECTORY ) return PluginFile_IO::kFMode_IsFolder;
	if ( fileAttrs & kOtherAttrs ) return PluginFile_IO::kFMode_IsOther;
	return PluginFile_IO::kFMode_IsFile;

}

PluginFile_IO::FileRef Open ( const std::string & widePath, bool readOnly, bool append /*=false*/)
{  
	DWORD access = GENERIC_READ;	// Assume read mode.
	DWORD share  = FILE_SHARE_READ | FILE_SHARE_WRITE;

	if ( ! readOnly  ) {
		if (!append)
		{
			access |= GENERIC_WRITE;
			share = 0;
		}
		else
		{
			access |= FILE_APPEND_DATA;
			share = 0;
		}
	}
	
	PluginFile_IO::FileRef fileHandle;
	fileHandle = CreateFileW ( (LPCWSTR)widePath.data(), access, share, 0, OPEN_EXISTING,
		(FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS), 0 );

	if ( fileHandle == INVALID_HANDLE_VALUE ) {
		DWORD osCode = GetLastError();
		 if ( osCode == ERROR_ACCESS_DENIED ) {
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_FilePermissionError)
		} else {
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_CannotOpenFile)
		}
	}

	return fileHandle;
}
