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



#include<string>
#include "SM_PluginBase.h"
#include "JsonConverter.h"
#include "FileMapper.h"
#include "JsonParser.h"
#include "IMgjsonStaticDataNum.h"
#include "IMgjsonStaticDataString.h"
#include "IMgjsonStaticDataBool.h"
#include "IMgjsonDynamicDataNumArr.h"
#include <iostream>


namespace SM_PLUGIN
{
	JsonConverter::JsonConverter(const SM_NAMESPACE::SM_FileExtension& inExt, const SM_NAMESPACE::SM_UTF8String& inFilePath, const SM_NAMESPACE::SM_UTF8String& inMGJSONPath)
    : SM_PluginBase(inExt, inFilePath, inMGJSONPath),matchNameCount(0) {}


	bool JsonConverter::convertToMGJSON() {
		
        spIMgjsonRoot root = IMgjsonRoot_v1::CreateRoot ( this , getMGJSONPath ( ).c_str ( ) );
        root->SetCreator(JSON_CREATOR_STRING);
        
        JsonParser parser(this->getFilePath());
		
        spJsonIterator domIterator = parser.GetDomIterator(isTopLevelArray);
        
        if(AppendItems(root, domIterator))
        {
            root->Commit();
            return true;
        }
        return false;
	}
    
    bool JsonConverter::AppendItems(spIMgjsonGroup group, spJsonIterator& iter)
    {
        bool isPropertyAddedInRoot = false;
        while(iter->HasNext())
        {
            JsonType propType = iter->GetPropertyValueType();
            switch (propType) {
                case JsonType_NUMBER:
                    isPropertyAddedInRoot |= AddDoubleData(group,iter);
                    break;
                case JsonType_STRING:
                    isPropertyAddedInRoot |= AddStringData(group, iter);
                    break;
                case JsonType_BOOLEAN:
                    isPropertyAddedInRoot |= AddBooleanData(group, iter);
                    break;
                case JsonType_ARRAY:
                    isPropertyAddedInRoot |= AddArrayData(group, iter);
                    break;
                case JsonType_OBJECT:
                    isPropertyAddedInRoot |= AddObjectData(group, iter);
                    break;
                case JsonType_NULL:
                default:
                    break;
            }
            
            iter->Increment();
        }
        return isPropertyAddedInRoot;
    }
             
   
    bool JsonConverter::AddDoubleData(spIMgjsonGroup group, const spJsonIterator& iter)
    {
        double propValue = 0;
        iter->GetPropertyDoubleValue(propValue);
        group->AddStaticData(IMgjsonStaticDataNum_v1::CreateStaticDataNum( this ,
                                                                  GetNewMatchName() ,
                                                                  iter->GetPropertyKey() ,
			propValue,SM_NAMESPACE::SM_NumberProperties()));
        return true;
    }
    
    bool JsonConverter::AddStringData(spIMgjsonGroup group, const spJsonIterator& iter)
    {
        SM_NAMESPACE::SM_UTF8String propValue;
        iter->GetPropertyStringValue(propValue);
        group->AddStaticData(IMgjsonStaticDataString_v1::CreateStaticDataString ( this ,
                                                                  GetNewMatchName() ,
                                                                  iter->GetPropertyKey() ,
			propValue, SM_NAMESPACE::SM_StringProperties()));
        return true;
    }
    
    bool JsonConverter::AddBooleanData(spIMgjsonGroup group, const spJsonIterator& iter)
    {
        bool propValue = false;
        iter->GetPropertyBoolValue(propValue);
        group->AddStaticData(IMgjsonStaticDataBool_v1::CreateStaticDataBool ( this ,
                                                                  GetNewMatchName(),
                                                                  iter->GetPropertyKey() ,
			propValue));
        return true;
    }
    
    bool JsonConverter::AddObjectData(spIMgjsonGroup group, const spJsonIterator& iter)
    {
        if(iter->GetMemberCount() == 0)
            return false;

        spIMgjsonGroup groupObj = IMgjsonGroup_v1::CreateGroup(this, GetNewMatchName(), iter->GetPropertyKey());
        spJsonIterator objIter = iter->GetObjectIterator();
        if(AppendItems(groupObj, objIter))
        {
            group->AddGroup(groupObj);
            return true;
        }
        return false;
    }
    
    bool JsonConverter::AddArrayData(spIMgjsonGroup group, const spJsonIterator& iter)
    {
        
        if(iter->GetMemberCount() == 0)
            return false;
        
        spIMgjsonGroup groupObj = IMgjsonGroup_v1::CreateGroup(this, GetNewMatchName(), iter->GetPropertyKey());
        spJsonIterator arrIter = iter->GetArrayIterator();
        if(AppendItems(groupObj, arrIter))
        {
            group->AddGroup(groupObj);
            return true;
        }
        return false;
    }
    
	
	bool JsonConverter::initialize() {
		return true;

	}

	bool JsonConverter::terminate() {
		return true;
	}
    
    bool JsonConverter::isTopLevelArray = false;

	bool JsonConverter::checkFileFormat(const SM_NAMESPACE::SM_UTF8String& inFilePath) {
        FileMapper mmappedFile(inFilePath);
        SM_NAMESPACE::SM_StringPtr fileStart = mmappedFile.GetFilePointer();
        SM_NAMESPACE::SM_Uns64 fileSize = mmappedFile.GetFileSize();
        SM_NAMESPACE::SM_StringPtr fileEnd = fileStart + fileSize - 1;
        
//        bool jsonStartFound = false;
//        bool jsonEndFound = false;
        
        /*JSON file must start with '{'*/
        while(fileStart != NULL && fileStart < fileEnd)
        {
            if(*fileStart == ' ' || *fileStart == '\n' || *fileStart == '\t' || *fileStart == '\r')
            {
                ++fileStart;
                continue;
            }
            
            if(*fileStart == '[')
            {
                isTopLevelArray = true;
                return true;
            }
            else if(*fileStart == '{')
            {
                isTopLevelArray = false;
                return true;
            }
            else
            {
                return false;
            }
        }
        
        return false;
        
//        if(!jsonStartFound)
//            return false;
//        
//        /*JSON file must end with '}'*/
//        while(fileEnd != NULL && fileEnd > fileStart)
//        {
//            if(*fileEnd == ' ' || *fileEnd == '\n' || *fileEnd == '\t' || *fileEnd == '\r')
//            {
//                --fileEnd;
//                continue;
//            }
//            
//            if(*fileEnd != '}')
//                return false;
//            else
//            {
//                jsonEndFound = true;
//                break;
//            }
//        }
//        
//        return (jsonStartFound && jsonEndFound);
//        
	}

    
    SM_NAMESPACE::SM_AsciiString JsonConverter::GetNewMatchName()
    {
        SM_NAMESPACE::SM_StringPtr matchNamePrefix = "matchName";
        return matchNamePrefix + std::to_string(matchNameCount++);
    }
	

	const char* GetPluginIdentifier() {
		// implementation
		return "com.adobe.sensor.plugin.jsonConverter";
	}

	void RegisterConverters() {
		// implementation
		SM_PluginRegistry::registerConverter(new SM_PluginCreator<JsonConverter>("com.adobe.sensor.jsonConverter"));
	}
}
