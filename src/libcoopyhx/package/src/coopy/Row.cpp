#include <hxcpp.h>

#ifndef INCLUDED_coopy_Row
#include <coopy/Row.h>
#endif
namespace coopy{

HX_DEFINE_DYNAMIC_FUNC1(Row_obj,getRowString,return )


static ::String sMemberFields[] = {
	HX_CSTRING("getRowString"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Row_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Row_obj::__mClass,"__mClass");
};

#endif

Class Row_obj::__mClass;

void Row_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Row"), hx::TCanCast< Row_obj> ,0,sMemberFields,
	0, 0,
	&super::__SGetClass(), 0, sMarkStatics
#ifdef HXCPP_VISIT_ALLOCS
    , sVisitStatics
#endif
#ifdef HXCPP_SCRIPTABLE
    , 0
#endif
);
}

void Row_obj::__boot()
{
}

} // end namespace coopy
