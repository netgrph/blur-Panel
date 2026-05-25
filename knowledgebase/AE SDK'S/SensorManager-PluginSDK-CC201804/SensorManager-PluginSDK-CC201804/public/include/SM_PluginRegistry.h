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

#ifndef __SM_PLUGINREGISTRY_H__
#define __SM_PLUGINREGISTRY_H__
#include "SM_PluginHandler.h"
#include "SM_HostAPIAccess.h"
#include <map>
#include <string>

namespace SM_PLUGIN
{

	class SM_PluginCreatorBase;
	class SM_PluginBase;

	class SM_PluginRegistry
	{
	public:
		static void registerConverter ( const SM_PluginCreatorBase* inCreator );
		static bool initialize ( );
		static bool terminate ( );

		static SM_PluginBase* create ( const std::string& uid , const SM_NAMESPACE::SM_FileExtension ext, const SM_NAMESPACE::SM_UTF8String& filePath , const SM_NAMESPACE::SM_UTF8String& mgJSONPath );
		static bool checkFileFormat ( const std::string& uid , const SM_NAMESPACE::SM_UTF8String& filePath/* const IOAdapter& file*/ );

	private:
		struct StringCompare : std::binary_function< const std::string & , const std::string & , bool >
		{
			bool operator()( const std::string & a , const std::string & b ) const
			{
				return (a.compare ( b ) < 0);
			}
		};
		typedef std::map<std::string , const SM_PluginCreatorBase* , StringCompare>		RegistryEntryMap;

		SM_PluginRegistry ( ) {}
		~SM_PluginRegistry ( );

		RegistryEntryMap mRegistryEntries;
		static SM_PluginRegistry* msRegistry;
	};


	class SM_PluginCreatorBase
	{
	public:
		SM_PluginCreatorBase ( ) {}
		virtual ~SM_PluginCreatorBase ( ) {}

		virtual SM_PluginBase* create (const SM_NAMESPACE::SM_FileExtension ext , const SM_NAMESPACE::SM_UTF8String& filePath , const SM_NAMESPACE::SM_UTF8String& mgJSONPath ) const = 0;
		virtual bool checkFileFormat ( const SM_NAMESPACE::SM_UTF8String& filePath/*, const IOAdapter& file */ ) const = 0;

		virtual const std::string & GetUID ( ) const = 0;
		virtual bool initialize ( ) const = 0;
		virtual bool terminate ( ) const = 0;
	};

	template<typename TConverter>
	class SM_PluginCreator : public SM_PluginCreatorBase
	{
	public:

		SM_PluginCreator ( const char* inUID )
			: mUID ( inUID )
		{}

		SM_PluginCreator ( const char* inUID , SM_NAMESPACE::SM_FileExtension ext)
			: mUID ( inUID )
		{}

		inline SM_PluginBase* create ( const SM_NAMESPACE::SM_FileExtension ext , const SM_NAMESPACE::SM_UTF8String& filePath , const SM_NAMESPACE::SM_UTF8String& mgJSONPath ) const
		{
			TConverter* instance = new TConverter ( ext, filePath, mgJSONPath );

			SM_PluginBase* converter = dynamic_cast<SM_PluginBase*>(instance);

			return converter;
		}

		inline bool checkFileFormat ( const SM_NAMESPACE::SM_UTF8String& filePath/*, const IOAdapter& file*/ ) const
		{
			return TConverter::checkFileFormat ( filePath /*, file*/ );
		}


		inline const std::string & GetUID ( ) const
		{
			return mUID;
		}

		inline bool initialize ( ) const
		{
			return TConverter::initialize ( );
		}

		inline bool terminate ( ) const
		{
			return TConverter::terminate ( );
		}

	private:
		std::string			mUID;
	};

} //namespace SM_PLUGIN
#endif // __SM_PLUGINREGISTRY_H__
