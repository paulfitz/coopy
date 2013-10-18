#include <hxcpp.h>

#ifndef INCLUDED_IMap
#include <IMap.h>
#endif
#ifndef INCLUDED_Std
#include <Std.h>
#endif
#ifndef INCLUDED_coopy_Alignment
#include <coopy/Alignment.h>
#endif
#ifndef INCLUDED_coopy_Compare
#include <coopy/Compare.h>
#endif
#ifndef INCLUDED_coopy_CompareFlags
#include <coopy/CompareFlags.h>
#endif
#ifndef INCLUDED_coopy_CompareTable
#include <coopy/CompareTable.h>
#endif
#ifndef INCLUDED_coopy_Coopy
#include <coopy/Coopy.h>
#endif
#ifndef INCLUDED_coopy_Csv
#include <coopy/Csv.h>
#endif
#ifndef INCLUDED_coopy_DiffRender
#include <coopy/DiffRender.h>
#endif
#ifndef INCLUDED_coopy_HighlightPatch
#include <coopy/HighlightPatch.h>
#endif
#ifndef INCLUDED_coopy_Index
#include <coopy/Index.h>
#endif
#ifndef INCLUDED_coopy_Mover
#include <coopy/Mover.h>
#endif
#ifndef INCLUDED_coopy_Report
#include <coopy/Report.h>
#endif
#ifndef INCLUDED_coopy_Row
#include <coopy/Row.h>
#endif
#ifndef INCLUDED_coopy_SimpleCell
#include <coopy/SimpleCell.h>
#endif
#ifndef INCLUDED_coopy_SimpleTable
#include <coopy/SimpleTable.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableComparisonState
#include <coopy/TableComparisonState.h>
#endif
#ifndef INCLUDED_coopy_TableDiff
#include <coopy/TableDiff.h>
#endif
#ifndef INCLUDED_coopy_TableIO
#include <coopy/TableIO.h>
#endif
#ifndef INCLUDED_coopy_TableModifier
#include <coopy/TableModifier.h>
#endif
#ifndef INCLUDED_coopy_TableView
#include <coopy/TableView.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
#ifndef INCLUDED_coopy_ViewedDatum
#include <coopy/ViewedDatum.h>
#endif
#ifndef INCLUDED_coopy_Viterbi
#include <coopy/Viterbi.h>
#endif
#ifndef INCLUDED_haxe_Log
#include <haxe/Log.h>
#endif
#ifndef INCLUDED_haxe_ds_StringMap
#include <haxe/ds/StringMap.h>
#endif
namespace coopy{

Void Coopy_obj::__construct()
{
HX_STACK_FRAME("coopy.Coopy","new",0x1b3b39fc,"coopy.Coopy.new","coopy/Coopy.hx",13,0x36f081f5)
{
}
;
	return null();
}

Coopy_obj::~Coopy_obj() { }

Dynamic Coopy_obj::__CreateEmpty() { return  new Coopy_obj; }
hx::ObjectPtr< Coopy_obj > Coopy_obj::__new()
{  hx::ObjectPtr< Coopy_obj > result = new Coopy_obj();
	result->__construct();
	return result;}

Dynamic Coopy_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Coopy_obj > result = new Coopy_obj();
	result->__construct();
	return result;}

::coopy::CompareTable Coopy_obj::compareTables( ::coopy::Table local,::coopy::Table remote){
	HX_STACK_FRAME("coopy.Coopy","compareTables",0x1cc06fc6,"coopy.Coopy.compareTables","coopy/Coopy.hx",16,0x36f081f5)
	HX_STACK_ARG(local,"local")
	HX_STACK_ARG(remote,"remote")
	HX_STACK_LINE(17)
	::coopy::CompareTable ct = ::coopy::CompareTable_obj::__new();		HX_STACK_VAR(ct,"ct");
	HX_STACK_LINE(18)
	::coopy::TableComparisonState comp = ::coopy::TableComparisonState_obj::__new();		HX_STACK_VAR(comp,"comp");
	HX_STACK_LINE(19)
	comp->a = local;
	HX_STACK_LINE(20)
	comp->b = remote;
	HX_STACK_LINE(21)
	ct->attach(comp);
	HX_STACK_LINE(22)
	return ct;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC2(Coopy_obj,compareTables,return )

::coopy::CompareTable Coopy_obj::compareTables3( ::coopy::Table parent,::coopy::Table local,::coopy::Table remote){
	HX_STACK_FRAME("coopy.Coopy","compareTables3",0x0ba15dad,"coopy.Coopy.compareTables3","coopy/Coopy.hx",25,0x36f081f5)
	HX_STACK_ARG(parent,"parent")
	HX_STACK_ARG(local,"local")
	HX_STACK_ARG(remote,"remote")
	HX_STACK_LINE(26)
	::coopy::CompareTable ct = ::coopy::CompareTable_obj::__new();		HX_STACK_VAR(ct,"ct");
	HX_STACK_LINE(27)
	::coopy::TableComparisonState comp = ::coopy::TableComparisonState_obj::__new();		HX_STACK_VAR(comp,"comp");
	HX_STACK_LINE(28)
	comp->p = parent;
	HX_STACK_LINE(29)
	comp->a = local;
	HX_STACK_LINE(30)
	comp->b = remote;
	HX_STACK_LINE(31)
	ct->attach(comp);
	HX_STACK_LINE(32)
	return ct;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC3(Coopy_obj,compareTables3,return )

int Coopy_obj::randomTests( ){
	HX_STACK_FRAME("coopy.Coopy","randomTests",0xe13f087a,"coopy.Coopy.randomTests","coopy/Coopy.hx",35,0x36f081f5)
	HX_STACK_LINE(38)
	::coopy::SimpleTable st = ::coopy::SimpleTable_obj::__new((int)15,(int)6);		HX_STACK_VAR(st,"st");
	HX_STACK_LINE(39)
	::coopy::Table tab = st;		HX_STACK_VAR(tab,"tab");
	HX_STACK_LINE(40)
	int _g5 = tab->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(_g5,"_g5");
	HX_STACK_LINE(40)
	int _g6 = tab->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(_g6,"_g6");
	HX_STACK_LINE(40)
	::haxe::Log_obj::trace((((HX_CSTRING("table size is ") + _g6) + HX_CSTRING("x")) + _g5),hx::SourceInfo(HX_CSTRING("Coopy.hx"),40,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(41)
	tab->setCell((int)3,(int)4,::coopy::SimpleCell_obj::__new((int)33));
	HX_STACK_LINE(42)
	::String _g7 = ::Std_obj::string(tab->getCell((int)3,(int)4));		HX_STACK_VAR(_g7,"_g7");
	HX_STACK_LINE(42)
	::haxe::Log_obj::trace((HX_CSTRING("element is ") + _g7),hx::SourceInfo(HX_CSTRING("Coopy.hx"),42,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(44)
	::coopy::Compare compare = ::coopy::Compare_obj::__new();		HX_STACK_VAR(compare,"compare");
	HX_STACK_LINE(45)
	::coopy::ViewedDatum d1 = ::coopy::ViewedDatum_obj::getSimpleView(::coopy::SimpleCell_obj::__new((int)10));		HX_STACK_VAR(d1,"d1");
	HX_STACK_LINE(46)
	::coopy::ViewedDatum d2 = ::coopy::ViewedDatum_obj::getSimpleView(::coopy::SimpleCell_obj::__new((int)10));		HX_STACK_VAR(d2,"d2");
	HX_STACK_LINE(47)
	::coopy::ViewedDatum d3 = ::coopy::ViewedDatum_obj::getSimpleView(::coopy::SimpleCell_obj::__new((int)20));		HX_STACK_VAR(d3,"d3");
	HX_STACK_LINE(48)
	::coopy::Report report = ::coopy::Report_obj::__new();		HX_STACK_VAR(report,"report");
	HX_STACK_LINE(49)
	compare->compare(d1,d2,d3,report);
	HX_STACK_LINE(50)
	::String _g8 = ::Std_obj::string(report);		HX_STACK_VAR(_g8,"_g8");
	HX_STACK_LINE(50)
	::haxe::Log_obj::trace((HX_CSTRING("report is ") + _g8),hx::SourceInfo(HX_CSTRING("Coopy.hx"),50,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(51)
	d2 = ::coopy::ViewedDatum_obj::getSimpleView(::coopy::SimpleCell_obj::__new((int)50));
	HX_STACK_LINE(52)
	report->clear();
	HX_STACK_LINE(53)
	compare->compare(d1,d2,d3,report);
	HX_STACK_LINE(54)
	::String _g9 = ::Std_obj::string(report);		HX_STACK_VAR(_g9,"_g9");
	HX_STACK_LINE(54)
	::haxe::Log_obj::trace((HX_CSTRING("report is ") + _g9),hx::SourceInfo(HX_CSTRING("Coopy.hx"),54,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(55)
	d2 = ::coopy::ViewedDatum_obj::getSimpleView(::coopy::SimpleCell_obj::__new((int)20));
	HX_STACK_LINE(56)
	report->clear();
	HX_STACK_LINE(57)
	compare->compare(d1,d2,d3,report);
	HX_STACK_LINE(58)
	::String _g10 = ::Std_obj::string(report);		HX_STACK_VAR(_g10,"_g10");
	HX_STACK_LINE(58)
	::haxe::Log_obj::trace((HX_CSTRING("report is ") + _g10),hx::SourceInfo(HX_CSTRING("Coopy.hx"),58,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(59)
	d1 = ::coopy::ViewedDatum_obj::getSimpleView(::coopy::SimpleCell_obj::__new((int)20));
	HX_STACK_LINE(60)
	report->clear();
	HX_STACK_LINE(61)
	compare->compare(d1,d2,d3,report);
	HX_STACK_LINE(62)
	::String _g11 = ::Std_obj::string(report);		HX_STACK_VAR(_g11,"_g11");
	HX_STACK_LINE(62)
	::haxe::Log_obj::trace((HX_CSTRING("report is ") + _g11),hx::SourceInfo(HX_CSTRING("Coopy.hx"),62,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(64)
	::coopy::TableView tv = ::coopy::TableView_obj::__new();		HX_STACK_VAR(tv,"tv");
	HX_STACK_LINE(66)
	::coopy::TableComparisonState comp = ::coopy::TableComparisonState_obj::__new();		HX_STACK_VAR(comp,"comp");
	HX_STACK_LINE(67)
	::coopy::CompareTable ct = ::coopy::CompareTable_obj::__new();		HX_STACK_VAR(ct,"ct");
	HX_STACK_LINE(68)
	comp->a = st;
	HX_STACK_LINE(69)
	comp->b = st;
	HX_STACK_LINE(70)
	ct->attach(comp);
	HX_STACK_LINE(72)
	::haxe::Log_obj::trace(HX_CSTRING("comparing tables"),hx::SourceInfo(HX_CSTRING("Coopy.hx"),72,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(73)
	::coopy::SimpleTable t1 = ::coopy::SimpleTable_obj::__new((int)3,(int)2);		HX_STACK_VAR(t1,"t1");
	HX_STACK_LINE(74)
	::coopy::SimpleTable t2 = ::coopy::SimpleTable_obj::__new((int)3,(int)2);		HX_STACK_VAR(t2,"t2");
	HX_STACK_LINE(75)
	::coopy::SimpleTable t3 = ::coopy::SimpleTable_obj::__new((int)3,(int)2);		HX_STACK_VAR(t3,"t3");
	HX_STACK_LINE(76)
	::coopy::ViewedDatum dt1 = ::coopy::ViewedDatum_obj::__new(t1,::coopy::TableView_obj::__new());		HX_STACK_VAR(dt1,"dt1");
	HX_STACK_LINE(77)
	::coopy::ViewedDatum dt2 = ::coopy::ViewedDatum_obj::__new(t2,::coopy::TableView_obj::__new());		HX_STACK_VAR(dt2,"dt2");
	HX_STACK_LINE(78)
	::coopy::ViewedDatum dt3 = ::coopy::ViewedDatum_obj::__new(t3,::coopy::TableView_obj::__new());		HX_STACK_VAR(dt3,"dt3");
	HX_STACK_LINE(79)
	compare->compare(dt1,dt2,dt3,report);
	HX_STACK_LINE(80)
	::String _g12 = ::Std_obj::string(report);		HX_STACK_VAR(_g12,"_g12");
	HX_STACK_LINE(80)
	::haxe::Log_obj::trace((HX_CSTRING("report is ") + _g12),hx::SourceInfo(HX_CSTRING("Coopy.hx"),80,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(81)
	t3->setCell((int)1,(int)1,::coopy::SimpleCell_obj::__new(HX_CSTRING("hello")));
	HX_STACK_LINE(82)
	compare->compare(dt1,dt2,dt3,report);
	HX_STACK_LINE(83)
	::String _g13 = ::Std_obj::string(report);		HX_STACK_VAR(_g13,"_g13");
	HX_STACK_LINE(83)
	::haxe::Log_obj::trace((HX_CSTRING("report is ") + _g13),hx::SourceInfo(HX_CSTRING("Coopy.hx"),83,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(84)
	t1->setCell((int)1,(int)1,::coopy::SimpleCell_obj::__new(HX_CSTRING("hello")));
	HX_STACK_LINE(85)
	compare->compare(dt1,dt2,dt3,report);
	HX_STACK_LINE(86)
	::String _g14 = ::Std_obj::string(report);		HX_STACK_VAR(_g14,"_g14");
	HX_STACK_LINE(86)
	::haxe::Log_obj::trace((HX_CSTRING("report is ") + _g14),hx::SourceInfo(HX_CSTRING("Coopy.hx"),86,HX_CSTRING("coopy.Coopy"),HX_CSTRING("randomTests")));
	HX_STACK_LINE(88)
	::coopy::Viterbi v = ::coopy::Viterbi_obj::__new();		HX_STACK_VAR(v,"v");
	HX_STACK_LINE(89)
	::coopy::TableDiff td = ::coopy::TableDiff_obj::__new(null(),null());		HX_STACK_VAR(td,"td");
	HX_STACK_LINE(90)
	::coopy::Index idx = ::coopy::Index_obj::__new();		HX_STACK_VAR(idx,"idx");
	HX_STACK_LINE(91)
	::coopy::DiffRender dr = ::coopy::DiffRender_obj::__new();		HX_STACK_VAR(dr,"dr");
	HX_STACK_LINE(92)
	::coopy::CompareFlags cf = ::coopy::CompareFlags_obj::__new();		HX_STACK_VAR(cf,"cf");
	HX_STACK_LINE(93)
	::coopy::HighlightPatch hp = ::coopy::HighlightPatch_obj::__new(null(),null());		HX_STACK_VAR(hp,"hp");
	HX_STACK_LINE(94)
	::coopy::Csv csv = ::coopy::Csv_obj::__new();		HX_STACK_VAR(csv,"csv");
	HX_STACK_LINE(95)
	::coopy::TableModifier tm = ::coopy::TableModifier_obj::__new(null());		HX_STACK_VAR(tm,"tm");
	HX_STACK_LINE(97)
	return (int)0;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC0(Coopy_obj,randomTests,return )

int Coopy_obj::main( ){
	HX_STACK_FRAME("coopy.Coopy","main",0xb7eb371d,"coopy.Coopy.main","coopy/Coopy.hx",294,0x36f081f5)
	HX_STACK_LINE(294)
	return (int)0;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC0(Coopy_obj,main,return )

Void Coopy_obj::show( ::coopy::Table t){
{
		HX_STACK_FRAME("coopy.Coopy","show",0xbbe7d3e1,"coopy.Coopy.show","coopy/Coopy.hx",298,0x36f081f5)
		HX_STACK_ARG(t,"t")
		HX_STACK_LINE(299)
		int w = t->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(w,"w");
		HX_STACK_LINE(300)
		int h = t->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(h,"h");
		HX_STACK_LINE(301)
		::String txt = HX_CSTRING("");		HX_STACK_VAR(txt,"txt");
		HX_STACK_LINE(302)
		{
			HX_STACK_LINE(302)
			int _g = (int)0;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(302)
			while(((_g < h))){
				HX_STACK_LINE(302)
				int y = (_g)++;		HX_STACK_VAR(y,"y");
				HX_STACK_LINE(303)
				{
					HX_STACK_LINE(303)
					int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
					HX_STACK_LINE(303)
					while(((_g1 < w))){
						HX_STACK_LINE(303)
						int x = (_g1)++;		HX_STACK_VAR(x,"x");
						HX_STACK_LINE(304)
						hx::AddEq(txt,::Std_obj::string(t->getCell(x,y)));
						HX_STACK_LINE(305)
						hx::AddEq(txt,HX_CSTRING(" "));
					}
				}
				HX_STACK_LINE(307)
				hx::AddEq(txt,HX_CSTRING("\n"));
			}
		}
		HX_STACK_LINE(309)
		::haxe::Log_obj::trace(txt,hx::SourceInfo(HX_CSTRING("Coopy.hx"),309,HX_CSTRING("coopy.Coopy"),HX_CSTRING("show")));
	}
return null();
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(Coopy_obj,show,(void))

Dynamic Coopy_obj::jsonify( ::coopy::Table t){
	HX_STACK_FRAME("coopy.Coopy","jsonify",0x653ff470,"coopy.Coopy.jsonify","coopy/Coopy.hx",313,0x36f081f5)
	HX_STACK_ARG(t,"t")
	HX_STACK_LINE(314)
	::haxe::ds::StringMap workbook = ::haxe::ds::StringMap_obj::__new();		HX_STACK_VAR(workbook,"workbook");
	HX_STACK_LINE(315)
	Dynamic sheet = Dynamic( Array_obj<Dynamic>::__new() );		HX_STACK_VAR(sheet,"sheet");
	HX_STACK_LINE(316)
	int w = t->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(w,"w");
	HX_STACK_LINE(317)
	int h = t->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(h,"h");
	HX_STACK_LINE(318)
	::String txt = HX_CSTRING("");		HX_STACK_VAR(txt,"txt");
	HX_STACK_LINE(319)
	{
		HX_STACK_LINE(319)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(319)
		while(((_g < h))){
			HX_STACK_LINE(319)
			int y = (_g)++;		HX_STACK_VAR(y,"y");
			HX_STACK_LINE(320)
			Dynamic row = Dynamic( Array_obj<Dynamic>::__new() );		HX_STACK_VAR(row,"row");
			HX_STACK_LINE(321)
			{
				HX_STACK_LINE(321)
				int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
				HX_STACK_LINE(321)
				while(((_g1 < w))){
					HX_STACK_LINE(321)
					int x = (_g1)++;		HX_STACK_VAR(x,"x");
					HX_STACK_LINE(322)
					Dynamic v = t->getCell(x,y);		HX_STACK_VAR(v,"v");
					HX_STACK_LINE(323)
					if (((v != null()))){
						HX_STACK_LINE(324)
						row->__Field(HX_CSTRING("push"),true)(v->toString());
					}
					else{
						HX_STACK_LINE(326)
						row->__Field(HX_CSTRING("push"),true)(null());
					}
				}
			}
			HX_STACK_LINE(329)
			sheet->__Field(HX_CSTRING("push"),true)(row);
		}
	}
	HX_STACK_LINE(331)
	workbook->set(HX_CSTRING("sheet"),sheet);
	HX_STACK_LINE(332)
	return workbook;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(Coopy_obj,jsonify,return )


Coopy_obj::Coopy_obj()
{
}

void Coopy_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Coopy);
	HX_MARK_MEMBER_NAME(format_preference,"format_preference");
	HX_MARK_MEMBER_NAME(io,"io");
	HX_MARK_MEMBER_NAME(mv,"mv");
	HX_MARK_END_CLASS();
}

void Coopy_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(format_preference,"format_preference");
	HX_VISIT_MEMBER_NAME(io,"io");
	HX_VISIT_MEMBER_NAME(mv,"mv");
}

Dynamic Coopy_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 2:
		if (HX_FIELD_EQ(inName,"io") ) { return io; }
		if (HX_FIELD_EQ(inName,"mv") ) { return mv; }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"main") ) { return main_dyn(); }
		if (HX_FIELD_EQ(inName,"show") ) { return show_dyn(); }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"jsonify") ) { return jsonify_dyn(); }
		break;
	case 11:
		if (HX_FIELD_EQ(inName,"randomTests") ) { return randomTests_dyn(); }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"compareTables") ) { return compareTables_dyn(); }
		break;
	case 14:
		if (HX_FIELD_EQ(inName,"compareTables3") ) { return compareTables3_dyn(); }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"format_preference") ) { return format_preference; }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Coopy_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 2:
		if (HX_FIELD_EQ(inName,"io") ) { io=inValue.Cast< ::coopy::TableIO >(); return inValue; }
		if (HX_FIELD_EQ(inName,"mv") ) { mv=inValue.Cast< ::coopy::Mover >(); return inValue; }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"format_preference") ) { format_preference=inValue.Cast< ::String >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void Coopy_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("format_preference"));
	outFields->push(HX_CSTRING("io"));
	outFields->push(HX_CSTRING("mv"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("compareTables"),
	HX_CSTRING("compareTables3"),
	HX_CSTRING("randomTests"),
	HX_CSTRING("main"),
	HX_CSTRING("show"),
	HX_CSTRING("jsonify"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsString,(int)offsetof(Coopy_obj,format_preference),HX_CSTRING("format_preference")},
	{hx::fsObject,(int)offsetof(Coopy_obj,io),HX_CSTRING("io")},
	{hx::fsObject,(int)offsetof(Coopy_obj,mv),HX_CSTRING("mv")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("format_preference"),
	HX_CSTRING("io"),
	HX_CSTRING("mv"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Coopy_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Coopy_obj::__mClass,"__mClass");
};

#endif

Class Coopy_obj::__mClass;

void Coopy_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Coopy"), hx::TCanCast< Coopy_obj> ,sStaticFields,sMemberFields,
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

void Coopy_obj::__boot()
{
}

} // end namespace coopy
