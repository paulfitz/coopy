#ifndef INCLUDED_coopy_Viterbi
#define INCLUDED_coopy_Viterbi

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,SparseSheet)
HX_DECLARE_CLASS1(coopy,Viterbi)
namespace coopy{


class HXCPP_CLASS_ATTRIBUTES  Viterbi_obj : public hx::Object{
	public:
		typedef hx::Object super;
		typedef Viterbi_obj OBJ_;
		Viterbi_obj();
		Void __construct();

	public:
		static hx::ObjectPtr< Viterbi_obj > __new();
		static Dynamic __CreateEmpty();
		static Dynamic __Create(hx::DynamicArray inArgs);
		~Viterbi_obj();

		HX_DO_RTTI;
		static void __boot();
		static void __register();
		void __Mark(HX_MARK_PARAMS);
		void __Visit(HX_VISIT_PARAMS);
		::String __ToString() const { return HX_CSTRING("Viterbi"); }

		int K;
		int T;
		int index;
		int mode;
		bool path_valid;
		Float best_cost;
		::coopy::SparseSheet cost;
		::coopy::SparseSheet src;
		::coopy::SparseSheet path;
		virtual Void reset( );
		Dynamic reset_dyn();

		virtual Void setSize( int states,int sequence_length);
		Dynamic setSize_dyn();

		virtual Void assertMode( int next);
		Dynamic assertMode_dyn();

		virtual Void addTransition( int s0,int s1,Float c);
		Dynamic addTransition_dyn();

		virtual Void endTransitions( );
		Dynamic endTransitions_dyn();

		virtual Void beginTransitions( );
		Dynamic beginTransitions_dyn();

		virtual Void calculatePath( );
		Dynamic calculatePath_dyn();

		virtual ::String toString( );
		Dynamic toString_dyn();

		virtual int length( );
		Dynamic length_dyn();

		virtual int get( int i);
		Dynamic get_dyn();

		virtual Float getCost( );
		Dynamic getCost_dyn();

};

} // end namespace coopy

#endif /* INCLUDED_coopy_Viterbi */ 
