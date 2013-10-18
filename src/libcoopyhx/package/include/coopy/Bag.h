#ifndef INCLUDED_coopy_Bag
#define INCLUDED_coopy_Bag

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Bag)
HX_DECLARE_CLASS1(coopy,View)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Bag_obj : public hx::Interface{
	public:
		typedef hx::Interface super;
		typedef Bag_obj OBJ_;
		HX_DO_INTERFACE_RTTI;
		static void __boot();
virtual Dynamic getItem( int x)=0;
		Dynamic getItem_dyn();
virtual ::coopy::View getItemView( )=0;
		Dynamic getItemView_dyn();
};

#define DELEGATE_coopy_Bag \
virtual Dynamic getItem( int x) { return mDelegate->getItem(x);}  \
virtual Dynamic getItem_dyn() { return mDelegate->getItem_dyn();}  \
virtual ::coopy::View getItemView( ) { return mDelegate->getItemView();}  \
virtual Dynamic getItemView_dyn() { return mDelegate->getItemView_dyn();}  \


template<typename IMPL>
class Bag_delegate_ : public Bag_obj
{
	protected:
		IMPL *mDelegate;
	public:
		Bag_delegate_(IMPL *inDelegate) : mDelegate(inDelegate) {}
		hx::Object *__GetRealObject() { return mDelegate; }
		void __Visit(HX_VISIT_PARAMS) { HX_VISIT_OBJECT(mDelegate); }
		DELEGATE_coopy_Bag
};

} // end namespace coopy

#endif /* INCLUDED_coopy_Bag */ 
