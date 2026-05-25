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
	class IMgjsonStaticDataNumProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class SM_PLUGIN::IMgjsonStaticDataNumProxy;

#include "IMgjsonStaticDataNum.h"

namespace SM_PLUGIN {

	class IMgjsonStaticDataNumProxy
		: public virtual IMgjsonStaticDataNum
	{
	private:
		pIMgjsonStaticDataNum mRawPtr;

	public:
		IMgjsonStaticDataNumProxy( pIMgjsonStaticDataNum ptr )
			: mRawPtr ( ptr ) {
			
			mRawPtr->Acquire();
		}

		~IMgjsonStaticDataNumProxy( ) NOTHROW {
		
			mRawPtr->Release();

		}

		void  Acquire ( ) const NOTHROW {
			assert ( false );

		}

		void  Release ( ) const NOTHROW {
			assert ( false );

		}
        
		pISharedObject_I APICALL GetISharedObject_I() NOTHROW {
			return mRawPtr->GetISharedObject_I();
		}

		void APICALL setNumberPattern(SM_NAMESPACE::SM_Uns64 digitsInteger, SM_NAMESPACE::SM_Uns64 digitsDecimal, SM_NAMESPACE::SM_Bool isSigned, void* errResultP) NOTHROW
		{
			mRawPtr->setNumberPattern(digitsInteger, digitsDecimal, isSigned, errResultP);
		}

		void APICALL setNumberRange(double minLegal, double maxLegal, double minOccuring, double maxOccuring, void* errResultP) NOTHROW
		{
			mRawPtr->setNumberRange(minLegal, maxLegal, minOccuring, maxOccuring, errResultP);
		}

		SM_NAMESPACE::eDataValueType APICALL GetType() const NOTHROW
		{
			
			SM_NAMESPACE::SM_Uns32 type = mRawPtr->GetType();
			return static_cast<SM_NAMESPACE::eDataValueType>( type );

		}

		pIMgjsonStaticData APICALL GetActualIMgjsonStaticData() NOTHROW { return mRawPtr; }

   	};

	spIMgjsonStaticDataNum IMgjsonStaticDataNum_v1::CreateStaticDataNum (ConverterRef inConverter, const SM_NAMESPACE::SM_AsciiString& inMatchName,
															 const SM_NAMESPACE::SM_UTF8String& inDisplayName,
															 const double& inValue,
															 const SM_NAMESPACE::SM_NumberProperties& inNumberProp) {
		if (inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		pIMgjsonStaticDataNum ptr;
		HostCreateStaticDataNum (inConverter, &ptr , inMatchName.c_str ( ) , inDisplayName.c_str ( ) , inValue);
		if( !ptr )
			return spIMgjsonStaticDataNum( );
        
		spIMgjsonStaticDataNum spProxy = spIMgjsonStaticDataNum();
		
		TRY_BLOCK
			spProxy = shared_ptr< IMgjsonStaticDataNum >(new IMgjsonStaticDataNumProxy(ptr));
		CATCH_RELEASE_THROW
		
		SM_PluginResult errorNumPattern;
		spProxy->setNumberPattern(inNumberProp.mPattern.mDigitsInteger, inNumberProp.mPattern.mDigitsDecimal, inNumberProp.mPattern.mIsSigned, &errorNumPattern);
		PROPOGATE_PLUGIN_EXCEPTION(errorNumPattern);


		SM_PluginResult errorNumRange;
		spProxy->setNumberRange(inNumberProp.mRange.mLegal.mMinimum, inNumberProp.mRange.mLegal.mMaximum, inNumberProp.mRange.mOccuring.mMinimum, inNumberProp.mRange.mOccuring.mMaximum, &errorNumRange);
		PROPOGATE_PLUGIN_EXCEPTION(errorNumRange);
	
		return spProxy;

	}
}

