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

#ifndef __SM_PLUGINFWDDECLARATIONS_H__
#define __SM_PLUGINFWDDECLARATIONS_H__

#include "SM_Const.h"
#include "SM_PluginTypedefs.h"
#include <vector>
#include "SM_LatestWriterInterfaceVersions.h"

namespace SM_PLUGIN {

	class IMgjsonRoot_v1;
	typedef IMgjsonRoot_v1																IMgjsonRoot_base;
	typedef LATEST_CLASS( IMgjsonRoot, IMGJSONROOT_VERSION )							IMgjsonRoot;
	typedef shared_ptr<IMgjsonRoot>														spIMgjsonRoot;
	typedef IMgjsonRoot *																pIMgjsonRoot;
	typedef const IMgjsonRoot *															pcIMgjsonRoot;
	
	
	class IMgjsonGroup_v1;
	typedef IMgjsonGroup_v1																IMgjsonGroup_base;
	typedef LATEST_CLASS( IMgjsonGroup, IMGJSONGROUP_VERSION )							IMgjsonGroup;
	typedef shared_ptr<IMgjsonGroup>													spIMgjsonGroup;
	typedef IMgjsonGroup*																pIMgjsonGroup;
	typedef const IMgjsonGroup*															pcIMgjsonGroup;
	typedef shared_ptr<IMgjsonGroup_base>											    spIMgjsonGroup_base;
	typedef IMgjsonGroup_base*													        pIMgjsonGroup_base;
    typedef std::vector<spIMgjsonGroup>													GroupList;
	
	class IMgjsonStaticData_v1;
	typedef IMgjsonStaticData_v1														IMgjsonStaticData_base;
	typedef LATEST_CLASS( IMgjsonStaticData, IMGJSONSTATICDATA_VERSION )				IMgjsonStaticData;
	typedef shared_ptr<IMgjsonStaticData>												spIMgjsonStaticData;
	typedef IMgjsonStaticData *															pIMgjsonStaticData;
	typedef const IMgjsonStaticData *													pcIMgjsonStaticData;
	typedef shared_ptr<IMgjsonStaticData_base>											spIMgjsonStaticData_base;
	typedef IMgjsonStaticData_base*													    pIMgjsonStaticData_base;
    typedef std::vector<spIMgjsonStaticData>											StaticDataList;

	class IMgjsonStaticDataNum_v1;
	typedef IMgjsonStaticDataNum_v1														IMgjsonStaticDataNum_base;
	typedef LATEST_CLASS( IMgjsonStaticDataNum, IMGJSONSTATICDATANUM_VERSION )			IMgjsonStaticDataNum;
	typedef shared_ptr<IMgjsonStaticDataNum>											spIMgjsonStaticDataNum;
	typedef IMgjsonStaticDataNum *														pIMgjsonStaticDataNum;
	typedef const IMgjsonStaticDataNum *												pcIMgjsonStaticDataNum;
	typedef std::vector<spIMgjsonStaticDataNum>											StaticDataNumList;

	class IMgjsonStaticDataNumArr_v1;
	typedef IMgjsonStaticDataNumArr_v1													IMgjsonStaticDataNumArr_base;
	typedef LATEST_CLASS( IMgjsonStaticDataNumArr, IMGJSONSTATICDATANUMARR_VERSION )	IMgjsonStaticDataNumArr;
	typedef shared_ptr<IMgjsonStaticDataNumArr>											spIMgjsonStaticDataNumArr;
	typedef IMgjsonStaticDataNumArr *													pIMgjsonStaticDataNumArr;
	typedef const IMgjsonStaticDataNumArr *												pcIMgjsonStaticDataNumArr;
	typedef std::vector<IMgjsonStaticDataNumArr>										StaticDataNumArrList;

	class IMgjsonStaticDataString_v1;
	typedef IMgjsonStaticDataString_v1													IMgjsonStaticDataString_base;
	typedef LATEST_CLASS( IMgjsonStaticDataString, IMGJSONSTATICDATASTRING_VERSION )	IMgjsonStaticDataString;
	typedef shared_ptr<IMgjsonStaticDataString>											spIMgjsonStaticDataString;
	typedef IMgjsonStaticDataString *													pIMgjsonStaticDataString;
	typedef const IMgjsonStaticDataString *												pcIMgjsonStaticDataString;
	typedef std::vector<IMgjsonStaticDataString>										StaticDataStringList;

	class IMgjsonStaticDataBool_v1;
	typedef IMgjsonStaticDataBool_v1													IMgjsonStatic_base;
	typedef LATEST_CLASS( IMgjsonStaticDataBool, IMGJSONSTATICDATABOOL_VERSION )		IMgjsonStaticDataBool;
	typedef shared_ptr<IMgjsonStaticDataBool>											spIMgjsonStaticDataBool;
	typedef IMgjsonStaticDataBool *														pIMgjsonStaticDataBool;
	typedef const IMgjsonStaticDataBool *												pcIMgjsonStaticDataBool;
	typedef std::vector<IMgjsonStaticDataBool>											StaticDataBoolList;

	class IMgjsonDynamicData_v1;
	typedef IMgjsonDynamicData_v1														IMgjsonDynamicData_base;
	typedef LATEST_CLASS( IMgjsonDynamicData, IMGJSONDYNAMICDATA_VERSION )				IMgjsonDynamicData;
	typedef shared_ptr<IMgjsonDynamicData>												spIMgjsonDynamicData;
	typedef IMgjsonDynamicData*															pIMgjsonDynamicData;
	typedef const IMgjsonDynamicData*													pcIMgjsonDynamicData;
	typedef shared_ptr<IMgjsonDynamicData_base>											spIMgjsonDynamicData_base;
	typedef IMgjsonDynamicData_base*													pIMgjsonDynamicData_base;
	typedef std::vector<spIMgjsonDynamicData>											DynamicDataList;

	class IMgjsonDynamicDataNum_v1;
	typedef IMgjsonDynamicDataNum_v1													IMgjsonDynamicDataNum_base;
	typedef LATEST_CLASS( IMgjsonDynamicDataNum, IMGJSONDYNAMICDATANUM_VERSION )		IMgjsonDynamicDataNum;
	typedef shared_ptr<IMgjsonDynamicDataNum>											spIMgjsonDynamicDataNum;
	typedef IMgjsonDynamicDataNum*														pIMgjsonDynamicDataNum;
	
	class IMgjsonDynamicDataString_v1;
	typedef IMgjsonDynamicDataString_v1													IMgjsonDynamicDataString_base;
	typedef LATEST_CLASS( IMgjsonDynamicDataString, IMGJSONDYNAMICDATASTRING_VERSION )	IMgjsonDynamicDataString;
	typedef shared_ptr<IMgjsonDynamicDataString>										spIMgjsonDynamicDataString;
	typedef IMgjsonDynamicDataString*													pIMgjsonDynamicDataString;

	class IMgjsonDynamicDataNumArr_v1;
	typedef IMgjsonDynamicDataNumArr_v1													IMgjsonDynamicDataNumArr_base;
	typedef LATEST_CLASS( IMgjsonDynamicDataNumArr, IMGJSONDYNAMICDATANUMARR_VERSION )	IMgjsonDynamicDataNumArr;
	typedef shared_ptr<IMgjsonDynamicDataNumArr>										spIMgjsonDynamicDataNumArr;
	typedef IMgjsonDynamicDataNumArr*													pIMgjsonDynamicDataNumArr;


	class ISharedObject_I;
	typedef ISharedObject_I *															pISharedObject_I;
	typedef const ISharedObject_I *														pcISharedObject_I;
    
}
#endif // __SM_PLUGINFWDDECLARATIONS_H__
