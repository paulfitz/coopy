#include <hxcpp.h>

#ifndef INCLUDED_coopy_Csv
#include <coopy/Csv.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
namespace coopy{

Void Csv_obj::__construct()
{
HX_STACK_FRAME("coopy.Csv","new",0x64cdce36,"coopy.Csv.new","coopy/Csv.hx",11,0xf1c8e0bb)
{
	HX_STACK_LINE(12)
	this->cursor = (int)0;
	HX_STACK_LINE(13)
	this->row_ended = false;
}
;
	return null();
}

Csv_obj::~Csv_obj() { }

Dynamic Csv_obj::__CreateEmpty() { return  new Csv_obj; }
hx::ObjectPtr< Csv_obj > Csv_obj::__new()
{  hx::ObjectPtr< Csv_obj > result = new Csv_obj();
	result->__construct();
	return result;}

Dynamic Csv_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Csv_obj > result = new Csv_obj();
	result->__construct();
	return result;}

::String Csv_obj::renderTable( ::coopy::Table t){
	HX_STACK_FRAME("coopy.Csv","renderTable",0xc6a1a8ae,"coopy.Csv.renderTable","coopy/Csv.hx",16,0xf1c8e0bb)
	HX_STACK_THIS(this)
	HX_STACK_ARG(t,"t")
	HX_STACK_LINE(17)
	::String result = HX_CSTRING("");		HX_STACK_VAR(result,"result");
	HX_STACK_LINE(18)
	int w = t->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(w,"w");
	HX_STACK_LINE(19)
	int h = t->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(h,"h");
	HX_STACK_LINE(20)
	::String txt = HX_CSTRING("");		HX_STACK_VAR(txt,"txt");
	HX_STACK_LINE(21)
	::coopy::View v = t->getCellView();		HX_STACK_VAR(v,"v");
	HX_STACK_LINE(22)
	{
		HX_STACK_LINE(22)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(22)
		while(((_g < h))){
			HX_STACK_LINE(22)
			int y = (_g)++;		HX_STACK_VAR(y,"y");
			HX_STACK_LINE(23)
			{
				HX_STACK_LINE(23)
				int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
				HX_STACK_LINE(23)
				while(((_g1 < w))){
					HX_STACK_LINE(23)
					int x = (_g1)++;		HX_STACK_VAR(x,"x");
					HX_STACK_LINE(24)
					if (((x > (int)0))){
						HX_STACK_LINE(25)
						hx::AddEq(txt,HX_CSTRING(","));
					}
					HX_STACK_LINE(27)
					hx::AddEq(txt,this->renderCell(v,t->getCell(x,y)));
				}
			}
			HX_STACK_LINE(29)
			hx::AddEq(txt,HX_CSTRING("\r\n"));
		}
	}
	HX_STACK_LINE(31)
	return txt;
}


HX_DEFINE_DYNAMIC_FUNC1(Csv_obj,renderTable,return )

::String Csv_obj::renderCell( ::coopy::View v,Dynamic d){
	HX_STACK_FRAME("coopy.Csv","renderCell",0x96622502,"coopy.Csv.renderCell","coopy/Csv.hx",34,0xf1c8e0bb)
	HX_STACK_THIS(this)
	HX_STACK_ARG(v,"v")
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(35)
	if (((d == null()))){
		HX_STACK_LINE(36)
		return HX_CSTRING("NULL");
	}
	HX_STACK_LINE(38)
	if ((v->equals(d,null()))){
		HX_STACK_LINE(39)
		return HX_CSTRING("NULL");
	}
	HX_STACK_LINE(41)
	::String str = v->toString(d);		HX_STACK_VAR(str,"str");
	HX_STACK_LINE(42)
	::String delim = HX_CSTRING(",");		HX_STACK_VAR(delim,"delim");
	HX_STACK_LINE(43)
	bool need_quote = false;		HX_STACK_VAR(need_quote,"need_quote");
	HX_STACK_LINE(44)
	{
		HX_STACK_LINE(44)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(44)
		int _g = str.length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(44)
		while(((_g1 < _g))){
			HX_STACK_LINE(44)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(45)
			::String ch = str.charAt(i);		HX_STACK_VAR(ch,"ch");
			HX_STACK_LINE(46)
			if (((bool((bool((bool((bool((bool((bool((ch == HX_CSTRING("\""))) || bool((ch == HX_CSTRING("'"))))) || bool((ch == delim)))) || bool((ch == HX_CSTRING("\r"))))) || bool((ch == HX_CSTRING("\n"))))) || bool((ch == HX_CSTRING("\t"))))) || bool((ch == HX_CSTRING(" ")))))){
				HX_STACK_LINE(47)
				need_quote = true;
				HX_STACK_LINE(48)
				break;
			}
		}
	}
	HX_STACK_LINE(52)
	::String result = HX_CSTRING("");		HX_STACK_VAR(result,"result");
	HX_STACK_LINE(53)
	if ((need_quote)){
		HX_STACK_LINE(53)
		hx::AddEq(result,HX_CSTRING("\""));
	}
	HX_STACK_LINE(54)
	::String line_buf = HX_CSTRING("");		HX_STACK_VAR(line_buf,"line_buf");
	HX_STACK_LINE(55)
	{
		HX_STACK_LINE(55)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(55)
		int _g = str.length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(55)
		while(((_g1 < _g))){
			HX_STACK_LINE(55)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(56)
			::String ch = str.charAt(i);		HX_STACK_VAR(ch,"ch");
			HX_STACK_LINE(57)
			if (((ch == HX_CSTRING("\"")))){
				HX_STACK_LINE(58)
				hx::AddEq(result,HX_CSTRING("\""));
			}
			HX_STACK_LINE(60)
			if (((bool((ch != HX_CSTRING("\r"))) && bool((ch != HX_CSTRING("\n")))))){
				HX_STACK_LINE(61)
				if (((line_buf.length > (int)0))){
					HX_STACK_LINE(62)
					hx::AddEq(result,line_buf);
					HX_STACK_LINE(63)
					line_buf = HX_CSTRING("");
				}
				HX_STACK_LINE(65)
				hx::AddEq(result,ch);
			}
			else{
				HX_STACK_LINE(67)
				hx::AddEq(line_buf,ch);
			}
		}
	}
	HX_STACK_LINE(70)
	if ((need_quote)){
		HX_STACK_LINE(70)
		hx::AddEq(result,HX_CSTRING("\""));
	}
	HX_STACK_LINE(71)
	return result;
}


HX_DEFINE_DYNAMIC_FUNC2(Csv_obj,renderCell,return )

Array< ::Dynamic > Csv_obj::parseTable( ::String txt){
	HX_STACK_FRAME("coopy.Csv","parseTable",0xec6007c5,"coopy.Csv.parseTable","coopy/Csv.hx",74,0xf1c8e0bb)
	HX_STACK_THIS(this)
	HX_STACK_ARG(txt,"txt")
	HX_STACK_LINE(75)
	this->cursor = (int)0;
	HX_STACK_LINE(76)
	this->row_ended = false;
	HX_STACK_LINE(77)
	this->has_structure = true;
	HX_STACK_LINE(78)
	Array< ::Dynamic > result = Array_obj< ::Dynamic >::__new();		HX_STACK_VAR(result,"result");
	HX_STACK_LINE(79)
	Array< ::String > row = Array_obj< ::String >::__new();		HX_STACK_VAR(row,"row");
	HX_STACK_LINE(80)
	while(((this->cursor < txt.length))){
		HX_STACK_LINE(81)
		::String cell = this->parseCell(txt);		HX_STACK_VAR(cell,"cell");
		HX_STACK_LINE(82)
		row->push(cell);
		HX_STACK_LINE(83)
		if ((this->row_ended)){
			HX_STACK_LINE(84)
			result->push(row);
			HX_STACK_LINE(85)
			row = Array_obj< ::String >::__new();
		}
		HX_STACK_LINE(87)
		(this->cursor)++;
	}
	HX_STACK_LINE(89)
	return result;
}


HX_DEFINE_DYNAMIC_FUNC1(Csv_obj,parseTable,return )

::String Csv_obj::parseCell( ::String txt){
	HX_STACK_FRAME("coopy.Csv","parseCell",0xc7ea75cb,"coopy.Csv.parseCell","coopy/Csv.hx",92,0xf1c8e0bb)
	HX_STACK_THIS(this)
	HX_STACK_ARG(txt,"txt")
	HX_STACK_LINE(93)
	if (((txt == null()))){
		HX_STACK_LINE(93)
		return null();
	}
	HX_STACK_LINE(94)
	this->row_ended = false;
	HX_STACK_LINE(95)
	int first_non_underscore = txt.length;		HX_STACK_VAR(first_non_underscore,"first_non_underscore");
	HX_STACK_LINE(96)
	int last_processed = (int)0;		HX_STACK_VAR(last_processed,"last_processed");
	HX_STACK_LINE(97)
	bool quoting = false;		HX_STACK_VAR(quoting,"quoting");
	HX_STACK_LINE(98)
	int quote = (int)0;		HX_STACK_VAR(quote,"quote");
	HX_STACK_LINE(99)
	::String result = HX_CSTRING("");		HX_STACK_VAR(result,"result");
	HX_STACK_LINE(100)
	int start = this->cursor;		HX_STACK_VAR(start,"start");
	HX_STACK_LINE(101)
	{
		HX_STACK_LINE(101)
		int _g1 = this->cursor;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(101)
		int _g = txt.length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(101)
		while(((_g1 < _g))){
			HX_STACK_LINE(101)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(102)
			int ch = txt.charCodeAt(i);		HX_STACK_VAR(ch,"ch");
			HX_STACK_LINE(103)
			last_processed = i;
			HX_STACK_LINE(104)
			if (((bool((ch != (int)95)) && bool((i < first_non_underscore))))){
				HX_STACK_LINE(105)
				first_non_underscore = i;
			}
			HX_STACK_LINE(107)
			if ((this->has_structure)){
				HX_STACK_LINE(108)
				if ((!(quoting))){
					HX_STACK_LINE(109)
					if (((ch == (int)44))){
						HX_STACK_LINE(110)
						break;
					}
					HX_STACK_LINE(112)
					if (((bool((ch == (int)13)) || bool((ch == (int)10))))){
						HX_STACK_LINE(113)
						Dynamic ch2 = txt.charCodeAt((i + (int)1));		HX_STACK_VAR(ch2,"ch2");
						HX_STACK_LINE(114)
						if (((ch2 != null()))){
							HX_STACK_LINE(115)
							if (((ch2 != ch))){
								HX_STACK_LINE(116)
								if (((bool((ch2 == (int)13)) || bool((ch2 == (int)10))))){
									HX_STACK_LINE(117)
									(last_processed)++;
								}
							}
						}
						HX_STACK_LINE(121)
						this->row_ended = true;
						HX_STACK_LINE(122)
						break;
					}
					HX_STACK_LINE(124)
					if (((bool((i == this->cursor)) && bool(((bool((ch == (int)34)) || bool((ch == (int)39)))))))){
						HX_STACK_LINE(125)
						quoting = true;
						HX_STACK_LINE(126)
						quote = ch;
						HX_STACK_LINE(127)
						if (((i != start))){
							HX_STACK_LINE(128)
							hx::AddEq(result,::String::fromCharCode(ch));
						}
						HX_STACK_LINE(130)
						continue;
					}
					HX_STACK_LINE(132)
					hx::AddEq(result,::String::fromCharCode(ch));
					HX_STACK_LINE(133)
					continue;
				}
				HX_STACK_LINE(135)
				if (((ch == quote))){
					HX_STACK_LINE(136)
					quoting = false;
					HX_STACK_LINE(137)
					continue;
				}
			}
			HX_STACK_LINE(140)
			hx::AddEq(result,::String::fromCharCode(ch));
		}
	}
	HX_STACK_LINE(142)
	this->cursor = last_processed;
	HX_STACK_LINE(143)
	if (((quote == (int)0))){
		HX_STACK_LINE(144)
		if (((result == HX_CSTRING("NULL")))){
			HX_STACK_LINE(145)
			return null();
		}
		HX_STACK_LINE(147)
		if (((first_non_underscore > start))){
			HX_STACK_LINE(148)
			int del = (first_non_underscore - start);		HX_STACK_VAR(del,"del");
			HX_STACK_LINE(149)
			if (((result.substr(del,null()) == HX_CSTRING("NULL")))){
				HX_STACK_LINE(150)
				return result.substr((int)1,null());
			}
		}
	}
	HX_STACK_LINE(154)
	return result;
}


HX_DEFINE_DYNAMIC_FUNC1(Csv_obj,parseCell,return )

::String Csv_obj::parseSingleCell( ::String txt){
	HX_STACK_FRAME("coopy.Csv","parseSingleCell",0x51b5f133,"coopy.Csv.parseSingleCell","coopy/Csv.hx",157,0xf1c8e0bb)
	HX_STACK_THIS(this)
	HX_STACK_ARG(txt,"txt")
	HX_STACK_LINE(158)
	this->cursor = (int)0;
	HX_STACK_LINE(159)
	this->row_ended = false;
	HX_STACK_LINE(160)
	this->has_structure = false;
	HX_STACK_LINE(161)
	return this->parseCell(txt);
}


HX_DEFINE_DYNAMIC_FUNC1(Csv_obj,parseSingleCell,return )


Csv_obj::Csv_obj()
{
}

void Csv_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Csv);
	HX_MARK_MEMBER_NAME(cursor,"cursor");
	HX_MARK_MEMBER_NAME(row_ended,"row_ended");
	HX_MARK_MEMBER_NAME(has_structure,"has_structure");
	HX_MARK_END_CLASS();
}

void Csv_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(cursor,"cursor");
	HX_VISIT_MEMBER_NAME(row_ended,"row_ended");
	HX_VISIT_MEMBER_NAME(has_structure,"has_structure");
}

Dynamic Csv_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 6:
		if (HX_FIELD_EQ(inName,"cursor") ) { return cursor; }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"row_ended") ) { return row_ended; }
		if (HX_FIELD_EQ(inName,"parseCell") ) { return parseCell_dyn(); }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"renderCell") ) { return renderCell_dyn(); }
		if (HX_FIELD_EQ(inName,"parseTable") ) { return parseTable_dyn(); }
		break;
	case 11:
		if (HX_FIELD_EQ(inName,"renderTable") ) { return renderTable_dyn(); }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"has_structure") ) { return has_structure; }
		break;
	case 15:
		if (HX_FIELD_EQ(inName,"parseSingleCell") ) { return parseSingleCell_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Csv_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 6:
		if (HX_FIELD_EQ(inName,"cursor") ) { cursor=inValue.Cast< int >(); return inValue; }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"row_ended") ) { row_ended=inValue.Cast< bool >(); return inValue; }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"has_structure") ) { has_structure=inValue.Cast< bool >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void Csv_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("cursor"));
	outFields->push(HX_CSTRING("row_ended"));
	outFields->push(HX_CSTRING("has_structure"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(Csv_obj,cursor),HX_CSTRING("cursor")},
	{hx::fsObject,(int)offsetof(Csv_obj,row_ended),HX_CSTRING("row_ended")},
	{hx::fsObject,(int)offsetof(Csv_obj,has_structure),HX_CSTRING("has_structure")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("cursor"),
	HX_CSTRING("row_ended"),
	HX_CSTRING("has_structure"),
	HX_CSTRING("renderTable"),
	HX_CSTRING("renderCell"),
	HX_CSTRING("parseTable"),
	HX_CSTRING("parseCell"),
	HX_CSTRING("parseSingleCell"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Csv_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Csv_obj::__mClass,"__mClass");
};

#endif

Class Csv_obj::__mClass;

void Csv_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Csv"), hx::TCanCast< Csv_obj> ,sStaticFields,sMemberFields,
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

void Csv_obj::__boot()
{
}

} // end namespace coopy
