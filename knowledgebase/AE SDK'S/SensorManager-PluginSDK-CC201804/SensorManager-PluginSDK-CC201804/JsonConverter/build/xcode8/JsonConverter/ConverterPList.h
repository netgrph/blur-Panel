#ifndef __CONVERTERPLIST_H__
#define __CONVERTERPLIST_H__

#include "./../../BuildInfo.h"
#include "./../../../include/VersionInfo.h"
#include "./../../../include/ChangeLog.h"


#ifdef NDEBUG
#define kConfig Release
#define kDebugSuffix
#define kBasicVersion CONVERTER_API_VERSION-s kConverterEngBuild
#else
#define kConfig Debug
#define kDebugSuffix 
#define kBasicVersion CONVERTER_API_VERSION-s kConverterEngBuild kDebugSuffix
#endif

#define kBuildInfo BUILDVERSION, BUILDDATE

#endif /* __CONVERTERPLIST_H__ */
