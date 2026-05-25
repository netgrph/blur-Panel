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

namespace SM_PLUGIN {
	class IMgjsonStaticDataStringProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class SM_PLUGIN::IMgjsonStaticDataStringProxy;

#include "IMgjsonStaticDataString.h"

namespace SM_PLUGIN {

	class IMgjsonStaticDataStringProxy
		: public virtual IMgjsonStaticDataString
	{
	private:
		pIMgjsonStaticDataString mRawPtr;

	public:
		IMgjsonStaticDataStringProxy(pIMgjsonStaticDataString ptr)
			: mRawPtr(ptr) {

			mRawPtr->Acquire();
		}

		~IMgjsonStaticDataStringProxy() NOTHROW {

			mRawPtr->Release();

		}

		void  Acquire() const NOTHROW {
			assert(false);

		}

		void  Release() const NOTHROW {
			assert(false);

		}

		pISharedObject_I APICALL GetISharedObject_I() NOTHROW {
			return mRawPtr->GetISharedObject_I();
		}

		SM_NAMESPACE::eDataValueType APICALL GetType() const NOTHROW
		{

			SM_NAMESPACE::SM_Uns32 type = mRawPtr->GetType();
			return static_cast<SM_NAMESPACE::eDataValueType>( type );

		}


		pIMgjsonStaticData APICALL GetActualIMgjsonStaticData() NOTHROW { return mRawPtr; }

		virtual void APICALL setStringProperties(SM_NAMESPACE::SM_Uns64 inMaxLen, SM_NAMESPACE::SM_Uns64 inMaxLenValDigit,
												 SM_NAMESPACE::SM_Bool inEventMarkerB, void* errResultP) NOTHROW
		{
			mRawPtr->setStringProperties(inMaxLen, inMaxLenValDigit, inEventMarkerB, errResultP);
		}
	};

	spIMgjsonStaticDataString IMgjsonStaticDataString_v1::CreateStaticDataString(ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName,
																			const SM_NAMESPACE::SM_UTF8String& inDisplayName,
																			const SM_NAMESPACE::SM_UTF8String& inValue,
																			const SM_NAMESPACE::SM_StringProperties& inStringProp) {
		if (inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		pIMgjsonStaticDataString ptr;
		HostCreateStaticDataString(inConverter, &ptr, inMatchName.c_str(), inDisplayName.c_str(), inValue.c_str());
		if(!ptr)
			return spIMgjsonStaticDataString();

		spIMgjsonStaticDataString spProxy = spIMgjsonStaticDataString();
		TRY_BLOCK
			spProxy = shared_ptr< IMgjsonStaticDataString >(new IMgjsonStaticDataStringProxy(ptr));
		CATCH_RELEASE_THROW

		SM_PluginResult errorPaddedStr;
		spProxy->setStringProperties(inStringProp.mMaxLen, inStringProp.mMaxLenValDigit, inStringProp.mEventMarkerB, &errorPaddedStr);
		PROPOGATE_PLUGIN_EXCEPTION(errorPaddedStr);

		return spProxy;

	}
}
