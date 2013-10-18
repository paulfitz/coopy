#include <hxcpp.h>

#ifndef INCLUDED_IMap
#include <IMap.h>
#endif
#ifndef INCLUDED_Std
#include <Std.h>
#endif
#ifndef INCLUDED_StringBuf
#include <StringBuf.h>
#endif
#ifndef INCLUDED_haxe_ds_IntMap
#include <haxe/ds/IntMap.h>
#endif
namespace haxe{
namespace ds{

Void IntMap_obj::__construct()
{
HX_STACK_FRAME("haxe.ds.IntMap","new",0x7222c4b6,"haxe.ds.IntMap.new","/usr/lib/haxe/std/cpp/_std/haxe/ds/IntMap.hx",29,0x0f19a47c)
{
	HX_STACK_LINE(29)
	this->h = ::__int_hash_create();
}
;
	return null();
}

IntMap_obj::~IntMap_obj() { }

Dynamic IntMap_obj::__CreateEmpty() { return  new IntMap_obj; }
hx::ObjectPtr< IntMap_obj > IntMap_obj::__new()
{  hx::ObjectPtr< IntMap_obj > result = new IntMap_obj();
	result->__construct();
	return result;}

Dynamic IntMap_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< IntMap_obj > result = new IntMap_obj();
	result->__construct();
	return result;}

hx::Object *IntMap_obj::__ToInterface(const hx::type_info &inType) {
	if (inType==typeid( ::IMap_obj)) return operator ::IMap_obj *();
	return super::__ToInterface(inType);
}

Void IntMap_obj::set( int key,Dynamic value){
{
		HX_STACK_FRAME("haxe.ds.IntMap","set",0x72268ff8,"haxe.ds.IntMap.set","/usr/lib/haxe/std/cpp/_std/haxe/ds/IntMap.hx",33,0x0f19a47c)
		HX_STACK_THIS(this)
		HX_STACK_ARG(key,"key")
		HX_STACK_ARG(value,"value")
		HX_STACK_LINE(33)
		::__int_hash_set(this->h,key,value);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC2(IntMap_obj,set,(void))

Dynamic IntMap_obj::get( int key){
	HX_STACK_FRAME("haxe.ds.IntMap","get",0x721d74ec,"haxe.ds.IntMap.get","/usr/lib/haxe/std/cpp/_std/haxe/ds/IntMap.hx",37,0x0f19a47c)
	HX_STACK_THIS(this)
	HX_STACK_ARG(key,"key")
	HX_STACK_LINE(37)
	return ::__int_hash_get(this->h,key);
}


HX_DEFINE_DYNAMIC_FUNC1(IntMap_obj,get,return )

bool IntMap_obj::exists( int key){
	HX_STACK_FRAME("haxe.ds.IntMap","exists",0x63ba0346,"haxe.ds.IntMap.exists","/usr/lib/haxe/std/cpp/_std/haxe/ds/IntMap.hx",41,0x0f19a47c)
	HX_STACK_THIS(this)
	HX_STACK_ARG(key,"key")
	HX_STACK_LINE(41)
	return ::__int_hash_exists(this->h,key);
}


HX_DEFINE_DYNAMIC_FUNC1(IntMap_obj,exists,return )

bool IntMap_obj::remove( int key){
	HX_STACK_FRAME("haxe.ds.IntMap","remove",0xa86281ae,"haxe.ds.IntMap.remove","/usr/lib/haxe/std/cpp/_std/haxe/ds/IntMap.hx",45,0x0f19a47c)
	HX_STACK_THIS(this)
	HX_STACK_ARG(key,"key")
	HX_STACK_LINE(45)
	return ::__int_hash_remove(this->h,key);
}


HX_DEFINE_DYNAMIC_FUNC1(IntMap_obj,remove,return )

Dynamic IntMap_obj::keys( ){
	HX_STACK_FRAME("haxe.ds.IntMap","keys",0x6a4db8de,"haxe.ds.IntMap.keys","/usr/lib/haxe/std/cpp/_std/haxe/ds/IntMap.hx",48,0x0f19a47c)
	HX_STACK_THIS(this)
	HX_STACK_LINE(49)
	Array< int > a = ::__int_hash_keys(this->h);		HX_STACK_VAR(a,"a");
	HX_STACK_LINE(50)
	return a->iterator();
}


HX_DEFINE_DYNAMIC_FUNC0(IntMap_obj,keys,return )

::String IntMap_obj::toString( ){
	HX_STACK_FRAME("haxe.ds.IntMap","toString",0xf29c6496,"haxe.ds.IntMap.toString","/usr/lib/haxe/std/cpp/_std/haxe/ds/IntMap.hx",58,0x0f19a47c)
	HX_STACK_THIS(this)
	HX_STACK_LINE(59)
	::StringBuf s = ::StringBuf_obj::__new();		HX_STACK_VAR(s,"s");
	HX_STACK_LINE(60)
	s->add(HX_CSTRING("{"));
	HX_STACK_LINE(61)
	Dynamic it = this->keys();		HX_STACK_VAR(it,"it");
	HX_STACK_LINE(62)
	for(::cpp::FastIterator_obj< int > *__it = ::cpp::CreateFastIterator< int >(it);  __it->hasNext(); ){
		int i = __it->next();
		{
			HX_STACK_LINE(63)
			s->add(i);
			HX_STACK_LINE(64)
			s->add(HX_CSTRING(" => "));
			HX_STACK_LINE(65)
			s->add(::Std_obj::string(this->get(i)));
			HX_STACK_LINE(66)
			if ((it->__Field(HX_CSTRING("hasNext"),true)())){
				HX_STACK_LINE(67)
				s->add(HX_CSTRING(", "));
			}
		}
;
	}
	HX_STACK_LINE(69)
	s->add(HX_CSTRING("}"));
	HX_STACK_LINE(70)
	return s->b->join(HX_CSTRING(""));
}


HX_DEFINE_DYNAMIC_FUNC0(IntMap_obj,toString,return )


IntMap_obj::IntMap_obj()
{
}

void IntMap_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(IntMap);
	HX_MARK_MEMBER_NAME(h,"h");
	HX_MARK_END_CLASS();
}

void IntMap_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(h,"h");
}

Dynamic IntMap_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"h") ) { return h; }
		break;
	case 3:
		if (HX_FIELD_EQ(inName,"set") ) { return set_dyn(); }
		if (HX_FIELD_EQ(inName,"get") ) { return get_dyn(); }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"keys") ) { return keys_dyn(); }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"exists") ) { return exists_dyn(); }
		if (HX_FIELD_EQ(inName,"remove") ) { return remove_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic IntMap_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"h") ) { h=inValue.Cast< Dynamic >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void IntMap_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("h"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(IntMap_obj,h),HX_CSTRING("h")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("h"),
	HX_CSTRING("set"),
	HX_CSTRING("get"),
	HX_CSTRING("exists"),
	HX_CSTRING("remove"),
	HX_CSTRING("keys"),
	HX_CSTRING("toString"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(IntMap_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(IntMap_obj::__mClass,"__mClass");
};

#endif

Class IntMap_obj::__mClass;

void IntMap_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("haxe.ds.IntMap"), hx::TCanCast< IntMap_obj> ,sStaticFields,sMemberFields,
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

void IntMap_obj::__boot()
{
}

} // end namespace haxe
} // end namespace ds
