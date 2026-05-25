#ifndef __JSONOBJECTITERATOR_H__
#define __JSONOBJECTITERATOR_H__

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

#include "JsonIterator.h"

// =================================================================================================
/// \file JsonObjectIterator.h
/// \brief class to iterate over Json objects.
///
// =================================================================================================

class JsonObjectIterator : public JsonIterator
{
public:
    JsonObjectIterator(ConstObjectIterator pMemberBegin, ConstObjectIterator pMemberEnd);
    ~JsonObjectIterator(){}
    
    bool HasNext() const;
    void Increment();
    
    SM_NAMESPACE::SM_UTF8String GetPropertyKey() const;
    JsonType GetPropertyValueType() const;
    
    //Property value getters
    void GetPropertyStringValue(SM_NAMESPACE::SM_UTF8String& outvalue) const;
    void GetPropertyDoubleValue(double& outValue) const;
    void GetPropertyBoolValue(bool& outValue) const;
    
    spJsonIterator GetObjectIterator() const;
    spJsonIterator GetArrayIterator() const;
    SM_NAMESPACE::SM_Uns32 GetMemberCount() const;
    
private:
    ConstObjectIterator mMemberBegin;
    ConstObjectIterator mMemberEnd;
    ConstObjectIterator mMemberCurrent;
};


#endif /*__JSONOBJECTITERATOR_H__*/
