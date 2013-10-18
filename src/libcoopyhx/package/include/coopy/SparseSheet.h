#ifndef INCLUDED_coopy_SparseSheet
#define INCLUDED_coopy_SparseSheet

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS0(IMap)
HX_DECLARE_CLASS1(coopy,SparseSheet)
HX_DECLARE_CLASS2(haxe,ds,IntMap)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  SparseSheet_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef SparseSheet_obj OBJ_;
		SparseSheet_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< SparseSheet_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~SparseSheet_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("SparseSheet"); }

		int h;
		int w;
		::haxe::ds::IntMap row;
		Dynamic zero;
		virtual Void resize( int w,int h,Dynamic zero);
		Dynamic resize_dyn();

		virtual Void nonDestructiveResize( int w,int h,Dynamic zero);
		Dynamic nonDestructiveResize_dyn();

		virtual Dynamic get( int x,int y);
		Dynamic get_dyn();

		virtual Void set( int x,int y,Dynamic val);
		Dynamic set_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_SparseSheet */ 
