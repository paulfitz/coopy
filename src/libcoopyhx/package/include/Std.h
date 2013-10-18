#ifndef INCLUDED_Std
#define INCLUDED_Std

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS0(Std)


class HXCPP_CLASS_ATTRIBUTES  Std_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Std_obj OBJ_;
		Std_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Std_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Std_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Std"); }

		static ::String string( Dynamic s);
		static Dynamic string_dyn();

};


#endif /* INCLUDED_Std */ 
