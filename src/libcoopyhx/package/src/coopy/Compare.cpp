#include <hxcpp.h>

#ifndef INCLUDED_coopy_Change
#include <coopy/Change.h>
#endif
#ifndef INCLUDED_coopy_ChangeType
#include <coopy/ChangeType.h>
#endif
#ifndef INCLUDED_coopy_Compare
#include <coopy/Compare.h>
#endif
#ifndef INCLUDED_coopy_CompareTable
#include <coopy/CompareTable.h>
#endif
#ifndef INCLUDED_coopy_Report
#include <coopy/Report.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableComparisonState
#include <coopy/TableComparisonState.h>
#endif
#ifndef INCLUDED_coopy_ViewedDatum
#include <coopy/ViewedDatum.h>
#endif
#ifndef INCLUDED_coopy_Workspace
#include <coopy/Workspace.h>
#endif
namespace coopy{

Void Compare_obj::__construct()
{
HX_STACK_FRAME("coopy.Compare","new",0x1d5cd595,"coopy.Compare.new","coopy/Compare.hx",7,0x4fa31ebc)
{
}
;
	return null();
}

Compare_obj::~Compare_obj() { }

Dynamic Compare_obj::__CreateEmpty() { return  new Compare_obj; }
hx::ObjectPtr< Compare_obj > Compare_obj::__new()
{  hx::ObjectPtr< Compare_obj > result = new Compare_obj();
	result->__construct();
	return result;}

Dynamic Compare_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Compare_obj > result = new Compare_obj();
	result->__construct();
	return result;}

bool Compare_obj::compare( ::coopy::ViewedDatum parent,::coopy::ViewedDatum local,::coopy::ViewedDatum remote,::coopy::Report report){
	HX_STACK_FRAME("coopy.Compare","compare",0xde155c5a,"coopy.Compare.compare","coopy/Compare.hx",12,0x4fa31ebc)
	HX_STACK_THIS(this)
	HX_STACK_ARG(parent,"parent")
	HX_STACK_ARG(local,"local")
	HX_STACK_ARG(remote,"remote")
	HX_STACK_ARG(report,"report")
	HX_STACK_LINE(13)
	::coopy::Workspace ws = ::coopy::Workspace_obj::__new();		HX_STACK_VAR(ws,"ws");
	HX_STACK_LINE(14)
	ws->parent = parent;
	HX_STACK_LINE(15)
	ws->local = local;
	HX_STACK_LINE(16)
	ws->remote = remote;
	HX_STACK_LINE(17)
	ws->report = report;
	HX_STACK_LINE(18)
	report->clear();
	HX_STACK_LINE(19)
	if (((bool((bool((parent == null())) || bool((local == null())))) || bool((remote == null()))))){
		HX_STACK_LINE(20)
		report->changes->push(::coopy::Change_obj::__new(HX_CSTRING("only 3-way comparison allowed right now")));
		HX_STACK_LINE(21)
		return false;
	}
	HX_STACK_LINE(23)
	if (((bool((bool(parent->hasStructure()) || bool(local->hasStructure()))) || bool(remote->hasStructure())))){
		HX_STACK_LINE(24)
		return this->compareStructured(ws);
	}
	HX_STACK_LINE(26)
	return this->comparePrimitive(ws);
}


HX_DEFINE_DYNAMIC_FUNC4(Compare_obj,compare,return )

bool Compare_obj::compareStructured( ::coopy::Workspace ws){
	HX_STACK_FRAME("coopy.Compare","compareStructured",0x7bdd6dcb,"coopy.Compare.compareStructured","coopy/Compare.hx",29,0x4fa31ebc)
	HX_STACK_THIS(this)
	HX_STACK_ARG(ws,"ws")
	HX_STACK_LINE(30)
	ws->tparent = ws->parent->getTable();
	HX_STACK_LINE(31)
	ws->tlocal = ws->local->getTable();
	HX_STACK_LINE(32)
	ws->tremote = ws->remote->getTable();
	HX_STACK_LINE(33)
	if (((bool((bool((ws->tparent == null())) || bool((ws->tlocal == null())))) || bool((ws->tremote == null()))))){
		HX_STACK_LINE(34)
		ws->report->changes->push(::coopy::Change_obj::__new(HX_CSTRING("structured comparisons that include non-tables are not available yet")));
		HX_STACK_LINE(35)
		return false;
	}
	HX_STACK_LINE(37)
	return this->compareTable(ws);
}


HX_DEFINE_DYNAMIC_FUNC1(Compare_obj,compareStructured,return )

bool Compare_obj::compareTable( ::coopy::Workspace ws){
	HX_STACK_FRAME("coopy.Compare","compareTable",0x652cc554,"coopy.Compare.compareTable","coopy/Compare.hx",40,0x4fa31ebc)
	HX_STACK_THIS(this)
	HX_STACK_ARG(ws,"ws")
	HX_STACK_LINE(41)
	ws->p2l = ::coopy::TableComparisonState_obj::__new();
	HX_STACK_LINE(42)
	ws->p2r = ::coopy::TableComparisonState_obj::__new();
	HX_STACK_LINE(43)
	ws->p2l->a = ws->tparent;
	HX_STACK_LINE(44)
	ws->p2l->b = ws->tlocal;
	HX_STACK_LINE(45)
	ws->p2r->a = ws->tparent;
	HX_STACK_LINE(46)
	ws->p2r->b = ws->tremote;
	HX_STACK_LINE(47)
	::coopy::CompareTable cmp = ::coopy::CompareTable_obj::__new();		HX_STACK_VAR(cmp,"cmp");
	HX_STACK_LINE(48)
	cmp->attach(ws->p2l);
	HX_STACK_LINE(49)
	cmp->attach(ws->p2r);
	HX_STACK_LINE(51)
	::coopy::Change c = ::coopy::Change_obj::__new(null());		HX_STACK_VAR(c,"c");
	HX_STACK_LINE(52)
	c->parent = ws->parent;
	HX_STACK_LINE(53)
	c->local = ws->local;
	HX_STACK_LINE(54)
	c->remote = ws->remote;
	HX_STACK_LINE(55)
	if (((bool(ws->p2l->is_equal) && bool(!(ws->p2r->is_equal))))){
		HX_STACK_LINE(56)
		c->mode = ::coopy::ChangeType_obj::REMOTE_CHANGE;
	}
	else{
		HX_STACK_LINE(57)
		if (((bool(!(ws->p2l->is_equal)) && bool(ws->p2r->is_equal)))){
			HX_STACK_LINE(58)
			c->mode = ::coopy::ChangeType_obj::LOCAL_CHANGE;
		}
		else{
			HX_STACK_LINE(59)
			if (((bool(!(ws->p2l->is_equal)) && bool(!(ws->p2r->is_equal))))){
				HX_STACK_LINE(61)
				ws->l2r = ::coopy::TableComparisonState_obj::__new();
				HX_STACK_LINE(62)
				ws->l2r->a = ws->tlocal;
				HX_STACK_LINE(63)
				ws->l2r->b = ws->tremote;
				HX_STACK_LINE(64)
				cmp->attach(ws->l2r);
				HX_STACK_LINE(65)
				if ((ws->l2r->is_equal)){
					HX_STACK_LINE(66)
					c->mode = ::coopy::ChangeType_obj::SAME_CHANGE;
				}
				else{
					HX_STACK_LINE(68)
					c->mode = ::coopy::ChangeType_obj::BOTH_CHANGE;
				}
			}
			else{
				HX_STACK_LINE(71)
				c->mode = ::coopy::ChangeType_obj::NO_CHANGE;
			}
		}
	}
	HX_STACK_LINE(73)
	if (((c->mode != ::coopy::ChangeType_obj::NO_CHANGE))){
		HX_STACK_LINE(74)
		ws->report->changes->push(c);
	}
	HX_STACK_LINE(76)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC1(Compare_obj,compareTable,return )

bool Compare_obj::comparePrimitive( ::coopy::Workspace ws){
	HX_STACK_FRAME("coopy.Compare","comparePrimitive",0x5db8b0ed,"coopy.Compare.comparePrimitive","coopy/Compare.hx",79,0x4fa31ebc)
	HX_STACK_THIS(this)
	HX_STACK_ARG(ws,"ws")
	HX_STACK_LINE(81)
	::String sparent = ws->parent->toString();		HX_STACK_VAR(sparent,"sparent");
	HX_STACK_LINE(82)
	::String slocal = ws->local->toString();		HX_STACK_VAR(slocal,"slocal");
	HX_STACK_LINE(83)
	::String sremote = ws->remote->toString();		HX_STACK_VAR(sremote,"sremote");
	HX_STACK_LINE(84)
	::coopy::Change c = ::coopy::Change_obj::__new(null());		HX_STACK_VAR(c,"c");
	HX_STACK_LINE(85)
	c->parent = ws->parent;
	HX_STACK_LINE(86)
	c->local = ws->local;
	HX_STACK_LINE(87)
	c->remote = ws->remote;
	HX_STACK_LINE(88)
	if (((bool((sparent == slocal)) && bool((sparent != sremote))))){
		HX_STACK_LINE(89)
		c->mode = ::coopy::ChangeType_obj::REMOTE_CHANGE;
	}
	else{
		HX_STACK_LINE(90)
		if (((bool((sparent == sremote)) && bool((sparent != slocal))))){
			HX_STACK_LINE(91)
			c->mode = ::coopy::ChangeType_obj::LOCAL_CHANGE;
		}
		else{
			HX_STACK_LINE(92)
			if (((bool((slocal == sremote)) && bool((sparent != slocal))))){
				HX_STACK_LINE(93)
				c->mode = ::coopy::ChangeType_obj::SAME_CHANGE;
			}
			else{
				HX_STACK_LINE(94)
				if (((bool((sparent != slocal)) && bool((sparent != sremote))))){
					HX_STACK_LINE(95)
					c->mode = ::coopy::ChangeType_obj::BOTH_CHANGE;
				}
				else{
					HX_STACK_LINE(97)
					c->mode = ::coopy::ChangeType_obj::NO_CHANGE;
				}
			}
		}
	}
	HX_STACK_LINE(99)
	if (((c->mode != ::coopy::ChangeType_obj::NO_CHANGE))){
		HX_STACK_LINE(100)
		ws->report->changes->push(c);
	}
	HX_STACK_LINE(102)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC1(Compare_obj,comparePrimitive,return )


Compare_obj::Compare_obj()
{
}

void Compare_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Compare);
	HX_MARK_END_CLASS();
}

void Compare_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic Compare_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 7:
		if (HX_FIELD_EQ(inName,"compare") ) { return compare_dyn(); }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"compareTable") ) { return compareTable_dyn(); }
		break;
	case 16:
		if (HX_FIELD_EQ(inName,"comparePrimitive") ) { return comparePrimitive_dyn(); }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"compareStructured") ) { return compareStructured_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Compare_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void Compare_obj::__GetFields(Array< ::String> &outFields)
{
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo *sMemberStorageInfo = 0;
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("compare"),
	HX_CSTRING("compareStructured"),
	HX_CSTRING("compareTable"),
	HX_CSTRING("comparePrimitive"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Compare_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Compare_obj::__mClass,"__mClass");
};

#endif

Class Compare_obj::__mClass;

void Compare_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Compare"), hx::TCanCast< Compare_obj> ,sStaticFields,sMemberFields,
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

void Compare_obj::__boot()
{
}

} // end namespace coopy
