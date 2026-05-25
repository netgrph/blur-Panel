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


#include "JsonArrayIterator.h"
#include "JsonObjectIterator.h"

JsonArrayIterator::JsonArrayIterator(const SM_NAMESPACE::SM_UTF8String& pKeyName,
                                     ConstArrayIterator pMemberBegin,
                                     ConstArrayIterator pMemberEnd) :
mKeyName(pKeyName), mMemberBegin(pMemberBegin),mMemberEnd(pMemberEnd), mMemberCurrent(pMemberBegin),mCurrentMemberIndex(0){}

bool JsonArrayIterator::HasNext() const
{
    return (mMemberCurrent != mMemberEnd);
}

void JsonArrayIterator::Increment()
{
    ++mCurrentMemberIndex;
    mMemberCurrent += 1;
}

spJsonIterator JsonArrayIterator::GetObjectIterator() const
{
    assert(mMemberCurrent->IsObject());
    return std::make_shared<JsonObjectIterator>(mMemberCurrent->MemberBegin(), mMemberCurrent->MemberEnd());
}

spJsonIterator JsonArrayIterator::GetArrayIterator() const
{
    assert(mMemberCurrent->IsArray());
    SM_NAMESPACE::SM_UTF8String currentKey =  mKeyName +" " + std::to_string(mCurrentMemberIndex);
    return std::make_shared<JsonArrayIterator>(currentKey, mMemberCurrent->Begin(), mMemberCurrent->End());
}

SM_NAMESPACE::SM_UTF8String JsonArrayIterator::GetPropertyKey() const
{
    return mKeyName +" " + std::to_string(mCurrentMemberIndex);
}

JsonType JsonArrayIterator::GetPropertyValueType() const
{
    switch (mMemberCurrent->GetType()) {
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

void JsonArrayIterator::GetPropertyStringValue(SM_NAMESPACE::SM_UTF8String& outvalue) const
{
    assert(mMemberCurrent->IsString());
    outvalue.clear();
    outvalue.assign(mMemberCurrent->GetString(), mMemberCurrent->GetStringLength());
}


void JsonArrayIterator::GetPropertyDoubleValue(double& outValue) const
{
    assert(mMemberCurrent->IsNumber());
    
    if(mMemberCurrent->IsInt())
        outValue = mMemberCurrent->GetInt();
    else if(mMemberCurrent->IsInt64())
        outValue = mMemberCurrent->GetInt64();
    else if(mMemberCurrent->IsUint())
        outValue = mMemberCurrent->GetUint();
    else if(mMemberCurrent->IsUint64())
        outValue = mMemberCurrent->GetUint64();
    else if (mMemberCurrent->IsDouble())
        outValue = mMemberCurrent->GetDouble();
    else
        assert(false);
}


void JsonArrayIterator::GetPropertyBoolValue(bool& outValue) const
{
    assert(mMemberCurrent->IsBool());
    outValue = mMemberCurrent->GetBool();
}


SM_NAMESPACE::SM_Uns32 JsonArrayIterator::GetMemberCount() const
{
    assert(mMemberCurrent->IsObject() || mMemberCurrent->IsArray());
    if(mMemberCurrent->IsObject())
        return mMemberCurrent->MemberCount();
    return mMemberCurrent->Size();
}
