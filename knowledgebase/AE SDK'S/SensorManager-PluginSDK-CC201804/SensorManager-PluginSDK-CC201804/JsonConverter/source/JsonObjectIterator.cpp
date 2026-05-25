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

#include "JsonObjectIterator.h"
#include "JsonArrayIterator.h"

JsonObjectIterator::JsonObjectIterator(ConstObjectIterator pMemberBegin,ConstObjectIterator pMemberEnd)
:mMemberBegin(pMemberBegin),mMemberEnd(pMemberEnd),mMemberCurrent(pMemberBegin){}


bool JsonObjectIterator::HasNext() const
{
    return (mMemberCurrent != mMemberEnd);
}


void JsonObjectIterator::Increment()
{
    mMemberCurrent += 1;
}

SM_NAMESPACE::SM_UTF8String JsonObjectIterator::GetPropertyKey() const
{
    return SM_NAMESPACE::SM_UTF8String(mMemberCurrent->name.GetString());
}

JsonType JsonObjectIterator::GetPropertyValueType() const
{
    switch (mMemberCurrent->value.GetType()) {
        case rapidjson::kNullType:
            return JsonType_NULL;
            break;
        case rapidjson::kFalseType:
        case rapidjson::kTrueType:
            return JsonType_BOOLEAN;
            break;
        case rapidjson::kObjectType:
            return JsonType_OBJECT;
            break;
        case rapidjson::kArrayType:
            return JsonType_ARRAY;
            break;
        case rapidjson::kStringType:
            return JsonType_STRING;
            break;
        case rapidjson::kNumberType:
            return JsonType_NUMBER;
            break;
        default:
            break;
    }
    return JsonType_NULL;
}

//Property value getters
void JsonObjectIterator::GetPropertyStringValue(SM_NAMESPACE::SM_UTF8String& outvalue) const
{
    assert(mMemberCurrent->value.IsString());
    outvalue.clear();
    outvalue.assign(mMemberCurrent->value.GetString(), mMemberCurrent->value.GetStringLength());
}

void JsonObjectIterator::GetPropertyDoubleValue(double& outValue) const
{
    assert(mMemberCurrent->value.IsNumber());
    
    ValueRef currentVal = mMemberCurrent->value;
    if(currentVal.IsInt())
        outValue = currentVal.GetInt();
    else if(currentVal.IsInt64())
        outValue = currentVal.GetInt64();
    else if(currentVal.IsUint())
        outValue = currentVal.GetUint();
    else if(currentVal.IsUint64())
        outValue = currentVal.GetUint64();
    else if (currentVal.IsDouble())
        outValue = currentVal.GetDouble();
    else
        assert(false);
}

void JsonObjectIterator::GetPropertyBoolValue(bool& outValue) const
{
    assert(mMemberCurrent->value.IsBool());
    outValue = mMemberCurrent->value.GetBool();
}

spJsonIterator JsonObjectIterator::GetObjectIterator() const
{
    assert(mMemberCurrent->value.IsObject());
    return std::make_shared<JsonObjectIterator>(mMemberCurrent->value.MemberBegin(), mMemberCurrent->value.MemberEnd());
}

spJsonIterator JsonObjectIterator::GetArrayIterator() const
{
    assert(mMemberCurrent->value.IsArray());
    return std::make_shared<JsonArrayIterator>(mMemberCurrent->name.GetString(),
                                               mMemberCurrent->value.Begin(),
                                               mMemberCurrent->value.End());
}

SM_NAMESPACE::SM_Uns32 JsonObjectIterator::GetMemberCount() const
{
    assert(mMemberCurrent->value.IsObject() || mMemberCurrent->value.IsArray());
    if(mMemberCurrent->value.IsObject())
        return mMemberCurrent->value.MemberCount();
    return mMemberCurrent->value.Size();
}

