#include <hxcpp.h>

#ifndef INCLUDED_IMap
#include <IMap.h>
#endif
#ifndef INCLUDED_coopy_SparseSheet
#include <coopy/SparseSheet.h>
#endif
#ifndef INCLUDED_haxe_ds_IntMap
#include <haxe/ds/IntMap.h>
#endif
namespace coopy{

Void SparseSheet_obj::__construct()
{
HX_STACK_FRAME("coopy.SparseSheet","new",0xcb16b08f,"coopy.SparseSheet.new","coopy/SparseSheet.hx",12,0x5da47582)
{
	HX_STACK_LINE(12)
	this->h = this->w = (int)0;
}
;
	return null();
}

SparseSheet_obj::~SparseSheet_obj() { }

Dynamic SparseSheet_obj::__CreateEmpty() { return  new SparseSheet_obj; }
hx::ObjectPtr< SparseSheet_obj > SparseSheet_obj::__new()
{  hx::ObjectPtr< SparseSheet_obj > result = new SparseSheet_obj();
	result->__construct();
	return result;}

Dynamic SparseSheet_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< SparseSheet_obj > result = new SparseSheet_obj();
	result->__construct();
	return result;}

Void SparseSheet_obj::resize( int w,int h,Dynamic zero){
{
		HX_STACK_FRAME("coopy.SparseSheet","resize",0x11d58125,"coopy.SparseSheet.resize","coopy/SparseSheet.hx",15,0x5da47582)
		HX_STACK_THIS(this)
		HX_STACK_ARG(w,"w")
		HX_STACK_ARG(h,"h")
		HX_STACK_ARG(zero,"zero")
		HX_STACK_LINE(16)
		this->row = ::haxe::ds::IntMap_obj::__new();
		HX_STACK_LINE(17)
		this->nonDestructiveResize(w,h,zero);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC3(SparseSheet_obj,resize,(void))

Void SparseSheet_obj::nonDestructiveResize( int w,int h,Dynamic zero){
{
		HX_STACK_FRAME("coopy.SparseSheet","nonDestructiveResize",0x00cecbba,"coopy.SparseSheet.nonDestructiveResize","coopy/SparseSheet.hx",20,0x5da47582)
		HX_STACK_THIS(this)
		HX_STACK_ARG(w,"w")
		HX_STACK_ARG(h,"h")
		HX_STACK_ARG(zero,"zero")
		HX_STACK_LINE(21)
		this->w = w;
		HX_STACK_LINE(22)
		this->h = h;
		HX_STACK_LINE(23)
		this->zero = zero;
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC3(SparseSheet_obj,nonDestructiveResize,(void))

Dynamic SparseSheet_obj::get( int x,int y){
	HX_STACK_FRAME("coopy.SparseSheet","get",0xcb1160c5,"coopy.SparseSheet.get","coopy/SparseSheet.hx",26,0x5da47582)
	HX_STACK_THIS(this)
	HX_STACK_ARG(x,"x")
	HX_STACK_ARG(y,"y")
	HX_STACK_LINE(27)
	::haxe::ds::IntMap cursor = this->row->get(y);		HX_STACK_VAR(cursor,"cursor");
	HX_STACK_LINE(28)
	if (((cursor == null()))){
		HX_STACK_LINE(28)
		return this->zero;
	}
	HX_STACK_LINE(29)
	Dynamic val = cursor->get(x);		HX_STACK_VAR(val,"val");
	HX_STACK_LINE(30)
	if (((val == null()))){
		HX_STACK_LINE(30)
		return this->zero;
	}
	HX_STACK_LINE(31)
	return val;
}


HX_DEFINE_DYNAMIC_FUNC2(SparseSheet_obj,get,return )

Void SparseSheet_obj::set( int x,int y,Dynamic val){
{
		HX_STACK_FRAME("coopy.SparseSheet","set",0xcb1a7bd1,"coopy.SparseSheet.set","coopy/SparseSheet.hx",34,0x5da47582)
		HX_STACK_THIS(this)
		HX_STACK_ARG(x,"x")
		HX_STACK_ARG(y,"y")
		HX_STACK_ARG(val,"val")
		HX_STACK_LINE(35)
		::haxe::ds::IntMap cursor = this->row->get(y);		HX_STACK_VAR(cursor,"cursor");
		HX_STACK_LINE(36)
		if (((cursor == null()))){
			HX_STACK_LINE(37)
			cursor = ::haxe::ds::IntMap_obj::__new();
			HX_STACK_LINE(38)
			this->row->set(y,cursor);
		}
		HX_STACK_LINE(40)
		cursor->set(x,val);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC3(SparseSheet_obj,set,(void))


SparseSheet_obj::SparseSheet_obj()
{
}

void SparseSheet_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(SparseSheet);
	HX_MARK_MEMBER_NAME(h,"h");
	HX_MARK_MEMBER_NAME(w,"w");
	HX_MARK_MEMBER_NAME(row,"row");
	HX_MARK_MEMBER_NAME(zero,"zero");
	HX_MARK_END_CLASS();
}

void SparseSheet_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(h,"h");
	HX_VISIT_MEMBER_NAME(w,"w");
	HX_VISIT_MEMBER_NAME(row,"row");
	HX_VISIT_MEMBER_NAME(zero,"zero");
}

Dynamic SparseSheet_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"h") ) { return h; }
		if (HX_FIELD_EQ(inName,"w") ) { return w; }
		break;
	case 3:
		if (HX_FIELD_EQ(inName,"row") ) { return row; }
		if (HX_FIELD_EQ(inName,"get") ) { return get_dyn(); }
		if (HX_FIELD_EQ(inName,"set") ) { return set_dyn(); }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"zero") ) { return zero; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"resize") ) { return resize_dyn(); }
		break;
	case 20:
		if (HX_FIELD_EQ(inName,"nonDestructiveResize") ) { return nonDestructiveResize_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic SparseSheet_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"h") ) { h=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"w") ) { w=inValue.Cast< int >(); return inValue; }
		break;
	case 3:
		if (HX_FIELD_EQ(inName,"row") ) { row=inValue.Cast< ::haxe::ds::IntMap >(); return inValue; }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"zero") ) { zero=inValue.Cast< Dynamic >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void SparseSheet_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("h"));
	outFields->push(HX_CSTRING("w"));
	outFields->push(HX_CSTRING("row"));
	outFields->push(HX_CSTRING("zero"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(SparseSheet_obj,h),HX_CSTRING("h")},
	{hx::fsObject,(int)offsetof(SparseSheet_obj,w),HX_CSTRING("w")},
	{hx::fsObject,(int)offsetof(SparseSheet_obj,row),HX_CSTRING("row")},
	{hx::fsObject,(int)offsetof(SparseSheet_obj,zero),HX_CSTRING("zero")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("h"),
	HX_CSTRING("w"),
	HX_CSTRING("row"),
	HX_CSTRING("zero"),
	HX_CSTRING("resize"),
	HX_CSTRING("nonDestructiveResize"),
	HX_CSTRING("get"),
	HX_CSTRING("set"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(SparseSheet_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(SparseSheet_obj::__mClass,"__mClass");
};

#endif

Class SparseSheet_obj::__mClass;

void SparseSheet_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.SparseSheet"), hx::TCanCast< SparseSheet_obj> ,sStaticFields,sMemberFields,
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

void SparseSheet_obj::__boot()
{
}

} // end namespace coopy
