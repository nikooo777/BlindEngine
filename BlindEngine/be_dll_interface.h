// DLL info:
#define LIB_NAME      "BlindEngine v0.1a"		///< Library credits
#define LIB_VERSION   10                        ///< Library version (divide by 10)

#ifdef WIN32 	
// Export API:
// Specifies i/o linkage (VC++ spec):
#ifdef SIMPLEDYNAMICLIB_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif      	
#else // Under Linux
#define LIB_API  // Dummy declaration
#endif