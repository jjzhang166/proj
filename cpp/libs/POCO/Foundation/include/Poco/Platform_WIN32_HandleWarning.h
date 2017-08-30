//#pragma once
/*
@author zhp
@date 2016/1/29 22:43
@purpose ½ûÖ¹Ä³Ð©¾¯¸æ
*/

#pragma warning(push)

//#pragma warning(disable:4018)	// signed/unsigned comparison
#pragma warning(disable:4250)	// VC++ 11.0: inheriting from std stream classes produces C4250 warning;
// see <http://connect.microsoft.com/VisualStudio/feedback/details/733720/inheriting-from-std-fstream-produces-c4250-warning>
#pragma warning(disable:4251)	// ... needs to have dll-interface warning
#pragma warning(disable:4275)	// non dll-interface class 'std::exception' used as base for dll-interface class 'Poco::Exception'
#pragma warning(disable:4344)	// behavior change: use of explicit template arguments results in call to '...' but '...' is a better match
#pragma warning(disable:4351)	// new behavior: elements of array '...' will be default initialized
#pragma warning(disable:4355)	// 'this' : used in base member initializer list
#pragma warning(disable:4675)	// resolved overload was found by argument-dependent lookup
#pragma warning(disable:4996)	// VC++ 8.0 deprecation warnings