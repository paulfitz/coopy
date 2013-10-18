#include <hxcpp.h>

#ifndef INCLUDED_coopy_SparseSheet
#include <coopy/SparseSheet.h>
#endif
#ifndef INCLUDED_coopy_Viterbi
#include <coopy/Viterbi.h>
#endif
#ifndef INCLUDED_haxe_Log
#include <haxe/Log.h>
#endif
namespace coopy{

Void Viterbi_obj::__construct()
{
HX_STACK_FRAME("coopy.Viterbi","new",0x266062c5,"coopy.Viterbi.new","coopy/Viterbi.hx",17,0x02c3478c)
{
	HX_STACK_LINE(18)
	this->K = this->T = (int)0;
	HX_STACK_LINE(19)
	this->reset();
	HX_STACK_LINE(20)
	this->cost = ::coopy::SparseSheet_obj::__new();
	HX_STACK_LINE(21)
	this->src = ::coopy::SparseSheet_obj::__new();
	HX_STACK_LINE(22)
	this->path = ::coopy::SparseSheet_obj::__new();
}
;
	return null();
}

Viterbi_obj::~Viterbi_obj() { }

Dynamic Viterbi_obj::__CreateEmpty() { return  new Viterbi_obj; }
hx::ObjectPtr< Viterbi_obj > Viterbi_obj::__new()
{  hx::ObjectPtr< Viterbi_obj > result = new Viterbi_obj();
	result->__construct();
	return result;}

Dynamic Viterbi_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< Viterbi_obj > result = new Viterbi_obj();
	result->__construct();
	return result;}

Void Viterbi_obj::reset( ){
{
		HX_STACK_FRAME("coopy.Viterbi","reset",0x16e9ff74,"coopy.Viterbi.reset","coopy/Viterbi.hx",25,0x02c3478c)
		HX_STACK_THIS(this)
		HX_STACK_LINE(26)
		this->index = (int)0;
		HX_STACK_LINE(27)
		this->mode = (int)0;
		HX_STACK_LINE(28)
		this->path_valid = false;
		HX_STACK_LINE(29)
		this->best_cost = (int)0;
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(Viterbi_obj,reset,(void))

Void Viterbi_obj::setSize( int states,int sequence_length){
{
		HX_STACK_FRAME("coopy.Viterbi","setSize",0xd60e0a68,"coopy.Viterbi.setSize","coopy/Viterbi.hx",32,0x02c3478c)
		HX_STACK_THIS(this)
		HX_STACK_ARG(states,"states")
		HX_STACK_ARG(sequence_length,"sequence_length")
		HX_STACK_LINE(33)
		this->K = states;
		HX_STACK_LINE(34)
		this->T = sequence_length;
		HX_STACK_LINE(35)
		this->cost->resize(this->K,this->T,(int)0);
		HX_STACK_LINE(36)
		this->src->resize(this->K,this->T,(int)-1);
		HX_STACK_LINE(37)
		this->path->resize((int)1,this->T,(int)-1);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC2(Viterbi_obj,setSize,(void))

Void Viterbi_obj::assertMode( int next){
{
		HX_STACK_FRAME("coopy.Viterbi","assertMode",0x941fd584,"coopy.Viterbi.assertMode","coopy/Viterbi.hx",40,0x02c3478c)
		HX_STACK_THIS(this)
		HX_STACK_ARG(next,"next")
		HX_STACK_LINE(41)
		if (((bool((next == (int)0)) && bool((this->mode == (int)1))))){
			HX_STACK_LINE(41)
			(this->index)++;
		}
		HX_STACK_LINE(42)
		this->mode = next;
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC1(Viterbi_obj,assertMode,(void))

Void Viterbi_obj::addTransition( int s0,int s1,Float c){
{
		HX_STACK_FRAME("coopy.Viterbi","addTransition",0x20f039bb,"coopy.Viterbi.addTransition","coopy/Viterbi.hx",45,0x02c3478c)
		HX_STACK_THIS(this)
		HX_STACK_ARG(s0,"s0")
		HX_STACK_ARG(s1,"s1")
		HX_STACK_ARG(c,"c")
		HX_STACK_LINE(46)
		bool resize = false;		HX_STACK_VAR(resize,"resize");
		HX_STACK_LINE(47)
		if (((s0 >= this->K))){
			HX_STACK_LINE(48)
			this->K = (s0 + (int)1);
			HX_STACK_LINE(49)
			resize = true;
		}
		HX_STACK_LINE(51)
		if (((s1 >= this->K))){
			HX_STACK_LINE(52)
			this->K = (s1 + (int)1);
			HX_STACK_LINE(53)
			resize = true;
		}
		HX_STACK_LINE(55)
		if ((resize)){
			HX_STACK_LINE(56)
			this->cost->nonDestructiveResize(this->K,this->T,(int)0);
			HX_STACK_LINE(57)
			this->src->nonDestructiveResize(this->K,this->T,(int)-1);
			HX_STACK_LINE(58)
			this->path->nonDestructiveResize((int)1,this->T,(int)-1);
		}
		HX_STACK_LINE(60)
		this->path_valid = false;
		HX_STACK_LINE(61)
		this->assertMode((int)1);
		HX_STACK_LINE(62)
		if (((this->index >= this->T))){
			HX_STACK_LINE(63)
			this->T = (this->index + (int)1);
			HX_STACK_LINE(64)
			this->cost->nonDestructiveResize(this->K,this->T,(int)0);
			HX_STACK_LINE(65)
			this->src->nonDestructiveResize(this->K,this->T,(int)-1);
			HX_STACK_LINE(66)
			this->path->nonDestructiveResize((int)1,this->T,(int)-1);
		}
		HX_STACK_LINE(68)
		bool sourced = false;		HX_STACK_VAR(sourced,"sourced");
		HX_STACK_LINE(69)
		if (((this->index > (int)0))){
			HX_STACK_LINE(70)
			hx::AddEq(c,this->cost->get(s0,(this->index - (int)1)));
			HX_STACK_LINE(71)
			sourced = (this->src->get(s0,(this->index - (int)1)) != (int)-1);
		}
		else{
			HX_STACK_LINE(73)
			sourced = true;
		}
		HX_STACK_LINE(76)
		if ((sourced)){
			HX_STACK_LINE(77)
			if (((bool((c < this->cost->get(s1,this->index))) || bool((this->src->get(s1,this->index) == (int)-1))))){
				HX_STACK_LINE(78)
				this->cost->set(s1,this->index,c);
				HX_STACK_LINE(79)
				this->src->set(s1,this->index,s0);
			}
		}
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC3(Viterbi_obj,addTransition,(void))

Void Viterbi_obj::endTransitions( ){
{
		HX_STACK_FRAME("coopy.Viterbi","endTransitions",0x757c7ede,"coopy.Viterbi.endTransitions","coopy/Viterbi.hx",84,0x02c3478c)
		HX_STACK_THIS(this)
		HX_STACK_LINE(85)
		this->path_valid = false;
		HX_STACK_LINE(86)
		this->assertMode((int)0);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(Viterbi_obj,endTransitions,(void))

Void Viterbi_obj::beginTransitions( ){
{
		HX_STACK_FRAME("coopy.Viterbi","beginTransitions",0x0727e410,"coopy.Viterbi.beginTransitions","coopy/Viterbi.hx",89,0x02c3478c)
		HX_STACK_THIS(this)
		HX_STACK_LINE(90)
		this->path_valid = false;
		HX_STACK_LINE(91)
		this->assertMode((int)1);
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(Viterbi_obj,beginTransitions,(void))

Void Viterbi_obj::calculatePath( ){
{
		HX_STACK_FRAME("coopy.Viterbi","calculatePath",0x04dab4d0,"coopy.Viterbi.calculatePath","coopy/Viterbi.hx",94,0x02c3478c)
		HX_STACK_THIS(this)
		HX_STACK_LINE(95)
		if ((this->path_valid)){
			HX_STACK_LINE(95)
			return null();
		}
		HX_STACK_LINE(96)
		this->endTransitions();
		HX_STACK_LINE(97)
		Float best = (int)0;		HX_STACK_VAR(best,"best");
		HX_STACK_LINE(98)
		int bestj = (int)-1;		HX_STACK_VAR(bestj,"bestj");
		HX_STACK_LINE(99)
		if (((this->index <= (int)0))){
			HX_STACK_LINE(101)
			this->path_valid = true;
			HX_STACK_LINE(102)
			return null();
		}
		HX_STACK_LINE(104)
		{
			HX_STACK_LINE(104)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(104)
			int _g = this->K;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(104)
			while(((_g1 < _g))){
				HX_STACK_LINE(104)
				int j = (_g1)++;		HX_STACK_VAR(j,"j");
				HX_STACK_LINE(105)
				if (((bool(((bool((this->cost->get(j,(this->index - (int)1)) < best)) || bool((bestj == (int)-1))))) && bool((this->src->get(j,(this->index - (int)1)) != (int)-1))))){
					HX_STACK_LINE(107)
					best = this->cost->get(j,(this->index - (int)1));
					HX_STACK_LINE(108)
					bestj = j;
				}
			}
		}
		HX_STACK_LINE(111)
		this->best_cost = best;
		HX_STACK_LINE(113)
		{
			HX_STACK_LINE(113)
			int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
			HX_STACK_LINE(113)
			int _g = this->index;		HX_STACK_VAR(_g,"_g");
			HX_STACK_LINE(113)
			while(((_g1 < _g))){
				HX_STACK_LINE(113)
				int j = (_g1)++;		HX_STACK_VAR(j,"j");
				HX_STACK_LINE(114)
				int i = ((this->index - (int)1) - j);		HX_STACK_VAR(i,"i");
				HX_STACK_LINE(115)
				this->path->set((int)0,i,bestj);
				HX_STACK_LINE(116)
				if ((!(((bool((bestj != (int)-1)) && bool(((bool((bestj >= (int)0)) && bool((bestj < this->K)))))))))){
					HX_STACK_LINE(117)
					::haxe::Log_obj::trace(HX_CSTRING("Problem in Viterbi"),hx::SourceInfo(HX_CSTRING("Viterbi.hx"),117,HX_CSTRING("coopy.Viterbi"),HX_CSTRING("calculatePath")));
				}
				HX_STACK_LINE(119)
				bestj = this->src->get(bestj,i);
			}
		}
		HX_STACK_LINE(121)
		this->path_valid = true;
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(Viterbi_obj,calculatePath,(void))

::String Viterbi_obj::toString( ){
	HX_STACK_FRAME("coopy.Viterbi","toString",0x239c7027,"coopy.Viterbi.toString","coopy/Viterbi.hx",124,0x02c3478c)
	HX_STACK_THIS(this)
	HX_STACK_LINE(125)
	this->calculatePath();
	HX_STACK_LINE(126)
	::String txt = HX_CSTRING("");		HX_STACK_VAR(txt,"txt");
	HX_STACK_LINE(127)
	{
		HX_STACK_LINE(127)
		int _g1 = (int)0;		HX_STACK_VAR(_g1,"_g1");
		HX_STACK_LINE(127)
		int _g = this->index;		HX_STACK_VAR(_g,"_g");
		HX_STACK_LINE(127)
		while(((_g1 < _g))){
			HX_STACK_LINE(127)
			int i = (_g1)++;		HX_STACK_VAR(i,"i");
			HX_STACK_LINE(128)
			if (((this->path->get((int)0,i) == (int)-1))){
				HX_STACK_LINE(129)
				hx::AddEq(txt,HX_CSTRING("*"));
			}
			else{
				HX_STACK_LINE(131)
				hx::AddEq(txt,this->path->get((int)0,i));
			}
			HX_STACK_LINE(133)
			if (((this->K >= (int)10))){
				HX_STACK_LINE(133)
				hx::AddEq(txt,HX_CSTRING(" "));
			}
		}
	}
	HX_STACK_LINE(135)
	hx::AddEq(txt,(HX_CSTRING(" costs ") + this->getCost()));
	HX_STACK_LINE(136)
	return txt;
}


HX_DEFINE_DYNAMIC_FUNC0(Viterbi_obj,toString,return )

int Viterbi_obj::length( ){
	HX_STACK_FRAME("coopy.Viterbi","length",0x8c64cfa1,"coopy.Viterbi.length","coopy/Viterbi.hx",139,0x02c3478c)
	HX_STACK_THIS(this)
	HX_STACK_LINE(140)
	if (((this->index > (int)0))){
		HX_STACK_LINE(141)
		this->calculatePath();
	}
	HX_STACK_LINE(143)
	return this->index;
}


HX_DEFINE_DYNAMIC_FUNC0(Viterbi_obj,length,return )

int Viterbi_obj::get( int i){
	HX_STACK_FRAME("coopy.Viterbi","get",0x265b12fb,"coopy.Viterbi.get","coopy/Viterbi.hx",146,0x02c3478c)
	HX_STACK_THIS(this)
	HX_STACK_ARG(i,"i")
	HX_STACK_LINE(147)
	this->calculatePath();
	HX_STACK_LINE(148)
	return this->path->get((int)0,i);
}


HX_DEFINE_DYNAMIC_FUNC1(Viterbi_obj,get,return )

Float Viterbi_obj::getCost( ){
	HX_STACK_FRAME("coopy.Viterbi","getCost",0xd87d96e8,"coopy.Viterbi.getCost","coopy/Viterbi.hx",151,0x02c3478c)
	HX_STACK_THIS(this)
	HX_STACK_LINE(152)
	this->calculatePath();
	HX_STACK_LINE(153)
	return this->best_cost;
}


HX_DEFINE_DYNAMIC_FUNC0(Viterbi_obj,getCost,return )


Viterbi_obj::Viterbi_obj()
{
}

void Viterbi_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(Viterbi);
	HX_MARK_MEMBER_NAME(K,"K");
	HX_MARK_MEMBER_NAME(T,"T");
	HX_MARK_MEMBER_NAME(index,"index");
	HX_MARK_MEMBER_NAME(mode,"mode");
	HX_MARK_MEMBER_NAME(path_valid,"path_valid");
	HX_MARK_MEMBER_NAME(best_cost,"best_cost");
	HX_MARK_MEMBER_NAME(cost,"cost");
	HX_MARK_MEMBER_NAME(src,"src");
	HX_MARK_MEMBER_NAME(path,"path");
	HX_MARK_END_CLASS();
}

void Viterbi_obj::__Visit(HX_VISIT_PARAMS)
{
	HX_VISIT_MEMBER_NAME(K,"K");
	HX_VISIT_MEMBER_NAME(T,"T");
	HX_VISIT_MEMBER_NAME(index,"index");
	HX_VISIT_MEMBER_NAME(mode,"mode");
	HX_VISIT_MEMBER_NAME(path_valid,"path_valid");
	HX_VISIT_MEMBER_NAME(best_cost,"best_cost");
	HX_VISIT_MEMBER_NAME(cost,"cost");
	HX_VISIT_MEMBER_NAME(src,"src");
	HX_VISIT_MEMBER_NAME(path,"path");
}

Dynamic Viterbi_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"K") ) { return K; }
		if (HX_FIELD_EQ(inName,"T") ) { return T; }
		break;
	case 3:
		if (HX_FIELD_EQ(inName,"src") ) { return src; }
		if (HX_FIELD_EQ(inName,"get") ) { return get_dyn(); }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"mode") ) { return mode; }
		if (HX_FIELD_EQ(inName,"cost") ) { return cost; }
		if (HX_FIELD_EQ(inName,"path") ) { return path; }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"index") ) { return index; }
		if (HX_FIELD_EQ(inName,"reset") ) { return reset_dyn(); }
		break;
	case 6:
		if (HX_FIELD_EQ(inName,"length") ) { return length_dyn(); }
		break;
	case 7:
		if (HX_FIELD_EQ(inName,"setSize") ) { return setSize_dyn(); }
		if (HX_FIELD_EQ(inName,"getCost") ) { return getCost_dyn(); }
		break;
	case 8:
		if (HX_FIELD_EQ(inName,"toString") ) { return toString_dyn(); }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"best_cost") ) { return best_cost; }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"path_valid") ) { return path_valid; }
		if (HX_FIELD_EQ(inName,"assertMode") ) { return assertMode_dyn(); }
		break;
	case 13:
		if (HX_FIELD_EQ(inName,"addTransition") ) { return addTransition_dyn(); }
		if (HX_FIELD_EQ(inName,"calculatePath") ) { return calculatePath_dyn(); }
		break;
	case 14:
		if (HX_FIELD_EQ(inName,"endTransitions") ) { return endTransitions_dyn(); }
		break;
	case 16:
		if (HX_FIELD_EQ(inName,"beginTransitions") ) { return beginTransitions_dyn(); }
	}
	return super::__Field(inName,inCallProp);
}

Dynamic Viterbi_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	switch(inName.length) {
	case 1:
		if (HX_FIELD_EQ(inName,"K") ) { K=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"T") ) { T=inValue.Cast< int >(); return inValue; }
		break;
	case 3:
		if (HX_FIELD_EQ(inName,"src") ) { src=inValue.Cast< ::coopy::SparseSheet >(); return inValue; }
		break;
	case 4:
		if (HX_FIELD_EQ(inName,"mode") ) { mode=inValue.Cast< int >(); return inValue; }
		if (HX_FIELD_EQ(inName,"cost") ) { cost=inValue.Cast< ::coopy::SparseSheet >(); return inValue; }
		if (HX_FIELD_EQ(inName,"path") ) { path=inValue.Cast< ::coopy::SparseSheet >(); return inValue; }
		break;
	case 5:
		if (HX_FIELD_EQ(inName,"index") ) { index=inValue.Cast< int >(); return inValue; }
		break;
	case 9:
		if (HX_FIELD_EQ(inName,"best_cost") ) { best_cost=inValue.Cast< Float >(); return inValue; }
		break;
	case 10:
		if (HX_FIELD_EQ(inName,"path_valid") ) { path_valid=inValue.Cast< bool >(); return inValue; }
	}
	return super::__SetField(inName,inValue,inCallProp);
}

void Viterbi_obj::__GetFields(Array< ::String> &outFields)
{
	outFields->push(HX_CSTRING("K"));
	outFields->push(HX_CSTRING("T"));
	outFields->push(HX_CSTRING("index"));
	outFields->push(HX_CSTRING("mode"));
	outFields->push(HX_CSTRING("path_valid"));
	outFields->push(HX_CSTRING("best_cost"));
	outFields->push(HX_CSTRING("cost"));
	outFields->push(HX_CSTRING("src"));
	outFields->push(HX_CSTRING("path"));
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(Viterbi_obj,K),HX_CSTRING("K")},
	{hx::fsObject,(int)offsetof(Viterbi_obj,T),HX_CSTRING("T")},
	{hx::fsObject,(int)offsetof(Viterbi_obj,index),HX_CSTRING("index")},
	{hx::fsObject,(int)offsetof(Viterbi_obj,mode),HX_CSTRING("mode")},
	{hx::fsObject,(int)offsetof(Viterbi_obj,path_valid),HX_CSTRING("path_valid")},
	{hx::fsFloat,(int)offsetof(Viterbi_obj,best_cost),HX_CSTRING("best_cost")},
	{hx::fsObject,(int)offsetof(Viterbi_obj,cost),HX_CSTRING("cost")},
	{hx::fsObject,(int)offsetof(Viterbi_obj,src),HX_CSTRING("src")},
	{hx::fsObject,(int)offsetof(Viterbi_obj,path),HX_CSTRING("path")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
	HX_CSTRING("K"),
	HX_CSTRING("T"),
	HX_CSTRING("index"),
	HX_CSTRING("mode"),
	HX_CSTRING("path_valid"),
	HX_CSTRING("best_cost"),
	HX_CSTRING("cost"),
	HX_CSTRING("src"),
	HX_CSTRING("path"),
	HX_CSTRING("reset"),
	HX_CSTRING("setSize"),
	HX_CSTRING("assertMode"),
	HX_CSTRING("addTransition"),
	HX_CSTRING("endTransitions"),
	HX_CSTRING("beginTransitions"),
	HX_CSTRING("calculatePath"),
	HX_CSTRING("toString"),
	HX_CSTRING("length"),
	HX_CSTRING("get"),
	HX_CSTRING("getCost"),
	String(null()) };

static void sMarkStatics(HX_MARK_PARAMS) {
	HX_MARK_MEMBER_NAME(Viterbi_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(Viterbi_obj::__mClass,"__mClass");
};

#endif

Class Viterbi_obj::__mClass;

void Viterbi_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.Viterbi"), hx::TCanCast< Viterbi_obj> ,sStaticFields,sMemberFields,
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

void Viterbi_obj::__boot()
{
}

} // end namespace coopy
