#ifndef INCLUDED_coopy_Ordering
#define INCLUDED_coopy_Ordering

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Ordering)
HX_DECLARE_CLASS1(coopy,Unit)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Ordering_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Ordering_obj OBJ_;
		Ordering_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Ordering_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Ordering_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Ordering"); }

		Array< ::Dynamic > order;
		bool ignore_parent;
		virtual Void add( int l,int r,hx::Null< int >  p);
		Dynamic add_dyn();

		virtual Array< ::Dynamic > getList( );
		Dynamic getList_dyn();

		virtual ::String toString( );
		Dynamic toString_dyn();

		virtual Void ignoreParent( );
		Dynamic ignoreParent_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Ordering */ 
