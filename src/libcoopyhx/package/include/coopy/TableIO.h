#ifndef INCLUDED_coopy_TableIO
#define INCLUDED_coopy_TableIO

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,TableIO)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  TableIO_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef TableIO_obj OBJ_;
		TableIO_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< TableIO_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~TableIO_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("TableIO"); }

		virtual ::String getContent( ::String name);
		Dynamic getContent_dyn();

		virtual bool saveContent( ::String name,::String txt);
		Dynamic saveContent_dyn();

		virtual Array< ::String > args( );
		Dynamic args_dyn();

		virtual Void writeStdout( ::String txt);
		Dynamic writeStdout_dyn();

		virtual Void writeStderr( ::String txt);
		Dynamic writeStderr_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_TableIO */ 
