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

#include "SM_PluginRegistry.h"
#include "SM_PluginBase.h"

namespace SM_PLUGIN
{

	SM_PluginRegistry* SM_PluginRegistry::msRegistry = NULL;

	SM_PluginRegistry::~SM_PluginRegistry ( )
	{
		RegistryEntryMap::iterator iter = msRegistry->mRegistryEntries.begin ( );
		RegistryEntryMap::iterator iterEnd = msRegistry->mRegistryEntries.end ( );

		for( ; iter != iterEnd; )
		{
			RegistryEntryMap::iterator temp = iter++;
			delete temp->second;
		}
	}

	void SM_PluginRegistry::registerConverter ( const SM_PluginCreatorBase* inCreator )
	{
		if( msRegistry == NULL )
			msRegistry = new SM_PluginRegistry ( );

		msRegistry->mRegistryEntries[inCreator->GetUID ( )] = inCreator;
	}

	SM_PluginBase* SM_PluginRegistry::create ( const std::string& uid , const SM_NAMESPACE::SM_FileExtension ext, const SM_NAMESPACE::SM_UTF8String& filePath , const SM_NAMESPACE::SM_UTF8String& mgJSONPath )
	{
		if( msRegistry != NULL )
		{
			RegistryEntryMap::const_iterator iter = msRegistry->mRegistryEntries.find ( uid );
			if( iter != msRegistry->mRegistryEntries.end ( ) )
				return iter->second->create ( ext, filePath, mgJSONPath );
		}

		return NULL;
	}

	bool SM_PluginRegistry::checkFileFormat ( const std::string& uid , const SM_NAMESPACE::SM_UTF8String& filePath/*, const IOAdapter& file */ )
	{
		if( msRegistry != NULL )
		{
			RegistryEntryMap::const_iterator iter = msRegistry->mRegistryEntries.find ( uid );
			if( iter != msRegistry->mRegistryEntries.end ( ) )
				return iter->second->checkFileFormat ( filePath/*, file*/ );
		}

		return false;
	}

	bool SM_PluginRegistry::initialize ( )
	{
		if( msRegistry != NULL )
		{
			RegistryEntryMap::const_iterator iter = msRegistry->mRegistryEntries.begin ( );
			RegistryEntryMap::const_iterator iterEnd = msRegistry->mRegistryEntries.end ( );

			for( ; iter != iterEnd; ++iter )
			{
				if( iter->second->initialize ( ) == false )
					return false;
			}
		}

		return true;
	}

	bool SM_PluginRegistry::terminate ( )
	{
		bool result = true;
		if( msRegistry != NULL )
		{
			RegistryEntryMap::const_iterator iter = msRegistry->mRegistryEntries.begin ( );
			RegistryEntryMap::const_iterator iterEnd = msRegistry->mRegistryEntries.end ( );
		
			for( ; iter != iterEnd; ++iter )
			{
				result =  iter->second->terminate ( ) && result;
			}

			delete msRegistry;
			msRegistry = NULL;
		}
		return result;
	}

} //namespace SM_PLUGIN
