#ifndef INCLUDED_coopy_Mover
#define INCLUDED_coopy_Mover

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Mover)
HX_DECLARE_CLASS1(coopy,Unit)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Mover_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Mover_obj OBJ_;
		Mover_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Mover_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Mover_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Mover"); }

		static Array< int > moveUnits( Array< ::Dynamic > units);
		static Dynamic moveUnits_dyn();

		static Array< int > moveWithExtras( Array< int > isrc,Array< int > idest);
		static Dynamic moveWithExtras_dyn();

		static Array< int > moveWithoutExtras( Array< int > src,Array< int > dest);
		static Dynamic moveWithoutExtras_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Mover */ 
