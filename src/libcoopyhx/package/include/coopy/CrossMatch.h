#ifndef INCLUDED_coopy_CrossMatch
#define INCLUDED_coopy_CrossMatch

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,CrossMatch)
HX_DECLARE_CLASS1(coopy,IndexItem)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  CrossMatch_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef CrossMatch_obj OBJ_;
		CrossMatch_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< CrossMatch_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~CrossMatch_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("CrossMatch"); }

		int spot_a;
		int spot_b;
		::coopy::IndexItem item_a;
		::coopy::IndexItem item_b;
};

} // end namespace coopy

#endif /* INCLUDED_coopy_CrossMatch */ 
