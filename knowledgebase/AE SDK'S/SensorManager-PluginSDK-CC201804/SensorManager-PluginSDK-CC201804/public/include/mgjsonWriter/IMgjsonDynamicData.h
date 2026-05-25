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

#ifndef __IMgjsonDynamicData_H__
#define __IMgjsonDynamicData_H__

/**************************************************************************
* @file IMgjsonDynamicData.h
* @brief This is a abstract class from which all the classes for specific type 
*		  of samples (NumberArray, Number and String) would be derived.
* Each of the derived class would contain the dynamic info data and 
* the sample values along with the timestamp. Each instance of a derived class 
* would correspond to a dynamic sample set
* AddDynamicData of Group/Root would collect an object of the derived types.
***************************************************************************/

#include "SM_DataTypesCommon.h"
#include "SM_FwdDeclarations.h"
#include "SM_HostAPIAccess.h"

namespace SM_PLUGIN {

	class IMgjsonDynamicData_v1 {
	public:
		
		//!
		//! @brief  Returns the type of DynamicData node.                             
		//! @return An object of type \#eDataValueType.
		//!
		virtual SM_NAMESPACE::eDataValueType APICALL GetType() const NOTHROW = 0;


		//!
		//! @brief Serializes all the added samples to the MGJSON file.  
		//! This must be the last and mandatory call on this DynamicData node. Any changes done to the node after this API call, would not take any effect.
		//! This should be called only once for one DynamicData node.
		//!                           
		//! @return    A bool value; true in case the commit is successful. False in case the node was already committed.
		//! 
		//! \note      -In case SM_PluginException is thrown, DynamicData node is marked as dirty and is invalidated. No operation should be performed on a dirty node, neither should it
		//!             be added in MGJSONRoot or MGJSONGroup. 
		//!             In case a dirty node is already added, then it would not be serialized on the Commit() call of IMGJSONRoot.
		//!            -In case SM_PluginException is thrown, then all the samples of this node are removed from the file. 
		//!            -In case SM_PluginException with ErrorCode SM_NAMESPACE::kSMWriter_MGJSONFileInvalidated is thrown, then the MGJSON file and the dynamic sample sets serialized till now would be cleared or invalidated.
		//!             The root is also set to an empty root and only the properties passed in \c CreateRoot() are preserved.
		//!
		//! \attention SM_PluginException is thrown in case
		//!            - A node marked as dirty is committed. 
		//!		       - There is a failure in serialization of samples.
		//!
		virtual bool APICALL Commit()  = 0;


		//!
		//! @brief  Would override the value of mSampleCount passed at the time of creation of node. In case 0 is passed, the calculated count of valid samples is written.   
		//! @param  inSampleCount   number of samples in this sample set.
		virtual void APICALL SetSampleCount(SM_NAMESPACE::SM_Uns64 inSampleCount) NOTHROW = 0;


		//!
		//! @brief  Would return the calculated count of valid samples successfully added in \c AddSample().
		//! @return Count of valid samples 
		//!
		virtual SM_NAMESPACE::SM_Uns64 APICALL GetSampleCount() const NOTHROW = 0;

		//!
		//! @brief Returns the actual raw pointer from the shared pointer.
		//! @return the actual raw pointer from the shared pointer.
		//!
		virtual pIMgjsonDynamicData APICALL GetActualIMgjsonDynamicData() NOTHROW = 0;
		pcIMgjsonDynamicData GetActualIMgjsonDynamicData() const NOTHROW {
			return const_cast< IMgjsonDynamicData * >(this)->GetActualIMgjsonDynamicData();
		}
		
		static SM_NAMESPACE::SM_Uns32	GetInterfaceVersion(){ return 1; }


	protected:

		// protected virtual destructor.
		virtual ~IMgjsonDynamicData_v1() NOTHROW {}
		
		virtual  SM_NAMESPACE::SM_Bool APICALL commit(void* errResultP) NOTHROW = 0;
		virtual void APICALL setDynamicDataInfo(SM_NAMESPACE::SM_StringPtr displayName, SM_NAMESPACE::SM_StringPtr matchName, SM_NAMESPACE::SM_StringPtr sampleSetID, SM_NAMESPACE::SM_Uns64 sampleCount, SM_NAMESPACE::SM_Uns32 interpolationType, SM_NAMESPACE::SM_Bool hasExpectedSampleFrequencyB, SM_NAMESPACE::SM_StringPtr expectedMaxInterSampleDuration, void* errResultP) NOTHROW = 0;
	};
	
}
#endif /*__IMgjsonDynamicData_H__*/
