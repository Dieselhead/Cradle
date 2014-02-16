#ifndef _MACROS_
#define _MACROS_

#define HRPRINT(hr, failed, success) wprintf((FAILED(hr)) ? failed : success);
#define ReleaseCOM(object) if (object) object->Release(); object = nullptr;
#define DeletePtr(ptr) if (ptr) delete ptr; ptr = nullptr;
#define DeleteArray(arr) if (arr) delete [] arr; arr = nullptr;

#endif