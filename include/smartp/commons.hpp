
#ifndef SMARTP_COMMONS_HPP
#define SMARTP_COMMONS_HPP

#ifdef SMARTP_BUILDING_LIB
#define SMARTP_API __declspec(dllexport)
#else
#define SMARTP_API __declspec(dllimport)
#endif

#ifdef SMARTP_USE_LOG
#define SMARTP_LOG(X) X;
#else
#define SMARTP_LOG(X)
#endif

#endif //SMARTP_COMMONS_HPP
