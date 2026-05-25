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

#include "SM_DataTypesCommon.h"

bool SM_NAMESPACE::SM_SyncTimeValue::GetFrameNumber(SM_NAMESPACE::SM_Int32& outVal) const{
	
	if (mTime.which() == SM_NAMESPACE::kSM_SyncTime_FRAME) {
		outVal = boost::get<SM_NAMESPACE::SM_Int32>(mTime);
		return true;
	}
	return false;
}

bool SM_NAMESPACE::SM_SyncTimeValue::GetTimeString(std::string& outVal) const {
	
	if (mTime.which() == SM_NAMESPACE::kSM_SyncTime_STRING) {
		outVal = boost::get<std::string>(mTime);
		return true;
	}
	return false;
}
