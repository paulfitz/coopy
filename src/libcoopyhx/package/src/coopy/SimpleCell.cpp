#include <hxcpp.h>

#ifndef INCLUDED_coopy_SimpleCell
#include <coopy/SimpleCell.h>
#endif
namespace coopy{

Void SimpleCell_obj::__construct(Dynamic x)
{
HX_STACK_FRAME("coopy.SimpleCell","new",0x47ea1b28,"coopy.SimpleCell.new","coopy/SimpleCell.hx",9,0xe0d8d4a7)

HX_STACK_ARG(x,"x")
{
	HX_STACK_LINE(9)
	this->datum = x;
}
;
	return null();
}

SimpleCell_obj::~SimpleCell_obj() { }

Dynamic SimpleCell_obj::__CreateEmpty() { return  new SimpleCell_obj; }
hx::ObjectPtr< SimpleCell_obj > SimpleCell_obj::__new(Dynamic x)
{  hx::ObjectPtr< SimpleCell_obj > result = new SimpleCell_obj();
	result->__construct(x);
	return result;}

Dynamic SimpleCell_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< SimpleCell_obj > result = new SimpleCell_obj();
	result->__construct(inArgs[0]);
	return result;}

::String SimpleCell_obj::toString( ){
	HX_STACK_FRAME("coopy.SimpleCell","toString",0x112c70e4,"coopy.SimpleCell.toString","coopy/SimpleCell.hx",12,0xe0d8d4a7)
	HX_STACK_THIS(this)
	HX_STACK_LINE(12)
	return this->datum;
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleCell_obj,toString,return )


SimpleCell_obj::SimpleCell_obj()
{
}

void SimpleCell_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(SimpleCell);
	HX_MARK_MEMBER_NAME(datum,"datum");
	HX_MARK_END_CLASS();
}

void SimpleCell_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(datum,"datum");
}

Dynamic SimpleCell_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 5:
		if (HX_FIELD_EQ(inName,"datum") ) { return datum; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic SimpleCell_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 5:
		if (HX_FIELD_EQ(inName,"datum") ) { datum=inValue.Cast< Dynamic >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void SimpleCell_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("datum"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(SimpleCell_obj,datum),HX_CSTRING("datum")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("datum"),
	HX_CSTRING("toString"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(SimpleCell_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(SimpleCell_obj::__mClass,"__mClass");
};

#endif

Class SimpleCell_obj::__mClass;

void SimpleCell_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.SimpleCell"), hx::TCanCast< SimpleCell_obj> ,sStaticFields,sMemberFields,
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

void SimpleCell_obj::__boot()
{
}

} // end namespace coopy
