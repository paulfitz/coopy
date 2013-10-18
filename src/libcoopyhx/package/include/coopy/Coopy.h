#ifndef INCLUDED_coopy_Coopy
#define INCLUDED_coopy_Coopy

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,CompareTable)
HX_DECLARE_CLASS1(coopy,Coopy)
HX_DECLARE_CLASS1(coopy,Mover)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,TableIO)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Coopy_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Coopy_obj OBJ_;
		Coopy_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Coopy_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Coopy_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Coopy"); }

		::String format_preference;
		::coopy::TableIO io;
		::coopy::Mover mv;
		static ::coopy::CompareTable compareTables( ::coopy::Table local,::coopy::Table remote);
		static Dynamic compareTables_dyn();

		static ::coopy::CompareTable compareTables3( ::coopy::Table parent,::coopy::Table local,::coopy::Table remote);
		static Dynamic compareTables3_dyn();

		static int randomTests( );
		static Dynamic randomTests_dyn();

		static int main( );
		static Dynamic main_dyn();

		static Void show( ::coopy::Table t);
		static Dynamic show_dyn();

		static Dynamic jsonify( ::coopy::Table t);
		static Dynamic jsonify_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Coopy */ 
