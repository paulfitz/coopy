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
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableView
#include <coopy/TableView.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
#ifndef INCLUDED_haxe_Log
#include <haxe/Log.h>
#endif
namespace coopy{

Void TableView_obj::__construct()
{
HX_STACK_FRAME("coopy.TableView","new",0x0576dd03,"coopy.TableView.new","coopy/TableView.hx",7,0x3ee3714e)
{
}
;
	return null();
}

TableView_obj::~TableView_obj() { }

Dynamic TableView_obj::__CreateEmpty() { return  new TableView_obj; }
hx::ObjectPtr< TableView_obj > TableView_obj::__new()
{  hx::ObjectPtr< TableView_obj > result = new TableView_obj();
	result->__construct();
	return result;}

Dynamic TableView_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< TableView_obj > result = new TableView_obj();
	result->__construct();
	return result;}

hx::Object *TableView_obj::__ToInterface(const hx::type_info &inType) {
	if (inType==typeid( ::coopy::View_obj)) return operator ::coopy::View_obj *();
	return super::__ToInterface(inType);
}

::String TableView_obj::toString( Dynamic d){
	HX_STACK_FRAME("coopy.TableView","toString",0x28ec1529,"coopy.TableView.toString","coopy/TableView.hx",11,0x3ee3714e)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(11)
	return (HX_CSTRING("") + ::Std_obj::string(d));
}


HX_DEFINE_DYNAMIC_FUNC1(TableView_obj,toString,return )

::coopy::Bag TableView_obj::getBag( Dynamic d){
	HX_STACK_FRAME("coopy.TableView","getBag",0x9024260f,"coopy.TableView.getBag","coopy/TableView.hx",15,0x3ee3714e)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(15)
	return null();
}


HX_DEFINE_DYNAMIC_FUNC1(TableView_obj,getBag,return )

::coopy::Table TableView_obj::getTable( Dynamic d){
	HX_STACK_FRAME("coopy.TableView","getTable",0x5b380c15,"coopy.TableView.getTable","coopy/TableView.hx",18,0x3ee3714e)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(19)
	::coopy::Table table = d;		HX_STACK_VAR(table,"table");
	HX_STACK_LINE(20)
	return table;
}


HX_DEFINE_DYNAMIC_FUNC1(TableView_obj,getTable,return )

bool TableView_obj::hasStructure( Dynamic d){
	HX_STACK_FRAME("coopy.TableView","hasStructure",0x80270076,"coopy.TableView.hasStructure","coopy/TableView.hx",24,0x3ee3714e)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(24)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC1(TableView_obj,hasStructure,return )

bool TableView_obj::equals( Dynamic d1,Dynamic d2){
	HX_STACK_FRAME("coopy.TableView","equals",0xacfd297c,"coopy.TableView.equals","coopy/TableView.hx",27,0x3ee3714e)
	HX_STACK_THIS(this)
	HX_STACK_ARG(d1,"d1")
	HX_STACK_ARG(d2,"d2")
	HX_STACK_LINE(28)
	::haxe::Log_obj::trace(HX_CSTRING("TableView.equals called"),hx::SourceInfo(HX_CSTRING("TableView.hx"),28,HX_CSTRING("coopy.TableView"),HX_CSTRING("equals")));
	HX_STACK_LINE(29)
	return false;
}


HX_DEFINE_DYNAMIC_FUNC2(TableView_obj,equals,return )

Dynamic TableView_obj::toDatum( ::String str){
	HX_STACK_FRAME("coopy.TableView","toDatum",0x1cf60717,"coopy.TableView.toDatum","coopy/TableView.hx",33,0x3ee3714e)
	HX_STACK_THIS(this)
	HX_STACK_ARG(str,"str")
	HX_STACK_LINE(33)
	return ::coopy::SimpleCell_obj::__new(str);
}


HX_DEFINE_DYNAMIC_FUNC1(TableView_obj,toDatum,return )


TableView_obj::TableView_obj()
{
}

void TableView_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(TableView);
	HX_MARK_END_CLASS();
}

void TableView_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic TableView_obj::__Field(const ::String &inName,bool inCallProp)
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

Dynamic TableView_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void TableView_obj::__GetFields(Array< ::String> &outFields)
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
	HX_MARK_MEMBER_NAME(TableView_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(TableView_obj::__mClass,"__mClass");
};

#endif

Class TableView_obj::__mClass;

void TableView_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.TableView"), hx::TCanCast< TableView_obj> ,sStaticFields,sMemberFields,
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

void TableView_obj::__boot()
{
}

} // end namespace coopy
