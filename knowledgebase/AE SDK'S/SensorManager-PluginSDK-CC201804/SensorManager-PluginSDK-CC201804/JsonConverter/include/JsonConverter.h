#ifndef __JSONCONVERTER_H__
#define __JSONCONVERTER_H__

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
#include "SM_FwdDeclarations.h"
#include "IMgjsonRoot.h"
#include "JsonIterator.h"

namespace SM_PLUGIN
{
    
#define JSON_CREATOR_STRING "Adobe JSON-MGJSON Converter"
	
	// =================================================================================================
	/// \file JsonConverter.h
	/// \brief Plugin converter to convert JSON file to MGJSON file.
	///
	/// Json can have 6 constructs : Objects, arrays, string, number, boolean and null.
	///
	/// Json to mgjson conversion rules:
	///
	///	Json Objects:  Json objects contain key-value pairs. Values can be any one of Json constructs.
	///					For value type Json strings, number and boolean, key value pair will be converted
	///					to mgjson dataStatic objects with key as display name.
	///					For value type Json arrays and objects, they will be converted into mgjson dataGroups.
	///					For value type json null, key value pair will be ignored and will not be part of converted
	///					mgjson.
	///
	/// Json Arrays:	Json arrays only contains value without keys as compared to Json objects. Since every mgjson
	///					object require display name, display names for array elements will be generated using parent name +
	///					index of element in the Json array. Rest of the handling of Json arrays is same as Json Objects.
	///
	/// Json strings, numbers,booleans and null : If input json contains only single string, number, boolean or null,
	///	JsonConverter will not convert these types of Json and JsonConverter::CheckFileFormat will return false.
	// =================================================================================================

	class JsonConverter : public SM_PluginBase
	{
	public:
		JsonConverter(const SM_NAMESPACE::SM_FileExtension& inExt,
                       const SM_NAMESPACE::SM_UTF8String& inFilePath,
                       const SM_NAMESPACE::SM_UTF8String& inMGJSONPath);

		virtual ~JsonConverter() {}

		// ---------------------------------------------------------------------------------------------
		/// @brief \c convertToMGJSON() convert the json file to mgjson file.
		///
		/// @return true if success otherwise false.
		///
		//! \attention SM_PluginException is thrown in case
		//!		- invalid json file.
		// ----------------------------------------------------------------------------------------------

		virtual bool convertToMGJSON();

		// ---------------------------------------------------------------------------------------------
		/// @brief \c This function is called to initialize the file converter.
		///	It may be an empty function if nothing needs to be initialized.
		///
		/// @return true if success otherwise false.
		///
		// ----------------------------------------------------------------------------------------------
		static bool initialize();


		// ---------------------------------------------------------------------------------------------
		/// @brief \c This function is called to terminate the file converter.
		///
		/// @return true if success otherwise false.
		///
		// ----------------------------------------------------------------------------------------------
		static bool terminate();

		// ---------------------------------------------------------------------------------------------
		/// @brief \c The following function need to be implemented to check the format.
		///
		/// @return true if success otherwise false.
		///
		// ----------------------------------------------------------------------------------------------
		static bool checkFileFormat(const SM_NAMESPACE::SM_UTF8String& inFilePath);
        
    private:
		
		/*Private functions*/
        bool AppendItems(spIMgjsonGroup root, spJsonIterator& iter);
        bool AddDoubleData(spIMgjsonGroup group, const spJsonIterator& iter);
        bool AddStringData(spIMgjsonGroup group, const spJsonIterator& iter);
        bool AddBooleanData(spIMgjsonGroup group, const spJsonIterator& iter);
        bool AddObjectData(spIMgjsonGroup group, const spJsonIterator& iter);
        bool AddArrayData(spIMgjsonGroup group, const spJsonIterator& iter);
        
        SM_NAMESPACE::SM_AsciiString GetNewMatchName();
        SM_NAMESPACE::SM_Uns32 matchNameCount;
        static bool isTopLevelArray;
	};
}

#endif //__JSONCONVERTER_H__

