#ifndef INCLUDED_coopy_Table
#define INCLUDED_coopy_Table

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,View)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Table_obj : public hx::Interface{
	public:
		typedef hx::Interface super;
		typedef Table_obj OBJ_;
		HX_DO_INTERFACE_RTTI;
		static void __boot();
virtual Dynamic getCell( int x,int y)=0;
		Dynamic getCell_dyn();
virtual Void setCell( int x,int y,Dynamic c)=0;
		Dynamic setCell_dyn();
virtual ::coopy::View getCellView( )=0;
		Dynamic getCellView_dyn();
virtual bool isResizable( )=0;
		Dynamic isResizable_dyn();
virtual bool resize( int w,int h)=0;
		Dynamic resize_dyn();
virtual Void clear( )=0;
		Dynamic clear_dyn();
virtual bool insertOrDeleteRows( Array< int > fate,int hfate)=0;
		Dynamic insertOrDeleteRows_dyn();
virtual bool insertOrDeleteColumns( Array< int > fate,int wfate)=0;
		Dynamic insertOrDeleteColumns_dyn();
virtual bool trimBlank( )=0;
		Dynamic trimBlank_dyn();
};

#define DELEGATE_coopy_Table \
virtual Dynamic getCell( int x,int y) { return mDelegate->getCell(x,y);}  \
virtual Dynamic getCell_dyn() { return mDelegate->getCell_dyn();}  \
virtual Void setCell( int x,int y,Dynamic c) { return mDelegate->setCell(x,y,c);}  \
virtual Dynamic setCell_dyn() { return mDelegate->setCell_dyn();}  \
virtual ::coopy::View getCellView( ) { return mDelegate->getCellView();}  \
virtual Dynamic getCellView_dyn() { return mDelegate->getCellView_dyn();}  \
virtual bool isResizable( ) { return mDelegate->isResizable();}  \
virtual Dynamic isResizable_dyn() { return mDelegate->isResizable_dyn();}  \
virtual bool resize( int w,int h) { return mDelegate->resize(w,h);}  \
virtual Dynamic resize_dyn() { return mDelegate->resize_dyn();}  \
virtual Void clear( ) { return mDelegate->clear();}  \
virtual Dynamic clear_dyn() { return mDelegate->clear_dyn();}  \
virtual bool insertOrDeleteRows( Array< int > fate,int hfate) { return mDelegate->insertOrDeleteRows(fate,hfate);}  \
virtual Dynamic insertOrDeleteRows_dyn() { return mDelegate->insertOrDeleteRows_dyn();}  \
virtual bool insertOrDeleteColumns( Array< int > fate,int wfate) { return mDelegate->insertOrDeleteColumns(fate,wfate);}  \
virtual Dynamic insertOrDeleteColumns_dyn() { return mDelegate->insertOrDeleteColumns_dyn();}  \
virtual bool trimBlank( ) { return mDelegate->trimBlank();}  \
virtual Dynamic trimBlank_dyn() { return mDelegate->trimBlank_dyn();}  \


template<typename IMPL>
class Table_delegate_ : public Table_obj
{
	protected:
		IMPL *mDelegate;
	public:
		Table_delegate_(IMPL *inDelegate) : mDelegate(inDelegate) {}
		hx::Object *__GetRealObject() { return mDelegate; }
		void __Visit(HX_VISIT_PARAMS) { HX_VISIT_OBJECT(mDelegate); }
		DELEGATE_coopy_Table
};

} // end namespace coopy

#endif /* INCLUDED_coopy_Table */ 
