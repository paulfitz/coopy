#ifndef INCLUDED_coopy_HighlightPatchUnit
#define INCLUDED_coopy_HighlightPatchUnit

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,HighlightPatchUnit)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  HighlightPatchUnit_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef HighlightPatchUnit_obj OBJ_;
		HighlightPatchUnit_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< HighlightPatchUnit_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~HighlightPatchUnit_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("HighlightPatchUnit"); }

		bool add;
		bool rem;
		bool update;
		::String code;
		int sourceRow;
		int sourceRowOffset;
		int sourcePrevRow;
		int sourceNextRow;
		int destRow;
		int patchRow;
		virtual ::String toString( );
		Dynamic toString_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_HighlightPatchUnit */ 
