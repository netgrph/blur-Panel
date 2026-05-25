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

#ifndef __PLUGINBASE_H__
#define __PLUGINBASE_H__

/**************************************************************************
* @file SM_PluginBase.h
* @brief Basic functionality of the plugin.
*
* This is the basic minimum functionalty which a  plugin should provide.
* Base class for the converter. All converter should be derived from SM_PluginBase,
* which is an abstract class. Plugin developer need to implement all of the virtual functions.

virtual bool convertToMGJSON( );

* Plugin developer also need to implement following static functions.

static bool initialize();
static bool terminate();
static bool checkFileFormat( const std::string& inFilePath );

/***************************************************************************/

#include "SM_Const.h"
#include "SM_HostAPIAccess.h"
#include "SM_PluginRegistry.h"

namespace SM_PLUGIN
{

	class SM_PluginBase
	{
	public:

		SM_PluginBase(const SM_NAMESPACE::SM_FileExtension& inExt, const SM_NAMESPACE::SM_UTF8String& inFilePath, const SM_NAMESPACE::SM_UTF8String& inMGJSONPath)
		:mExtension(inExt), mFilePath(inFilePath), mMGJSONPath(inMGJSONPath){}

		virtual ~SM_PluginBase() {}

		//!
		//! @brief Converts the native file to MGJSON file.  
		//! This must be implemented for conversion. This API would be called by the host.  
		//! The conversion can be done using the WriterAPI interfaces. 
		//!                           
		//! @return    A bool value; true in case the conversion is successful, false otherwise.
		//! 
		//! \attention Can throw SM_PluginException
		//!            
		virtual bool convertToMGJSON() = 0;

		//!
		//! @brief     Get the file extension of the native file to be converted.                             
		//! @return    native file extension
		//!            
		inline const SM_NAMESPACE::SM_FileExtension& getExtension() const { return mExtension; }


		//!
		//! @brief     Get the file path of the native file to be converted.                             
		//! @return    Path string
		//! 
		inline const SM_NAMESPACE::SM_UTF8String& getFilePath() const { return mFilePath; }

		
		//!
		//! @brief     Get the file path of the converted MGJSON file.                             
		//! @return    Path string
		//!
		inline const SM_NAMESPACE::SM_UTF8String& getMGJSONPath() const { return mMGJSONPath; }

	private:
		const SM_NAMESPACE::SM_UTF8String mFilePath;
		const SM_NAMESPACE::SM_UTF8String mMGJSONPath;
		SM_NAMESPACE::SM_FileExtension mExtension;

	};


	//!
	//! External functions which need to be implemented by Plugin Developer.
	//! This function are unique for a plugin. Unlike SM_PluginBase class which is used
	//! for each converter, these function are written only once for each plug-in.
	//!

	//!
	//! @brief   Returns the unique identifier string of the plug-in.
	//! @return  the uid string of the plugin.
	//!
	const char* GetPluginIdentifier();

	//!
	//! @brief Register the converters available in the plug-in.
	//!
	//!  This function need to be implemented by plugin-developer. This function registers the converters available in the plugin.
	//!  Plug-in developer may want to register only few of the available converters as per requirement.
	//!  Only registered converters will be loaded.
	//!
	void RegisterConverters();

} //namespace SM_PLUGIN
#endif // __PLUGINBASE_H__
