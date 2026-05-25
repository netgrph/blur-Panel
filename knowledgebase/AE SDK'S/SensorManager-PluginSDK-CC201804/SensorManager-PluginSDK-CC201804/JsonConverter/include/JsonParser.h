#ifndef __JSONPARSER_H__
#define __JSONPARSER_H__

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

#include "rapidjson/document.h"
#include "SM_Const.h"
#include "JsonIterator.h"

// =================================================================================================
/// \file JsonParser.h
/// \brief Class to parse json files using rapidjson library.
///
// =================================================================================================

class JsonParser
{
public:
   explicit JsonParser(const SM_NAMESPACE::SM_UTF8String& inFilePath);
    ~JsonParser();
    spJsonIterator GetDomIterator(bool needArrayIterator) const;
    
private:
    rapidjson::Document mJsonDom;
};


#endif /*__JSONPARSER_H__*/
