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
	class IMgjsonStaticDataBoolProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class SM_PLUGIN::IMgjsonStaticDataBoolProxy;

#include "IMgjsonStaticDataBool.h"

namespace SM_PLUGIN {

	class IMgjsonStaticDataBoolProxy
		: public virtual IMgjsonStaticDataBool
	{
	private:
		pIMgjsonStaticDataBool mRawPtr;

	public:
		IMgjsonStaticDataBoolProxy(pIMgjsonStaticDataBool ptr)
			: mRawPtr(ptr) {

			mRawPtr->Acquire();
		}

		~IMgjsonStaticDataBoolProxy() NOTHROW {

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

	};

	spIMgjsonStaticDataBool IMgjsonStaticDataBool_v1::CreateStaticDataBool(ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName,
																		const SM_NAMESPACE::SM_UTF8String& inDisplayName,
																		const bool& inValue) {
		if (inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		pIMgjsonStaticDataBool ptr;
		HostCreateStaticDataBool(inConverter, &ptr, inMatchName.c_str(), inDisplayName.c_str(), static_cast<SM_NAMESPACE::SM_Bool>(inValue));
		if(!ptr)
			return spIMgjsonStaticDataBool();
		
		spIMgjsonStaticDataBool spProxy = spIMgjsonStaticDataBool();
		TRY_BLOCK
			spProxy = shared_ptr< IMgjsonStaticDataBool > ( new IMgjsonStaticDataBoolProxy ( ptr ) );
		CATCH_RELEASE_THROW

		return spProxy;
	}
}
