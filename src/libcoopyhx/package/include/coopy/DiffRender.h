#ifndef INCLUDED_coopy_DiffRender
#define INCLUDED_coopy_DiffRender

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,CellInfo)
HX_DECLARE_CLASS1(coopy,DiffRender)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,TableText)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  DiffRender_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef DiffRender_obj OBJ_;
		DiffRender_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< DiffRender_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~DiffRender_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("DiffRender"); }

		Array< ::String > text_to_insert;
		::String td_open;
		::String td_close;
		bool open;
		bool pretty_arrows;
		virtual Void usePrettyArrows( bool flag);
		Dynamic usePrettyArrows_dyn();

		virtual Void insert( ::String str);
		Dynamic insert_dyn();

		virtual Void beginTable( );
		Dynamic beginTable_dyn();

		virtual Void beginRow( ::String mode);
		Dynamic beginRow_dyn();

		virtual Void insertCell( ::String txt,::String mode);
		Dynamic insertCell_dyn();

		virtual Void endRow( );
		Dynamic endRow_dyn();

		virtual Void endTable( );
		Dynamic endTable_dyn();

		virtual ::String html( );
		Dynamic html_dyn();

		virtual ::String toString( );
		Dynamic toString_dyn();

		virtual Void render( ::coopy::Table rows);
		Dynamic render_dyn();

		virtual ::String sampleCss( );
		Dynamic sampleCss_dyn();

		virtual Void completeHtml( );
		Dynamic completeHtml_dyn();

		static Void examineCell( int x,int y,::String value,::String vcol,::String vrow,::String vcorner,::coopy::CellInfo cell);
		static Dynamic examineCell_dyn();

		static ::coopy::CellInfo renderCell( ::coopy::TableText tt,int x,int y);
		static Dynamic renderCell_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_DiffRender */ 
