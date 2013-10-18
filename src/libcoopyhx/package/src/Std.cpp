#include <hxcpp.h>

#ifndef INCLUDED_Std
#include <Std.h>
#endif

Void Std_obj::__construct()
{
	return null();
}

Std_obj::~Std_obj() { }

Dynamic Std_obj::__CreateEmpty() { return  new Std_obj; }
hx::ObjectPtr< Std_obj > Std_obj::__new()
{  hx::ObjectPtr< Std_obj > result = new Std_obj();
	result->__construct();
	return result;}

Dynamic Std_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Std_obj > result = new Std_obj();
	result->__construct();
	return result;}

::String Std_obj::string( Dynamic s){
	HX_STACK_FRAME("Std","string",0xcf48855c,"Std.string","/usr/lib/haxe/std/cpp/_std/Std.hx",32,0x1614e339)
	HX_STACK_ARG(s,"s")
	HX_STACK_LINE(32)
	if (((s == null()))){
		HX_STACK_LINE(32)
		return HX_CSTRING("null");
	}
	else{
		HX_STACK_LINE(32)
		return s->toString();
	}
	HX_STACK_LINE(32)
	return null();
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(Std_obj,string,return )


Std_obj::Std_obj()
{
}

void Std_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Std);
	HX_MARK_END_CLASS();
}

void Std_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic Std_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 6:
		if (HX_FIELD_EQ(inName,"string") ) { return string_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Std_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void Std_obj::__GetFields(Array< ::String> &outFields)
{
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("string"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo *sMemberStorageInfo = 0;
#endif

static ::String sMemberFields[] = {
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Std_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Std_obj::__mClass,"__mClass");
};

#endif

Class Std_obj::__mClass;

void Std_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("Std"), hx::TCanCast< Std_obj> ,sStaticFields,sMemberFields,
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

void Std_obj::__boot()
{
}

