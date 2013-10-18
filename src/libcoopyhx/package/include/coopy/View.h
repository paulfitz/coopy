#ifndef INCLUDED_coopy_View
#define INCLUDED_coopy_View

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Bag)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,View)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  View_obj : public hx::Interface{
	public:
		typedef hx::Interface super;
		typedef View_obj OBJ_;
		HX_DO_INTERFACE_RTTI;
		static void __boot();
virtual ::String toString( Dynamic d)=0;
		Dynamic toString_dyn();
virtual ::coopy::Bag getBag( Dynamic d)=0;
		Dynamic getBag_dyn();
virtual ::coopy::Table getTable( Dynamic d)=0;
		Dynamic getTable_dyn();
virtual bool hasStructure( Dynamic d)=0;
		Dynamic hasStructure_dyn();
virtual bool equals( Dynamic d1,Dynamic d2)=0;
		Dynamic equals_dyn();
virtual Dynamic toDatum( ::String str)=0;
		Dynamic toDatum_dyn();
};

#define DELEGATE_coopy_View \
virtual ::String toString( Dynamic d) { return mDelegate->toString(d);}  \
virtual Dynamic toString_dyn() { return mDelegate->toString_dyn();}  \
virtual ::coopy::Bag getBag( Dynamic d) { return mDelegate->getBag(d);}  \
virtual Dynamic getBag_dyn() { return mDelegate->getBag_dyn();}  \
virtual ::coopy::Table getTable( Dynamic d) { return mDelegate->getTable(d);}  \
virtual Dynamic getTable_dyn() { return mDelegate->getTable_dyn();}  \
virtual bool hasStructure( Dynamic d) { return mDelegate->hasStructure(d);}  \
virtual Dynamic hasStructure_dyn() { return mDelegate->hasStructure_dyn();}  \
virtual bool equals( Dynamic d1,Dynamic d2) { return mDelegate->equals(d1,d2);}  \
virtual Dynamic equals_dyn() { return mDelegate->equals_dyn();}  \
virtual Dynamic toDatum( ::String str) { return mDelegate->toDatum(str);}  \
virtual Dynamic toDatum_dyn() { return mDelegate->toDatum_dyn();}  \


template<typename IMPL>
class View_delegate_ : public View_obj
{
	protected:
		IMPL *mDelegate;
	public:
		View_delegate_(IMPL *inDelegate) : mDelegate(inDelegate) {}
		hx::Object *__GetRealObject() { return mDelegate; }
		void __Visit(HX_VISIT_PARAMS) { HX_VISIT_OBJECT(mDelegate); }
		DELEGATE_coopy_View
};

} // end namespace coopy

#endif /* INCLUDED_coopy_View */ 
