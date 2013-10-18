#ifndef INCLUDED_coopy_CellInfo
#define INCLUDED_coopy_CellInfo

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,CellInfo)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  CellInfo_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef CellInfo_obj OBJ_;
		CellInfo_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< CellInfo_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~CellInfo_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("CellInfo"); }

		::String value;
		::String pretty_value;
		::String category;
		::String category_given_tr;
		::String separator;
		bool updated;
		bool conflicted;
		::String pvalue;
		::String lvalue;
		::String rvalue;
		virtual ::String toString( );
		Dynamic toString_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_CellInfo */ 
