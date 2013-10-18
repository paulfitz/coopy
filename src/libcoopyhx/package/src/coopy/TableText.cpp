#include <hxcpp.h>

#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableText
#include <coopy/TableText.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
namespace coopy{

Void TableText_obj::__construct(::coopy::Table rows)
{
HX_STACK_FRAME("coopy.TableText","new",0xcb503b4b,"coopy.TableText.new","coopy/TableText.hx",10,0x40ec7406)

HX_STACK_ARG(rows,"rows")
{
	HX_STACK_LINE(11)
	this->rows = rows;
	HX_STACK_LINE(12)
	this->view = rows->getCellView();
}
;
	return null();
}

TableText_obj::~TableText_obj() { }

Dynamic TableText_obj::__CreateEmpty() { return  new TableText_obj; }
hx::ObjectPtr< TableText_obj > TableText_obj::__new(::coopy::Table rows)
{  hx::ObjectPtr< TableText_obj > result = new TableText_obj();
	result->__construct(rows);
	return result;}

Dynamic TableText_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< TableText_obj > result = new TableText_obj();
	result->__construct(inArgs[0]);
	return result;}

::String TableText_obj::getCellText( int x,int y){
	HX_STACK_FRAME("coopy.TableText","getCellText",0x02242410,"coopy.TableText.getCellText","coopy/TableText.hx",16,0x40ec7406)
	HX_STACK_THIS(this)
	HX_STACK_ARG(x,"x")
	HX_STACK_ARG(y,"y")
	HX_STACK_LINE(16)
	return this->view->toString(this->rows->getCell(x,y));
}


HX_DEFINE_DYNAMIC_FUNC2(TableText_obj,getCellText,return )


TableText_obj::TableText_obj()
{
}

void TableText_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(TableText);
	HX_MARK_MEMBER_NAME(rows,"rows");
	HX_MARK_MEMBER_NAME(view,"view");
	HX_MARK_END_CLASS();
}

void TableText_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(rows,"rows");
	HX_VISIT_MEMBER_NAME(view,"view");
}

Dynamic TableText_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"rows") ) { return rows; }
		if (HX_FIELD_EQ(inName,"view") ) { return view; }
		break;
	case 11:
		if (HX_FIELD_EQ(inName,"getCellText") ) { return getCellText_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic TableText_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"rows") ) { rows=inValue.Cast< ::coopy::Table >(); return inValue; }
		if (HX_FIELD_EQ(inName,"view") ) { view=inValue.Cast< ::coopy::View >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void TableText_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("rows"));
	outFields->push(HX_CSTRING("view"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(TableText_obj,rows),HX_CSTRING("rows")},
	{hx::fsObject,(int)offsetof(TableText_obj,view),HX_CSTRING("view")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("rows"),
	HX_CSTRING("view"),
	HX_CSTRING("getCellText"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(TableText_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(TableText_obj::__mClass,"__mClass");
};

#endif

Class TableText_obj::__mClass;

void TableText_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.TableText"), hx::TCanCast< TableText_obj> ,sStaticFields,sMemberFields,
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

void TableText_obj::__boot()
{
}

} // end namespace coopy
