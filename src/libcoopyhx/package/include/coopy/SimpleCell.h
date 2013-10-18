#ifndef INCLUDED_coopy_SimpleCell
#define INCLUDED_coopy_SimpleCell

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,SimpleCell)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  SimpleCell_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef SimpleCell_obj OBJ_;
		SimpleCell_obj();
		Void __construct(Dynamic x);

	public:
		static hx::ObjectPtr< SimpleCell_obj > __new(Dynamic x);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~SimpleCell_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("SimpleCell"); }

		Dynamic datum;
		virtual ::String toString( );
		Dynamic toString_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_SimpleCell */ 
