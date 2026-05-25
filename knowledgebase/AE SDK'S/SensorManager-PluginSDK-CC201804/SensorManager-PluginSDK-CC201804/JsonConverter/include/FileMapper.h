#ifndef __FILEMAPPER_H__
#define __FILEMAPPER_H__

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


#include <string>
#include "SM_Const.h"

#if SM_WinBuild
#include <windows.h>
#endif

class FileMapper {
public:
    FileMapper(const SM_NAMESPACE::SM_UTF8String& filePath);
    ~FileMapper();
    SM_NAMESPACE::SM_StringPtr GetFilePointer() const;
    SM_NAMESPACE::SM_Uns64 GetFileSize() const;
    
private:
#if SM_WinBuild
	HANDLE hFile;
	HANDLE hFileMappingObject;
	void* CreateMapObjectAndFileView(HANDLE fileHandle);
#endif
    void* pfileMappingAddress;
    SM_NAMESPACE::SM_Uns64 fileSize;
    void CreateMapping(const SM_NAMESPACE::SM_UTF8String& filePath);
    void CloseMapping();
};



#endif /*__FILEMAPPER_H__*/
