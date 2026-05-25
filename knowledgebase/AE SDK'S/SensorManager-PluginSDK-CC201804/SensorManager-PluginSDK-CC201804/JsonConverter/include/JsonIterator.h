#ifndef __JSONITERATOR_H__
#define __JSONITERATOR_H__

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

#include "SM_Const.h"
#include "SM_DataTypesCommon.h"
#include "rapidjson/document.h"
#include <memory>

// =================================================================================================
/// \file JsonIterator.h
/// \brief base class of json objects and arrays iterator.
///
// =================================================================================================

class JsonIterator;

typedef std::shared_ptr<JsonIterator> spJsonIterator;
typedef rapidjson::Value::ConstMemberIterator ConstObjectIterator;
typedef rapidjson::Value::ConstValueIterator ConstArrayIterator;
typedef const rapidjson::Value& ValueRef;


typedef enum{
    JsonType_NULL = 0,
    JsonType_STRING,
    JsonType_NUMBER,
    JsonType_BOOLEAN,
    JsonType_ARRAY,
    JsonType_OBJECT
}JsonType;

class JsonIterator
{
public:
    virtual ~JsonIterator(){}
    virtual bool HasNext() const = 0;
    virtual void Increment() = 0;
    
    virtual SM_NAMESPACE::SM_UTF8String GetPropertyKey() const = 0;
    virtual JsonType GetPropertyValueType() const = 0;
    
    //Property value getters
    virtual void GetPropertyStringValue(SM_NAMESPACE::SM_UTF8String& outvalue) const = 0;
    virtual void GetPropertyDoubleValue(double& outValue) const = 0;
    virtual void GetPropertyBoolValue(bool& outValue) const = 0;
    
    virtual spJsonIterator GetObjectIterator() const = 0;
    virtual spJsonIterator GetArrayIterator() const = 0;
    
    virtual SM_NAMESPACE::SM_Uns32 GetMemberCount() const = 0;
};


#endif /*__JSONITERATOR_H__*/
