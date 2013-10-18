#include <hxcpp.h>

#ifndef INCLUDED_Std
#include <Std.h>
#endif
#ifndef INCLUDED_coopy_Ordering
#include <coopy/Ordering.h>
#endif
#ifndef INCLUDED_coopy_Unit
#include <coopy/Unit.h>
#endif
namespace coopy{

Void Ordering_obj::__construct()
{
HX_STACK_FRAME("coopy.Ordering","new",0xef538cc8,"coopy.Ordering.new","coopy/Ordering.hx",9,0x488f9147)
{
	HX_STACK_LINE(10)
	this->order = Array_obj< ::Dynamic >::__new();
	HX_STACK_LINE(11)
	this->ignore_parent = false;
}
;
	return null();
}

Ordering_obj::~Ordering_obj() { }

Dynamic Ordering_obj::__CreateEmpty() { return  new Ordering_obj; }
hx::ObjectPtr< Ordering_obj > Ordering_obj::__new()
{  hx::ObjectPtr< Ordering_obj > result = new Ordering_obj();
	result->__construct();
	return result;}

Dynamic Ordering_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Ordering_obj > result = new Ordering_obj();
	result->__construct();
	return result;}

Void Ordering_obj::add( int l,int r,hx::Null< int >  __o_p){
int p = __o_p.Default(-2);
	HX_STACK_FRAME("coopy.Ordering","add",0xef49ae89,"coopy.Ordering.add","coopy/Ordering.hx",14,0x488f9147)
	HX_STACK_THIS(this)
	HX_STACK_ARG(l,"l")
	HX_STACK_ARG(r,"r")
	HX_STACK_ARG(p,"p")
{
		HX_STACK_LINE(15)
		if ((this->ignore_parent)){
			HX_STACK_LINE(15)
			p = (int)-2;
		}
		HX_STACK_LINE(16)
		this->order->push(::coopy::Unit_obj::__new(l,r,p));
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC3(Ordering_obj,add,(void))

Array< ::Dynamic > Ordering_obj::getList( ){
	HX_STACK_FRAME("coopy.Ordering","getList",0x2b6fdc7c,"coopy.Ordering.getList","coopy/Ordering.hx",20,0x488f9147)
	HX_STACK_THIS(this)
	HX_STACK_LINE(20)
	return this->order;
}


HX_DEFINE_DYNAMIC_FUNC0(Ordering_obj,getList,return )

::String Ordering_obj::toString( ){
	HX_STACK_FRAME("coopy.Ordering","toString",0x3a031b44,"coopy.Ordering.toString","coopy/Ordering.hx",23,0x488f9147)
	HX_STACK_THIS(this)
	HX_STACK_LINE(24)
	::String txt = HX_CSTRING("");		HX_STACK_VAR(txt,"txt");
	HX_STACK_LINE(25)
	{
		HX_STACK_LINE(25)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(25)
		int _g = this->order->length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(25)
		while(((_g1 < _g))){
			HX_STACK_LINE(25)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(26)
			if (((i > (int)0))){
				HX_STACK_LINE(26)
				hx::AddEq(txt,HX_CSTRING(", "));
			}
			HX_STACK_LINE(27)
			hx::AddEq(txt,::Std_obj::string(this->order->__get(i).StaticCast< ::coopy::Unit >()));
		}
	}
	HX_STACK_LINE(29)
	return txt;
}


HX_DEFINE_DYNAMIC_FUNC0(Ordering_obj,toString,return )

Void Ordering_obj::ignoreParent( ){
{
		HX_STACK_FRAME("coopy.Ordering","ignoreParent",0x7197c274,"coopy.Ordering.ignoreParent","coopy/Ordering.hx",33,0x488f9147)
		HX_STACK_THIS(this)
		HX_STACK_LINE(33)
		this->ignore_parent = true;
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(Ordering_obj,ignoreParent,(void))


Ordering_obj::Ordering_obj()
{
}

void Ordering_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Ordering);
	HX_MARK_MEMBER_NAME(order,"order");
	HX_MARK_MEMBER_NAME(ignore_parent,"ignore_parent");
	HX_MARK_END_CLASS();
}

void Ordering_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(order,"order");
	HX_VISIT_MEMBER_NAME(ignore_parent,"ignore_parent");
}

Dynamic Ordering_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 3:
		if (HX_FIELD_EQ(inName,"add") ) { return add_dyn(); }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"order") ) { return order; }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"getList") ) { return getList_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"ignoreParent") ) { return ignoreParent_dyn(); }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"ignore_parent") ) { return ignore_parent; }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Ordering_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 5:
		if (HX_FIELD_EQ(inName,"order") ) { order=inValue.Cast< Array< ::Dynamic > >(); return inValue; }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"ignore_parent") ) { ignore_parent=inValue.Cast< bool >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void Ordering_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("order"));
	outFields->push(HX_CSTRING("ignore_parent"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(Ordering_obj,order),HX_CSTRING("order")},
	{hx::fsObject,(int)offsetof(Ordering_obj,ignore_parent),HX_CSTRING("ignore_parent")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("order"),
	HX_CSTRING("ignore_parent"),
	HX_CSTRING("add"),
	HX_CSTRING("getList"),
	HX_CSTRING("toString"),
	HX_CSTRING("ignoreParent"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Ordering_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Ordering_obj::__mClass,"__mClass");
};

#endif

Class Ordering_obj::__mClass;

void Ordering_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Ordering"), hx::TCanCast< Ordering_obj> ,sStaticFields,sMemberFields,
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

void Ordering_obj::__boot()
{
}

} // end namespace coopy
