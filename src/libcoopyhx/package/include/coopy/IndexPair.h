#ifndef INCLUDED_coopy_IndexPair
#define INCLUDED_coopy_IndexPair

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,CrossMatch)
HX_DECLARE_CLASS1(coopy,Index)
HX_DECLARE_CLASS1(coopy,IndexPair)
HX_DECLARE_CLASS1(coopy,Row)
HX_DECLARE_CLASS1(coopy,Table)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  IndexPair_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef IndexPair_obj OBJ_;
		IndexPair_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< IndexPair_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~IndexPair_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("IndexPair"); }

		::coopy::Index ia;
		::coopy::Index ib;
		Float quality;
		virtual Void addColumn( int i);
		Dynamic addColumn_dyn();

		virtual Void addColumns( int ca,int cb);
		Dynamic addColumns_dyn();

		virtual Void indexTables( ::coopy::Table a,::coopy::Table b);
		Dynamic indexTables_dyn();

		virtual ::coopy::CrossMatch queryByKey( ::String ka);
		Dynamic queryByKey_dyn();

		virtual ::coopy::CrossMatch queryByContent( ::coopy::Row row);
		Dynamic queryByContent_dyn();

		virtual ::coopy::CrossMatch queryLocal( int row);
		Dynamic queryLocal_dyn();

		virtual int getTopFreq( );
		Dynamic getTopFreq_dyn();

		virtual Float getQuality( );
		Dynamic getQuality_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_IndexPair */ 
