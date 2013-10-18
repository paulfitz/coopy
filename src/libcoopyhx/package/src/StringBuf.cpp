#include <hxcpp.h>

#ifndef INCLUDED_StringBuf
#include <StringBuf.h>
#endif

Void StringBuf_obj::__construct()
{
HX_STACK_FRAME("StringBuf","new",0xaaa8f4b4,"StringBuf.new","/usr/lib/haxe/std/cpp/_std/StringBuf.hx",30,0x629ed4da)
{
	HX_STACK_LINE(30)
	this->b = Array_obj< ::String >::__new();
}
;
	return null();
}

StringBuf_obj::~StringBuf_obj() { }

Dynamic StringBuf_obj::__CreateEmpty() { return  new StringBuf_obj; }
hx::ObjectPtr< StringBuf_obj > StringBuf_obj::__new()
{  hx::ObjectPtr< StringBuf_obj > result = new StringBuf_obj();
	result->__construct();
	return result;}

Dynamic StringBuf_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< StringBuf_obj > result = new StringBuf_obj();
	result->__construct();
	return result;}

Void StringBuf_obj::add( Dynamic x){
{
		HX_STACK_FRAME("StringBuf","add",0xaa9f1675,"StringBuf.add","/usr/lib/haxe/std/cpp/_std/StringBuf.hx",41,0x629ed4da)
		HX_STACK_THIS(this)
		HX_STACK_ARG(x,"x")
		HX_STACK_LINE(41)
		this->b->push(x);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(StringBuf_obj,add,(void))


StringBuf_obj::StringBuf_obj()
{
}

void StringBuf_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(StringBuf);
	HX_MARK_MEMBER_NAME(b,"b");
	HX_MARK_END_CLASS();
}

void StringBuf_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(b,"b");
}

Dynamic StringBuf_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"b") ) { return b; }
		break;
	case 3:
		if (HX_FIELD_EQ(inName,"add") ) { return add_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic StringBuf_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"b") ) { b=inValue.Cast< Array< ::String > >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void StringBuf_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("b"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(StringBuf_obj,b),HX_CSTRING("b")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("b"),
	HX_CSTRING("add"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(StringBuf_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(StringBuf_obj::__mClass,"__mClass");
};

#endif

Class StringBuf_obj::__mClass;

void StringBuf_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("StringBuf"), hx::TCanCast< StringBuf_obj> ,sStaticFields,sMemberFields,
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

void StringBuf_obj::__boot()
{
}

