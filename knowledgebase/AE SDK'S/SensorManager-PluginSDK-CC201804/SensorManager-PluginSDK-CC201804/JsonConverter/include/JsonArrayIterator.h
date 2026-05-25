#ifndef __JSONARRAYITERATOR_H__
#define __JSONARRAYITERATOR_H__

// =================================================================================================
// ADOBE SYSTEMS INCORPORATED
// Copyright 2017 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE: Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it.
// =================================================================================================

#include "JsonIterator.h"
#include "SM_Const.h"

// =================================================================================================
/// \file JsonArrayIterator.h
/// \brief class to iterate over Json arrays.
///
// =================================================================================================

class JsonArrayIterator : public JsonIterator
{
public:
    JsonArrayIterator(const SM_NAMESPACE::SM_UTF8String& keyName,ConstArrayIterator pMemberBegin,ConstArrayIterator pMemberEnd);
    
    ~JsonArrayIterator(){}
    
    bool HasNext() const;
    void Increment();
    
    spJsonIterator GetObjectIterator() const;
    spJsonIterator GetArrayIterator() const;
    
    SM_NAMESPACE::SM_UTF8String GetPropertyKey() const;
    JsonType GetPropertyValueType() const;
    
    //Property value getters
    void GetPropertyStringValue(SM_NAMESPACE::SM_UTF8String& outvalue) const;
    void GetPropertyDoubleValue(double& outValue) const;
    void GetPropertyBoolValue(bool& outValue) const;
    SM_NAMESPACE::SM_Uns32 GetMemberCount() const;
    
private:
    SM_NAMESPACE::SM_UTF8String mKeyName;
    ConstArrayIterator mMemberBegin;
    ConstArrayIterator mMemberEnd;
    ConstArrayIterator mMemberCurrent;
    SM_NAMESPACE::SM_Uns32 mCurrentMemberIndex;
};

#endif /*__JSONARRAYITERATOR_H__*/
