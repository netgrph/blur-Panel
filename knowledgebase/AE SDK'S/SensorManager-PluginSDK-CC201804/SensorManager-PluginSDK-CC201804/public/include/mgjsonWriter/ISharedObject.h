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

#ifndef __ISHARED_OBJECT_H__
#define __ISHARED_OBJECT_H__ 

/**************************************************************************
* @file ISharedObject.h
* @brief It provides basic functionality of the shared pointers.
*		This allows all interfaces to be used as shared pointers so as to reduce 
*		the burden of scope management from the client or library code.
***************************************************************************/

#include "SM_Const.h"
#include "SM_FwdDeclarations.h"
 
namespace SM_PLUGIN {
	
	class ISharedObject {
	public:
		// called by the clients of the object to indicate that he has acquired the shared ownership of the object.
		virtual void APICALL Acquire() const NOTHROW = 0;

		// called by the clients of the object to indicate he has released his shared ownership of the object.
		virtual void APICALL Release() const NOTHROW = 0;

		// return the pointer to the internal Shared Object interface
		virtual pISharedObject_I APICALL GetISharedObject_I() NOTHROW = 0;

		pcISharedObject_I GetISharedObject_I() const NOTHROW {
			return const_cast< ISharedObject * >(this)->GetISharedObject_I();

		}

	protected:

		// protected virtual destructor.
		virtual ~ISharedObject() NOTHROW = 0;

	};

	inline ISharedObject::~ISharedObject() NOTHROW { }

}

#endif  // __ISHARED_OBJECT_H__
