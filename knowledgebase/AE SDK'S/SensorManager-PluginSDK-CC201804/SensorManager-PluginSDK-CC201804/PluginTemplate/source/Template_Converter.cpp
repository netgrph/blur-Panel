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

#include "SM_PluginBase.h"
#include "Template_Converter.h"
#include "SM_PluginTypes.h"
#include "SM_DataTypesCommon.h"
#include "IMgjsonRoot.h"
#include "IMgjsonGroup.h"
#include "IMgjsonStaticDataString.h"
#include "IMgjsonStaticDataNum.h"
#include "IMgjsonStaticDataNumArr.h"
#include "IMgjsonStaticDataBool.h"
#include "IMgjsonDynamicDataNum.h"
#include "IMgjsonDynamicDataNumArr.h"
#include "IMgjsonDynamicDataString.h"

namespace SM_PLUGIN {

	Template_Converter::Template_Converter(const SM_NAMESPACE::SM_FileExtension& inExt, const SM_NAMESPACE::SM_UTF8String& inFilePath, const SM_NAMESPACE::SM_UTF8String& inMGJSONPath)
		: SM_PluginBase(inExt, inFilePath, inMGJSONPath) {}


	bool Template_Converter::convertToMGJSON() {

		// Sample code to demonstrate the usage of writer apis 

		SM_NAMESPACE::SM_DynamicFileInfo  dynamicInfo;
		dynamicInfo.mTimeInfo = SM_NAMESPACE::SM_UTCInfo(3, true);

		spIMgjsonRoot root = IMgjsonRoot_v1::CreateRoot(this, getMGJSONPath().c_str(), &dynamicInfo);
		root->SetCreator("Sample code");

		/******************************************************************/

		//writer API's sample code to add static data node
		spIMgjsonStaticData staticdata1 = IMgjsonStaticDataString_v1::CreateStaticDataString(this, "Season", "Season", "2018");
		root->AddStaticData(staticdata1);

		spIMgjsonStaticData staticdata2 = IMgjsonStaticDataNum_v1::CreateStaticDataNum(this, "RaceNumber", "Racer", 6);
		root->AddStaticData(staticdata2);

		spIMgjsonStaticData staticdata3 = IMgjsonStaticDataBool_v1::CreateStaticDataBool(this, "clearWeather", "clearWeather", true);
		root->AddStaticData(staticdata3);

		spIMgjsonStaticData staticdata4 = IMgjsonStaticDataNumArr_v1::CreateStaticDataNumArr(this, "Teammates", "Teammates", std::vector<double>{ 8, 17, 21 });
		root->AddStaticData(staticdata4);

		//writer API's sample code to create group

		spIMgjsonGroup group = IMgjsonGroup_v1::CreateGroup(this, "telemetry", "telemetry");

		/******************************************************************/

		//writer API's sample code to add dynamic node in a group
		SM_NAMESPACE::SM_DynamicDataFields inDynamicDataFields1("Speed", "Speed", "001", SM_NAMESPACE::kSM_InterpolationType_LINEAR, 0);
		//specifying legal range
		SM_NAMESPACE::SM_NumberProperties numProp(SM_NAMESPACE::SM_NumberRange(SM_NAMESPACE::SM_NumberMinMax(0, 200)));
		spIMgjsonDynamicDataNum dynamicNumData = IMgjsonDynamicDataNum_v1::CreateDynamicDataNum(this, inDynamicDataFields1, numProp);

		/*		Add two samples of type number
		*		time:2017-06-02T12:40:25.000Z, value: 166
		*		time:2017-06-02T12:40:26.000Z, value: 168
		*/

		SM_NAMESPACE::SM_TimeValue time1 = SM_NAMESPACE::SM_DateTimeUTC(2017, 06, 02, 12, 40, 25, 'Z', 0, 0, 0);
		SM_NAMESPACE::SM_TimeValue time2 = SM_NAMESPACE::SM_DateTimeUTC(2017, 06, 02, 12, 40, 26, 'Z', 0, 0, 0);

		dynamicNumData->AddSample(time1, 166);
		dynamicNumData->AddSample(time2, 168);

		SM_NAMESPACE::SM_Uns64 cnt = dynamicNumData->GetSampleCount();

		if(cnt > 0) {
			dynamicNumData->SetSampleCount(cnt);
			dynamicNumData->Commit();
			//Add dynamic samples of type number 
			group->AddDynamicData(dynamicNumData);
		}

		/******************************************************************/

		//Add dynamic samples of type string 
		SM_NAMESPACE::SM_DynamicDataFields inDynamicDataFields2("Gear", "Gear", "002", SM_NAMESPACE::kSM_InterpolationType_HOLD, 0, 2);
		spIMgjsonDynamicDataString dynamicStringData = IMgjsonDynamicDataString_v1::CreateDynamicDataString(this, inDynamicDataFields2);

		/*		Add two samples of type number
		*		time:2017-06-02T12:40:25.000Z, value: Fourth
		*		time:2017-06-02T12:40:26.000Z, value: Fifth
		*/
		dynamicStringData->AddSample(time1, "Fourth");
		dynamicStringData->AddSample(time2, "Fifth");
		dynamicStringData->Commit();
		group->AddDynamicData(dynamicStringData);

		/******************************************************************/

		//Add dynamic samples of type number array 
		SM_NAMESPACE::SM_DynamicDataFields inDynamicDataFields3("GPS (Lat\/Lon)", "GPS", "003", SM_NAMESPACE::kSM_InterpolationType_LINEAR, 1);
		SM_NAMESPACE::SM_NumberArrayProperties prop(2, 0, SM_NAMESPACE::SM_NumberArrayRange(), SM_NAMESPACE::SM_StringArray { "Latitude", "Longitude" });
		spIMgjsonDynamicDataNumArr dynamicNumArrData = IMgjsonDynamicDataNumArr_v1::CreateDynamicDataNumArr(this, inDynamicDataFields3, prop);

		/*		Add two samples of type number
		*		time:2017-06-02T12:40:25.000Z, value: 043.99416346, 011.36938006
		*		time:2017-06-02T12:40:26.000Z, value: 043.99418535, 011.36939430
		*/
		dynamicNumArrData->AddSample(time1, std::vector<double>{ 043.99416346, 011.36938006 });
		dynamicNumArrData->AddSample(time2, std::vector<double>{ 043.99418535, 011.36939430 });
		dynamicNumArrData->Commit();
		group->AddDynamicData(dynamicNumArrData);

		/******************************************************************/

		root->AddGroup(group);
		root->Commit();

		// note, if intermediatory mgjson file is needed , copy the file at desired location from path obtained by getMGJSONPath() api.
		// client can process the exception thrown by writer apis.
		// client can throw any exception using the macro THROW_PLUGIN_EXCEPTION. 
		// client can throw the exception occuring during the native file parsing using the macro THROW_PLUGIN_EXCEPTION_IN_PARSING 
		// so that the offset and line number of the native file could also be provided.

		return true;
	}

	bool Template_Converter::initialize() {
		// implementation
		return true;

	}

	bool Template_Converter::terminate() {
		// implementation
		return true;
	}

	bool Template_Converter::checkFileFormat(const SM_NAMESPACE::SM_UTF8String& inFilePath) {
		// implementation
		return true;

	}


	const char* GetPluginIdentifier() {
		// implementation
		return "com.adobe.sensor.plugin.template";
	}

	void RegisterConverters() {
		// implementation
		SM_PluginRegistry::registerConverter(
			new SM_PluginCreator<Template_Converter>("com.adobe.sensor.template"));
	}

}
