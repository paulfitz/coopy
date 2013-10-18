#ifndef INCLUDED_coopy_CompareTable
#define INCLUDED_coopy_CompareTable

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Alignment)
HX_DECLARE_CLASS1(coopy,CompareTable)
HX_DECLARE_CLASS1(coopy,IndexPair)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,TableComparisonState)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  CompareTable_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef CompareTable_obj OBJ_;
		CompareTable_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< CompareTable_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~CompareTable_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("CompareTable"); }

		::coopy::TableComparisonState comp;
		Array< ::Dynamic > indexes;
		virtual bool attach( ::coopy::TableComparisonState comp);
		Dynamic attach_dyn();

		virtual ::coopy::Alignment align( );
		Dynamic align_dyn();

		virtual ::coopy::TableComparisonState getComparisonState( );
		Dynamic getComparisonState_dyn();

		virtual Void alignCore( ::coopy::Alignment align);
		Dynamic alignCore_dyn();

		virtual Void alignCore2( ::coopy::Alignment align,::coopy::Table a,::coopy::Table b);
		Dynamic alignCore2_dyn();

		virtual Void alignColumns( ::coopy::Alignment align,::coopy::Table a,::coopy::Table b);
		Dynamic alignColumns_dyn();

		virtual bool testHasSameColumns( );
		Dynamic testHasSameColumns_dyn();

		virtual bool hasSameColumns2( ::coopy::Table a,::coopy::Table b);
		Dynamic hasSameColumns2_dyn();

		virtual bool testIsEqual( );
		Dynamic testIsEqual_dyn();

		virtual bool isEqual2( ::coopy::Table a,::coopy::Table b);
		Dynamic isEqual2_dyn();

		virtual bool compareCore( );
		Dynamic compareCore_dyn();

		virtual Void storeIndexes( );
		Dynamic storeIndexes_dyn();

		virtual Array< ::Dynamic > getIndexes( );
		Dynamic getIndexes_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_CompareTable */ 
