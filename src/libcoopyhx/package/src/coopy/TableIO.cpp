#include <hxcpp.h>

#ifndef INCLUDED_coopy_TableIO
#include <coopy/TableIO.h>
#endif
namespace coopy{

Void TableIO_obj::__construct()
{
HX_STACK_FRAME("coopy.TableIO","new",0xc9c967e4,"coopy.TableIO.new","coopy/TableIO.hx",7,0x5f558b4d)
{
}
;
	return null();
}

TableIO_obj::~TableIO_obj() { }

Dynamic TableIO_obj::__CreateEmpty() { return  new TableIO_obj; }
hx::ObjectPtr< TableIO_obj > TableIO_obj::__new()
{  hx::ObjectPtr< TableIO_obj > result = new TableIO_obj();
	result->__construct();
	return result;}

Dynamic TableIO_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< TableIO_obj > result = new TableIO_obj();
	result->__construct();
	return result;}

::String TableIO_obj::getContent( ::String name){
	HX_STACK_FRAME("coopy.TableIO","getContent",0x2757343f,"coopy.TableIO.getContent","coopy/TableIO.hx",14,0x5f558b4d)
	HX_STACK_THIS(this)
	HX_STACK_ARG(name,"name")
	HX_STACK_LINE(14)
	return HX_CSTRING("");
}


HX_DEFINE_DYNAMIC_FUNC1(TableIO_obj,getContent,return )

bool TableIO_obj::saveContent( ::String name,::String txt){
	HX_STACK_FRAME("coopy.TableIO","saveContent",0x62f67700,"coopy.TableIO.saveContent","coopy/TableIO.hx",23,0x5f558b4d)
	HX_STACK_THIS(this)
	HX_STACK_ARG(name,"name")
	HX_STACK_ARG(txt,"txt")
	HX_STACK_LINE(23)
	return false;
}


HX_DEFINE_DYNAMIC_FUNC2(TableIO_obj,saveContent,return )

Array< ::String > TableIO_obj::args( ){
	HX_STACK_FRAME("coopy.TableIO","args",0xbde38959,"coopy.TableIO.args","coopy/TableIO.hx",31,0x5f558b4d)
	HX_STACK_THIS(this)
	HX_STACK_LINE(31)
	return Array_obj< ::String >::__new();
}


HX_DEFINE_DYNAMIC_FUNC0(TableIO_obj,args,return )

Void TableIO_obj::writeStdout( ::String txt){
{
		HX_STACK_FRAME("coopy.TableIO","writeStdout",0x8ccbcd8e,"coopy.TableIO.writeStdout","coopy/TableIO.hx",35,0x5f558b4d)
		HX_STACK_THIS(this)
		HX_STACK_ARG(txt,"txt")
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(TableIO_obj,writeStdout,(void))

Void TableIO_obj::writeStderr( ::String txt){
{
		HX_STACK_FRAME("coopy.TableIO","writeStderr",0x8cc43465,"coopy.TableIO.writeStderr","coopy/TableIO.hx",41,0x5f558b4d)
		HX_STACK_THIS(this)
		HX_STACK_ARG(txt,"txt")
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(TableIO_obj,writeStderr,(void))


TableIO_obj::TableIO_obj()
{
}

void TableIO_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(TableIO);
	HX_MARK_END_CLASS();
}

void TableIO_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic TableIO_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"args") ) { return args_dyn(); }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"getContent") ) { return getContent_dyn(); }
		break;
	case 11:
		if (HX_FIELD_EQ(inName,"saveContent") ) { return saveContent_dyn(); }
		if (HX_FIELD_EQ(inName,"writeStdout") ) { return writeStdout_dyn(); }
		if (HX_FIELD_EQ(inName,"writeStderr") ) { return writeStderr_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic TableIO_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void TableIO_obj::__GetFields(Array< ::String> &outFields)
{
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo *sMemberStorageInfo = 0;
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("getContent"),
	HX_CSTRING("saveContent"),
	HX_CSTRING("args"),
	HX_CSTRING("writeStdout"),
	HX_CSTRING("writeStderr"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(TableIO_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(TableIO_obj::__mClass,"__mClass");
};

#endif

Class TableIO_obj::__mClass;

void TableIO_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.TableIO"), hx::TCanCast< TableIO_obj> ,sStaticFields,sMemberFields,
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

void TableIO_obj::__boot()
{
}

} // end namespace coopy
