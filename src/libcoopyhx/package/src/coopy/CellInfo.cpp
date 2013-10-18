#include <hxcpp.h>

#ifndef INCLUDED_coopy_CellInfo
#include <coopy/CellInfo.h>
#endif
namespace coopy{

Void CellInfo_obj::__construct()
{
HX_STACK_FRAME("coopy.CellInfo","new",0x252d60e4,"coopy.CellInfo.new","coopy/CellInfo.hx",20,0x6334b4ab)
{
}
;
	return null();
}

CellInfo_obj::~CellInfo_obj() { }

Dynamic CellInfo_obj::__CreateEmpty() { return  new CellInfo_obj; }
hx::ObjectPtr< CellInfo_obj > CellInfo_obj::__new()
{  hx::ObjectPtr< CellInfo_obj > result = new CellInfo_obj();
	result->__construct();
	return result;}

Dynamic CellInfo_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< CellInfo_obj > result = new CellInfo_obj();
	result->__construct();
	return result;}

::String CellInfo_obj::toString( ){
	HX_STACK_FRAME("coopy.CellInfo","toString",0x0c06e1a8,"coopy.CellInfo.toString","coopy/CellInfo.hx",22,0x6334b4ab)
	HX_STACK_THIS(this)
	HX_STACK_LINE(23)
	if ((!(this->updated))){
		HX_STACK_LINE(23)
		return this->value;
	}
	HX_STACK_LINE(24)
	if ((!(this->conflicted))){
		HX_STACK_LINE(24)
		return ((this->lvalue + HX_CSTRING("::")) + this->rvalue);
	}
	HX_STACK_LINE(25)
	return ((((this->pvalue + HX_CSTRING("||")) + this->lvalue) + HX_CSTRING("::")) + this->rvalue);
}


HX_DEFINE_DYNAMIC_FUNC0(CellInfo_obj,toString,return )


CellInfo_obj::CellInfo_obj()
{
}

void CellInfo_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(CellInfo);
	HX_MARK_MEMBER_NAME(value,"value");
	HX_MARK_MEMBER_NAME(pretty_value,"pretty_value");
	HX_MARK_MEMBER_NAME(category,"category");
	HX_MARK_MEMBER_NAME(category_given_tr,"category_given_tr");
	HX_MARK_MEMBER_NAME(separator,"separator");
	HX_MARK_MEMBER_NAME(updated,"updated");
	HX_MARK_MEMBER_NAME(conflicted,"conflicted");
	HX_MARK_MEMBER_NAME(pvalue,"pvalue");
	HX_MARK_MEMBER_NAME(lvalue,"lvalue");
	HX_MARK_MEMBER_NAME(rvalue,"rvalue");
	HX_MARK_END_CLASS();
}

void CellInfo_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(value,"value");
	HX_VISIT_MEMBER_NAME(pretty_value,"pretty_value");
	HX_VISIT_MEMBER_NAME(category,"category");
	HX_VISIT_MEMBER_NAME(category_given_tr,"category_given_tr");
	HX_VISIT_MEMBER_NAME(separator,"separator");
	HX_VISIT_MEMBER_NAME(updated,"updated");
	HX_VISIT_MEMBER_NAME(conflicted,"conflicted");
	HX_VISIT_MEMBER_NAME(pvalue,"pvalue");
	HX_VISIT_MEMBER_NAME(lvalue,"lvalue");
	HX_VISIT_MEMBER_NAME(rvalue,"rvalue");
}

Dynamic CellInfo_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 5:
		if (HX_FIELD_EQ(inName,"value") ) { return value; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"pvalue") ) { return pvalue; }
		if (HX_FIELD_EQ(inName,"lvalue") ) { return lvalue; }
		if (HX_FIELD_EQ(inName,"rvalue") ) { return rvalue; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"updated") ) { return updated; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"category") ) { return category; }
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"separator") ) { return separator; }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"conflicted") ) { return conflicted; }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"pretty_value") ) { return pretty_value; }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"category_given_tr") ) { return category_given_tr; }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic CellInfo_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 5:
		if (HX_FIELD_EQ(inName,"value") ) { value=inValue.Cast< ::String >(); return inValue; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"pvalue") ) { pvalue=inValue.Cast< ::String >(); return inValue; }
		if (HX_FIELD_EQ(inName,"lvalue") ) { lvalue=inValue.Cast< ::String >(); return inValue; }
		if (HX_FIELD_EQ(inName,"rvalue") ) { rvalue=inValue.Cast< ::String >(); return inValue; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"updated") ) { updated=inValue.Cast< bool >(); return inValue; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"category") ) { category=inValue.Cast< ::String >(); return inValue; }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"separator") ) { separator=inValue.Cast< ::String >(); return inValue; }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"conflicted") ) { conflicted=inValue.Cast< bool >(); return inValue; }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"pretty_value") ) { pretty_value=inValue.Cast< ::String >(); return inValue; }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"category_given_tr") ) { category_given_tr=inValue.Cast< ::String >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void CellInfo_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("value"));
	outFields->push(HX_CSTRING("pretty_value"));
	outFields->push(HX_CSTRING("category"));
	outFields->push(HX_CSTRING("category_given_tr"));
	outFields->push(HX_CSTRING("separator"));
	outFields->push(HX_CSTRING("updated"));
	outFields->push(HX_CSTRING("conflicted"));
	outFields->push(HX_CSTRING("pvalue"));
	outFields->push(HX_CSTRING("lvalue"));
	outFields->push(HX_CSTRING("rvalue"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsString,(int)offsetof(CellInfo_obj,value),HX_CSTRING("value")},
	{hx::fsString,(int)offsetof(CellInfo_obj,pretty_value),HX_CSTRING("pretty_value")},
	{hx::fsString,(int)offsetof(CellInfo_obj,category),HX_CSTRING("category")},
	{hx::fsString,(int)offsetof(CellInfo_obj,category_given_tr),HX_CSTRING("category_given_tr")},
	{hx::fsString,(int)offsetof(CellInfo_obj,separator),HX_CSTRING("separator")},
	{hx::fsObject,(int)offsetof(CellInfo_obj,updated),HX_CSTRING("updated")},
	{hx::fsObject,(int)offsetof(CellInfo_obj,conflicted),HX_CSTRING("conflicted")},
	{hx::fsString,(int)offsetof(CellInfo_obj,pvalue),HX_CSTRING("pvalue")},
	{hx::fsString,(int)offsetof(CellInfo_obj,lvalue),HX_CSTRING("lvalue")},
	{hx::fsString,(int)offsetof(CellInfo_obj,rvalue),HX_CSTRING("rvalue")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("value"),
	HX_CSTRING("pretty_value"),
	HX_CSTRING("category"),
	HX_CSTRING("category_given_tr"),
	HX_CSTRING("separator"),
	HX_CSTRING("updated"),
	HX_CSTRING("conflicted"),
	HX_CSTRING("pvalue"),
	HX_CSTRING("lvalue"),
	HX_CSTRING("rvalue"),
	HX_CSTRING("toString"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(CellInfo_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(CellInfo_obj::__mClass,"__mClass");
};

#endif

Class CellInfo_obj::__mClass;

void CellInfo_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.CellInfo"), hx::TCanCast< CellInfo_obj> ,sStaticFields,sMemberFields,
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

void CellInfo_obj::__boot()
{
}

} // end namespace coopy
