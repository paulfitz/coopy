#ifndef INCLUDED_coopy_TableModifier
#define INCLUDED_coopy_TableModifier

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,TableModifier)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  TableModifier_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef TableModifier_obj OBJ_;
		TableModifier_obj();
		Void __construct(::coopy::Table t);

	public:
		static hx::ObjectPtr< TableModifier_obj > __new(::coopy::Table t);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~TableModifier_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("TableModifier"); }

		::coopy::Table t;
		virtual bool removeColumn( int at);
		Dynamic removeColumn_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_TableModifier */ 
