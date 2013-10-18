#ifndef INCLUDED_coopy_Csv
#define INCLUDED_coopy_Csv

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Csv)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,View)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Csv_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Csv_obj OBJ_;
		Csv_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Csv_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Csv_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Csv"); }

		int cursor;
		bool row_ended;
		bool has_structure;
		virtual ::String renderTable( ::coopy::Table t);
		Dynamic renderTable_dyn();

		virtual ::String renderCell( ::coopy::View v,Dynamic d);
		Dynamic renderCell_dyn();

		virtual Array< ::Dynamic > parseTable( ::String txt);
		Dynamic parseTable_dyn();

		virtual ::String parseCell( ::String txt);
		Dynamic parseCell_dyn();

		virtual ::String parseSingleCell( ::String txt);
		Dynamic parseSingleCell_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Csv */ 
