#include <hxcpp.h>

#ifndef INCLUDED_coopy_HighlightPatchUnit
#include <coopy/HighlightPatchUnit.h>
#endif
namespace coopy{

Void HighlightPatchUnit_obj::__construct()
{
HX_STACK_FRAME("coopy.HighlightPatchUnit","new",0x73c0a0cc,"coopy.HighlightPatchUnit.new","coopy/HighlightPatchUnit.hx",19,0xaafec683)
{
	HX_STACK_LINE(20)
	this->add = false;
	HX_STACK_LINE(21)
	this->rem = false;
	HX_STACK_LINE(22)
	this->update = false;
	HX_STACK_LINE(23)
	this->sourceRow = (int)-1;
	HX_STACK_LINE(24)
	this->sourceRowOffset = (int)0;
	HX_STACK_LINE(25)
	this->sourcePrevRow = (int)-1;
	HX_STACK_LINE(26)
	this->sourceNextRow = (int)-1;
	HX_STACK_LINE(27)
	this->destRow = (int)-1;
	HX_STACK_LINE(28)
	this->patchRow = (int)-1;
	HX_STACK_LINE(29)
	this->code = HX_CSTRING("");
}
;
	return null();
}

HighlightPatchUnit_obj::~HighlightPatchUnit_obj() { }

Dynamic HighlightPatchUnit_obj::__CreateEmpty() { return  new HighlightPatchUnit_obj; }
hx::ObjectPtr< HighlightPatchUnit_obj > HighlightPatchUnit_obj::__new()
{  hx::ObjectPtr< HighlightPatchUnit_obj > result = new HighlightPatchUnit_obj();
	result->__construct();
	return result;}

Dynamic HighlightPatchUnit_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< HighlightPatchUnit_obj > result = new HighlightPatchUnit_obj();
	result->__construct();
	return result;}

::String HighlightPatchUnit_obj::toString( ){
	HX_STACK_FRAME("coopy.HighlightPatchUnit","toString",0x471af8c0,"coopy.HighlightPatchUnit.toString","coopy/HighlightPatchUnit.hx",33,0xaafec683)
	HX_STACK_THIS(this)
	HX_STACK_LINE(33)
	return ((((((((((this->code + HX_CSTRING(" patchRow ")) + this->patchRow) + HX_CSTRING(" sourceRows ")) + this->sourcePrevRow) + HX_CSTRING(",")) + this->sourceRow) + HX_CSTRING(",")) + this->sourceNextRow) + HX_CSTRING(" destRow ")) + this->destRow);
}


HX_DEFINE_DYNAMIC_FUNC0(HighlightPatchUnit_obj,toString,return )


HighlightPatchUnit_obj::HighlightPatchUnit_obj()
{
}

void HighlightPatchUnit_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(HighlightPatchUnit);
	HX_MARK_MEMBER_NAME(add,"add");
	HX_MARK_MEMBER_NAME(rem,"rem");
	HX_MARK_MEMBER_NAME(update,"update");
	HX_MARK_MEMBER_NAME(code,"code");
	HX_MARK_MEMBER_NAME(sourceRow,"sourceRow");
	HX_MARK_MEMBER_NAME(sourceRowOffset,"sourceRowOffset");
	HX_MARK_MEMBER_NAME(sourcePrevRow,"sourcePrevRow");
	HX_MARK_MEMBER_NAME(sourceNextRow,"sourceNextRow");
	HX_MARK_MEMBER_NAME(destRow,"destRow");
	HX_MARK_MEMBER_NAME(patchRow,"patchRow");
	HX_MARK_END_CLASS();
}

void HighlightPatchUnit_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(add,"add");
	HX_VISIT_MEMBER_NAME(rem,"rem");
	HX_VISIT_MEMBER_NAME(update,"update");
	HX_VISIT_MEMBER_NAME(code,"code");
	HX_VISIT_MEMBER_NAME(sourceRow,"sourceRow");
	HX_VISIT_MEMBER_NAME(sourceRowOffset,"sourceRowOffset");
	HX_VISIT_MEMBER_NAME(sourcePrevRow,"sourcePrevRow");
	HX_VISIT_MEMBER_NAME(sourceNextRow,"sourceNextRow");
	HX_VISIT_MEMBER_NAME(destRow,"destRow");
	HX_VISIT_MEMBER_NAME(patchRow,"patchRow");
}

Dynamic HighlightPatchUnit_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 3:
		if (HX_FIELD_EQ(inName,"add") ) { return add; }
		if (HX_FIELD_EQ(inName,"rem") ) { return rem; }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"code") ) { return code; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"update") ) { return update; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"destRow") ) { return destRow; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"patchRow") ) { return patchRow; }
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"sourceRow") ) { return sourceRow; }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"sourcePrevRow") ) { return sourcePrevRow; }
		if (HX_FIELD_EQ(inName,"sourceNextRow") ) { return sourceNextRow; }
		break;
	case 15:
		if (HX_FIELD_EQ(inName,"sourceRowOffset") ) { return sourceRowOffset; }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic HighlightPatchUnit_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 3:
		if (HX_FIELD_EQ(inName,"add") ) { add=inValue.Cast< bool >(); return inValue; }
		if (HX_FIELD_EQ(inName,"rem") ) { rem=inValue.Cast< bool >(); return inValue; }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"code") ) { code=inValue.Cast< ::String >(); return inValue; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"update") ) { update=inValue.Cast< bool >(); return inValue; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"destRow") ) { destRow=inValue.Cast< int >(); return inValue; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"patchRow") ) { patchRow=inValue.Cast< int >(); return inValue; }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"sourceRow") ) { sourceRow=inValue.Cast< int >(); return inValue; }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"sourcePrevRow") ) { sourcePrevRow=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"sourceNextRow") ) { sourceNextRow=inValue.Cast< int >(); return inValue; }
		break;
	case 15:
		if (HX_FIELD_EQ(inName,"sourceRowOffset") ) { sourceRowOffset=inValue.Cast< int >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void HighlightPatchUnit_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("add"));
	outFields->push(HX_CSTRING("rem"));
	outFields->push(HX_CSTRING("update"));
	outFields->push(HX_CSTRING("code"));
	outFields->push(HX_CSTRING("sourceRow"));
	outFields->push(HX_CSTRING("sourceRowOffset"));
	outFields->push(HX_CSTRING("sourcePrevRow"));
	outFields->push(HX_CSTRING("sourceNextRow"));
	outFields->push(HX_CSTRING("destRow"));
	outFields->push(HX_CSTRING("patchRow"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,add),HX_CSTRING("add")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,rem),HX_CSTRING("rem")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,update),HX_CSTRING("update")},
	{hx::fsString,(int)offsetof(HighlightPatchUnit_obj,code),HX_CSTRING("code")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,sourceRow),HX_CSTRING("sourceRow")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,sourceRowOffset),HX_CSTRING("sourceRowOffset")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,sourcePrevRow),HX_CSTRING("sourcePrevRow")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,sourceNextRow),HX_CSTRING("sourceNextRow")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,destRow),HX_CSTRING("destRow")},
	{hx::fsObject,(int)offsetof(HighlightPatchUnit_obj,patchRow),HX_CSTRING("patchRow")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("add"),
	HX_CSTRING("rem"),
	HX_CSTRING("update"),
	HX_CSTRING("code"),
	HX_CSTRING("sourceRow"),
	HX_CSTRING("sourceRowOffset"),
	HX_CSTRING("sourcePrevRow"),
	HX_CSTRING("sourceNextRow"),
	HX_CSTRING("destRow"),
	HX_CSTRING("patchRow"),
	HX_CSTRING("toString"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(HighlightPatchUnit_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(HighlightPatchUnit_obj::__mClass,"__mClass");
};

#endif

Class HighlightPatchUnit_obj::__mClass;

void HighlightPatchUnit_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.HighlightPatchUnit"), hx::TCanCast< HighlightPatchUnit_obj> ,sStaticFields,sMemberFields,
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

void HighlightPatchUnit_obj::__boot()
{
}

} // end namespace coopy
