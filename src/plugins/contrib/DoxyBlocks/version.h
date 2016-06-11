#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.07";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 8;
	static const long BUILD = 730;
	static const long REVISION = 8737;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1393;
	#define RC_FILEVERSION 1,8,730,8737
	#define RC_FILEVERSION_STRING "1, 8, 730, 8737\0"
	static const char FULLVERSION_STRING[] = "1.8.730.8737";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 3;
	

}
#endif //VERSION_H
