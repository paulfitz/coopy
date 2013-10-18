#ifndef INCLUDED_coopy_Row
#define INCLUDED_coopy_Row

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Row)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Row_obj : public hx::Interface{
	public:
		typedef hx::Interface super;
		typedef Row_obj OBJ_;
		HX_DO_INTERFACE_RTTI;
		static void __boot();
virtual ::String getRowString( int c)=0;
		Dynamic getRowString_dyn();
};

#define DELEGATE_coopy_Row \
virtual ::String getRowString( int c) { return mDelegate->getRowString(c);}  \
virtual Dynamic getRowString_dyn() { return mDelegate->getRowString_dyn();}  \


template<typename IMPL>
class Row_delegate_ : public Row_obj
{
	protected:
		IMPL *mDelegate;
	public:
		Row_delegate_(IMPL *inDelegate) : mDelegate(inDelegate) {}
		hx::Object *__GetRealObject() { return mDelegate; }
		void __Visit(HX_VISIT_PARAMS) { HX_VISIT_OBJECT(mDelegate); }
		DELEGATE_coopy_Row
};

} // end namespace coopy

#endif /* INCLUDED_coopy_Row */ 
