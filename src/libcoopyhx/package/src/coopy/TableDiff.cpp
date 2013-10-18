#include <hxcpp.h>

#ifndef INCLUDED_IMap
#include <IMap.h>
#endif
#ifndef INCLUDED_coopy_Alignment
#include <coopy/Alignment.h>
#endif
#ifndef INCLUDED_coopy_Change
#include <coopy/Change.h>
#endif
#ifndef INCLUDED_coopy_CompareFlags
#include <coopy/CompareFlags.h>
#endif
#ifndef INCLUDED_coopy_Mover
#include <coopy/Mover.h>
#endif
#ifndef INCLUDED_coopy_Ordering
#include <coopy/Ordering.h>
#endif
#ifndef INCLUDED_coopy_Report
#include <coopy/Report.h>
#endif
#ifndef INCLUDED_coopy_Table
#include <coopy/Table.h>
#endif
#ifndef INCLUDED_coopy_TableDiff
#include <coopy/TableDiff.h>
#endif
#ifndef INCLUDED_coopy_Unit
#include <coopy/Unit.h>
#endif
#ifndef INCLUDED_coopy_View
#include <coopy/View.h>
#endif
#ifndef INCLUDED_haxe_ds_IntMap
#include <haxe/ds/IntMap.h>
#endif
namespace coopy{

Void TableDiff_obj::__construct(::coopy::Alignment align,::coopy::CompareFlags flags)
{
HX_STACK_FRAME("coopy.TableDiff","new",0x9c7656a3,"coopy.TableDiff.new","coopy/TableDiff.hx",12,0xda8b0bae)

HX_STACK_ARG(align,"align")

HX_STACK_ARG(flags,"flags")
{
	HX_STACK_LINE(13)
	this->align = align;
	HX_STACK_LINE(14)
	this->flags = flags;
}
;
	return null();
}

TableDiff_obj::~TableDiff_obj() { }

Dynamic TableDiff_obj::__CreateEmpty() { return  new TableDiff_obj; }
hx::ObjectPtr< TableDiff_obj > TableDiff_obj::__new(::coopy::Alignment align,::coopy::CompareFlags flags)
{  hx::ObjectPtr< TableDiff_obj > result = new TableDiff_obj();
	result->__construct(align,flags);
	return result;}

Dynamic TableDiff_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< TableDiff_obj > result = new TableDiff_obj();
	result->__construct(inArgs[0],inArgs[1]);
	return result;}

::String TableDiff_obj::getSeparator( ::coopy::Table t,::coopy::Table t2,::String root){
	HX_STACK_FRAME("coopy.TableDiff","getSeparator",0x5eb0164c,"coopy.TableDiff.getSeparator","coopy/TableDiff.hx",18,0xda8b0bae)
	HX_STACK_THIS(this)
	HX_STACK_ARG(t,"t")
	HX_STACK_ARG(t2,"t2")
	HX_STACK_ARG(root,"root")
	HX_STACK_LINE(19)
	::String sep = root;		HX_STACK_VAR(sep,"sep");
	HX_STACK_LINE(20)
	int w = t->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(w,"w");
	HX_STACK_LINE(21)
	int h = t->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(h,"h");
	HX_STACK_LINE(22)
	::coopy::View view = t->getCellView();		HX_STACK_VAR(view,"view");
	HX_STACK_LINE(23)
	{
		HX_STACK_LINE(23)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(23)
		while(((_g < h))){
			HX_STACK_LINE(23)
			int y = (_g)++;		HX_STACK_VAR(y,"y");
			HX_STACK_LINE(24)
			{
				HX_STACK_LINE(24)
				int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
				HX_STACK_LINE(24)
				while(((_g1 < w))){
					HX_STACK_LINE(24)
					int x = (_g1)++;		HX_STACK_VAR(x,"x");
					HX_STACK_LINE(25)
					::String txt = view->toString(t->getCell(x,y));		HX_STACK_VAR(txt,"txt");
					HX_STACK_LINE(26)
					if (((txt == null()))){
						HX_STACK_LINE(26)
						continue;
					}
					HX_STACK_LINE(27)
					while(((txt.indexOf(sep,null()) >= (int)0))){
						HX_STACK_LINE(28)
						sep = (HX_CSTRING("-") + sep);
					}
				}
			}
		}
	}
	HX_STACK_LINE(32)
	if (((t2 != null()))){
		HX_STACK_LINE(33)
		w = t2->__Field(HX_CSTRING("get_width"),true)();
		HX_STACK_LINE(34)
		h = t2->__Field(HX_CSTRING("get_height"),true)();
		HX_STACK_LINE(35)
		{
			HX_STACK_LINE(35)
			int _g = (int)0;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(35)
			while(((_g < h))){
				HX_STACK_LINE(35)
				int y = (_g)++;		HX_STACK_VAR(y,"y");
				HX_STACK_LINE(36)
				{
					HX_STACK_LINE(36)
					int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
					HX_STACK_LINE(36)
					while(((_g1 < w))){
						HX_STACK_LINE(36)
						int x = (_g1)++;		HX_STACK_VAR(x,"x");
						HX_STACK_LINE(37)
						::String txt = view->toString(t2->getCell(x,y));		HX_STACK_VAR(txt,"txt");
						HX_STACK_LINE(38)
						if (((txt == null()))){
							HX_STACK_LINE(38)
							continue;
						}
						HX_STACK_LINE(39)
						while(((txt.indexOf(sep,null()) >= (int)0))){
							HX_STACK_LINE(40)
							sep = (HX_CSTRING("-") + sep);
						}
					}
				}
			}
		}
	}
	HX_STACK_LINE(45)
	return sep;
}


HX_DEFINE_DYNAMIC_FUNC3(TableDiff_obj,getSeparator,return )

::String TableDiff_obj::quoteForDiff( ::coopy::View v,Dynamic d){
	HX_STACK_FRAME("coopy.TableDiff","quoteForDiff",0x2101452f,"coopy.TableDiff.quoteForDiff","coopy/TableDiff.hx",48,0xda8b0bae)
	HX_STACK_THIS(this)
	HX_STACK_ARG(v,"v")
	HX_STACK_ARG(d,"d")
	HX_STACK_LINE(49)
	::String nil = HX_CSTRING("NULL");		HX_STACK_VAR(nil,"nil");
	HX_STACK_LINE(50)
	if ((v->equals(d,null()))){
		HX_STACK_LINE(51)
		return nil;
	}
	HX_STACK_LINE(53)
	::String str = v->toString(d);		HX_STACK_VAR(str,"str");
	HX_STACK_LINE(54)
	int score = (int)0;		HX_STACK_VAR(score,"score");
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
			if (((str.charCodeAt(score) != (int)95))){
				HX_STACK_LINE(56)
				break;
			}
			HX_STACK_LINE(57)
			(score)++;
		}
	}
	HX_STACK_LINE(59)
	if (((str.substr(score,null()) == nil))){
		HX_STACK_LINE(60)
		str = (HX_CSTRING("_") + str);
	}
	HX_STACK_LINE(62)
	return str;
}


HX_DEFINE_DYNAMIC_FUNC2(TableDiff_obj,quoteForDiff,return )

bool TableDiff_obj::isReordered( ::haxe::ds::IntMap m,int ct){
	HX_STACK_FRAME("coopy.TableDiff","isReordered",0xf2d88913,"coopy.TableDiff.isReordered","coopy/TableDiff.hx",65,0xda8b0bae)
	HX_STACK_THIS(this)
	HX_STACK_ARG(m,"m")
	HX_STACK_ARG(ct,"ct")
	HX_STACK_LINE(66)
	bool reordered = false;		HX_STACK_VAR(reordered,"reordered");
	HX_STACK_LINE(67)
	int l = (int)-1;		HX_STACK_VAR(l,"l");
	HX_STACK_LINE(68)
	int r = (int)-1;		HX_STACK_VAR(r,"r");
	HX_STACK_LINE(69)
	{
		HX_STACK_LINE(69)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(69)
		while(((_g < ct))){
			HX_STACK_LINE(69)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(70)
			::coopy::Unit unit = m->get(i);		HX_STACK_VAR(unit,"unit");
			HX_STACK_LINE(71)
			if (((unit == null()))){
				HX_STACK_LINE(71)
				continue;
			}
			HX_STACK_LINE(72)
			if (((unit->l >= (int)0))){
				HX_STACK_LINE(73)
				if (((unit->l < l))){
					HX_STACK_LINE(74)
					reordered = true;
					HX_STACK_LINE(75)
					break;
				}
				HX_STACK_LINE(77)
				l = unit->l;
			}
			HX_STACK_LINE(79)
			if (((unit->r >= (int)0))){
				HX_STACK_LINE(80)
				if (((unit->r < r))){
					HX_STACK_LINE(81)
					reordered = true;
					HX_STACK_LINE(82)
					break;
				}
				HX_STACK_LINE(84)
				r = unit->r;
			}
		}
	}
	HX_STACK_LINE(87)
	return reordered;
}


HX_DEFINE_DYNAMIC_FUNC2(TableDiff_obj,isReordered,return )

::String TableDiff_obj::reportUnit( ::coopy::Unit unit){
	HX_STACK_FRAME("coopy.TableDiff","reportUnit",0xec03b575,"coopy.TableDiff.reportUnit","coopy/TableDiff.hx",90,0xda8b0bae)
	HX_STACK_THIS(this)
	HX_STACK_ARG(unit,"unit")
	HX_STACK_LINE(91)
	::String txt = unit->toString();		HX_STACK_VAR(txt,"txt");
	HX_STACK_LINE(92)
	bool reordered = false;		HX_STACK_VAR(reordered,"reordered");
	HX_STACK_LINE(93)
	if (((unit->l >= (int)0))){
		HX_STACK_LINE(94)
		if (((unit->l < this->l_prev))){
			HX_STACK_LINE(95)
			reordered = true;
		}
		HX_STACK_LINE(97)
		this->l_prev = unit->l;
	}
	HX_STACK_LINE(99)
	if (((unit->r >= (int)0))){
		HX_STACK_LINE(100)
		if (((unit->r < this->r_prev))){
			HX_STACK_LINE(101)
			reordered = true;
		}
		HX_STACK_LINE(103)
		this->r_prev = unit->r;
	}
	HX_STACK_LINE(105)
	if ((reordered)){
		HX_STACK_LINE(105)
		txt = ((HX_CSTRING("[") + txt) + HX_CSTRING("]"));
	}
	HX_STACK_LINE(106)
	return txt;
}


HX_DEFINE_DYNAMIC_FUNC1(TableDiff_obj,reportUnit,return )

bool TableDiff_obj::hilite( ::coopy::Table output){
	HX_STACK_FRAME("coopy.TableDiff","hilite",0x35e2124c,"coopy.TableDiff.hilite","coopy/TableDiff.hx",109,0xda8b0bae)
	HX_STACK_THIS(this)
	HX_STACK_ARG(output,"output")
	HX_STACK_LINE(110)
	if ((!(output->isResizable()))){
		HX_STACK_LINE(110)
		return false;
	}
	HX_STACK_LINE(111)
	output->resize((int)0,(int)0);
	HX_STACK_LINE(112)
	output->clear();
	HX_STACK_LINE(114)
	::haxe::ds::IntMap row_map = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(row_map,"row_map");
	HX_STACK_LINE(115)
	::haxe::ds::IntMap col_map = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(col_map,"col_map");
	HX_STACK_LINE(117)
	::coopy::Ordering order = this->align->toOrder();		HX_STACK_VAR(order,"order");
	HX_STACK_LINE(118)
	Array< ::Dynamic > units = order->getList();		HX_STACK_VAR(units,"units");
	HX_STACK_LINE(119)
	bool has_parent = (this->align->reference != null());		HX_STACK_VAR(has_parent,"has_parent");
	HX_STACK_LINE(120)
	::coopy::Table a;		HX_STACK_VAR(a,"a");
	HX_STACK_LINE(121)
	::coopy::Table b;		HX_STACK_VAR(b,"b");
	HX_STACK_LINE(122)
	::coopy::Table p;		HX_STACK_VAR(p,"p");
	HX_STACK_LINE(123)
	int ra_header = (int)0;		HX_STACK_VAR(ra_header,"ra_header");
	HX_STACK_LINE(124)
	int rb_header = (int)0;		HX_STACK_VAR(rb_header,"rb_header");
	HX_STACK_LINE(125)
	if ((has_parent)){
		HX_STACK_LINE(126)
		p = this->align->getSource();
		HX_STACK_LINE(127)
		a = this->align->reference->getTarget();
		HX_STACK_LINE(128)
		b = this->align->getTarget();
		HX_STACK_LINE(129)
		ra_header = this->align->reference->meta->getTargetHeader();
		HX_STACK_LINE(130)
		rb_header = this->align->meta->getTargetHeader();
	}
	else{
		HX_STACK_LINE(132)
		a = this->align->getSource();
		HX_STACK_LINE(133)
		b = this->align->getTarget();
		HX_STACK_LINE(134)
		p = a;
		HX_STACK_LINE(135)
		ra_header = this->align->meta->getSourceHeader();
		HX_STACK_LINE(136)
		rb_header = this->align->meta->getTargetHeader();
	}
	HX_STACK_LINE(139)
	::coopy::Ordering column_order = this->align->meta->toOrder();		HX_STACK_VAR(column_order,"column_order");
	HX_STACK_LINE(140)
	Array< ::Dynamic > column_units = column_order->getList();		HX_STACK_VAR(column_units,"column_units");
	HX_STACK_LINE(142)
	bool show_rc_numbers = false;		HX_STACK_VAR(show_rc_numbers,"show_rc_numbers");
	HX_STACK_LINE(143)
	::haxe::ds::IntMap row_moves = null();		HX_STACK_VAR(row_moves,"row_moves");
	HX_STACK_LINE(144)
	::haxe::ds::IntMap col_moves = null();		HX_STACK_VAR(col_moves,"col_moves");
	HX_STACK_LINE(145)
	if ((this->flags->ordered)){
		HX_STACK_LINE(146)
		row_moves = ::haxe::ds::IntMap_obj::__new();
		HX_STACK_LINE(147)
		Array< int > moves = ::coopy::Mover_obj::moveUnits(units);		HX_STACK_VAR(moves,"moves");
		HX_STACK_LINE(148)
		{
			HX_STACK_LINE(148)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(148)
			int _g = moves->length;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(148)
			while(((_g1 < _g))){
				HX_STACK_LINE(148)
				int i = (_g1)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(149)
				{
					HX_STACK_LINE(149)
					row_moves->set(moves->__get(i),i);
					HX_STACK_LINE(149)
					i;
				}
			}
		}
		HX_STACK_LINE(151)
		col_moves = ::haxe::ds::IntMap_obj::__new();
		HX_STACK_LINE(152)
		moves = ::coopy::Mover_obj::moveUnits(column_units);
		HX_STACK_LINE(153)
		{
			HX_STACK_LINE(153)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(153)
			int _g = moves->length;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(153)
			while(((_g1 < _g))){
				HX_STACK_LINE(153)
				int i = (_g1)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(154)
				{
					HX_STACK_LINE(154)
					col_moves->set(moves->__get(i),i);
					HX_STACK_LINE(154)
					i;
				}
			}
		}
	}
	HX_STACK_LINE(158)
	int outer_reps_needed;		HX_STACK_VAR(outer_reps_needed,"outer_reps_needed");
	HX_STACK_LINE(158)
	if ((this->flags->show_unchanged)){
		HX_STACK_LINE(158)
		outer_reps_needed = (int)1;
	}
	else{
		HX_STACK_LINE(158)
		outer_reps_needed = (int)2;
	}
	HX_STACK_LINE(160)
	::coopy::View v = a->getCellView();		HX_STACK_VAR(v,"v");
	HX_STACK_LINE(161)
	::String sep = HX_CSTRING("");		HX_STACK_VAR(sep,"sep");
	HX_STACK_LINE(162)
	::String conflict_sep = HX_CSTRING("");		HX_STACK_VAR(conflict_sep,"conflict_sep");
	HX_STACK_LINE(164)
	Array< ::String > schema = Array_obj< ::String >::__new();		HX_STACK_VAR(schema,"schema");
	HX_STACK_LINE(165)
	bool have_schema = false;		HX_STACK_VAR(have_schema,"have_schema");
	HX_STACK_LINE(166)
	{
		HX_STACK_LINE(166)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(166)
		int _g = column_units->length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(166)
		while(((_g1 < _g))){
			HX_STACK_LINE(166)
			int j = (_g1)++;		HX_STACK_VAR(j,"j");
			HX_STACK_LINE(167)
			::coopy::Unit cunit = column_units->__get(j).StaticCast< ::coopy::Unit >();		HX_STACK_VAR(cunit,"cunit");
			HX_STACK_LINE(168)
			bool reordered = false;		HX_STACK_VAR(reordered,"reordered");
			HX_STACK_LINE(170)
			if ((this->flags->ordered)){
				HX_STACK_LINE(171)
				if ((col_moves->exists(j))){
					HX_STACK_LINE(172)
					reordered = true;
				}
				HX_STACK_LINE(174)
				if ((reordered)){
					HX_STACK_LINE(174)
					show_rc_numbers = true;
				}
			}
			HX_STACK_LINE(177)
			::String act = HX_CSTRING("");		HX_STACK_VAR(act,"act");
			HX_STACK_LINE(178)
			if (((bool((cunit->r >= (int)0)) && bool((cunit->lp() == (int)-1))))){
				HX_STACK_LINE(179)
				have_schema = true;
				HX_STACK_LINE(180)
				act = HX_CSTRING("+++");
			}
			HX_STACK_LINE(182)
			if (((bool((cunit->r < (int)0)) && bool((cunit->lp() >= (int)0))))){
				HX_STACK_LINE(183)
				have_schema = true;
				HX_STACK_LINE(184)
				act = HX_CSTRING("---");
			}
			HX_STACK_LINE(186)
			if (((bool((cunit->r >= (int)0)) && bool((cunit->lp() >= (int)0))))){
				HX_STACK_LINE(187)
				if (((bool((a->__Field(HX_CSTRING("get_height"),true)() >= ra_header)) && bool((b->__Field(HX_CSTRING("get_height"),true)() >= rb_header))))){
					HX_STACK_LINE(188)
					Dynamic aa = a->getCell(cunit->lp(),ra_header);		HX_STACK_VAR(aa,"aa");
					HX_STACK_LINE(189)
					Dynamic bb = b->getCell(cunit->r,rb_header);		HX_STACK_VAR(bb,"bb");
					HX_STACK_LINE(190)
					if ((!(v->equals(aa,bb)))){
						HX_STACK_LINE(191)
						have_schema = true;
						HX_STACK_LINE(192)
						act = HX_CSTRING("(");
						HX_STACK_LINE(193)
						hx::AddEq(act,v->toString(aa));
						HX_STACK_LINE(194)
						hx::AddEq(act,HX_CSTRING(")"));
					}
				}
			}
			HX_STACK_LINE(198)
			if ((reordered)){
				HX_STACK_LINE(199)
				act = (HX_CSTRING(":") + act);
				HX_STACK_LINE(200)
				have_schema = true;
			}
			HX_STACK_LINE(203)
			schema->push(act);
		}
	}
	HX_STACK_LINE(205)
	if ((have_schema)){
		HX_STACK_LINE(206)
		int at = output->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(at,"at");
		HX_STACK_LINE(207)
		output->resize((column_units->length + (int)1),(at + (int)1));
		HX_STACK_LINE(208)
		output->setCell((int)0,at,v->toDatum(HX_CSTRING("!")));
		HX_STACK_LINE(209)
		{
			HX_STACK_LINE(209)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(209)
			int _g = column_units->length;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(209)
			while(((_g1 < _g))){
				HX_STACK_LINE(209)
				int j = (_g1)++;		HX_STACK_VAR(j,"j");
				HX_STACK_LINE(210)
				output->setCell((j + (int)1),at,v->toDatum(schema->__get(j)));
			}
		}
	}
	HX_STACK_LINE(214)
	bool top_line_done = false;		HX_STACK_VAR(top_line_done,"top_line_done");
	HX_STACK_LINE(215)
	if ((this->flags->always_show_header)){
		HX_STACK_LINE(216)
		int at = output->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(at,"at");
		HX_STACK_LINE(217)
		output->resize((column_units->length + (int)1),(at + (int)1));
		HX_STACK_LINE(218)
		output->setCell((int)0,at,v->toDatum(HX_CSTRING("@@")));
		HX_STACK_LINE(219)
		{
			HX_STACK_LINE(219)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(219)
			int _g = column_units->length;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(219)
			while(((_g1 < _g))){
				HX_STACK_LINE(219)
				int j = (_g1)++;		HX_STACK_VAR(j,"j");
				HX_STACK_LINE(220)
				::coopy::Unit cunit = column_units->__get(j).StaticCast< ::coopy::Unit >();		HX_STACK_VAR(cunit,"cunit");
				HX_STACK_LINE(221)
				if (((cunit->r >= (int)0))){
					HX_STACK_LINE(222)
					if (((b->__Field(HX_CSTRING("get_height"),true)() > (int)0))){
						HX_STACK_LINE(223)
						output->setCell((j + (int)1),at,b->getCell(cunit->r,rb_header));
					}
				}
				else{
					HX_STACK_LINE(226)
					if (((cunit->lp() >= (int)0))){
						HX_STACK_LINE(227)
						if (((a->__Field(HX_CSTRING("get_height"),true)() > (int)0))){
							HX_STACK_LINE(228)
							output->setCell((j + (int)1),at,a->getCell(cunit->lp(),ra_header));
						}
					}
				}
				HX_STACK_LINE(232)
				col_map->set((j + (int)1),cunit);
			}
		}
		HX_STACK_LINE(234)
		top_line_done = true;
	}
	HX_STACK_LINE(237)
	Array< int > active = Array_obj< int >::__new();		HX_STACK_VAR(active,"active");
	HX_STACK_LINE(238)
	if ((!(this->flags->show_unchanged))){
		HX_STACK_LINE(239)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(239)
		int _g = units->length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(239)
		while(((_g1 < _g))){
			HX_STACK_LINE(239)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(240)
			active[i] = (int)0;
		}
	}
	HX_STACK_LINE(245)
	{
		HX_STACK_LINE(245)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(245)
		while(((_g < outer_reps_needed))){
			HX_STACK_LINE(245)
			int out = (_g)++;		HX_STACK_VAR(out,"out");
			HX_STACK_LINE(246)
			if (((out == (int)1))){
				HX_STACK_LINE(247)
				int del = this->flags->unchanged_context;		HX_STACK_VAR(del,"del");
				HX_STACK_LINE(248)
				if (((del > (int)0))){
					HX_STACK_LINE(250)
					int mark = (-(del) - (int)1);		HX_STACK_VAR(mark,"mark");
					HX_STACK_LINE(251)
					{
						HX_STACK_LINE(251)
						int _g2 = (int)0;		HX_STACK_VAR(_g2,"_g2");
						HX_STACK_LINE(251)
						int _g1 = units->length;		HX_STACK_VAR(_g1,"_g1");
						HX_STACK_LINE(251)
						while(((_g2 < _g1))){
							HX_STACK_LINE(251)
							int i = (_g2)++;		HX_STACK_VAR(i,"i");
							HX_STACK_LINE(252)
							if (((bool((active->__get(i) == (int)0)) || bool((active->__get(i) == (int)3))))){
								HX_STACK_LINE(253)
								if ((((i - mark) <= del))){
									HX_STACK_LINE(254)
									active[i] = (int)2;
								}
								else{
									HX_STACK_LINE(255)
									if ((((i - mark) == (del + (int)1)))){
										HX_STACK_LINE(256)
										active[i] = (int)3;
									}
								}
							}
							else{
								HX_STACK_LINE(258)
								if (((active->__get(i) == (int)1))){
									HX_STACK_LINE(259)
									mark = i;
								}
							}
						}
					}
					HX_STACK_LINE(264)
					mark = ((units->length + del) + (int)1);
					HX_STACK_LINE(265)
					{
						HX_STACK_LINE(265)
						int _g2 = (int)0;		HX_STACK_VAR(_g2,"_g2");
						HX_STACK_LINE(265)
						int _g1 = units->length;		HX_STACK_VAR(_g1,"_g1");
						HX_STACK_LINE(265)
						while(((_g2 < _g1))){
							HX_STACK_LINE(265)
							int j = (_g2)++;		HX_STACK_VAR(j,"j");
							HX_STACK_LINE(266)
							int i = ((units->length - (int)1) - j);		HX_STACK_VAR(i,"i");
							HX_STACK_LINE(267)
							if (((bool((active->__get(i) == (int)0)) || bool((active->__get(i) == (int)3))))){
								HX_STACK_LINE(268)
								if ((((mark - i) <= del))){
									HX_STACK_LINE(269)
									active[i] = (int)2;
								}
								else{
									HX_STACK_LINE(270)
									if ((((mark - i) == (del + (int)1)))){
										HX_STACK_LINE(271)
										active[i] = (int)3;
									}
								}
							}
							else{
								HX_STACK_LINE(273)
								if (((active->__get(i) == (int)1))){
									HX_STACK_LINE(274)
									mark = i;
								}
							}
						}
					}
				}
			}
			HX_STACK_LINE(279)
			bool showed_dummy = false;		HX_STACK_VAR(showed_dummy,"showed_dummy");
			HX_STACK_LINE(280)
			int l = (int)-1;		HX_STACK_VAR(l,"l");
			HX_STACK_LINE(281)
			int r = (int)-1;		HX_STACK_VAR(r,"r");
			HX_STACK_LINE(282)
			{
				HX_STACK_LINE(282)
				int _g2 = (int)0;		HX_STACK_VAR(_g2,"_g2");
				HX_STACK_LINE(282)
				int _g1 = units->length;		HX_STACK_VAR(_g1,"_g1");
				HX_STACK_LINE(282)
				while(((_g2 < _g1))){
					HX_STACK_LINE(282)
					int i = (_g2)++;		HX_STACK_VAR(i,"i");
					HX_STACK_LINE(283)
					::coopy::Unit unit = units->__get(i).StaticCast< ::coopy::Unit >();		HX_STACK_VAR(unit,"unit");
					HX_STACK_LINE(284)
					bool reordered = false;		HX_STACK_VAR(reordered,"reordered");
					HX_STACK_LINE(286)
					if ((this->flags->ordered)){
						HX_STACK_LINE(287)
						if ((row_moves->exists(i))){
							HX_STACK_LINE(288)
							reordered = true;
						}
						HX_STACK_LINE(290)
						if ((reordered)){
							HX_STACK_LINE(290)
							show_rc_numbers = true;
						}
					}
					HX_STACK_LINE(293)
					if (((bool((unit->r < (int)0)) && bool((unit->l < (int)0))))){
						HX_STACK_LINE(293)
						continue;
					}
					HX_STACK_LINE(295)
					if (((bool((bool((unit->r == (int)0)) && bool((unit->lp() == (int)0)))) && bool(top_line_done)))){
						HX_STACK_LINE(295)
						continue;
					}
					HX_STACK_LINE(297)
					::String act = HX_CSTRING("");		HX_STACK_VAR(act,"act");
					HX_STACK_LINE(299)
					if ((reordered)){
						HX_STACK_LINE(299)
						act = HX_CSTRING(":");
					}
					HX_STACK_LINE(301)
					bool publish = this->flags->show_unchanged;		HX_STACK_VAR(publish,"publish");
					HX_STACK_LINE(302)
					bool dummy = false;		HX_STACK_VAR(dummy,"dummy");
					HX_STACK_LINE(303)
					if (((out == (int)1))){
						HX_STACK_LINE(304)
						publish = (active->__get(i) > (int)0);
						HX_STACK_LINE(305)
						dummy = (active->__get(i) == (int)3);
						HX_STACK_LINE(306)
						if (((bool(dummy) && bool(showed_dummy)))){
							HX_STACK_LINE(306)
							continue;
						}
						HX_STACK_LINE(307)
						if ((!(publish))){
							HX_STACK_LINE(307)
							continue;
						}
					}
					HX_STACK_LINE(310)
					if ((!(dummy))){
						HX_STACK_LINE(310)
						showed_dummy = false;
					}
					HX_STACK_LINE(312)
					int at = output->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(at,"at");
					HX_STACK_LINE(313)
					if ((publish)){
						HX_STACK_LINE(314)
						output->resize((column_units->length + (int)1),(at + (int)1));
					}
					HX_STACK_LINE(317)
					if ((dummy)){
						HX_STACK_LINE(318)
						{
							HX_STACK_LINE(318)
							int _g4 = (int)0;		HX_STACK_VAR(_g4,"_g4");
							HX_STACK_LINE(318)
							int _g3 = (column_units->length + (int)1);		HX_STACK_VAR(_g3,"_g3");
							HX_STACK_LINE(318)
							while(((_g4 < _g3))){
								HX_STACK_LINE(318)
								int j = (_g4)++;		HX_STACK_VAR(j,"j");
								HX_STACK_LINE(319)
								output->setCell(j,at,v->toDatum(HX_CSTRING("...")));
								HX_STACK_LINE(320)
								showed_dummy = true;
							}
						}
						HX_STACK_LINE(322)
						continue;
					}
					HX_STACK_LINE(325)
					bool have_addition = false;		HX_STACK_VAR(have_addition,"have_addition");
					HX_STACK_LINE(327)
					if (((bool((bool((unit->p < (int)0)) && bool((unit->l < (int)0)))) && bool((unit->r >= (int)0))))){
						HX_STACK_LINE(328)
						act = HX_CSTRING("+++");
					}
					HX_STACK_LINE(330)
					if (((bool((bool(((bool((unit->p >= (int)0)) || bool(!(has_parent))))) && bool((unit->l >= (int)0)))) && bool((unit->r < (int)0))))){
						HX_STACK_LINE(331)
						act = HX_CSTRING("---");
					}
					HX_STACK_LINE(334)
					{
						HX_STACK_LINE(334)
						int _g4 = (int)0;		HX_STACK_VAR(_g4,"_g4");
						HX_STACK_LINE(334)
						int _g3 = column_units->length;		HX_STACK_VAR(_g3,"_g3");
						HX_STACK_LINE(334)
						while(((_g4 < _g3))){
							HX_STACK_LINE(334)
							int j = (_g4)++;		HX_STACK_VAR(j,"j");
							HX_STACK_LINE(335)
							::coopy::Unit cunit = column_units->__get(j).StaticCast< ::coopy::Unit >();		HX_STACK_VAR(cunit,"cunit");
							HX_STACK_LINE(336)
							Dynamic pp = null();		HX_STACK_VAR(pp,"pp");
							HX_STACK_LINE(337)
							Dynamic ll = null();		HX_STACK_VAR(ll,"ll");
							HX_STACK_LINE(338)
							Dynamic rr = null();		HX_STACK_VAR(rr,"rr");
							HX_STACK_LINE(339)
							Dynamic dd = null();		HX_STACK_VAR(dd,"dd");
							HX_STACK_LINE(340)
							Dynamic dd_to = null();		HX_STACK_VAR(dd_to,"dd_to");
							HX_STACK_LINE(341)
							bool have_dd_to = false;		HX_STACK_VAR(have_dd_to,"have_dd_to");
							HX_STACK_LINE(342)
							Dynamic dd_to_alt = null();		HX_STACK_VAR(dd_to_alt,"dd_to_alt");
							HX_STACK_LINE(343)
							bool have_dd_to_alt = false;		HX_STACK_VAR(have_dd_to_alt,"have_dd_to_alt");
							HX_STACK_LINE(344)
							bool have_pp = false;		HX_STACK_VAR(have_pp,"have_pp");
							HX_STACK_LINE(345)
							bool have_ll = false;		HX_STACK_VAR(have_ll,"have_ll");
							HX_STACK_LINE(346)
							bool have_rr = false;		HX_STACK_VAR(have_rr,"have_rr");
							HX_STACK_LINE(347)
							if (((bool((cunit->p >= (int)0)) && bool((unit->p >= (int)0))))){
								HX_STACK_LINE(348)
								pp = p->getCell(cunit->p,unit->p);
								HX_STACK_LINE(349)
								have_pp = true;
							}
							HX_STACK_LINE(351)
							if (((bool((cunit->l >= (int)0)) && bool((unit->l >= (int)0))))){
								HX_STACK_LINE(352)
								ll = a->getCell(cunit->l,unit->l);
								HX_STACK_LINE(353)
								have_ll = true;
							}
							HX_STACK_LINE(355)
							if (((bool((cunit->r >= (int)0)) && bool((unit->r >= (int)0))))){
								HX_STACK_LINE(356)
								rr = b->getCell(cunit->r,unit->r);
								HX_STACK_LINE(357)
								have_rr = true;
								HX_STACK_LINE(358)
								if (((((  ((have_pp)) ? int(cunit->p) : int(cunit->l) )) < (int)0))){
									HX_STACK_LINE(359)
									if (((rr != null()))){
										HX_STACK_LINE(360)
										if (((v->toString(rr) != HX_CSTRING("")))){
											HX_STACK_LINE(361)
											have_addition = true;
										}
									}
								}
							}
							HX_STACK_LINE(368)
							if ((have_pp)){
								HX_STACK_LINE(369)
								if ((!(have_rr))){
									HX_STACK_LINE(370)
									dd = pp;
								}
								else{
									HX_STACK_LINE(373)
									if ((v->equals(pp,rr))){
										HX_STACK_LINE(374)
										dd = pp;
									}
									else{
										HX_STACK_LINE(377)
										dd = pp;
										HX_STACK_LINE(378)
										dd_to = rr;
										HX_STACK_LINE(379)
										have_dd_to = true;
										HX_STACK_LINE(381)
										if ((!(v->equals(pp,ll)))){
											HX_STACK_LINE(382)
											if ((!(v->equals(pp,rr)))){
												HX_STACK_LINE(383)
												dd_to_alt = ll;
												HX_STACK_LINE(384)
												have_dd_to_alt = true;
											}
										}
									}
								}
							}
							else{
								HX_STACK_LINE(389)
								if ((have_ll)){
									HX_STACK_LINE(390)
									if ((!(have_rr))){
										HX_STACK_LINE(391)
										dd = ll;
									}
									else{
										HX_STACK_LINE(393)
										if ((v->equals(ll,rr))){
											HX_STACK_LINE(394)
											dd = ll;
										}
										else{
											HX_STACK_LINE(397)
											dd = ll;
											HX_STACK_LINE(398)
											dd_to = rr;
											HX_STACK_LINE(399)
											have_dd_to = true;
										}
									}
								}
								else{
									HX_STACK_LINE(403)
									dd = rr;
								}
							}
							HX_STACK_LINE(406)
							::String txt = null();		HX_STACK_VAR(txt,"txt");
							HX_STACK_LINE(407)
							if ((have_dd_to)){
								HX_STACK_LINE(408)
								txt = this->quoteForDiff(v,dd);
								HX_STACK_LINE(410)
								if (((sep == HX_CSTRING("")))){
									HX_STACK_LINE(413)
									sep = this->getSeparator(a,b,HX_CSTRING("->"));
								}
								HX_STACK_LINE(415)
								bool is_conflict = false;		HX_STACK_VAR(is_conflict,"is_conflict");
								HX_STACK_LINE(416)
								if ((have_dd_to_alt)){
									HX_STACK_LINE(417)
									if ((!(v->equals(dd_to,dd_to_alt)))){
										HX_STACK_LINE(418)
										is_conflict = true;
									}
								}
								HX_STACK_LINE(421)
								if ((!(is_conflict))){
									HX_STACK_LINE(422)
									txt = ((txt + sep) + this->quoteForDiff(v,dd_to));
									HX_STACK_LINE(423)
									if (((sep.length > act.length))){
										HX_STACK_LINE(424)
										act = sep;
									}
								}
								else{
									HX_STACK_LINE(427)
									if (((conflict_sep == HX_CSTRING("")))){
										HX_STACK_LINE(428)
										conflict_sep = (this->getSeparator(p,a,HX_CSTRING("!")) + sep);
									}
									HX_STACK_LINE(430)
									txt = ((((txt + conflict_sep) + this->quoteForDiff(v,dd_to_alt)) + conflict_sep) + this->quoteForDiff(v,dd_to));
									HX_STACK_LINE(433)
									act = conflict_sep;
								}
							}
							HX_STACK_LINE(436)
							if (((bool((act == HX_CSTRING(""))) && bool(have_addition)))){
								HX_STACK_LINE(437)
								act = HX_CSTRING("+");
							}
							HX_STACK_LINE(439)
							if ((publish)){
								HX_STACK_LINE(440)
								if (((txt != null()))){
									HX_STACK_LINE(441)
									output->setCell((j + (int)1),at,v->toDatum(txt));
								}
								else{
									HX_STACK_LINE(443)
									output->setCell((j + (int)1),at,dd);
								}
							}
						}
					}
					HX_STACK_LINE(448)
					if ((publish)){
						HX_STACK_LINE(449)
						output->setCell((int)0,at,v->toDatum(act));
						HX_STACK_LINE(450)
						row_map->set(at,unit);
					}
					HX_STACK_LINE(452)
					if (((act != HX_CSTRING("")))){
						HX_STACK_LINE(453)
						if ((!(publish))){
							HX_STACK_LINE(454)
							if (((active != null()))){
								HX_STACK_LINE(455)
								active[i] = (int)1;
							}
						}
					}
				}
			}
		}
	}
	HX_STACK_LINE(463)
	if ((!(show_rc_numbers))){
		HX_STACK_LINE(464)
		if ((this->flags->always_show_order)){
			HX_STACK_LINE(465)
			show_rc_numbers = true;
		}
		else{
			HX_STACK_LINE(466)
			if ((this->flags->ordered)){
				HX_STACK_LINE(467)
				show_rc_numbers = this->isReordered(row_map,output->__Field(HX_CSTRING("get_height"),true)());
				HX_STACK_LINE(468)
				if ((!(show_rc_numbers))){
					HX_STACK_LINE(469)
					show_rc_numbers = this->isReordered(col_map,output->__Field(HX_CSTRING("get_width"),true)());
				}
			}
		}
	}
	HX_STACK_LINE(474)
	if (((bool(show_rc_numbers) && bool(!(this->flags->never_show_order))))){
		HX_STACK_LINE(475)
		Array< int > target = Array_obj< int >::__new();		HX_STACK_VAR(target,"target");
		HX_STACK_LINE(476)
		{
			HX_STACK_LINE(476)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(476)
			int _g = output->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(476)
			while(((_g1 < _g))){
				HX_STACK_LINE(476)
				int i = (_g1)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(477)
				target->push((i + (int)1));
			}
		}
		HX_STACK_LINE(479)
		output->insertOrDeleteColumns(target,(output->__Field(HX_CSTRING("get_width"),true)() + (int)1));
		HX_STACK_LINE(480)
		this->l_prev = (int)-1;
		HX_STACK_LINE(481)
		this->r_prev = (int)-1;
		HX_STACK_LINE(482)
		{
			HX_STACK_LINE(482)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(482)
			int _g = output->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(482)
			while(((_g1 < _g))){
				HX_STACK_LINE(482)
				int i = (_g1)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(483)
				::coopy::Unit unit = row_map->get(i);		HX_STACK_VAR(unit,"unit");
				HX_STACK_LINE(484)
				if (((unit == null()))){
					HX_STACK_LINE(484)
					continue;
				}
				HX_STACK_LINE(485)
				output->setCell((int)0,i,this->reportUnit(unit));
			}
		}
		HX_STACK_LINE(487)
		target = Array_obj< int >::__new();
		HX_STACK_LINE(488)
		{
			HX_STACK_LINE(488)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(488)
			int _g = output->__Field(HX_CSTRING("get_height"),true)();		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(488)
			while(((_g1 < _g))){
				HX_STACK_LINE(488)
				int i = (_g1)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(489)
				target->push((i + (int)1));
			}
		}
		HX_STACK_LINE(491)
		output->insertOrDeleteRows(target,(output->__Field(HX_CSTRING("get_height"),true)() + (int)1));
		HX_STACK_LINE(492)
		this->l_prev = (int)-1;
		HX_STACK_LINE(493)
		this->r_prev = (int)-1;
		HX_STACK_LINE(494)
		{
			HX_STACK_LINE(494)
			int _g1 = (int)1;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(494)
			int _g = output->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(494)
			while(((_g1 < _g))){
				HX_STACK_LINE(494)
				int i = (_g1)++;		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(495)
				::coopy::Unit unit = col_map->get((i - (int)1));		HX_STACK_VAR(unit,"unit");
				HX_STACK_LINE(496)
				if (((unit == null()))){
					HX_STACK_LINE(496)
					continue;
				}
				HX_STACK_LINE(497)
				output->setCell(i,(int)0,this->reportUnit(unit));
			}
		}
		HX_STACK_LINE(499)
		output->setCell((int)0,(int)0,HX_CSTRING("@:@"));
	}
	HX_STACK_LINE(501)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC1(TableDiff_obj,hilite,return )

::coopy::Report TableDiff_obj::test( ){
	HX_STACK_FRAME("coopy.TableDiff","test",0x4f0cbcaf,"coopy.TableDiff.test","coopy/TableDiff.hx",505,0xda8b0bae)
	HX_STACK_THIS(this)
	HX_STACK_LINE(506)
	::coopy::Report report = ::coopy::Report_obj::__new();		HX_STACK_VAR(report,"report");
	HX_STACK_LINE(507)
	::coopy::Ordering order = this->align->toOrder();		HX_STACK_VAR(order,"order");
	HX_STACK_LINE(508)
	Array< ::Dynamic > units = order->getList();		HX_STACK_VAR(units,"units");
	HX_STACK_LINE(509)
	bool has_parent = (this->align->reference != null());		HX_STACK_VAR(has_parent,"has_parent");
	HX_STACK_LINE(510)
	::coopy::Table a;		HX_STACK_VAR(a,"a");
	HX_STACK_LINE(511)
	::coopy::Table b;		HX_STACK_VAR(b,"b");
	HX_STACK_LINE(512)
	::coopy::Table p;		HX_STACK_VAR(p,"p");
	HX_STACK_LINE(513)
	if ((has_parent)){
		HX_STACK_LINE(514)
		p = this->align->getSource();
		HX_STACK_LINE(515)
		a = this->align->reference->getTarget();
		HX_STACK_LINE(516)
		b = this->align->getTarget();
	}
	else{
		HX_STACK_LINE(518)
		a = this->align->getSource();
		HX_STACK_LINE(519)
		b = this->align->getTarget();
		HX_STACK_LINE(520)
		p = a;
	}
	HX_STACK_LINE(523)
	{
		HX_STACK_LINE(523)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(523)
		int _g = units->length;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(523)
		while(((_g1 < _g))){
			HX_STACK_LINE(523)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(524)
			::coopy::Unit unit = units->__get(i).StaticCast< ::coopy::Unit >();		HX_STACK_VAR(unit,"unit");
			HX_STACK_LINE(525)
			if (((bool((bool((unit->p < (int)0)) && bool((unit->l < (int)0)))) && bool((unit->r >= (int)0))))){
				HX_STACK_LINE(526)
				report->changes->push(::coopy::Change_obj::__new((HX_CSTRING("inserted row r:") + unit->r)));
			}
			HX_STACK_LINE(528)
			if (((bool((bool(((bool((unit->p >= (int)0)) || bool(!(has_parent))))) && bool((unit->l >= (int)0)))) && bool((unit->r < (int)0))))){
				HX_STACK_LINE(529)
				report->changes->push(::coopy::Change_obj::__new((HX_CSTRING("deleted row l:") + unit->l)));
			}
			HX_STACK_LINE(531)
			if (((bool((unit->l >= (int)0)) && bool((unit->r >= (int)0))))){
				HX_STACK_LINE(532)
				bool mod = false;		HX_STACK_VAR(mod,"mod");
				HX_STACK_LINE(533)
				::coopy::View av = a->getCellView();		HX_STACK_VAR(av,"av");
				HX_STACK_LINE(534)
				{
					HX_STACK_LINE(534)
					int _g3 = (int)0;		HX_STACK_VAR(_g3,"_g3");
					HX_STACK_LINE(534)
					int _g2 = a->__Field(HX_CSTRING("get_width"),true)();		HX_STACK_VAR(_g2,"_g2");
					HX_STACK_LINE(534)
					while(((_g3 < _g2))){
						HX_STACK_LINE(534)
						int j = (_g3)++;		HX_STACK_VAR(j,"j");
					}
				}
			}
		}
	}
	HX_STACK_LINE(540)
	return report;
}


HX_DEFINE_DYNAMIC_FUNC0(TableDiff_obj,test,return )


TableDiff_obj::TableDiff_obj()
{
}

void TableDiff_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(TableDiff);
	HX_MARK_MEMBER_NAME(align,"align");
	HX_MARK_MEMBER_NAME(flags,"flags");
	HX_MARK_MEMBER_NAME(l_prev,"l_prev");
	HX_MARK_MEMBER_NAME(r_prev,"r_prev");
	HX_MARK_END_CLASS();
}

void TableDiff_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(align,"align");
	HX_VISIT_MEMBER_NAME(flags,"flags");
	HX_VISIT_MEMBER_NAME(l_prev,"l_prev");
	HX_VISIT_MEMBER_NAME(r_prev,"r_prev");
}

Dynamic TableDiff_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 4:
		if (HX_FIELD_EQ(inName,"test") ) { return test_dyn(); }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"align") ) { return align; }
		if (HX_FIELD_EQ(inName,"flags") ) { return flags; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"l_prev") ) { return l_prev; }
		if (HX_FIELD_EQ(inName,"r_prev") ) { return r_prev; }
		if (HX_FIELD_EQ(inName,"hilite") ) { return hilite_dyn(); }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"reportUnit") ) { return reportUnit_dyn(); }
		break;
	case 11:
		if (HX_FIELD_EQ(inName,"isReordered") ) { return isReordered_dyn(); }
		break;
	case 12:
		if (HX_FIELD_EQ(inName,"getSeparator") ) { return getSeparator_dyn(); }
		if (HX_FIELD_EQ(inName,"quoteForDiff") ) { return quoteForDiff_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic TableDiff_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 5:
		if (HX_FIELD_EQ(inName,"align") ) { align=inValue.Cast< ::coopy::Alignment >(); return inValue; }
		if (HX_FIELD_EQ(inName,"flags") ) { flags=inValue.Cast< ::coopy::CompareFlags >(); return inValue; }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"l_prev") ) { l_prev=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"r_prev") ) { r_prev=inValue.Cast< int >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void TableDiff_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("align"));
	outFields->push(HX_CSTRING("flags"));
	outFields->push(HX_CSTRING("l_prev"));
	outFields->push(HX_CSTRING("r_prev"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(TableDiff_obj,align),HX_CSTRING("align")},
	{hx::fsObject,(int)offsetof(TableDiff_obj,flags),HX_CSTRING("flags")},
	{hx::fsObject,(int)offsetof(TableDiff_obj,l_prev),HX_CSTRING("l_prev")},
	{hx::fsObject,(int)offsetof(TableDiff_obj,r_prev),HX_CSTRING("r_prev")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("align"),
	HX_CSTRING("flags"),
	HX_CSTRING("l_prev"),
	HX_CSTRING("r_prev"),
	HX_CSTRING("getSeparator"),
	HX_CSTRING("quoteForDiff"),
	HX_CSTRING("isReordered"),
	HX_CSTRING("reportUnit"),
	HX_CSTRING("hilite"),
	HX_CSTRING("test"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(TableDiff_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(TableDiff_obj::__mClass,"__mClass");
};

#endif

Class TableDiff_obj::__mClass;

void TableDiff_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.TableDiff"), hx::TCanCast< TableDiff_obj> ,sStaticFields,sMemberFields,
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

void TableDiff_obj::__boot()
{
}

} // end namespace coopy
