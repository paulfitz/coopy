#include <hxcpp.h>

#ifndef INCLUDED_Std
#include <Std.h>
#endif
#ifndef INCLUDED_coopy_Bag
#include <coopy/Bag.h>
#endif
#ifndef INCLUDED_coopy_SimpleCell
#include <coopy/SimpleCell.h>
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
namespace coopy{

Void SimpleView_obj::__construct()
{
HX_STACK_FRAME("coopy.SimpleView","new",0xb5e7496b,"coopy.SimpleView.new","coopy/SimpleView.hx",7,0x8fd55fc4)
{
}
;
	return null();
}

SimpleView_obj::~SimpleView_obj() { }

Dynamic SimpleView_obj::__CreateEmpty() { return  new SimpleView_obj; }
hx::ObjectPtr< SimpleView_obj > SimpleView_obj::__new()
{  hx::ObjectPtr< SimpleView_obj > result = new SimpleView_obj();
	result->__construct();
	return result;}

Dynamic SimpleView_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< SimpleView_obj > result = new SimpleView_obj();
	result->__construct();
	return result;}

hx::Object *SimpleView_obj::__ToInterface(const hx::type_info &inType) {
	if (inType==typeid( ::coopy::View_obj)) return operator ::coopy::View_obj *();
	return super::__ToInterface(inType);
}

::String SimpleView_obj::toString( Dynamic d){
	HX_STACK_FRAME("coopy.SimpleView","toString",0x0291afc1,"coopy.SimpleView.toString","coopy/SimpleView.hx",10,0x8fd55fc4)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(11)
	if (((d == null()))){
		HX_STACK_LINE(11)
		return null();
	}
	HX_STACK_LINE(12)
	return (HX_CSTRING("") + ::Std_obj::string(d));
}


HX_DEFINE_DYNAMIC_FUNC1(SimpleView_obj,toString,return )

::coopy::Bag SimpleView_obj::getBag( Dynamic d){
	HX_STACK_FRAME("coopy.SimpleView","getBag",0x6d7d6aa7,"coopy.SimpleView.getBag","coopy/SimpleView.hx",16,0x8fd55fc4)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(16)
	return null();
}


HX_DEFINE_DYNAMIC_FUNC1(SimpleView_obj,getBag,return )

::coopy::Table SimpleView_obj::getTable( Dynamic d){
	HX_STACK_FRAME("coopy.SimpleView","getTable",0x34dda6ad,"coopy.SimpleView.getTable","coopy/SimpleView.hx",20,0x8fd55fc4)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(20)
	return null();
}


HX_DEFINE_DYNAMIC_FUNC1(SimpleView_obj,getTable,return )

bool SimpleView_obj::hasStructure( Dynamic d){
	HX_STACK_FRAME("coopy.SimpleView","hasStructure",0xca89c70e,"coopy.SimpleView.hasStructure","coopy/SimpleView.hx",24,0x8fd55fc4)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(24)
	return false;
}


HX_DEFINE_DYNAMIC_FUNC1(SimpleView_obj,hasStructure,return )

bool SimpleView_obj::equals( Dynamic d1,Dynamic d2){
	HX_STACK_FRAME("coopy.SimpleView","equals",0x8a566e14,"coopy.SimpleView.equals","coopy/SimpleView.hx",27,0x8fd55fc4)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d1,"d1")
	HX_STACK_ARG(d2,"d2")
	HX_STACK_LINE(29)
	if (((bool((d1 == null())) && bool((d2 == null()))))){
		HX_STACK_LINE(29)
		return true;
	}
	HX_STACK_LINE(30)
	if (((bool((d1 == null())) && bool(((HX_CSTRING("") + ::Std_obj::string(d2)) == HX_CSTRING("")))))){
		HX_STACK_LINE(30)
		return true;
	}
	HX_STACK_LINE(31)
	if (((bool(((HX_CSTRING("") + ::Std_obj::string(d1)) == HX_CSTRING(""))) && bool((d2 == null()))))){
		HX_STACK_LINE(31)
		return true;
	}
	HX_STACK_LINE(32)
	return ((HX_CSTRING("") + ::Std_obj::string(d1)) == (HX_CSTRING("") + ::Std_obj::string(d2)));
}


HX_DEFINE_DYNAMIC_FUNC2(SimpleView_obj,equals,return )

Dynamic SimpleView_obj::toDatum( ::String str){
	HX_STACK_FRAME("coopy.SimpleView","toDatum",0xedb8c77f,"coopy.SimpleView.toDatum","coopy/SimpleView.hx",35,0x8fd55fc4)
	HX_STACK_THIS(this)
	HX_STACK_ARG(str,"str")
	HX_STACK_LINE(36)
	if (((str == null()))){
		HX_STACK_LINE(36)
		return null();
	}
	HX_STACK_LINE(40)
	return ::coopy::SimpleCell_obj::__new(str);
}


HX_DEFINE_DYNAMIC_FUNC1(SimpleView_obj,toDatum,return )


SimpleView_obj::SimpleView_obj()
{
}

void SimpleView_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(SimpleView);
	HX_MARK_END_CLASS();
}

void SimpleView_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic SimpleView_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 6:
		if (HX_FIELD_EQ(inName,"getBag") ) { return getBag_dyn(); }
		if (HX_FIELD_EQ(inName,"equals") ) { return equals_dyn(); }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"toDatum") ) { return toDatum_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		if (HX_FIELD_EQ(inName,"getTable") ) { return getTable_dyn(); }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"hasStructure") ) { return hasStructure_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic SimpleView_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void SimpleView_obj::__GetFields(Array< ::String> &outFields)
{
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo *sMemberStorageInfo = 0;
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("toString"),
	HX_CSTRING("getBag"),
	HX_CSTRING("getTable"),
	HX_CSTRING("hasStructure"),
	HX_CSTRING("equals"),
	HX_CSTRING("toDatum"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(SimpleView_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(SimpleView_obj::__mClass,"__mClass");
};

#endif

Class SimpleView_obj::__mClass;

void SimpleView_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.SimpleView"), hx::TCanCast< SimpleView_obj> ,sStaticFields,sMemberFields,
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

void SimpleView_obj::__boot()
{
}

} // end namespace coopy
