#ifndef INCLUDED_coopy_IndexItem
#define INCLUDED_coopy_IndexItem

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,IndexItem)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  IndexItem_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef IndexItem_obj OBJ_;
		IndexItem_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< IndexItem_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~IndexItem_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("IndexItem"); }

		Array< int > lst;
		virtual int add( int i);
		Dynamic add_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_IndexItem */ 
