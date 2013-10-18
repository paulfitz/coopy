#include <hxcpp.h>

#ifndef INCLUDED_coopy_Bag
#include <coopy/Bag.h>
#endif
#ifndef INCLUDED_coopy_SimpleView
#include <coopy/SimpleView.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
#ifndef INCLUDED_coopy_ViewedDatum
#include <coopy/ViewedDatum.h>
#endif
namespace coopy{

Void ViewedDatum_obj::__construct(Dynamic datum,::coopy::View view)
{
HX_STACK_FRAME("coopy.ViewedDatum","new",0xabba72bb,"coopy.ViewedDatum.new","coopy/ViewedDatum.hx",10,0xa5d31cd6)

HX_STACK_ARG(datum,"datum")

HX_STACK_ARG(view,"view")
{
	HX_STACK_LINE(11)
	this->datum = datum;
	HX_STACK_LINE(12)
	this->view = view;
}
;
	return null();
}

ViewedDatum_obj::~ViewedDatum_obj() { }

Dynamic ViewedDatum_obj::__CreateEmpty() { return  new ViewedDatum_obj; }
hx::ObjectPtr< ViewedDatum_obj > ViewedDatum_obj::__new(Dynamic datum,::coopy::View view)
{  hx::ObjectPtr< ViewedDatum_obj > result = new ViewedDatum_obj();
	result->__construct(datum,view);
	return result;}

Dynamic ViewedDatum_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< ViewedDatum_obj > result = new ViewedDatum_obj();
	result->__construct(inArgs[0],inArgs[1]);
	return result;}

::String ViewedDatum_obj::toString( ){
	HX_STACK_FRAME("coopy.ViewedDatum","toString",0xae36c471,"coopy.ViewedDatum.toString","coopy/ViewedDatum.hx",21,0xa5d31cd6)
	HX_STACK_THIS(this)
	HX_STACK_LINE(21)
	return this->view->toString(this->datum);
}


HX_DEFINE_DYNAMIC_FUNC0(ViewedDatum_obj,toString,return )

::coopy::Bag ViewedDatum_obj::getBag( ){
	HX_STACK_FRAME("coopy.ViewedDatum","getBag",0xec2af357,"coopy.ViewedDatum.getBag","coopy/ViewedDatum.hx",25,0xa5d31cd6)
	HX_STACK_THIS(this)
	HX_STACK_LINE(25)
	return this->view->getBag(this->datum);
}


HX_DEFINE_DYNAMIC_FUNC0(ViewedDatum_obj,getBag,return )

::coopy::Table ViewedDatum_obj::getTable( ){
	HX_STACK_FRAME("coopy.ViewedDatum","getTable",0xe082bb5d,"coopy.ViewedDatum.getTable","coopy/ViewedDatum.hx",29,0xa5d31cd6)
	HX_STACK_THIS(this)
	HX_STACK_LINE(29)
	return this->view->getTable(this->datum);
}


HX_DEFINE_DYNAMIC_FUNC0(ViewedDatum_obj,getTable,return )

bool ViewedDatum_obj::hasStructure( ){
	HX_STACK_FRAME("coopy.ViewedDatum","hasStructure",0x06cef3be,"coopy.ViewedDatum.hasStructure","coopy/ViewedDatum.hx",33,0xa5d31cd6)
	HX_STACK_THIS(this)
	HX_STACK_LINE(33)
	return this->view->hasStructure(this->datum);
}


HX_DEFINE_DYNAMIC_FUNC0(ViewedDatum_obj,hasStructure,return )

::coopy::ViewedDatum ViewedDatum_obj::getSimpleView( Dynamic datum){
	HX_STACK_FRAME("coopy.ViewedDatum","getSimpleView",0x13ca0208,"coopy.ViewedDatum.getSimpleView","coopy/ViewedDatum.hx",16,0xa5d31cd6)
	HX_STACK_ARG(datum,"datum")
	HX_STACK_LINE(16)
	return ::coopy::ViewedDatum_obj::__new(datum,::coopy::SimpleView_obj::__new());
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(ViewedDatum_obj,getSimpleView,return )


ViewedDatum_obj::ViewedDatum_obj()
{
}

void ViewedDatum_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(ViewedDatum);
	HX_MARK_MEMBER_NAME(datum,"datum");
	HX_MARK_MEMBER_NAME(view,"view");
	HX_MARK_END_CLASS();
}

void ViewedDatum_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(datum,"datum");
	HX_VISIT_MEMBER_NAME(view,"view");
}

Dynamic ViewedDatum_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"view") ) { return view; }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"datum") ) { return datum; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"getBag") ) { return getBag_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		if (HX_FIELD_EQ(inName,"getTable") ) { return getTable_dyn(); }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"hasStructure") ) { return hasStructure_dyn(); }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"getSimpleView") ) { return getSimpleView_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic ViewedDatum_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"view") ) { view=inValue.Cast< ::coopy::View >(); return inValue; }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"datum") ) { datum=inValue.Cast< Dynamic >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void ViewedDatum_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("datum"));
	outFields->push(HX_CSTRING("view"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("getSimpleView"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(ViewedDatum_obj,datum),HX_CSTRING("datum")},
	{hx::fsObject,(int)offsetof(ViewedDatum_obj,view),HX_CSTRING("view")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("datum"),
	HX_CSTRING("view"),
	HX_CSTRING("toString"),
	HX_CSTRING("getBag"),
	HX_CSTRING("getTable"),
	HX_CSTRING("hasStructure"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(ViewedDatum_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(ViewedDatum_obj::__mClass,"__mClass");
};

#endif

Class ViewedDatum_obj::__mClass;

void ViewedDatum_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.ViewedDatum"), hx::TCanCast< ViewedDatum_obj> ,sStaticFields,sMemberFields,
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

void ViewedDatum_obj::__boot()
{
}

} // end namespace coopy
