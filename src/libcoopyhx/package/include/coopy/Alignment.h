#ifndef INCLUDED_coopy_Alignment
#define INCLUDED_coopy_Alignment

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS0(IMap)
HX_DECLARE_CLASS1(coopy,Alignment)
HX_DECLARE_CLASS1(coopy,Ordering)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS2(haxe,ds,IntMap)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Alignment_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Alignment_obj OBJ_;
		Alignment_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Alignment_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Alignment_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Alignment"); }

		::haxe::ds::IntMap map_a2b;
		::haxe::ds::IntMap map_b2a;
		int ha;
		int hb;
		::coopy::Table ta;
		::coopy::Table tb;
		int ia;
		int ib;
		int map_count;
		::coopy::Ordering order_cache;
		bool order_cache_has_reference;
		::coopy::Alignment reference;
		::coopy::Alignment meta;
		virtual Void range( int ha,int hb);
		Dynamic range_dyn();

		virtual Void tables( ::coopy::Table ta,::coopy::Table tb);
		Dynamic tables_dyn();

		virtual Void headers( int ia,int ib);
		Dynamic headers_dyn();

		virtual Void setRowlike( bool flag);
		Dynamic setRowlike_dyn();

		virtual Void link( int a,int b);
		Dynamic link_dyn();

		virtual Dynamic a2b( int a);
		Dynamic a2b_dyn();

		virtual Dynamic b2a( int b);
		Dynamic b2a_dyn();

		virtual int count( );
		Dynamic count_dyn();

		virtual ::String toString( );
		Dynamic toString_dyn();

		virtual ::coopy::Ordering toOrder( );
		Dynamic toOrder_dyn();

		virtual ::coopy::Table getSource( );
		Dynamic getSource_dyn();

		virtual ::coopy::Table getTarget( );
		Dynamic getTarget_dyn();

		virtual int getSourceHeader( );
		Dynamic getSourceHeader_dyn();

		virtual int getTargetHeader( );
		Dynamic getTargetHeader_dyn();

		virtual ::coopy::Ordering toOrder3( );
		Dynamic toOrder3_dyn();

		virtual ::coopy::Ordering toOrder2( );
		Dynamic toOrder2_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Alignment */ 
