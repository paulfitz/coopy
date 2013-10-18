#include <hxcpp.h>

#ifndef INCLUDED_Lambda
#include <Lambda.h>
#endif
#ifndef INCLUDED_List
#include <List.h>
#endif

Void Lambda_obj::__construct()
{
	return null();
}

Lambda_obj::~Lambda_obj() { }

Dynamic Lambda_obj::__CreateEmpty() { return  new Lambda_obj; }
hx::ObjectPtr< Lambda_obj > Lambda_obj::__new()
{  hx::ObjectPtr< Lambda_obj > result = new Lambda_obj();
	result->__construct();
	return result;}

Dynamic Lambda_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Lambda_obj > result = new Lambda_obj();
	result->__construct();
	return result;}

Dynamic Lambda_obj::array( Dynamic it){
	HX_STACK_FRAME("Lambda","array",0x9c8b0512,"Lambda.array","/usr/lib/haxe/std/Lambda.hx",42,0xc6d51bb6)
	HX_STACK_ARG(it,"it")
	HX_STACK_LINE(43)
	Dynamic a = Dynamic( Array_obj<Dynamic>::__new() );		HX_STACK_VAR(a,"a");
	HX_STACK_LINE(44)
	for(::cpp::FastIterator_obj< Dynamic > *__it = ::cpp::CreateFastIterator< Dynamic >(it->__Field(HX_CSTRING("iterator"),true)());  __it->hasNext(); ){
		Dynamic i = __it->next();
		a->__Field(HX_CSTRING("push"),true)(i);
	}
	HX_STACK_LINE(46)
	return a;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(Lambda_obj,array,return )

::List Lambda_obj::map( Dynamic it,Dynamic f){
	HX_STACK_FRAME("Lambda","map",0x3d8fa1d5,"Lambda.map","/usr/lib/haxe/std/Lambda.hx",68,0xc6d51bb6)
	HX_STACK_ARG(it,"it")
	HX_STACK_ARG(f,"f")
	HX_STACK_LINE(69)
	::List l = ::List_obj::__new();		HX_STACK_VAR(l,"l");
	HX_STACK_LINE(70)
	for(::cpp::FastIterator_obj< Dynamic > *__it = ::cpp::CreateFastIterator< Dynamic >(it->__Field(HX_CSTRING("iterator"),true)());  __it->hasNext(); ){
		Dynamic x = __it->next();
		l->add(f(x));
	}
	HX_STACK_LINE(72)
	return l;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC2(Lambda_obj,map,return )


Lambda_obj::Lambda_obj()
{
}

void Lambda_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Lambda);
	HX_MARK_END_CLASS();
}

void Lambda_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic Lambda_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 3:
		if (HX_FIELD_EQ(inName,"map") ) { return map_dyn(); }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"array") ) { return array_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Lambda_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void Lambda_obj::__GetFields(Array< ::String> &outFields)
{
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("array"),
	HX_CSTRING("map"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo *sMemberStorageInfo = 0;
#endif

static ::String sMemberFields[] = {
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Lambda_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Lambda_obj::__mClass,"__mClass");
};

#endif

Class Lambda_obj::__mClass;

void Lambda_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("Lambda"), hx::TCanCast< Lambda_obj> ,sStaticFields,sMemberFields,
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

void Lambda_obj::__boot()
{
}

