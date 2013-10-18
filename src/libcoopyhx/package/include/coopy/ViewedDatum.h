#ifndef INCLUDED_coopy_ViewedDatum
#define INCLUDED_coopy_ViewedDatum

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Bag)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,View)
HX_DECLARE_CLASS1(coopy,ViewedDatum)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  ViewedDatum_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef ViewedDatum_obj OBJ_;
		ViewedDatum_obj();
		Void __construct(Dynamic datum,::coopy::View view);

	public:
		static hx::ObjectPtr< ViewedDatum_obj > __new(Dynamic datum,::coopy::View view);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~ViewedDatum_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("ViewedDatum"); }

		Dynamic datum;
		::coopy::View view;
		virtual ::String toString( );
		Dynamic toString_dyn();

		virtual ::coopy::Bag getBag( );
		Dynamic getBag_dyn();

		virtual ::coopy::Table getTable( );
		Dynamic getTable_dyn();

		virtual bool hasStructure( );
		Dynamic hasStructure_dyn();

		static ::coopy::ViewedDatum getSimpleView( Dynamic datum);
		static Dynamic getSimpleView_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_ViewedDatum */ 
