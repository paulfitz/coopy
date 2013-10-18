#include <coopy/CoopyhxLink.h>

#define HX_DECLARE_MAIN 1
#include <hxcpp.h>

#include <stdio.h>

#include <coopy/SimpleTable.h>
#include <coopy/SimpleCell.h>
#include <coopy/Coopy.h>
#include <coopy/Alignment.h>
#include <coopy/CompareTable.h>
#include <coopy/CompareFlags.h>
#include <coopy/TableDiff.h>
#include <IMap.h>
#include <Std.h>
#include <coopy/SimpleView.h>
#include <coopy/Table.h>
#include <coopy/View.h>
#include <haxe/ds/IntMap.h>

HX_DECLARE_CLASS1(coopy,DataSheetTable)
namespace coopy {
class HXCPP_CLASS_ATTRIBUTES  DataSheetTable_obj : public hx::Object{
 private:
  CoopyhxTable *tab;
	public:
		typedef hx::Object super;
		typedef DataSheetTable_obj OBJ_;
		DataSheetTable_obj();
		Void __construct(CoopyhxTable& tab);

	public:
		static hx::ObjectPtr< DataSheetTable_obj > __new(CoopyhxTable& tab);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~DataSheetTable_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		// hxcpp output massaged for SWIG //SWIGFIX
		inline ::hx::ObjectPtr< ::coopy::Table_obj> asTable() //SWIGFIX
			{ return ::hx::ObjectPtr< ::coopy::Table_obj>(this); } //SWIGFIX
#ifndef SWIG //SWIGFIX
		inline operator ::coopy::Table_obj *()
			{ return new ::coopy::Table_delegate_< DataSheetTable_obj >(this); }
#endif //SWIGFIX
		hx::Object *__ToInterface(const hx::type_info &inType);
		::String __ToString() const { return HX_CSTRING("DataSheetTable"); }

		virtual ::coopy::Table getTable( );
		Dynamic getTable_dyn();

		virtual int get_width( );
		Dynamic get_width_dyn();

		virtual int get_height( );
		Dynamic get_height_dyn();

		virtual int get_size( );
		Dynamic get_size_dyn();

		virtual Dynamic getCell( int x,int y);
		Dynamic getCell_dyn();

		virtual Void setCell( int x,int y,Dynamic c);
		Dynamic setCell_dyn();

		virtual ::String toString( );
		Dynamic toString_dyn();

		virtual ::coopy::View getCellView( );
		Dynamic getCellView_dyn();

		virtual bool isResizable( );
		Dynamic isResizable_dyn();

		virtual bool resize( int w,int h);
		Dynamic resize_dyn();

		virtual Void clear( );
		Dynamic clear_dyn();

		virtual bool insertOrDeleteRows( Array< int > fate,int hfate);
		Dynamic insertOrDeleteRows_dyn();

		virtual bool insertOrDeleteColumns( Array< int > fate,int wfate);
		Dynamic insertOrDeleteColumns_dyn();

		virtual bool trimBlank( );
		Dynamic trimBlank_dyn();

		static ::String tableToString( ::coopy::Table tab);
		static Dynamic tableToString_dyn();

};


Void DataSheetTable_obj::__construct(CoopyhxTable& tab)
{
{
  this->tab = &tab;
}
;
	return null();
}

DataSheetTable_obj::~DataSheetTable_obj() { }

Dynamic DataSheetTable_obj::__CreateEmpty() { return  new DataSheetTable_obj; }
hx::ObjectPtr< DataSheetTable_obj > DataSheetTable_obj::__new(CoopyhxTable& tab)
{  hx::ObjectPtr< DataSheetTable_obj > result = new DataSheetTable_obj();
	result->__construct(tab);
	return result;}

Dynamic DataSheetTable_obj::__Create(hx::DynamicArray inArgs)
{  hx::ObjectPtr< DataSheetTable_obj > result = new DataSheetTable_obj();
  fprintf(stderr,"__Create variant not implemented\n");
	return result;}

hx::Object *DataSheetTable_obj::__ToInterface(const hx::type_info &inType) {
	if (inType==typeid( ::coopy::Table_obj)) return operator ::coopy::Table_obj *();
	return super::__ToInterface(inType);
}

::coopy::Table DataSheetTable_obj::getTable( ){
	HX_STACK_FRAME("coopy.DataSheetTable","getTable",0xb450428c,"coopy.DataSheetTable.getTable","coopy/DataSheetTable.hx",18,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(18)
	return hx::ObjectPtr<OBJ_>(this);
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,getTable,return )

int DataSheetTable_obj::get_width( ){
	HX_STACK_FRAME("coopy.DataSheetTable","get_width",0x27422ce9,"coopy.DataSheetTable.get_width","coopy/DataSheetTable.hx",26,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(26)
	  return tab->width();
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,get_width,return )

int DataSheetTable_obj::get_height( ){
	HX_STACK_FRAME("coopy.DataSheetTable","get_height",0xe8f497a4,"coopy.DataSheetTable.get_height","coopy/DataSheetTable.hx",30,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(30)
	  return tab->height();
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,get_height,return )

int DataSheetTable_obj::get_size( ){
	HX_STACK_FRAME("coopy.DataSheetTable","get_size",0x15a3d73e,"coopy.DataSheetTable.get_size","coopy/DataSheetTable.hx",34,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(34)
	  return tab->height();
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,get_size,return )

Dynamic DataSheetTable_obj::getCell( int x,int y){
	HX_STACK_FRAME("coopy.DataSheetTable","getCell",0x1805f0e4,"coopy.DataSheetTable.getCell","coopy/DataSheetTable.hx",38,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_ARG(x,"x")
	HX_STACK_ARG(y,"y")
	HX_STACK_LINE(38)
	  bool isNull = false;
	Dynamic result = Dynamic(tab->getCell(x,y,isNull).c_str());
	return isNull?Dynamic():result;
}


HX_DEFINE_DYNAMIC_FUNC2(DataSheetTable_obj,getCell,return )

Void DataSheetTable_obj::setCell( int x,int y,Dynamic c){
{
		HX_STACK_FRAME("coopy.DataSheetTable","setCell",0x0b0781f0,"coopy.DataSheetTable.setCell","coopy/DataSheetTable.hx",42,0x278206c5)
		HX_STACK_THIS(this)
		HX_STACK_ARG(x,"x")
		HX_STACK_ARG(y,"y")
		HX_STACK_ARG(c,"c")
		HX_STACK_LINE(42)
		Dynamic value = c;		HX_STACK_VAR(value,"value");
		HX_STACK_LINE(42)
		  fprintf(stderr,"setCell not implemented\n");
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC3(DataSheetTable_obj,setCell,(void))

::String DataSheetTable_obj::toString( ){
	HX_STACK_FRAME("coopy.DataSheetTable","toString",0x82044ba0,"coopy.DataSheetTable.toString","coopy/DataSheetTable.hx",46,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(46)
	return ::coopy::DataSheetTable_obj::tableToString(hx::ObjectPtr<OBJ_>(this));
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,toString,return )

::coopy::View DataSheetTable_obj::getCellView( ){
	HX_STACK_FRAME("coopy.DataSheetTable","getCellView",0xaa429169,"coopy.DataSheetTable.getCellView","coopy/DataSheetTable.hx",62,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(62)
	return ::coopy::SimpleView_obj::__new();
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,getCellView,return )

bool DataSheetTable_obj::isResizable( ){
	HX_STACK_FRAME("coopy.DataSheetTable","isResizable",0xeeabc3ad,"coopy.DataSheetTable.isResizable","coopy/DataSheetTable.hx",66,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_LINE(66)
	return true;
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,isResizable,return )

bool DataSheetTable_obj::resize( int w,int h){
	HX_STACK_FRAME("coopy.DataSheetTable","resize",0x3c286fe8,"coopy.DataSheetTable.resize","coopy/DataSheetTable.hx",69,0x278206c5)
	HX_STACK_THIS(this)
	HX_STACK_ARG(w,"w")
	HX_STACK_ARG(h,"h")
	HX_STACK_LINE(70)
	  fprintf(stderr,"resize not implemented\n");
	return false;
}


HX_DEFINE_DYNAMIC_FUNC2(DataSheetTable_obj,resize,return )

Void DataSheetTable_obj::clear( ){
{
		HX_STACK_FRAME("coopy.DataSheetTable","clear",0x74365619,"coopy.DataSheetTable.clear","coopy/DataSheetTable.hx",76,0x278206c5)
		HX_STACK_THIS(this)
		HX_STACK_LINE(76)
		  fprintf(stderr,"clear not implemented\n");
	}
return null();
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,clear,(void))

bool DataSheetTable_obj::insertOrDeleteRows( Array< int > fate,int hfate){
  fprintf(stderr,"insertOrDeleteRows not implemented\n");
  return false;
}


HX_DEFINE_DYNAMIC_FUNC2(DataSheetTable_obj,insertOrDeleteRows,return )

bool DataSheetTable_obj::insertOrDeleteColumns( Array< int > fate,int wfate){
  fprintf(stderr,"insertOrDeleteColumns not implemented\n");
  return false;
}


HX_DEFINE_DYNAMIC_FUNC2(DataSheetTable_obj,insertOrDeleteColumns,return )

bool DataSheetTable_obj::trimBlank( ){
  fprintf(stderr,"trimBlank not implemented\n");
  return false;
}


HX_DEFINE_DYNAMIC_FUNC0(DataSheetTable_obj,trimBlank,return )

::String DataSheetTable_obj::tableToString( ::coopy::Table tab){
	HX_STACK_FRAME("coopy.DataSheetTable","tableToString",0x38145366,"coopy.DataSheetTable.tableToString","coopy/DataSheetTable.hx",49,0x278206c5)
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


STATIC_HX_DEFINE_DYNAMIC_FUNC1(DataSheetTable_obj,tableToString,return )


DataSheetTable_obj::DataSheetTable_obj()
{
  tab = NULL;
}

void DataSheetTable_obj::__Mark(HX_MARK_PARAMS)
{
	HX_MARK_BEGIN_CLASS(DataSheetTable);
	HX_MARK_END_CLASS();
}

void DataSheetTable_obj::__Visit(HX_VISIT_PARAMS)
{
}

Dynamic DataSheetTable_obj::__Field(const ::String &inName,bool inCallProp)
{
	switch(inName.length) {
	case 4:
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

Dynamic DataSheetTable_obj::__SetField(const ::String &inName,const Dynamic &inValue,bool inCallProp)
{
	return super::__SetField(inName,inValue,inCallProp);
}

void DataSheetTable_obj::__GetFields(Array< ::String> &outFields)
{
	super::__GetFields(outFields);
};

static ::String sStaticFields[] = {
	HX_CSTRING("tableToString"),
	String(null()) };

#if HXCPP_SCRIPTABLE
static hx::StorageInfo sMemberStorageInfo[] = {
	{hx::fsObject,(int)offsetof(DataSheetTable_obj,data),HX_CSTRING("data")},
	{hx::fsObject,(int)offsetof(DataSheetTable_obj,w),HX_CSTRING("w")},
	{hx::fsObject,(int)offsetof(DataSheetTable_obj,h),HX_CSTRING("h")},
	{ hx::fsUnknown, 0, null()}
};
#endif

static ::String sMemberFields[] = {
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
	HX_MARK_MEMBER_NAME(DataSheetTable_obj::__mClass,"__mClass");
};

#ifdef HXCPP_VISIT_ALLOCS
static void sVisitStatics(HX_VISIT_PARAMS) {
	HX_VISIT_MEMBER_NAME(DataSheetTable_obj::__mClass,"__mClass");
};

#endif

Class DataSheetTable_obj::__mClass;

void DataSheetTable_obj::__register()
{
	hx::Static(__mClass) = hx::RegisterClass(HX_CSTRING("coopy.DataSheetTable"), hx::TCanCast< DataSheetTable_obj> ,sStaticFields,sMemberFields,
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

void DataSheetTable_obj::__boot()
{
}

} // end namespace coopy

::coopy::SimpleCell zap(const char *txt) {
  return ::coopy::SimpleCell_obj::__new(String(txt));
}

void CoopyhxLink::compare(CoopyhxTable& pivot, CoopyhxTable& local,
			  CoopyhxTable& remote, CoopyhxResult& result) {
  ::coopy::DataSheetTable x = ::coopy::DataSheetTable_obj::__new(pivot);
  ::coopy::DataSheetTable y = ::coopy::DataSheetTable_obj::__new(local);
  ::coopy::DataSheetTable z = ::coopy::DataSheetTable_obj::__new(remote);
  ::coopy::CompareTable cmp = ::coopy::Coopy_obj::compareTables3(x,y,z);
  ::coopy::Alignment alignment = cmp->align();

  int dpivot = -pivot.getOffset();
  int dlocal = -local.getOffset();
  int dremote = -remote.getOffset();

  for (int i=0; i<alignment->ha; i++) {
    Dynamic r = alignment->map_a2b->get(i);
    if (r!=null()) {
      result.order(i+dpivot,(int)r+dremote,true,false);
    }
  }
  for (int i=0; i<alignment->reference->ha; i++) {
    Dynamic r = alignment->reference->map_a2b->get(i);
    if (r!=null()) {
      result.order(i+dpivot,(int)r+dlocal,true,true);
    }
  }
  for (int i=0; i<alignment->meta->ha; i++) {
    Dynamic r = alignment->meta->map_a2b->get(i);
    if (r!=null()) {
      result.order(i,(int)r,false,false);
    }
  }
  for (int i=0; i<alignment->reference->meta->ha; i++) {
    Dynamic r = alignment->reference->meta->map_a2b->get(i);
    if (r!=null()) {
      result.order(i,(int)r,false,true);
    }
  }
  //::coopy::CompareFlags flags = ::coopy::CompareFlags_obj::__new();
  //::coopy::TableDiff highlighter = ::coopy::TableDiff_obj::__new(alignment,flags);
  //::coopy::SimpleTable table_diff = ::coopy::SimpleTable_obj::__new(0,0);
  //highlighter->hilite(table_diff);
  //::String tab = table_diff->tableToString(table_diff);
  //printf("diff: %s\n", tab.__CStr());
}

int my_main() {
  try{
    ::coopy::SimpleTable output = ::coopy::SimpleTable_obj::__new(10,20);
    ::coopy::SimpleCell cell = ::coopy::SimpleCell_obj::__new(16);
    output->setCell(3,3,cell);
    Dynamic datum = output->getCell(3,3);
    printf("value %s\n", datum->toString().__CStr());
    ::coopy::SimpleTable t1 = ::coopy::SimpleTable_obj::__new(3,3);
    ::coopy::SimpleTable t2 = ::coopy::SimpleTable_obj::__new(3,3);
    t1->setCell(0,0,zap("NAME"));
    t1->setCell(1,0,zap("AGE"));
    t1->setCell(2,0,zap("LOCATION"));
    t1->setCell(0,1,zap("Paul"));
    t1->setCell(1,1,zap("11"));
    t1->setCell(2,1,zap("Space"));
    t2->setCell(0,0,zap("NAME"));
    t2->setCell(1,0,zap("AGE"));
    t2->setCell(2,0,zap("LOCATION"));
    t2->setCell(0,1,zap("Paul"));
    t2->setCell(1,1,zap("88"));
    t2->setCell(2,1,zap("Space"));
    datum = t2->getCell(1,1);
    printf("value2 %s\n", datum->toString().__CStr());
    printf("t1: %s\n", t1->toString().__CStr());
    printf("t2: %s\n", t2->toString().__CStr());
    ::coopy::SimpleTable table_diff = ::coopy::SimpleTable_obj::__new(0,0);
    ::coopy::CompareTable cmp = ::coopy::Coopy_obj::compareTables(t1,t2);
    ::coopy::Alignment alignment = cmp->align();
    printf("align:\n", alignment->toString().__CStr());

    

    ::coopy::CompareFlags flags = ::coopy::CompareFlags_obj::__new();
    ::coopy::TableDiff highlighter = ::coopy::TableDiff_obj::__new(alignment,flags);
    highlighter->hilite(table_diff);
    ::String tab = table_diff->tableToString(table_diff);
    printf("diff: %s\n", tab.__CStr());
  } catch (Dynamic e){
    printf("Bailing out, error : %s\n",e->toString().__CStr());
    __hx_dump_stack();
  }
  return 0;
}

void CoopyhxLink::init() {
  HX_TOP_OF_STACK
    hx::Boot();
  __boot_all();
}

void CoopyhxLink::probe() {
  my_main();
}
