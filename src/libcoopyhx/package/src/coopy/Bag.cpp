#include <hxcpp.h>

#ifndef INCLUDED_coopy_Bag
#include <coopy/Bag.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
namespace coopy{

HX_DEFINE_DYNAMIC_FUNC1(Bag_obj,getItem,return )

HX_DEFINE_DYNAMIC_FUNC0(Bag_obj,getItemView,return )


static ::String sMemberFields[] = {
	HX_CSTRING("getItem"),
	HX_CSTRING("getItemView"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Bag_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Bag_obj::__mClass,"__mClass");
};

#endif

Class Bag_obj::__mClass;

void Bag_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Bag"), hx::TCanCast< Bag_obj> ,0,sMemberFields,
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

void Bag_obj::__boot()
{
}

} // end namespace coopy
