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

#ifndef __IMgjsonStaticData_H__
#define __IMgjsonStaticData_H__

/**************************************************************************
* @file IMgjsonStaticData.h
* @brief This is a abstract class from which all the classes for specific type 
*		  of samples (NumberArray, Number, Bool and String) would be derived.
* Each instance of a derived class would correspond to a static data value.
* AddStaticData of Group/Root would collect an object of the derived types.
***************************************************************************/

#include "SM_PluginTypes.h"
#include "SM_FwdDeclarations.h"
#include "ISharedObject.h"
#include "SM_HostAPIAccess.h"

namespace SM_PLUGIN {
	
	class IMgjsonStaticData_v1
	{
	public:

		//!
		//! @brief  Returns the type of StaticData node.                             
		//! @return An object of type \#eDataValueType.
		//!
		virtual SM_NAMESPACE::eDataValueType APICALL GetType() const NOTHROW = 0;

		//!
		//! @brief Returns the actual raw pointer from the shared pointer.
		//! @return the actual raw pointer from the shared pointer.
		//!
		virtual pIMgjsonStaticData APICALL GetActualIMgjsonStaticData ( ) NOTHROW = 0;
		pcIMgjsonStaticData GetActualIMgjsonStaticData ( ) const NOTHROW {
			return const_cast< IMgjsonStaticData * >(this)->GetActualIMgjsonStaticData ( );
		}
		
		static SM_NAMESPACE::SM_Uns32 GetInterfaceVersion() { return 1; }

	protected:

		// protected virtual destructor.
		virtual ~IMgjsonStaticData_v1 ( ) NOTHROW {}
       
#ifdef REQUIRED_FRIEND_CLASS
	MAKE_CLASS_FRIEND
#endif
	};
}

#endif /* __IMgjsonStaticData_H__ */
