#ifndef INCLUDED_coopy_Unit
#define INCLUDED_coopy_Unit

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Unit)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Unit_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Unit_obj OBJ_;
		Unit_obj();
		Void __construct(hx::Null< int >  __o_l,hx::Null< int >  __o_r,hx::Null< int >  __o_p);

	public:
		static hx::ObjectPtr< Unit_obj > __new(hx::Null< int >  __o_l,hx::Null< int >  __o_r,hx::Null< int >  __o_p);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Unit_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Unit"); }

		int l;
		int r;
		int p;
		virtual int lp( );
		Dynamic lp_dyn();

		virtual ::String toString( );
		Dynamic toString_dyn();

		virtual bool fromString( ::String txt);
		Dynamic fromString_dyn();

		static ::String describe( int i);
		static Dynamic describe_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Unit */ 
