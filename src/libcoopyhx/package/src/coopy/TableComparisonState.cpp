#include <hxcpp.h>

#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableComparisonState
#include <coopy/TableComparisonState.h>
#endif
namespace coopy{

Void TableComparisonState_obj::__construct()
{
HX_STACK_FRAME("coopy.TableComparisonState","new",0x5e7e4b4e,"coopy.TableComparisonState.new","coopy/TableComparisonState.hx",23,0xe177f801)
{
	HX_STACK_LINE(23)
	this->reset();
}
;
	return null();
}

TableComparisonState_obj::~TableComparisonState_obj() { }

Dynamic TableComparisonState_obj::__CreateEmpty() { return  new TableComparisonState_obj; }
hx::ObjectPtr< TableComparisonState_obj > TableComparisonState_obj::__new()
{  hx::ObjectPtr< TableComparisonState_obj > result = new TableComparisonState_obj();
	result->__construct();
	return result;}

Dynamic TableComparisonState_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< TableComparisonState_obj > result = new TableComparisonState_obj();
	result->__construct();
	return result;}

Void TableComparisonState_obj::reset( ){
{
		HX_STACK_FRAME("coopy.TableComparisonState","reset",0x00b9dc3d,"coopy.TableComparisonState.reset","coopy/TableComparisonState.hx",26,0xe177f801)
		HX_STACK_THIS(this)
		HX_STACK_LINE(27)
		this->completed = false;
		HX_STACK_LINE(28)
		this->run_to_completion = true;
		HX_STACK_LINE(29)
		this->is_equal_known = false;
		HX_STACK_LINE(30)
		this->is_equal = false;
		HX_STACK_LINE(31)
		this->has_same_columns = false;
		HX_STACK_LINE(32)
		this->has_same_columns_known = false;
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(TableComparisonState_obj,reset,(void))


TableComparisonState_obj::TableComparisonState_obj()
{
}

void TableComparisonState_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(TableComparisonState);
	HX_MARK_MEMBER_NAME(p,"p");
	HX_MARK_MEMBER_NAME(a,"a");
	HX_MARK_MEMBER_NAME(b,"b");
	HX_MARK_MEMBER_NAME(completed,"completed");
	HX_MARK_MEMBER_NAME(run_to_completion,"run_to_completion");
	HX_MARK_MEMBER_NAME(is_equal,"is_equal");
	HX_MARK_MEMBER_NAME(is_equal_known,"is_equal_known");
	HX_MARK_MEMBER_NAME(has_same_columns,"has_same_columns");
	HX_MARK_MEMBER_NAME(has_same_columns_known,"has_same_columns_known");
	HX_MARK_END_CLASS();
}

void TableComparisonState_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(p,"p");
	HX_VISIT_MEMBER_NAME(a,"a");
	HX_VISIT_MEMBER_NAME(b,"b");
	HX_VISIT_MEMBER_NAME(completed,"completed");
	HX_VISIT_MEMBER_NAME(run_to_completion,"run_to_completion");
	HX_VISIT_MEMBER_NAME(is_equal,"is_equal");
	HX_VISIT_MEMBER_NAME(is_equal_known,"is_equal_known");
	HX_VISIT_MEMBER_NAME(has_same_columns,"has_same_columns");
	HX_VISIT_MEMBER_NAME(has_same_columns_known,"has_same_columns_known");
}

Dynamic TableComparisonState_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"p") ) { return p; }
		if (HX_FIELD_EQ(inName,"a") ) { return a; }
		if (HX_FIELD_EQ(inName,"b") ) { return b; }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"reset") ) { return reset_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"is_equal") ) { return is_equal; }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"completed") ) { return completed; }
		break;
	case 14:
		if (HX_FIELD_EQ(inName,"is_equal_known") ) { return is_equal_known; }
		break;
	case 16:
		if (HX_FIELD_EQ(inName,"has_same_columns") ) { return has_same_columns; }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"run_to_completion") ) { return run_to_completion; }
		break;
	case 22:
		if (HX_FIELD_EQ(inName,"has_same_columns_known") ) { return has_same_columns_known; }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic TableComparisonState_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"p") ) { p=inValue.Cast< ::coopy::Table >(); return inValue; }
		if (HX_FIELD_EQ(inName,"a") ) { a=inValue.Cast< ::coopy::Table >(); return inValue; }
		if (HX_FIELD_EQ(inName,"b") ) { b=inValue.Cast< ::coopy::Table >(); return inValue; }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"is_equal") ) { is_equal=inValue.Cast< bool >(); return inValue; }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"completed") ) { completed=inValue.Cast< bool >(); return inValue; }
		break;
	case 14:
		if (HX_FIELD_EQ(inName,"is_equal_known") ) { is_equal_known=inValue.Cast< bool >(); return inValue; }
		break;
	case 16:
		if (HX_FIELD_EQ(inName,"has_same_columns") ) { has_same_columns=inValue.Cast< bool >(); return inValue; }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"run_to_completion") ) { run_to_completion=inValue.Cast< bool >(); return inValue; }
		break;
	case 22:
		if (HX_FIELD_EQ(inName,"has_same_columns_known") ) { has_same_columns_known=inValue.Cast< bool >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void TableComparisonState_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("p"));
	outFields->push(HX_CSTRING("a"));
	outFields->push(HX_CSTRING("b"));
	outFields->push(HX_CSTRING("completed"));
	outFields->push(HX_CSTRING("run_to_completion"));
	outFields->push(HX_CSTRING("is_equal"));
	outFields->push(HX_CSTRING("is_equal_known"));
	outFields->push(HX_CSTRING("has_same_columns"));
	outFields->push(HX_CSTRING("has_same_columns_known"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,p),HX_CSTRING("p")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,a),HX_CSTRING("a")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,b),HX_CSTRING("b")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,completed),HX_CSTRING("completed")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,run_to_completion),HX_CSTRING("run_to_completion")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,is_equal),HX_CSTRING("is_equal")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,is_equal_known),HX_CSTRING("is_equal_known")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,has_same_columns),HX_CSTRING("has_same_columns")},
	{hx::fsObject,(int)offsetof(TableComparisonState_obj,has_same_columns_known),HX_CSTRING("has_same_columns_known")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("p"),
	HX_CSTRING("a"),
	HX_CSTRING("b"),
	HX_CSTRING("completed"),
	HX_CSTRING("run_to_completion"),
	HX_CSTRING("is_equal"),
	HX_CSTRING("is_equal_known"),
	HX_CSTRING("has_same_columns"),
	HX_CSTRING("has_same_columns_known"),
	HX_CSTRING("reset"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(TableComparisonState_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(TableComparisonState_obj::__mClass,"__mClass");
};

#endif

Class TableComparisonState_obj::__mClass;

void TableComparisonState_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.TableComparisonState"), hx::TCanCast< TableComparisonState_obj> ,sStaticFields,sMemberFields,
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

void TableComparisonState_obj::__boot()
{
}

} // end namespace coopy
