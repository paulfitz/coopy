#include <hxcpp.h>

#ifndef INCLUDED_IMap
#include <IMap.h>
#endif
#ifndef INCLUDED_coopy_Mover
#include <coopy/Mover.h>
#endif
#ifndef INCLUDED_coopy_Unit
#include <coopy/Unit.h>
#endif
#ifndef INCLUDED_haxe_ds_IntMap
#include <haxe/ds/IntMap.h>
#endif
namespace coopy{

Void Mover_obj::__construct()
{
HX_STACK_FRAME("coopy.Mover","new",0x0e048bb1,"coopy.Mover.new","coopy/Mover.hx",8,0xe55f9de0)
{
}
;
	return null();
}

Mover_obj::~Mover_obj() { }

Dynamic Mover_obj::__CreateEmpty() { return  new Mover_obj; }
hx::ObjectPtr< Mover_obj > Mover_obj::__new()
{  hx::ObjectPtr< Mover_obj > result = new Mover_obj();
	result->__construct();
	return result;}

Dynamic Mover_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Mover_obj > result = new Mover_obj();
	result->__construct();
	return result;}

Array< int > Mover_obj::moveUnits( Array< ::Dynamic > units){
	HX_STACK_FRAME("coopy.Mover","moveUnits",0x2c67c6af,"coopy.Mover.moveUnits","coopy/Mover.hx",11,0xe55f9de0)
	HX_STACK_ARG(units,"units")
	HX_STACK_LINE(12)
	Array< int > isrc = Array_obj< int >::__new();		HX_STACK_VAR(isrc,"isrc");
	HX_STACK_LINE(13)
	Array< int > idest = Array_obj< int >::__new();		HX_STACK_VAR(idest,"idest");
	HX_STACK_LINE(14)
	int len = units->length;		HX_STACK_VAR(len,"len");
	HX_STACK_LINE(15)
	int ltop = (int)-1;		HX_STACK_VAR(ltop,"ltop");
	HX_STACK_LINE(16)
	int rtop = (int)-1;		HX_STACK_VAR(rtop,"rtop");
	HX_STACK_LINE(17)
	::haxe::ds::IntMap in_src = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(in_src,"in_src");
	HX_STACK_LINE(18)
	::haxe::ds::IntMap in_dest = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(in_dest,"in_dest");
	HX_STACK_LINE(19)
	{
		HX_STACK_LINE(19)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(19)
		while(((_g < len))){
			HX_STACK_LINE(19)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(20)
			::coopy::Unit unit = units->__get(i).StaticCast< ::coopy::Unit >();		HX_STACK_VAR(unit,"unit");
			HX_STACK_LINE(21)
			if (((bool((unit->l >= (int)0)) && bool((unit->r >= (int)0))))){
				HX_STACK_LINE(22)
				if (((ltop < unit->l))){
					HX_STACK_LINE(22)
					ltop = unit->l;
				}
				HX_STACK_LINE(23)
				if (((rtop < unit->r))){
					HX_STACK_LINE(23)
					rtop = unit->r;
				}
				HX_STACK_LINE(24)
				{
					HX_STACK_LINE(24)
					in_src->set(unit->l,i);
					HX_STACK_LINE(24)
					i;
				}
				HX_STACK_LINE(25)
				{
					HX_STACK_LINE(25)
					in_dest->set(unit->r,i);
					HX_STACK_LINE(25)
					i;
				}
			}
		}
	}
	HX_STACK_LINE(28)
	Dynamic v;		HX_STACK_VAR(v,"v");
	HX_STACK_LINE(29)
	{
		HX_STACK_LINE(29)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(29)
		int _g = (ltop + (int)1);		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(29)
		while(((_g1 < _g))){
			HX_STACK_LINE(29)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(30)
			v = in_src->get(i);
			HX_STACK_LINE(31)
			if (((v != null()))){
				HX_STACK_LINE(31)
				isrc->push(v);
			}
		}
	}
	HX_STACK_LINE(33)
	{
		HX_STACK_LINE(33)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(33)
		int _g = (rtop + (int)1);		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(33)
		while(((_g1 < _g))){
			HX_STACK_LINE(33)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(34)
			v = in_dest->get(i);
			HX_STACK_LINE(35)
			if (((v != null()))){
				HX_STACK_LINE(35)
				idest->push(v);
			}
		}
	}
	HX_STACK_LINE(37)
	return ::coopy::Mover_obj::moveWithoutExtras(isrc,idest);
}


STATIC_HX_DEFINE_DYNAMIC_FUNC1(Mover_obj,moveUnits,return )

Array< int > Mover_obj::moveWithExtras( Array< int > isrc,Array< int > idest){
	HX_STACK_FRAME("coopy.Mover","moveWithExtras",0x04d47149,"coopy.Mover.moveWithExtras","coopy/Mover.hx",40,0xe55f9de0)
	HX_STACK_ARG(isrc,"isrc")
	HX_STACK_ARG(idest,"idest")
	HX_STACK_LINE(42)
	int len = isrc->length;		HX_STACK_VAR(len,"len");
	HX_STACK_LINE(43)
	int len2 = idest->length;		HX_STACK_VAR(len2,"len2");
	HX_STACK_LINE(44)
	::haxe::ds::IntMap in_src = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(in_src,"in_src");
	HX_STACK_LINE(45)
	::haxe::ds::IntMap in_dest = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(in_dest,"in_dest");
	HX_STACK_LINE(46)
	{
		HX_STACK_LINE(46)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(46)
		while(((_g < len))){
			HX_STACK_LINE(46)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(47)
			{
				HX_STACK_LINE(47)
				in_src->set(isrc->__get(i),i);
				HX_STACK_LINE(47)
				i;
			}
		}
	}
	HX_STACK_LINE(49)
	{
		HX_STACK_LINE(49)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(49)
		while(((_g < len2))){
			HX_STACK_LINE(49)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(50)
			{
				HX_STACK_LINE(50)
				in_dest->set(idest->__get(i),i);
				HX_STACK_LINE(50)
				i;
			}
		}
	}
	HX_STACK_LINE(52)
	Array< int > src = Array_obj< int >::__new();		HX_STACK_VAR(src,"src");
	HX_STACK_LINE(53)
	Array< int > dest = Array_obj< int >::__new();		HX_STACK_VAR(dest,"dest");
	HX_STACK_LINE(54)
	int v;		HX_STACK_VAR(v,"v");
	HX_STACK_LINE(55)
	{
		HX_STACK_LINE(55)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(55)
		while(((_g < len))){
			HX_STACK_LINE(55)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(56)
			v = isrc->__get(i);
			HX_STACK_LINE(57)
			if ((in_dest->exists(v))){
				HX_STACK_LINE(57)
				src->push(v);
			}
		}
	}
	HX_STACK_LINE(59)
	{
		HX_STACK_LINE(59)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(59)
		while(((_g < len2))){
			HX_STACK_LINE(59)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(60)
			v = idest->__get(i);
			HX_STACK_LINE(61)
			if ((in_src->exists(v))){
				HX_STACK_LINE(61)
				dest->push(v);
			}
		}
	}
	HX_STACK_LINE(64)
	return ::coopy::Mover_obj::moveWithoutExtras(src,dest);
}


STATIC_HX_DEFINE_DYNAMIC_FUNC2(Mover_obj,moveWithExtras,return )

Array< int > Mover_obj::moveWithoutExtras( Array< int > src,Array< int > dest){
	HX_STACK_FRAME("coopy.Mover","moveWithoutExtras",0xca34cd8b,"coopy.Mover.moveWithoutExtras","coopy/Mover.hx",67,0xe55f9de0)
	HX_STACK_ARG(src,"src")
	HX_STACK_ARG(dest,"dest")
	HX_STACK_LINE(68)
	if (((src->length != dest->length))){
		HX_STACK_LINE(68)
		return null();
	}
	HX_STACK_LINE(69)
	if (((src->length <= (int)1))){
		HX_STACK_LINE(69)
		return Array_obj< int >::__new();
	}
	HX_STACK_LINE(71)
	int len = src->length;		HX_STACK_VAR(len,"len");
	HX_STACK_LINE(72)
	::haxe::ds::IntMap in_src = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(in_src,"in_src");
	HX_STACK_LINE(73)
	Array< ::Dynamic > blk_len = Array_obj< ::Dynamic >::__new().Add(::haxe::ds::IntMap_obj::__new());		HX_STACK_VAR(blk_len,"blk_len");
	HX_STACK_LINE(74)
	::haxe::ds::IntMap blk_src_loc = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(blk_src_loc,"blk_src_loc");
	HX_STACK_LINE(75)
	::haxe::ds::IntMap blk_dest_loc = ::haxe::ds::IntMap_obj::__new();		HX_STACK_VAR(blk_dest_loc,"blk_dest_loc");
	HX_STACK_LINE(76)
	{
		HX_STACK_LINE(76)
		int _g = (int)0;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(76)
		while(((_g < len))){
			HX_STACK_LINE(76)
			int i = (_g)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(77)
			{
				HX_STACK_LINE(77)
				in_src->set(src->__get(i),i);
				HX_STACK_LINE(77)
				i;
			}
		}
	}
	HX_STACK_LINE(79)
	int ct = (int)0;		HX_STACK_VAR(ct,"ct");
	HX_STACK_LINE(80)
	int in_cursor = (int)-2;		HX_STACK_VAR(in_cursor,"in_cursor");
	HX_STACK_LINE(81)
	int out_cursor = (int)0;		HX_STACK_VAR(out_cursor,"out_cursor");
	HX_STACK_LINE(82)
	int next;		HX_STACK_VAR(next,"next");
	HX_STACK_LINE(83)
	int blk = (int)-1;		HX_STACK_VAR(blk,"blk");
	HX_STACK_LINE(84)
	int v;		HX_STACK_VAR(v,"v");
	HX_STACK_LINE(85)
	while(((out_cursor < len))){
		HX_STACK_LINE(86)
		v = dest->__get(out_cursor);
		HX_STACK_LINE(87)
		next = in_src->get(v);
		HX_STACK_LINE(88)
		if (((next != (in_cursor + (int)1)))){
			HX_STACK_LINE(89)
			blk = v;
			HX_STACK_LINE(90)
			ct = (int)1;
			HX_STACK_LINE(91)
			blk_src_loc->set(blk,next);
			HX_STACK_LINE(92)
			blk_dest_loc->set(blk,out_cursor);
		}
		else{
			HX_STACK_LINE(94)
			(ct)++;
		}
		HX_STACK_LINE(96)
		blk_len->__get((int)0).StaticCast< ::haxe::ds::IntMap >()->set(blk,ct);
		HX_STACK_LINE(97)
		in_cursor = next;
		HX_STACK_LINE(98)
		(out_cursor)++;
	}
	HX_STACK_LINE(101)
	Array< int > blks = Array_obj< int >::__new();		HX_STACK_VAR(blks,"blks");
	HX_STACK_LINE(102)
	for(::cpp::FastIterator_obj< int > *__it = ::cpp::CreateFastIterator< int >(blk_len->__get((int)0).StaticCast< ::haxe::ds::IntMap >()->keys());  __it->hasNext(); ){
		int k = __it->next();
		blks->push(k);
	}

	HX_BEGIN_LOCAL_FUNC_S1(hx::LocalFunc,_Function_1_1,Array< ::Dynamic >,blk_len)
	int run(int a,int b){
		HX_STACK_FRAME("*","_Function_1_1",0x5200ed37,"*._Function_1_1","coopy/Mover.hx",103,0xe55f9de0)
		HX_STACK_ARG(a,"a")
		HX_STACK_ARG(b,"b")
		{
			HX_STACK_LINE(103)
			return (blk_len->__get((int)0).StaticCast< ::haxe::ds::IntMap >()->get(b) - blk_len->__get((int)0).StaticCast< ::haxe::ds::IntMap >()->get(a));
		}
		return null();
	}
	HX_END_LOCAL_FUNC2(return)

	HX_STACK_LINE(103)
	blks->sort( Dynamic(new _Function_1_1(blk_len)));
	HX_STACK_LINE(105)
	Array< int > moved = Array_obj< int >::__new();		HX_STACK_VAR(moved,"moved");
	HX_STACK_LINE(107)
	while(((blks->length > (int)0))){
		HX_STACK_LINE(108)
		int blk1 = blks->shift();		HX_STACK_VAR(blk1,"blk1");
		HX_STACK_LINE(109)
		int blen = blks->length;		HX_STACK_VAR(blen,"blen");
		HX_STACK_LINE(110)
		int ref_src_loc = blk_src_loc->get(blk1);		HX_STACK_VAR(ref_src_loc,"ref_src_loc");
		HX_STACK_LINE(111)
		int ref_dest_loc = blk_dest_loc->get(blk1);		HX_STACK_VAR(ref_dest_loc,"ref_dest_loc");
		HX_STACK_LINE(112)
		int i = (blen - (int)1);		HX_STACK_VAR(i,"i");
		HX_STACK_LINE(113)
		while(((i >= (int)0))){
			HX_STACK_LINE(114)
			int blki = blks->__get(i);		HX_STACK_VAR(blki,"blki");
			HX_STACK_LINE(115)
			int blki_src_loc = blk_src_loc->get(blki);		HX_STACK_VAR(blki_src_loc,"blki_src_loc");
			HX_STACK_LINE(116)
			bool to_left_src = (blki_src_loc < ref_src_loc);		HX_STACK_VAR(to_left_src,"to_left_src");
			HX_STACK_LINE(117)
			bool to_left_dest = (blk_dest_loc->get(blki) < ref_dest_loc);		HX_STACK_VAR(to_left_dest,"to_left_dest");
			HX_STACK_LINE(118)
			if (((to_left_src != to_left_dest))){
				HX_STACK_LINE(119)
				int ct1 = blk_len->__get((int)0).StaticCast< ::haxe::ds::IntMap >()->get(blki);		HX_STACK_VAR(ct1,"ct1");
				HX_STACK_LINE(120)
				{
					HX_STACK_LINE(120)
					int _g = (int)0;		HX_STACK_VAR(_g,"_g");
					HX_STACK_LINE(120)
					while(((_g < ct1))){
						HX_STACK_LINE(120)
						int j = (_g)++;		HX_STACK_VAR(j,"j");
						HX_STACK_LINE(121)
						moved->push(src->__get(blki_src_loc));
						HX_STACK_LINE(122)
						(blki_src_loc)++;
					}
				}
				HX_STACK_LINE(124)
				blks->splice(i,(int)1);
			}
			HX_STACK_LINE(126)
			(i)--;
		}
	}
	HX_STACK_LINE(129)
	return moved;
}


STATIC_HX_DEFINE_DYNAMIC_FUNC2(Mover_obj,moveWithoutExtras,return )


Mover_obj::Mover_obj()
{
}

void Mover_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Mover);
	HX_MARK_END_CLASS();
}

void Mover_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic Mover_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 9:
		if (HX_FIELD_EQ(inName,"moveUnits") ) { return moveUnits_dyn(); }
		break;
	case 14:
		if (HX_FIELD_EQ(inName,"moveWithExtras") ) { return moveWithExtras_dyn(); }
		break;
	case 17:
		if (HX_FIELD_EQ(inName,"moveWithoutExtras") ) { return moveWithoutExtras_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Mover_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void Mover_obj::__GetFields(Array< ::String> &outFields)
{
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("moveUnits"),
	HX_CSTRING("moveWithExtras"),
	HX_CSTRING("moveWithoutExtras"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo *sMemberStorageInfo = 0;
#endif

static ::String sMemberFields[] = {
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Mover_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Mover_obj::__mClass,"__mClass");
};

#endif

Class Mover_obj::__mClass;

void Mover_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Mover"), hx::TCanCast< Mover_obj> ,sStaticFields,sMemberFields,
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

void Mover_obj::__boot()
{
}

} // end namespace coopy
