#include <hxcpp.h>

#ifndef INCLUDED_coopy_ChangeType
#include <coopy/ChangeType.h>
#endif
namespace coopy{

::coopy::ChangeType ChangeType_obj::BOTH_CHANGE;

::coopy::ChangeType ChangeType_obj::LOCAL_CHANGE;

::coopy::ChangeType ChangeType_obj::NOTE_CHANGE;

::coopy::ChangeType ChangeType_obj::NO_CHANGE;

::coopy::ChangeType ChangeType_obj::REMOTE_CHANGE;

::coopy::ChangeType ChangeType_obj::SAME_CHANGE;

HX_DEFINE_CREATE_ENUM(ChangeType_obj)

int ChangeType_obj::__FindIndex(::String inName)
{
	if (inName==HX_CSTRING("BOTH_CHANGE")) return 3;
	if (inName==HX_CSTRING("LOCAL_CHANGE")) return 2;
	if (inName==HX_CSTRING("NOTE_CHANGE")) return 5;
	if (inName==HX_CSTRING("NO_CHANGE")) return 0;
	if (inName==HX_CSTRING("REMOTE_CHANGE")) return 1;
	if (inName==HX_CSTRING("SAME_CHANGE")) return 4;
	return super::__FindIndex(inName);
}

int ChangeType_obj::__FindArgCount(::String inName)
{
	if (inName==HX_CSTRING("BOTH_CHANGE")) return 0;
	if (inName==HX_CSTRING("LOCAL_CHANGE")) return 0;
	if (inName==HX_CSTRING("NOTE_CHANGE")) return 0;
	if (inName==HX_CSTRING("NO_CHANGE")) return 0;
	if (inName==HX_CSTRING("REMOTE_CHANGE")) return 0;
	if (inName==HX_CSTRING("SAME_CHANGE")) return 0;
	return super::__FindArgCount(inName);
}

Dynamic ChangeType_obj::__Field(const ::String &inName,bool inCallProp)
{
	if (inName==HX_CSTRING("BOTH_CHANGE")) return BOTH_CHANGE;
	if (inName==HX_CSTRING("LOCAL_CHANGE")) return LOCAL_CHANGE;
	if (inName==HX_CSTRING("NOTE_CHANGE")) return NOTE_CHANGE;
	if (inName==HX_CSTRING("NO_CHANGE")) return NO_CHANGE;
	if (inName==HX_CSTRING("REMOTE_CHANGE")) return REMOTE_CHANGE;
	if (inName==HX_CSTRING("SAME_CHANGE")) return SAME_CHANGE;
	return super::__Field(inName,inCallProp);
}

static ::String sStaticFields[] = {
	HX_CSTRING("NO_CHANGE"),
	HX_CSTRING("REMOTE_CHANGE"),
	HX_CSTRING("LOCAL_CHANGE"),
	HX_CSTRING("BOTH_CHANGE"),
	HX_CSTRING("SAME_CHANGE"),
	HX_CSTRING("NOTE_CHANGE"),
	::String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(ChangeType_obj::BOTH_CHANGE,"BOTH_CHANGE");
	HX_MARK_MEMBER_NAME(ChangeType_obj::LOCAL_CHANGE,"LOCAL_CHANGE");
	HX_MARK_MEMBER_NAME(ChangeType_obj::NOTE_CHANGE,"NOTE_CHANGE");
	HX_MARK_MEMBER_NAME(ChangeType_obj::NO_CHANGE,"NO_CHANGE");
	HX_MARK_MEMBER_NAME(ChangeType_obj::REMOTE_CHANGE,"REMOTE_CHANGE");
	HX_MARK_MEMBER_NAME(ChangeType_obj::SAME_CHANGE,"SAME_CHANGE");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatic(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(ChangeType_obj::__mClass,"__mClass");
	HX_VISIT_MEMBER_NAME(ChangeType_obj::BOTH_CHANGE,"BOTH_CHANGE");
	HX_VISIT_MEMBER_NAME(ChangeType_obj::LOCAL_CHANGE,"LOCAL_CHANGE");
	HX_VISIT_MEMBER_NAME(ChangeType_obj::NOTE_CHANGE,"NOTE_CHANGE");
	HX_VISIT_MEMBER_NAME(ChangeType_obj::NO_CHANGE,"NO_CHANGE");
	HX_VISIT_MEMBER_NAME(ChangeType_obj::REMOTE_CHANGE,"REMOTE_CHANGE");
	HX_VISIT_MEMBER_NAME(ChangeType_obj::SAME_CHANGE,"SAME_CHANGE");
};
#endif

static ::String sMemberFields[] = { ::String(null()) };
Class ChangeType_obj::__mClass;

Dynamic __Create_ChangeType_obj() { return new ChangeType_obj; }

void ChangeType_obj::__register()
{

hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.ChangeType"), hx::TCanCast< ChangeType_obj >,sStaticFields,sMemberFields,
	&__Create_ChangeType_obj, &__Create,
	&super::__SGetClass(), &CreateChangeType_obj, sMarkStatics
#ifdef HXCPP_VISIT_ALLOCS
    , sVisitStatic
#endif
#ifdef HXCPP_SCRIPTABLE
    , 0
#endif
);
}

void ChangeType_obj::__boot()
{
hx::Static(BOTH_CHANGE) = hx::CreateEnum< ChangeType_obj >(HX_CSTRING("BOTH_CHANGE"),3);
hx::Static(LOCAL_CHANGE) = hx::CreateEnum< ChangeType_obj >(HX_CSTRING("LOCAL_CHANGE"),2);
hx::Static(NOTE_CHANGE) = hx::CreateEnum< ChangeType_obj >(HX_CSTRING("NOTE_CHANGE"),5);
hx::Static(NO_CHANGE) = hx::CreateEnum< ChangeType_obj >(HX_CSTRING("NO_CHANGE"),0);
hx::Static(REMOTE_CHANGE) = hx::CreateEnum< ChangeType_obj >(HX_CSTRING("REMOTE_CHANGE"),1);
hx::Static(SAME_CHANGE) = hx::CreateEnum< ChangeType_obj >(HX_CSTRING("SAME_CHANGE"),4);
}


} // end namespace coopy
