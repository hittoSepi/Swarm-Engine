#pragma once

// Define DLL export/import
#ifdef _MSC_VER
#define SWARMEXPORT __declspec(dllexport)
#define SWARMIMPORT __declspec(dllimport)
#elif defined(__GNUC__) // _MSC_VER
#define SWARMEXPORT __attribute__ ((visibility ("default")))
#define SWARMIMPORT extern
#endif // _MSC_VER

#ifdef SWARM_DLL
#define dlldecl  SWARMEXPORT
#else   // BUILDING_SHARED_DLL
#define dlldecl  SWARMIMPORT
#endif // BUILDING_SHARED_DLL
