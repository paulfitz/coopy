#ifndef INCLUDED_coopy_HighlightPatch
#define INCLUDED_coopy_HighlightPatch

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <coopy/Row.h>
HX_DECLARE_CLASS0(IMap)
HX_DECLARE_CLASS1(coopy,CellInfo)
HX_DECLARE_CLASS1(coopy,Csv)
HX_DECLARE_CLASS1(coopy,HighlightPatch)
HX_DECLARE_CLASS1(coopy,HighlightPatchUnit)
HX_DECLARE_CLASS1(coopy,IndexPair)
HX_DECLARE_CLASS1(coopy,Row)
HX_DECLARE_CLASS1(coopy,Table)
HX_DECLARE_CLASS1(coopy,View)
HX_DECLARE_CLASS2(haxe,ds,IntMap)
HX_DECLARE_CLASS2(haxe,ds,StringMap)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  HighlightPatch_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef HighlightPatch_obj OBJ_;
		HighlightPatch_obj();
		Void __construct(::coopy::Table source,::coopy::Table patch);

	public:
		static hx::ObjectPtr< HighlightPatch_obj > __new(::coopy::Table source,::coopy::Table patch);
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~HighlightPatch_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		// hxcpp output massaged for SWIG //SWIGFIX
		inline ::hx::ObjectPtr< ::coopy::Row_obj> asRow() //SWIGFIX
			{ return ::hx::ObjectPtr< ::coopy::Row_obj>(this); } //SWIGFIX
#ifndef SWIG //SWIGFIX
		inline operator ::coopy::Row_obj *()
			{ return new ::coopy::Row_delegate_< HighlightPatch_obj >(this); }
#endif //SWIGFIX
		hx::Object *__ToInterface(const hx::type_info &inType);
		::String __ToString() const { return HX_CSTRING("HighlightPatch"); }

		::coopy::Table source;
		::coopy::Table patch;
		::coopy::View view;
		::coopy::Csv csv;
		::haxe::ds::IntMap header;
		::haxe::ds::StringMap headerPre;
		::haxe::ds::StringMap headerPost;
		::haxe::ds::StringMap headerRename;
		::haxe::ds::StringMap headerMove;
		::haxe::ds::IntMap modifier;
		int currentRow;
		int payloadCol;
		int payloadTop;
		Array< ::Dynamic > mods;
		Array< ::Dynamic > cmods;
		::coopy::CellInfo rowInfo;
		::coopy::CellInfo cellInfo;
		int rcOffset;
		Array< ::Dynamic > indexes;
		::haxe::ds::IntMap sourceInPatchCol;
		::haxe::ds::IntMap patchInSourceCol;
		::haxe::ds::IntMap patchInSourceRow;
		int lastSourceRow;
		Array< ::String > actions;
		Array< int > rowPermutation;
		Array< int > rowPermutationRev;
		Array< int > colPermutation;
		Array< int > colPermutationRev;
		virtual Void reset( );
		Dynamic reset_dyn();

		virtual bool apply( );
		Dynamic apply_dyn();

		virtual Void needSourceColumns( );
		Dynamic needSourceColumns_dyn();

		virtual Void needSourceIndex( );
		Dynamic needSourceIndex_dyn();

		virtual Void applyRow( int r);
		Dynamic applyRow_dyn();

		virtual Dynamic getDatum( int c);
		Dynamic getDatum_dyn();

		virtual ::String getString( int c);
		Dynamic getString_dyn();

		virtual Void applyMeta( );
		Dynamic applyMeta_dyn();

		virtual Void applyHeader( );
		Dynamic applyHeader_dyn();

		virtual int lookUp( hx::Null< int >  del);
		Dynamic lookUp_dyn();

		virtual Void applyAction( ::String code);
		Dynamic applyAction_dyn();

		virtual Void checkAct( );
		Dynamic checkAct_dyn();

		virtual ::String getPreString( ::String txt);
		Dynamic getPreString_dyn();

		virtual ::String getRowString( int c);
		Dynamic getRowString_dyn();

		virtual int sortMods( ::coopy::HighlightPatchUnit a,::coopy::HighlightPatchUnit b);
		Dynamic sortMods_dyn();

		virtual int processMods( Array< ::Dynamic > rmods,Array< int > fate,int len);
		Dynamic processMods_dyn();

		virtual Void computeOrdering( Array< ::Dynamic > mods,Array< int > permutation,Array< int > permutationRev,int dim);
		Dynamic computeOrdering_dyn();

		virtual Void permuteRows( );
		Dynamic permuteRows_dyn();

		virtual Void finishRows( );
		Dynamic finishRows_dyn();

		virtual Void permuteColumns( );
		Dynamic permuteColumns_dyn();

		virtual Void finishColumns( );
		Dynamic finishColumns_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_HighlightPatch */ 
