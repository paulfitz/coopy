#include <hxcpp.h>

#ifndef INCLUDED_IMap
#include <IMap.h>
#endif
#ifndef INCLUDED_Std
#include <Std.h>
#endif
#ifndef INCLUDED_coopy_SimpleTable
#include <coopy/SimpleTable.h>
#endif
#ifndef INCLUDED_coopy_SimpleView
#include <coopy/SimpleView.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
#ifndef INCLUDED_haxe_ds_IntMap
#include <haxe/ds/IntMap.h>
#endif
namespace coopy{

Void SimpleTable_obj::__construct(int w,int h)
{
HX_STACK_FRAME("coopy.SimpleTable","new",0xa31839ec,"coopy.SimpleTable.new","coopy/SimpleTable.hx",11,0x278206c5)

HX_STACK_ARG(w,"w")

HX_STACK_ARG(h,"h")
{
	HX_STACK_LINE(12)
	this->data = ::haxe::ds::IntMap_obj::__new();
	HX_STACK_LINE(13)
	this->w = w;
	HX_STACK_LINE(14)
	this->h = h;
}
;
	return null();
}

SimpleTable_obj::~SimpleTable_obj() { }

Dynamic SimpleTable_obj::__CreateEmpty() { return  new SimpleTable_obj; }
hx::ObjectPtr< SimpleTable_obj > SimpleTable_obj::__new(int w,int h)
{  hx::ObjectPtr< SimpleTable_obj > result = new SimpleTable_obj();
	result->__construct(w,h);
	return result;}

Dynamic SimpleTable_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< SimpleTable_obj > result = new SimpleTable_obj();
	result->__construct(inArgs[0],inArgs[1]);
	return result;}

hx::Object *SimpleTable_obj::__ToInterface(const hx::type_info &inType) {
	if (inType==typeid( ::coopy::Table_obj)) return operator ::coopy::Table_obj *();
	return super::__ToInterface(inType);
}

::coopy::Table SimpleTable_obj::getTable( ){
	HX_STACK_FRAME("coopy.SimpleTable","getTable",0xb450428c,"coopy.SimpleTable.getTable","coopy/SimpleTable.hx",18,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(18)
	return hx::ObjectPtr<OBJ_>(this);
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,getTable,return )

int SimpleTable_obj::get_width( ){
	HX_STACK_FRAME("coopy.SimpleTable","get_width",0x27422ce9,"coopy.SimpleTable.get_width","coopy/SimpleTable.hx",26,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(26)
	return this->w;
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,get_width,return )

int SimpleTable_obj::get_height( ){
	HX_STACK_FRAME("coopy.SimpleTable","get_height",0xe8f497a4,"coopy.SimpleTable.get_height","coopy/SimpleTable.hx",30,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(30)
	return this->h;
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,get_height,return )

int SimpleTable_obj::get_size( ){
	HX_STACK_FRAME("coopy.SimpleTable","get_size",0x15a3d73e,"coopy.SimpleTable.get_size","coopy/SimpleTable.hx",34,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(34)
	return this->h;
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,get_size,return )

Dynamic SimpleTable_obj::getCell( int x,int y){
	HX_STACK_FRAME("coopy.SimpleTable","getCell",0x1805f0e4,"coopy.SimpleTable.getCell","coopy/SimpleTable.hx",38,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_ARG(x,"x")
	HX_STACK_ARG(y,"y")
	HX_STACK_LINE(38)
	return this->data->get((x + (y * this->w)));
}


HX_DEFINE_DYNAMIC_FUNC2(SimpleTable_obj,getCell,return )

Void SimpleTable_obj::setCell( int x,int y,Dynamic c){
{
		HX_STACK_FRAME("coopy.SimpleTable","setCell",0x0b0781f0,"coopy.SimpleTable.setCell","coopy/SimpleTable.hx",42,0x278206c5)
		HX_STACK_THIS(this)
		HX_STACK_ARG(x,"x")
		HX_STACK_ARG(y,"y")
		HX_STACK_ARG(c,"c")
		HX_STACK_LINE(42)
		Dynamic value = c;		HX_STACK_VAR(value,"value");
		HX_STACK_LINE(42)
		this->data->set((x + (y * this->w)),value);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC3(SimpleTable_obj,setCell,(void))

::String SimpleTable_obj::toString( ){
	HX_STACK_FRAME("coopy.SimpleTable","toString",0x82044ba0,"coopy.SimpleTable.toString","coopy/SimpleTable.hx",46,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(46)
	return ::coopy::SimpleTable_obj::tableToString(hx::ObjectPtr<OBJ_>(this));
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,toString,return )

::coopy::View SimpleTable_obj::getCellView( ){
	HX_STACK_FRAME("coopy.SimpleTable","getCellView",0xaa429169,"coopy.SimpleTable.getCellView","coopy/SimpleTable.hx",62,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(62)
	return ::coopy::SimpleView_obj::__new();
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,getCellView,return )

bool SimpleTable_obj::isResizable( ){
	HX_STACK_FRAME("coopy.SimpleTable","isResizable",0xeeabc3ad,"coopy.SimpleTable.isResizable","coopy/SimpleTable.hx",66,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(66)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,isResizable,return )

bool SimpleTable_obj::resize( int w,int h){
	HX_STACK_FRAME("coopy.SimpleTable","resize",0x3c286fe8,"coopy.SimpleTable.resize","coopy/SimpleTable.hx",69,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_ARG(w,"w")
	HX_STACK_ARG(h,"h")
	HX_STACK_LINE(70)
	this->w = w;
	HX_STACK_LINE(71)
	this->h = h;
	HX_STACK_LINE(72)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC2(SimpleTable_obj,resize,return )

Void SimpleTable_obj::clear( ){
{
		HX_STACK_FRAME("coopy.SimpleTable","clear",0x74365619,"coopy.SimpleTable.clear","coopy/SimpleTable.hx",76,0x278206c5)
		HX_STACK_THIS(this)
		HX_STACK_LINE(76)
		this->data = ::haxe::ds::IntMap_obj::__new();
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,clear,(void))

bool SimpleTable_obj::insertOrDeleteRows( Array< int > fate,int hfate){
	HX_STACK_FRAME("coopy.SimpleTable","insertOrDeleteRows",0x69cd56b4,"coopy.SimpleTable.insertOrDeleteRows","coopy/SimpleTable.hx",79,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_ARG(fate,"fate")
	HX_STACK_ARG(hfate,"hfate")
	HX_STACK_LINE(80)
	::haxe::ds::IntMap data2 = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(data2,"data2");
	HX_STACK_LINE(81)
	{
		HX_STACK_LINE(81)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(81)
		int _g = fate->length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(81)
		while(((_g1 < _g))){
			HX_STACK_LINE(81)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(82)
			int j = fate->__get(i);		HX_STACK_VAR(j,"j");
			HX_STACK_LINE(83)
			if (((j != (int)-1))){
				HX_STACK_LINE(84)
				int _g3 = (int)0;		HX_STACK_VAR(_g3,"_g3");
				HX_STACK_LINE(84)
				int _g2 = this->w;		HX_STACK_VAR(_g2,"_g2");
				HX_STACK_LINE(84)
				while(((_g3 < _g2))){
					HX_STACK_LINE(84)
					int c = (_g3)++;		HX_STACK_VAR(c,"c");
					HX_STACK_LINE(85)
					int idx = ((i * this->w) + c);		HX_STACK_VAR(idx,"idx");
					HX_STACK_LINE(86)
					if ((this->data->exists(idx))){
						HX_STACK_LINE(87)
						Dynamic value = this->data->get(idx);		HX_STACK_VAR(value,"value");
						HX_STACK_LINE(87)
						data2->set(((j * this->w) + c),value);
					}
				}
			}
		}
	}
	HX_STACK_LINE(92)
	this->h = hfate;
	HX_STACK_LINE(93)
	this->data = data2;
	HX_STACK_LINE(94)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC2(SimpleTable_obj,insertOrDeleteRows,return )

bool SimpleTable_obj::insertOrDeleteColumns( Array< int > fate,int wfate){
	HX_STACK_FRAME("coopy.SimpleTable","insertOrDeleteColumns",0x8021fe22,"coopy.SimpleTable.insertOrDeleteColumns","coopy/SimpleTable.hx",97,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_ARG(fate,"fate")
	HX_STACK_ARG(wfate,"wfate")
	HX_STACK_LINE(98)
	::haxe::ds::IntMap data2 = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(data2,"data2");
	HX_STACK_LINE(99)
	{
		HX_STACK_LINE(99)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(99)
		int _g = fate->length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(99)
		while(((_g1 < _g))){
			HX_STACK_LINE(99)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(100)
			int j = fate->__get(i);		HX_STACK_VAR(j,"j");
			HX_STACK_LINE(101)
			if (((j != (int)-1))){
				HX_STACK_LINE(102)
				int _g3 = (int)0;		HX_STACK_VAR(_g3,"_g3");
				HX_STACK_LINE(102)
				int _g2 = this->h;		HX_STACK_VAR(_g2,"_g2");
				HX_STACK_LINE(102)
				while(((_g3 < _g2))){
					HX_STACK_LINE(102)
					int r = (_g3)++;		HX_STACK_VAR(r,"r");
					HX_STACK_LINE(103)
					int idx = ((r * this->w) + i);		HX_STACK_VAR(idx,"idx");
					HX_STACK_LINE(104)
					if ((this->data->exists(idx))){
						HX_STACK_LINE(105)
						Dynamic value = this->data->get(idx);		HX_STACK_VAR(value,"value");
						HX_STACK_LINE(105)
						data2->set(((r * wfate) + j),value);
					}
				}
			}
		}
	}
	HX_STACK_LINE(110)
	this->w = wfate;
	HX_STACK_LINE(111)
	this->data = data2;
	HX_STACK_LINE(112)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC2(SimpleTable_obj,insertOrDeleteColumns,return )

bool SimpleTable_obj::trimBlank( ){
	HX_STACK_FRAME("coopy.SimpleTable","trimBlank",0x1263d99e,"coopy.SimpleTable.trimBlank","coopy/SimpleTable.hx",115,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(116)
	if (((this->h == (int)0))){
		HX_STACK_LINE(116)
		return true;
	}
	HX_STACK_LINE(117)
	int h_test = this->h;		HX_STACK_VAR(h_test,"h_test");
	HX_STACK_LINE(118)
	if (((h_test >= (int)3))){
		HX_STACK_LINE(118)
		h_test = (int)3;
	}
	HX_STACK_LINE(119)
	::coopy::View view = this->getCellView();		HX_STACK_VAR(view,"view");
	HX_STACK_LINE(120)
	Dynamic space = view->toDatum(HX_CSTRING(""));		HX_STACK_VAR(space,"space");
	HX_STACK_LINE(121)
	bool more = true;		HX_STACK_VAR(more,"more");
	HX_STACK_LINE(122)
	while((more)){
		HX_STACK_LINE(123)
		{
			HX_STACK_LINE(123)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(123)
			int _g = this->get_width();		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(123)
			while(((_g1 < _g))){
				HX_STACK_LINE(123)
				int i = (_g1)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(124)
				Dynamic c = this->getCell(i,(this->h - (int)1));		HX_STACK_VAR(c,"c");
				HX_STACK_LINE(125)
				if ((!(((bool(view->equals(c,space)) || bool((c == null()))))))){
					HX_STACK_LINE(126)
					more = false;
					HX_STACK_LINE(127)
					break;
				}
			}
		}
		HX_STACK_LINE(130)
		if ((more)){
			HX_STACK_LINE(130)
			(this->h)--;
		}
	}
	HX_STACK_LINE(132)
	more = true;
	HX_STACK_LINE(133)
	int nw = this->w;		HX_STACK_VAR(nw,"nw");
	HX_STACK_LINE(134)
	while((more)){
		HX_STACK_LINE(135)
		if (((this->w == (int)0))){
			HX_STACK_LINE(135)
			break;
		}
		HX_STACK_LINE(136)
		{
			HX_STACK_LINE(136)
			int _g = (int)0;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(136)
			while(((_g < h_test))){
				HX_STACK_LINE(136)
				int i = (_g)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(137)
				Dynamic c = this->getCell((nw - (int)1),i);		HX_STACK_VAR(c,"c");
				HX_STACK_LINE(138)
				if ((!(((bool(view->equals(c,space)) || bool((c == null()))))))){
					HX_STACK_LINE(139)
					more = false;
					HX_STACK_LINE(140)
					break;
				}
			}
		}
		HX_STACK_LINE(143)
		if ((more)){
			HX_STACK_LINE(143)
			(nw)--;
		}
	}
	HX_STACK_LINE(145)
	if (((nw == this->w))){
		HX_STACK_LINE(145)
		return true;
	}
	HX_STACK_LINE(146)
	::haxe::ds::IntMap data2 = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(data2,"data2");
	HX_STACK_LINE(147)
	{
		HX_STACK_LINE(147)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(147)
		while(((_g < nw))){
			HX_STACK_LINE(147)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(148)
			{
				HX_STACK_LINE(148)
				int _g2 = (int)0;		HX_STACK_VAR(_g2,"_g2");
				HX_STACK_LINE(148)
				int _g1 = this->h;		HX_STACK_VAR(_g1,"_g1");
				HX_STACK_LINE(148)
				while(((_g2 < _g1))){
					HX_STACK_LINE(148)
					int r = (_g2)++;		HX_STACK_VAR(r,"r");
					HX_STACK_LINE(149)
					int idx = ((r * this->w) + i);		HX_STACK_VAR(idx,"idx");
					HX_STACK_LINE(150)
					if ((this->data->exists(idx))){
						HX_STACK_LINE(151)
						Dynamic value = this->data->get(idx);		HX_STACK_VAR(value,"value");
						HX_STACK_LINE(151)
						data2->set(((r * nw) + i),value);
					}
				}
			}
		}
	}
	HX_STACK_LINE(155)
	this->w = nw;
	HX_STACK_LINE(156)
	this->data = data2;
	HX_STACK_LINE(157)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC0(SimpleTable_obj,trimBlank,return )

::String SimpleTable_obj::tableToString( ::coopy::Table tab){
	HX_STACK_FRAME("coopy.SimpleTable","tableToString",0x38145366,"coopy.SimpleTable.tableToString","coopy/SimpleTable.hx",49,0x278206c5)
	HX_STACK_ARG(tab,"tab")
	HX_STACK_LINE(50)
	::String x = HX_CSTRING("");		HX_STACK_VAR(x,"x");
	HX_STACK_LINE(51)
	{
		HX_STACK_LINE(51)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(51)
		int _g = tab->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(51)
		while(((_g1 < _g))){
			HX_STACK_LINE(51)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(52)
			{
				HX_STACK_LINE(52)
				int _g3 = (int)0;		HX_STACK_VAR(_g3,"_g3");
				HX_STACK_LINE(52)
				int _g2 = tab->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(_g2,"_g2");
				HX_STACK_LINE(52)
				while(((_g3 < _g2))){
					HX_STACK_LINE(52)
					int j = (_g3)++;		HX_STACK_VAR(j,"j");
					HX_STACK_LINE(53)
					if (((j > (int)0))){
						HX_STACK_LINE(53)
						hx::AddEq(x,HX_CSTRING(" "));
					}
					HX_STACK_LINE(54)
					hx::AddEq(x,::Std_obj::string(tab->getCell(j,i)));
				}
			}
			HX_STACK_LINE(56)
			hx::AddEq(x,HX_CSTRING("\n"));
		}
	}
	HX_STACK_LINE(58)
	return x;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(SimpleTable_obj,tableToString,return )


SimpleTable_obj::SimpleTable_obj()
{
}

void SimpleTable_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(SimpleTable);
	HX_MARK_MEMBER_NAME(data,"data");
	HX_MARK_MEMBER_NAME(w,"w");
	HX_MARK_MEMBER_NAME(h,"h");
	HX_MARK_END_CLASS();
}

void SimpleTable_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(data,"data");
	HX_VISIT_MEMBER_NAME(w,"w");
	HX_VISIT_MEMBER_NAME(h,"h");
}

Dynamic SimpleTable_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"w") ) { return w; }
		if (HX_FIELD_EQ(inName,"h") ) { return h; }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"data") ) { return data; }
		if (HX_FIELD_EQ(inName,"size") ) { return get_size(); }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"width") ) { return get_width(); }
		if (HX_FIELD_EQ(inName,"clear") ) { return clear_dyn(); }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"height") ) { return get_height(); }
		if (HX_FIELD_EQ(inName,"resize") ) { return resize_dyn(); }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"getCell") ) { return getCell_dyn(); }
		if (HX_FIELD_EQ(inName,"setCell") ) { return setCell_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"getTable") ) { return getTable_dyn(); }
		if (HX_FIELD_EQ(inName,"get_size") ) { return get_size_dyn(); }
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"get_width") ) { return get_width_dyn(); }
		if (HX_FIELD_EQ(inName,"trimBlank") ) { return trimBlank_dyn(); }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"get_height") ) { return get_height_dyn(); }
		break;
	case 11:
		if (HX_FIELD_EQ(inName,"getCellView") ) { return getCellView_dyn(); }
		if (HX_FIELD_EQ(inName,"isResizable") ) { return isResizable_dyn(); }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"tableToString") ) { return tableToString_dyn(); }
		break;
	case 18:
		if (HX_FIELD_EQ(inName,"insertOrDeleteRows") ) { return insertOrDeleteRows_dyn(); }
		break;
	case 21:
		if (HX_FIELD_EQ(inName,"insertOrDeleteColumns") ) { return insertOrDeleteColumns_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic SimpleTable_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"w") ) { w=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"h") ) { h=inValue.Cast< int >(); return inValue; }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"data") ) { data=inValue.Cast< ::haxe::ds::IntMap >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void SimpleTable_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("data"));
	outFields->push(HX_CSTRING("w"));
	outFields->push(HX_CSTRING("h"));
	outFields->push(HX_CSTRING("height"));
	outFields->push(HX_CSTRING("width"));
	outFields->push(HX_CSTRING("size"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("tableToString"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(SimpleTable_obj,data),HX_CSTRING("data")},
	{hx::fsObject,(int)offsetof(SimpleTable_obj,w),HX_CSTRING("w")},
	{hx::fsObject,(int)offsetof(SimpleTable_obj,h),HX_CSTRING("h")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("data"),
	HX_CSTRING("w"),
	HX_CSTRING("h"),
	HX_CSTRING("getTable"),
	HX_CSTRING("get_width"),
	HX_CSTRING("get_height"),
	HX_CSTRING("get_size"),
	HX_CSTRING("getCell"),
	HX_CSTRING("setCell"),
	HX_CSTRING("toString"),
	HX_CSTRING("getCellView"),
	HX_CSTRING("isResizable"),
	HX_CSTRING("resize"),
	HX_CSTRING("clear"),
	HX_CSTRING("insertOrDeleteRows"),
	HX_CSTRING("insertOrDeleteColumns"),
	HX_CSTRING("trimBlank"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(SimpleTable_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(SimpleTable_obj::__mClass,"__mClass");
};

#endif

Class SimpleTable_obj::__mClass;

void SimpleTable_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.SimpleTable"), hx::TCanCast< SimpleTable_obj> ,sStaticFields,sMemberFields,
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

void SimpleTable_obj::__boot()
{
}

} // end namespace coopy
