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


#include "FileMapper.h"
#include <cassert>
#include <sys/stat.h>
#include "SM_PluginTypedefs.h"

#if SM_MacBuild
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#elif SM_WinBuild
#else
#error "SM environment error"
#endif

FileMapper::FileMapper(const SM_NAMESPACE::SM_UTF8String& filePath) : pfileMappingAddress(NULL), fileSize(0)
{
    CreateMapping(filePath);
}

FileMapper::~FileMapper(){
	try {
		CloseMapping();
	}
	catch (...)
	{
		//to-do
		//Absorb all exceptions since dtors should never throw...may be better soln
	}
}

SM_NAMESPACE::SM_StringPtr FileMapper::GetFilePointer() const
{
    return static_cast<SM_NAMESPACE::SM_StringPtr>(pfileMappingAddress);
}

SM_NAMESPACE::SM_Uns64 FileMapper::GetFileSize() const
{
    return fileSize;
}

#if SM_MacBuild
void FileMapper::CreateMapping(const SM_NAMESPACE::SM_UTF8String& filePath) {
    
    SM_NAMESPACE::SM_Int32 fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
       THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed)
        
    struct stat s;
    SM_NAMESPACE::SM_Int32 status = fstat(fd, &s);
    
    if (status != 0) {
        close(fd);
        THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed)
    }
    
    SM_NAMESPACE::SM_Uns64 size = s.st_size;
    void * pMappingAddress = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    if(pMappingAddress == MAP_FAILED){
        close(fd);
        THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed)
    }
    pfileMappingAddress = pMappingAddress;
    fileSize = size;
    close(fd);
}

void FileMapper::CloseMapping() {
    
    SM_NAMESPACE::SM_Int16 errno = munmap(pfileMappingAddress,fileSize);
    assert(errno == 0); //ideally it should not happen.Also we dont have any way to report this error.
    pfileMappingAddress = NULL;
    fileSize = 0;
}

#elif SM_WinBuild

bool IsLongPath(const std::string& path) {
	if (path.find("\\\\?\\") == 0) return true;
	return false;
}

bool IsNetworkPath(const std::string& path) {
	if (path.find("\\\\") == 0) return true;
	return false;
}

bool IsRelativePath(const std::string& path) {
	if (path.length() > 2) {
		char driveLetter = path[0];
		if ((driveLetter >= 'a' && driveLetter <= 'z') || (driveLetter >= 'A' && driveLetter <= 'Z')) {
			if (path[1] == ':' && path[2] == '\\') {
				if (path.find(".\\") == std::string::npos)
					return false;
}
		}
	}
	return true;
}

std::string & CorrectSlashes(std::string & path) {
	size_t idx = 0;

	while ((idx = path.find_first_of('/', idx)) != std::string::npos)
		path.replace(idx, 1, "\\");
	return path;
}

bool GetWidePath(const char* path, std::string & widePath) {
	std::string utfPath(path);
	CorrectSlashes(utfPath);

	if (!IsLongPath(utfPath)) {
		if (IsNetworkPath(utfPath)) {
			utfPath = "\\\\?\\UNC\\" + utfPath.substr(2);
		}
		else if (IsRelativePath(utfPath)) {
			//don't do anything
		}
		else { // absolute path
			utfPath = "\\\\?\\" + utfPath;
		}
	}

	widePath.clear();
	const size_t sizeReq = MultiByteToWideChar(CP_UTF8, 0, utfPath.c_str(), -1, NULL, 0);

	widePath.reserve(sizeReq * sizeof(WCHAR));
	widePath.assign(sizeReq * sizeof(WCHAR), '\0');
	int wideLen = MultiByteToWideChar(CP_UTF8, 0, utfPath.c_str(), -1, (LPWSTR)widePath.data(), (int)sizeReq);
	if (wideLen == 0) return false;
	return true;
}

void FileMapper::CreateMapping(const SM_NAMESPACE::SM_UTF8String& filePath) {

	std::string wideName;
	if (!GetWidePath(filePath.c_str(), wideName) || wideName.length() == 0)
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed)


	hFile = CreateFileW((LPCWSTR)wideName.data(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_OVERLAPPED, NULL);

	unsigned long osCode = GetLastError();
	if (hFile == INVALID_HANDLE_VALUE || osCode != 0) {
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed)
	}

	LARGE_INTEGER lFileSize;
	if (!GetFileSizeEx(hFile, &lFileSize))
	{
		CloseHandle(hFile);
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed)
	}

	fileSize = lFileSize.QuadPart;

	pfileMappingAddress = CreateMapObjectAndFileView(hFile);
	if (pfileMappingAddress == NULL)
	{
		CloseHandle(hFile);
		THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed)
	}
}

void* FileMapper::CreateMapObjectAndFileView(HANDLE fileHandle)
{
	hFileMappingObject = CreateFileMapping(fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
	DWORD e = GetLastError();
	if (hFileMappingObject == NULL) {
		CloseHandle(fileHandle);
		return NULL;
	}

	pfileMappingAddress = MapViewOfFile(hFileMappingObject, FILE_MAP_READ, 0, 0, 0);
	if (pfileMappingAddress == NULL) {
		CloseHandle(hFileMappingObject);
		CloseHandle(hFile);
		return NULL;
	}
	return pfileMappingAddress;
}

void FileMapper::CloseMapping() {
	UnmapViewOfFile(pfileMappingAddress);
	CloseHandle(hFileMappingObject);
	CloseHandle(hFile);
	pfileMappingAddress = NULL;
	fileSize = 0;
}

#else

#error "SM environment error"

#endif

