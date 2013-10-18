#ifndef INCLUDED_coopy_Index
#define INCLUDED_coopy_Index

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS0(IMap)
HX_DECLARE_CLASS1(coopy,Index)
HX_DECLARE_CLASS1(coopy,Row)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,View)
HX_DECLARE_CLASS2(haxe,ds,StringMap)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Index_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Index_obj OBJ_;
		Index_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Index_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Index_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Index"); }

		::haxe::ds::StringMap items;
		Array< ::String > keys;
		int top_freq;
		int height;
		Array< int > cols;
		::coopy::View v;
		::coopy::Table indexed_table;
		virtual Void addColumn( int i);
		Dynamic addColumn_dyn();

		virtual Void indexTable( ::coopy::Table t);
		Dynamic indexTable_dyn();

		virtual ::String toKey( ::coopy::Table t,int i);
		Dynamic toKey_dyn();

		virtual ::String toKeyByContent( ::coopy::Row row);
		Dynamic toKeyByContent_dyn();

		virtual ::coopy::Table getTable( );
		Dynamic getTable_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Index */ 
