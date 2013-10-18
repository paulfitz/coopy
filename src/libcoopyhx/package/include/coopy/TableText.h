#ifndef INCLUDED_coopy_TableText
#define INCLUDED_coopy_TableText

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,TableText)
HX_DECLARE_CLASS1(coopy,View)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  TableText_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef TableText_obj OBJ_;
		TableText_obj();
		Void __construct(::coopy::Table rows);

	public:
		static hx::ObjectPtr< TableText_obj > __new(::coopy::Table rows);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~TableText_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("TableText"); }

		::coopy::Table rows;
		::coopy::View view;
		virtual ::String getCellText( int x,int y);
		Dynamic getCellText_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_TableText */ 
