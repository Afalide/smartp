
#ifndef SMARTP_COMMONS_HPP
#define SMARTP_COMMONS_HPP

///////////////////////////////////////////

//windows __declspec for dlls

#ifdef WIN32
    #ifdef SMARTP_BUILDING_LIB
        #define SMARTP_API __declspec(dllexport)
    #else
        #define SMARTP_API __declspec(dllimport)
    #endif
#else
    #define SMARTP_API
#endif

///////////////////////////////////////////

// debug outputs

#ifdef SMARTP_USE_LOG
    #define SMARTP_LOG(X) X;
#else
    #define SMARTP_LOG(X)
#endif

#endif //SMARTP_COMMONS_HPP
