#ifndef INCLUDED_Lambda
#define INCLUDED_Lambda

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS0(Lambda)
HX_DECLARE_CLASS0(List)


class HXCPP_CLASS_ATTRIBUTES  Lambda_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Lambda_obj OBJ_;
		Lambda_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Lambda_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Lambda_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Lambda"); }

		static Dynamic array( Dynamic it);
		static Dynamic array_dyn();

		static ::List map( Dynamic it,Dynamic f);
		static Dynamic map_dyn();

};


#endif /* INCLUDED_Lambda */ 
