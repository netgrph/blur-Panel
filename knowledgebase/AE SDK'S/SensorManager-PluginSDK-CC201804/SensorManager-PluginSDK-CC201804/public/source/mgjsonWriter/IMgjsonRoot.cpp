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
	class IMgjsonRootProxy;
}

#define REQUIRED_FRIEND_CLASS
#define MAKE_CLASS_FRIEND friend class SM_PLUGIN::IMgjsonRootProxy;

#include "IMgjsonRoot.h"

namespace SM_PLUGIN {

#if SM_WinBuild
	#pragma warning( push )
	#pragma warning( disable : 4250 )
#endif
	class IMgjsonRootProxy
		: public virtual IMgjsonRoot,
          public virtual IMgjsonGroupProxy
	{
	private:
		pIMgjsonRoot mRawPtr;

	public:
		IMgjsonRootProxy ( pIMgjsonRoot ptr )
			: mRawPtr ( ptr ),
              IMgjsonGroupProxy( ptr )
		{
			mRawPtr->Acquire();
		}

		~IMgjsonRootProxy ( ) NOTHROW 
		{
			mRawPtr->Release();
		}

		pIMgjsonRoot APICALL GetActualIMgjsonRoot ( ) NOTHROW { return mRawPtr; }

        void  Acquire ( ) const NOTHROW {
			assert ( false );

		}
		
		void  Release ( ) const NOTHROW {
			assert ( false );

		}

		 pISharedObject_I APICALL GetISharedObject_I ( ) NOTHROW {
			 return mRawPtr->GetISharedObject_I ( );

		}

        SM_NAMESPACE::SM_Bool APICALL commit ( void* errResultP ) NOTHROW {
            assert ( false );
            return mRawPtr->commit(errResultP);
            
        }
        void APICALL setCreator ( SM_NAMESPACE::SM_StringPtr val, void * errResultP ) NOTHROW {
            assert ( false );
            return mRawPtr->setCreator(val, errResultP);   
        }

		virtual void APICALL writeVersionToFile(void * errResultP) NOTHROW 
		{
			mRawPtr->writeVersionToFile(errResultP);
		}

        void APICALL setSMPTETimeInfo(SM_NAMESPACE::SM_Bool dropFrame,SM_NAMESPACE::SM_Uns32 frameRateValue,
                              SM_NAMESPACE::SM_Uns32 frameRateScale, void * errResultP) NOTHROW {
            mRawPtr->setSMPTETimeInfo(dropFrame, frameRateValue, frameRateScale, errResultP);

        }
        void APICALL setUTCTimeInfo(SM_NAMESPACE::SM_Uns32 precisionLength, SM_NAMESPACE::SM_Bool isGMT, void * errResultP) NOTHROW {
            mRawPtr->setUTCTimeInfo(precisionLength, isGMT, errResultP);

        }
        void APICALL addSyncTimeFrameNum(SM_NAMESPACE::SM_StringPtr fileName, SM_NAMESPACE::SM_Int32 frameNumber, void * errResultP) NOTHROW {

            mRawPtr->addSyncTimeFrameNum(fileName, frameNumber, errResultP);

        }
        void APICALL addSyncTimeStr(SM_NAMESPACE::SM_StringPtr fileName, SM_NAMESPACE::SM_StringPtr timeString, SM_NAMESPACE::SM_StringLen timeStringLen, void * errResultP) NOTHROW {
            mRawPtr->addSyncTimeStr(fileName, timeString, timeStringLen, errResultP);
        }
        
        bool APICALL Commit()  {
			SM_PluginResult error;
            SM_NAMESPACE::SM_Bool committed = mRawPtr->commit(&error);
			PROPOGATE_PLUGIN_EXCEPTION(error)
			return ConvertSM_BoolToBool(committed);
      }
        
        void APICALL SetCreator ( const SM_NAMESPACE::SM_UTF8String& inVal ) {
			SM_PluginResult error;
            mRawPtr->setCreator(inVal.c_str(), &error);
			PROPOGATE_PLUGIN_EXCEPTION(error)
        }

      
    };

    spIMgjsonRoot IMgjsonRoot_v1::CreateRoot (ConverterRef inConverter, SM_NAMESPACE::SM_UTF8FilePath inPath, const SM_NAMESPACE::SM_DynamicFileInfo* inDynamicFileInfo /*= NULL*/ ) {
      
		if(inConverter == NULL)
			THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_ParametersNotAsExpected)
		pIMgjsonRoot ptr;

		SM_NAMESPACE::SM_Bool alreadyCreated;
		
			HostCreateRoot(inConverter, inPath, &ptr, &alreadyCreated);
			if (!ptr)
				return spIMgjsonRoot();

			spIMgjsonRoot spProxy = spIMgjsonRoot();

			try {
				TRY_BLOCK
					spProxy = shared_ptr< IMgjsonRoot >(new IMgjsonRootProxy(ptr));
				CATCH_RELEASE_THROW

				// add inDynamicFileInfo values
				if (alreadyCreated == 0)
				{
					if (inDynamicFileInfo)
					{
						if (inDynamicFileInfo->mTimeInfo.which() == SM_NAMESPACE::kSM_TimeCodeInfo) {

							SM_PluginResult error;
							SM_NAMESPACE::SM_TimeCodeInfo timeInfo = boost::get<SM_NAMESPACE::SM_TimeCodeInfo>(inDynamicFileInfo->mTimeInfo);
							spProxy->setSMPTETimeInfo(timeInfo.mIsDrop, timeInfo.mFrameRate.mValue, timeInfo.mFrameRate.mScale, &error);
							PROPOGATE_PLUGIN_EXCEPTION(error);
						}
						else if (inDynamicFileInfo->mTimeInfo.which() == SM_NAMESPACE::kSM_UTCInfo) {

							SM_PluginResult error;
							SM_NAMESPACE::SM_UTCInfo timeInfo = boost::get<SM_NAMESPACE::SM_UTCInfo>(inDynamicFileInfo->mTimeInfo);
							spProxy->setUTCTimeInfo(timeInfo.mUTCPrecision, timeInfo.mIsGMT, &error);
							PROPOGATE_PLUGIN_EXCEPTION(error);

						}
						else {
							THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidTimeType)
						}   //error

						for (SM_NAMESPACE::SM_ConstVideoSyncTimeIterator it = inDynamicFileInfo->mVideoSyncTimeList.begin();
						it != inDynamicFileInfo->mVideoSyncTimeList.end(); it++) {

							SM_PluginResult error;

							if (it->mType == SM_NAMESPACE::kSM_SyncTime_STRING && it->mSyncTime.mTime.which() == SM_NAMESPACE::kSM_SyncTime_STRING) {
								SM_NAMESPACE::SM_UTF8String str;
								it->mSyncTime.GetTimeString(str);
								spProxy->addSyncTimeStr(it->mFileName.c_str(), str.c_str(), str.size(), &error);
								PROPOGATE_PLUGIN_EXCEPTION(error);

							}
							else if (it->mType == SM_NAMESPACE::kSM_SyncTime_FRAME && it->mSyncTime.mTime.which() == SM_NAMESPACE::kSM_SyncTime_FRAME) {

								SM_NAMESPACE::SM_Int32 frameNumber;
								it->mSyncTime.GetFrameNumber(frameNumber);
								spProxy->addSyncTimeFrameNum(it->mFileName.c_str(), frameNumber, &error);
								PROPOGATE_PLUGIN_EXCEPTION(error);

							}
							else {
								THROW_PLUGIN_EXCEPTION(SM_NAMESPACE::kSMWriter_InvalidSyncTimeType)
							} //error
						}
					}

					SM_PluginResult errorVersion;
					spProxy->writeVersionToFile(&errorVersion);
					PROPOGATE_PLUGIN_EXCEPTION(errorVersion);
				}
				return spProxy;
		}
		catch (...)
		{
			if (alreadyCreated == 0)
				HostRemoveConverterFromMap(inConverter);
			throw;
		}
    }

#if SM_WinBuild
	#pragma warning( pop )
#endif
}
