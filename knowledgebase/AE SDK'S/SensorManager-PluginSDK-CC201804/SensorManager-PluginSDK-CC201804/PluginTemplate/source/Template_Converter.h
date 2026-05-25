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

#ifndef __TEMPLATE_CONVERTER_H__
#define __TEMPLATE_CONVERTER_H__

/**************************************************************************
* @file Template_Converter.h
* @brief A sample template for converting different file formats to MGJSON.
***************************************************************************/

#include "SM_PluginBase.h"

namespace SM_PLUGIN
{

	// Plugin template file converter class.
	
	class Template_Converter : public SM_PluginBase
	{
	public:
		Template_Converter(const SM_NAMESPACE::SM_FileExtension& inExt, 
							const SM_NAMESPACE::SM_UTF8String& inFilePath,
							const SM_NAMESPACE::SM_UTF8String& inMGJSONPath);

		virtual ~Template_Converter() {}

		// ---------------------------------------------------------------------------------------------
		/// @brief \c convertToMGJSON() convert the native file to mgjson file.
		///
		/// @return true if success otherwise false.
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

	};
}

#endif //__TEMPLATE_CONVERTER_H__

