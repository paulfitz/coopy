#ifndef INCLUDED_coopy_Compare
#define INCLUDED_coopy_Compare

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Compare)
HX_DECLARE_CLASS1(coopy,Report)
HX_DECLARE_CLASS1(coopy,ViewedDatum)
HX_DECLARE_CLASS1(coopy,Workspace)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Compare_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Compare_obj OBJ_;
		Compare_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Compare_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Compare_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Compare"); }

		virtual bool compare( ::coopy::ViewedDatum parent,::coopy::ViewedDatum local,::coopy::ViewedDatum remote,::coopy::Report report);
		Dynamic compare_dyn();

		virtual bool compareStructured( ::coopy::Workspace ws);
		Dynamic compareStructured_dyn();

		virtual bool compareTable( ::coopy::Workspace ws);
		Dynamic compareTable_dyn();

		virtual bool comparePrimitive( ::coopy::Workspace ws);
		Dynamic comparePrimitive_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Compare */ 
