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

#include "JsonParser.h"
#include "FileMapper.h"
#include "JsonObjectIterator.h"
#include "JsonArrayIterator.h"
#include "SM_PluginException.h"
#include "SM_PluginTypedefs.h"
#include "SM_Const.h"
#include <cassert>

JsonParser::JsonParser(const SM_NAMESPACE::SM_UTF8String& inFilePath)
{
    FileMapper mMappedFile(inFilePath);
    mJsonDom.Parse(mMappedFile.GetFilePointer());
    if(mJsonDom.HasParseError())
        THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMPlugin_ConversionFailed);
}

JsonParser::~JsonParser()
{
    
}

spJsonIterator JsonParser::GetDomIterator(bool needArrayIterator) const
{
	if(needArrayIterator)
		return std::make_shared<JsonArrayIterator>("dataOutline",mJsonDom.Begin(), mJsonDom.End());
	
    return std::make_shared<JsonObjectIterator>(mJsonDom.MemberBegin(), mJsonDom.MemberEnd());
}
