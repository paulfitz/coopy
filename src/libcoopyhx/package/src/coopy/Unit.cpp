#include <hxcpp.h>

#ifndef INCLUDED_coopy_Unit
#include <coopy/Unit.h>
#endif
namespace coopy{

Void Unit_obj::__construct(hx::Null< int >  __o_l,hx::Null< int >  __o_r,hx::Null< int >  __o_p)
{
HX_STACK_FRAME("coopy.Unit","new",0xf6095d38,"coopy.Unit.new","coopy/Unit.hx",10,0x3c98a357)

HX_STACK_ARG(__o_l,"l")

HX_STACK_ARG(__o_r,"r")

HX_STACK_ARG(__o_p,"p")
int l = __o_l.Default(-2);
int r = __o_r.Default(-2);
int p = __o_p.Default(-2);
{
	HX_STACK_LINE(11)
	this->l = l;
	HX_STACK_LINE(12)
	this->r = r;
	HX_STACK_LINE(13)
	this->p = p;
}
;
	return null();
}

Unit_obj::~Unit_obj() { }

Dynamic Unit_obj::__CreateEmpty() { return  new Unit_obj; }
hx::ObjectPtr< Unit_obj > Unit_obj::__new(hx::Null< int >  __o_l,hx::Null< int >  __o_r,hx::Null< int >  __o_p)
{  hx::ObjectPtr< Unit_obj > result = new Unit_obj();
	result->__construct(__o_l,__o_r,__o_p);
	return result;}

Dynamic Unit_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Unit_obj > result = new Unit_obj();
	result->__construct(inArgs[0],inArgs[1],inArgs[2]);
	return result;}

int Unit_obj::lp( ){
	HX_STACK_FRAME("coopy.Unit","lp",0x036634ac,"coopy.Unit.lp","coopy/Unit.hx",17,0x3c98a357)
	HX_STACK_THIS(this)
	HX_STACK_LINE(17)
	if (((this->p == (int)-2))){
		HX_STACK_LINE(17)
		return this->l;
	}
	else{
		HX_STACK_LINE(17)
		return this->p;
	}
	HX_STACK_LINE(17)
	return (int)0;
}


HX_DEFINE_DYNAMIC_FUNC0(Unit_obj,lp,return )

::String Unit_obj::toString( ){
	HX_STACK_FRAME("coopy.Unit","toString",0xc633b4d4,"coopy.Unit.toString","coopy/Unit.hx",25,0x3c98a357)
	HX_STACK_THIS(this)
	HX_STACK_LINE(26)
	if (((this->p >= (int)-1))){
		HX_STACK_LINE(26)
		return ((((::coopy::Unit_obj::describe(this->p) + HX_CSTRING("|")) + ::coopy::Unit_obj::describe(this->l)) + HX_CSTRING(":")) + ::coopy::Unit_obj::describe(this->r));
	}
	HX_STACK_LINE(27)
	return ((::coopy::Unit_obj::describe(this->l) + HX_CSTRING(":")) + ::coopy::Unit_obj::describe(this->r));
}


HX_DEFINE_DYNAMIC_FUNC0(Unit_obj,toString,return )

bool Unit_obj::fromString( ::String txt){
	HX_STACK_FRAME("coopy.Unit","fromString",0x88586c03,"coopy.Unit.fromString","coopy/Unit.hx",30,0x3c98a357)
	HX_STACK_THIS(this)
	HX_STACK_ARG(txt,"txt")
	HX_STACK_LINE(31)
	hx::AddEq(txt,HX_CSTRING("]"));
	HX_STACK_LINE(32)
	int at = (int)0;		HX_STACK_VAR(at,"at");
	HX_STACK_LINE(33)
	{
		HX_STACK_LINE(33)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(33)
		int _g = txt.length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(33)
		while(((_g1 < _g))){
			HX_STACK_LINE(33)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(34)
			int ch = txt.charCodeAt(i);		HX_STACK_VAR(ch,"ch");
			HX_STACK_LINE(35)
			if (((bool((ch >= (int)48)) && bool((ch <= (int)57))))){
				HX_STACK_LINE(36)
				hx::MultEq(at,(int)10);
				HX_STACK_LINE(37)
				hx::AddEq(at,(ch - (int)48));
			}
			else{
				HX_STACK_LINE(38)
				if (((ch == (int)45))){
					HX_STACK_LINE(39)
					at = (int)-1;
				}
				else{
					HX_STACK_LINE(40)
					if (((ch == (int)124))){
						HX_STACK_LINE(41)
						this->p = at;
						HX_STACK_LINE(42)
						at = (int)0;
					}
					else{
						HX_STACK_LINE(43)
						if (((ch == (int)58))){
							HX_STACK_LINE(44)
							this->l = at;
							HX_STACK_LINE(45)
							at = (int)0;
						}
						else{
							HX_STACK_LINE(46)
							if (((ch == (int)93))){
								HX_STACK_LINE(47)
								this->r = at;
								HX_STACK_LINE(48)
								return true;
							}
						}
					}
				}
			}
		}
	}
	HX_STACK_LINE(51)
	return false;
}


HX_DEFINE_DYNAMIC_FUNC1(Unit_obj,fromString,return )

::String Unit_obj::describe( int i){
	HX_STACK_FRAME("coopy.Unit","describe",0x94d19433,"coopy.Unit.describe","coopy/Unit.hx",22,0x3c98a357)
	HX_STACK_ARG(i,"i")
	HX_STACK_LINE(22)
	if (((i >= (int)0))){
		HX_STACK_LINE(22)
		return (HX_CSTRING("") + i);
	}
	else{
		HX_STACK_LINE(22)
		return HX_CSTRING("-");
	}
	HX_STACK_LINE(22)
	return null();
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(Unit_obj,describe,return )


Unit_obj::Unit_obj()
{
}

void Unit_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Unit);
	HX_MARK_MEMBER_NAME(l,"l");
	HX_MARK_MEMBER_NAME(r,"r");
	HX_MARK_MEMBER_NAME(p,"p");
	HX_MARK_END_CLASS();
}

void Unit_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(l,"l");
	HX_VISIT_MEMBER_NAME(r,"r");
	HX_VISIT_MEMBER_NAME(p,"p");
}

Dynamic Unit_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"l") ) { return l; }
		if (HX_FIELD_EQ(inName,"r") ) { return r; }
		if (HX_FIELD_EQ(inName,"p") ) { return p; }
		break;
	case 2:
		if (HX_FIELD_EQ(inName,"lp") ) { return lp_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"describe") ) { return describe_dyn(); }
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"fromString") ) { return fromString_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Unit_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"l") ) { l=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"r") ) { r=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"p") ) { p=inValue.Cast< int >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void Unit_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("l"));
	outFields->push(HX_CSTRING("r"));
	outFields->push(HX_CSTRING("p"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("describe"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(Unit_obj,l),HX_CSTRING("l")},
	{hx::fsObject,(int)offsetof(Unit_obj,r),HX_CSTRING("r")},
	{hx::fsObject,(int)offsetof(Unit_obj,p),HX_CSTRING("p")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("l"),
	HX_CSTRING("r"),
	HX_CSTRING("p"),
	HX_CSTRING("lp"),
	HX_CSTRING("toString"),
	HX_CSTRING("fromString"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Unit_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Unit_obj::__mClass,"__mClass");
};

#endif

Class Unit_obj::__mClass;

void Unit_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Unit"), hx::TCanCast< Unit_obj> ,sStaticFields,sMemberFields,
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

void Unit_obj::__boot()
{
}

} // end namespace coopy
