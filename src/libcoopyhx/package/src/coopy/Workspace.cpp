#include <hxcpp.h>

#ifndef INCLUDED_coopy_Report
#include <coopy/Report.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableComparisonState
#include <coopy/TableComparisonState.h>
#endif
#ifndef INCLUDED_coopy_ViewedDatum
#include <coopy/ViewedDatum.h>
#endif
#ifndef INCLUDED_coopy_Workspace
#include <coopy/Workspace.h>
#endif
namespace coopy{

Void Workspace_obj::__construct()
{
HX_STACK_FRAME("coopy.Workspace","new",0x9e8705c5,"coopy.Workspace.new","coopy/Workspace.hx",19,0x366416cc)
{
}
;
	return null();
}

Workspace_obj::~Workspace_obj() { }

Dynamic Workspace_obj::__CreateEmpty() { return  new Workspace_obj; }
hx::ObjectPtr< Workspace_obj > Workspace_obj::__new()
{  hx::ObjectPtr< Workspace_obj > result = new Workspace_obj();
	result->__construct();
	return result;}

Dynamic Workspace_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Workspace_obj > result = new Workspace_obj();
	result->__construct();
	return result;}


Workspace_obj::Workspace_obj()
{
}

void Workspace_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Workspace);
	HX_MARK_MEMBER_NAME(parent,"parent");
	HX_MARK_MEMBER_NAME(local,"local");
	HX_MARK_MEMBER_NAME(remote,"remote");
	HX_MARK_MEMBER_NAME(report,"report");
	HX_MARK_MEMBER_NAME(tparent,"tparent");
	HX_MARK_MEMBER_NAME(tlocal,"tlocal");
	HX_MARK_MEMBER_NAME(tremote,"tremote");
	HX_MARK_MEMBER_NAME(p2l,"p2l");
	HX_MARK_MEMBER_NAME(p2r,"p2r");
	HX_MARK_MEMBER_NAME(l2r,"l2r");
	HX_MARK_END_CLASS();
}

void Workspace_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(parent,"parent");
	HX_VISIT_MEMBER_NAME(local,"local");
	HX_VISIT_MEMBER_NAME(remote,"remote");
	HX_VISIT_MEMBER_NAME(report,"report");
	HX_VISIT_MEMBER_NAME(tparent,"tparent");
	HX_VISIT_MEMBER_NAME(tlocal,"tlocal");
	HX_VISIT_MEMBER_NAME(tremote,"tremote");
	HX_VISIT_MEMBER_NAME(p2l,"p2l");
	HX_VISIT_MEMBER_NAME(p2r,"p2r");
	HX_VISIT_MEMBER_NAME(l2r,"l2r");
}

Dynamic Workspace_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 3:
		if (HX_FIELD_EQ(inName,"p2l") ) { return p2l; }
		if (HX_FIELD_EQ(inName,"p2r") ) { return p2r; }
		if (HX_FIELD_EQ(inName,"l2r") ) { return l2r; }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"local") ) { return local; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"parent") ) { return parent; }
		if (HX_FIELD_EQ(inName,"remote") ) { return remote; }
		if (HX_FIELD_EQ(inName,"report") ) { return report; }
		if (HX_FIELD_EQ(inName,"tlocal") ) { return tlocal; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"tparent") ) { return tparent; }
		if (HX_FIELD_EQ(inName,"tremote") ) { return tremote; }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Workspace_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 3:
		if (HX_FIELD_EQ(inName,"p2l") ) { p2l=inValue.Cast< ::coopy::TableComparisonState >(); return inValue; }
		if (HX_FIELD_EQ(inName,"p2r") ) { p2r=inValue.Cast< ::coopy::TableComparisonState >(); return inValue; }
		if (HX_FIELD_EQ(inName,"l2r") ) { l2r=inValue.Cast< ::coopy::TableComparisonState >(); return inValue; }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"local") ) { local=inValue.Cast< ::coopy::ViewedDatum >(); return inValue; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"parent") ) { parent=inValue.Cast< ::coopy::ViewedDatum >(); return inValue; }
		if (HX_FIELD_EQ(inName,"remote") ) { remote=inValue.Cast< ::coopy::ViewedDatum >(); return inValue; }
		if (HX_FIELD_EQ(inName,"report") ) { report=inValue.Cast< ::coopy::Report >(); return inValue; }
		if (HX_FIELD_EQ(inName,"tlocal") ) { tlocal=inValue.Cast< ::coopy::Table >(); return inValue; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"tparent") ) { tparent=inValue.Cast< ::coopy::Table >(); return inValue; }
		if (HX_FIELD_EQ(inName,"tremote") ) { tremote=inValue.Cast< ::coopy::Table >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void Workspace_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("parent"));
	outFields->push(HX_CSTRING("local"));
	outFields->push(HX_CSTRING("remote"));
	outFields->push(HX_CSTRING("report"));
	outFields->push(HX_CSTRING("tparent"));
	outFields->push(HX_CSTRING("tlocal"));
	outFields->push(HX_CSTRING("tremote"));
	outFields->push(HX_CSTRING("p2l"));
	outFields->push(HX_CSTRING("p2r"));
	outFields->push(HX_CSTRING("l2r"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(Workspace_obj,parent),HX_CSTRING("parent")},
	{hx::fsObject,(int)offsetof(Workspace_obj,local),HX_CSTRING("local")},
	{hx::fsObject,(int)offsetof(Workspace_obj,remote),HX_CSTRING("remote")},
	{hx::fsObject,(int)offsetof(Workspace_obj,report),HX_CSTRING("report")},
	{hx::fsObject,(int)offsetof(Workspace_obj,tparent),HX_CSTRING("tparent")},
	{hx::fsObject,(int)offsetof(Workspace_obj,tlocal),HX_CSTRING("tlocal")},
	{hx::fsObject,(int)offsetof(Workspace_obj,tremote),HX_CSTRING("tremote")},
	{hx::fsObject,(int)offsetof(Workspace_obj,p2l),HX_CSTRING("p2l")},
	{hx::fsObject,(int)offsetof(Workspace_obj,p2r),HX_CSTRING("p2r")},
	{hx::fsObject,(int)offsetof(Workspace_obj,l2r),HX_CSTRING("l2r")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("parent"),
	HX_CSTRING("local"),
	HX_CSTRING("remote"),
	HX_CSTRING("report"),
	HX_CSTRING("tparent"),
	HX_CSTRING("tlocal"),
	HX_CSTRING("tremote"),
	HX_CSTRING("p2l"),
	HX_CSTRING("p2r"),
	HX_CSTRING("l2r"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Workspace_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Workspace_obj::__mClass,"__mClass");
};

#endif

Class Workspace_obj::__mClass;

void Workspace_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Workspace"), hx::TCanCast< Workspace_obj> ,sStaticFields,sMemberFields,
	&__CreateEmpty, &__Create,
	&super::__SGetClass(), 0, sMarkStatics
#ifdef HXCPP_VISIT_ALLOCS
    , sVisitStatics
#endif
#ifdef HXCPP_SCRIPTABLE
    , sMemberStorageInfo
#endif
);
}

void Workspace_obj::__boot()
{
}

} // end namespace coopy
