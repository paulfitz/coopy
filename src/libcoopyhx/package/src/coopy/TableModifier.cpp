#include <hxcpp.h>

#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableModifier
#include <coopy/TableModifier.h>
#endif
namespace coopy{

Void TableModifier_obj::__construct(::coopy::Table t)
{
HX_STACK_FRAME("coopy.TableModifier","new",0x9db2ecf5,"coopy.TableModifier.new","coopy/TableModifier.hx",10,0xfd587a1c)

HX_STACK_ARG(t,"t")
{
	HX_STACK_LINE(10)
	this->t = t;
}
;
	return null();
}

TableModifier_obj::~TableModifier_obj() { }

Dynamic TableModifier_obj::__CreateEmpty() { return  new TableModifier_obj; }
hx::ObjectPtr< TableModifier_obj > TableModifier_obj::__new(::coopy::Table t)
{  hx::ObjectPtr< TableModifier_obj > result = new TableModifier_obj();
	result->__construct(t);
	return result;}

Dynamic TableModifier_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< TableModifier_obj > result = new TableModifier_obj();
	result->__construct(inArgs[0]);
	return result;}

bool TableModifier_obj::removeColumn( int at){
	HX_STACK_FRAME("coopy.TableModifier","removeColumn",0x84ee1305,"coopy.TableModifier.removeColumn","coopy/TableModifier.hx",13,0xfd587a1c)
	HX_STACK_THIS(this)
	HX_STACK_ARG(at,"at")
	HX_STACK_LINE(14)
	Array< int > fate = Array_obj< int >::__new();		HX_STACK_VAR(fate,"fate");
	HX_STACK_LINE(15)
	{
		HX_STACK_LINE(15)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(15)
		int _g = this->t->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(15)
		while(((_g1 < _g))){
			HX_STACK_LINE(15)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(16)
			if (((i < at))){
				HX_STACK_LINE(17)
				fate->push(i);
			}
			else{
				HX_STACK_LINE(18)
				if (((i > at))){
					HX_STACK_LINE(19)
					fate->push((i - (int)1));
				}
				else{
					HX_STACK_LINE(21)
					fate->push((int)-1);
				}
			}
		}
	}
	HX_STACK_LINE(24)
	return this->t->insertOrDeleteColumns(fate,(this->t->__Field(HX_CSTRING("get_width"),true)() - (int)1));
}


HX_DEFINE_DYNAMIC_FUNC1(TableModifier_obj,removeColumn,return )


TableModifier_obj::TableModifier_obj()
{
}

void TableModifier_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(TableModifier);
	HX_MARK_MEMBER_NAME(t,"t");
	HX_MARK_END_CLASS();
}

void TableModifier_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(t,"t");
}

Dynamic TableModifier_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"t") ) { return t; }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"removeColumn") ) { return removeColumn_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic TableModifier_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"t") ) { t=inValue.Cast< ::coopy::Table >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void TableModifier_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("t"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(TableModifier_obj,t),HX_CSTRING("t")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("t"),
	HX_CSTRING("removeColumn"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(TableModifier_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(TableModifier_obj::__mClass,"__mClass");
};

#endif

Class TableModifier_obj::__mClass;

void TableModifier_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.TableModifier"), hx::TCanCast< TableModifier_obj> ,sStaticFields,sMemberFields,
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

void TableModifier_obj::__boot()
{
}

} // end namespace coopy
