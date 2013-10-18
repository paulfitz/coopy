#ifndef INCLUDED_coopy_CompareFlags
#define INCLUDED_coopy_CompareFlags

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,CompareFlags)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  CompareFlags_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef CompareFlags_obj OBJ_;
		CompareFlags_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< CompareFlags_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~CompareFlags_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("CompareFlags"); }

		bool always_show_header;
		bool show_unchanged;
		int unchanged_context;
		bool always_show_order;
		bool never_show_order;
		bool ordered;
};

} // end namespace coopy

#endif /* INCLUDED_coopy_CompareFlags */ 
