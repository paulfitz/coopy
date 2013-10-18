#ifndef INCLUDED_coopy_Change
#define INCLUDED_coopy_Change

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Change)
HX_DECLARE_CLASS1(coopy,ChangeType)
HX_DECLARE_CLASS1(coopy,ViewedDatum)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Change_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Change_obj OBJ_;
		Change_obj();
		Void __construct(::String txt);

	public:
		static hx::ObjectPtr< Change_obj > __new(::String txt);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Change_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Change"); }

		::String change;
		::coopy::ViewedDatum parent;
		::coopy::ViewedDatum local;
		::coopy::ViewedDatum remote;
		::coopy::ChangeType mode;
		virtual ::String toString( );
		Dynamic toString_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Change */ 
