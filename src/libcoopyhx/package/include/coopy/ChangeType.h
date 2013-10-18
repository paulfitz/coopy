#ifndef INCLUDED_coopy_ChangeType
#define INCLUDED_coopy_ChangeType

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

HX_DECLARE_CLASS1(coopy,ChangeType)
namespace coopy{


class ChangeType_obj : public hx::EnumBase_obj
{
	typedef hx::EnumBase_obj super;
		typedef ChangeType_obj OBJ_;

	public:
		ChangeType_obj() {};
		HX_DO_ENUM_RTTI;
		static void __boot();
		static void __register();
		::String GetEnumName( ) const { return HX_CSTRING("coopy.ChangeType"); }
		::String __ToString() const { return HX_CSTRING("ChangeType.") + tag; }

		static ::coopy::ChangeType BOTH_CHANGE;
		static inline ::coopy::ChangeType BOTH_CHANGE_dyn() { return BOTH_CHANGE; }
		static ::coopy::ChangeType LOCAL_CHANGE;
		static inline ::coopy::ChangeType LOCAL_CHANGE_dyn() { return LOCAL_CHANGE; }
		static ::coopy::ChangeType NOTE_CHANGE;
		static inline ::coopy::ChangeType NOTE_CHANGE_dyn() { return NOTE_CHANGE; }
		static ::coopy::ChangeType NO_CHANGE;
		static inline ::coopy::ChangeType NO_CHANGE_dyn() { return NO_CHANGE; }
		static ::coopy::ChangeType REMOTE_CHANGE;
		static inline ::coopy::ChangeType REMOTE_CHANGE_dyn() { return REMOTE_CHANGE; }
		static ::coopy::ChangeType SAME_CHANGE;
		static inline ::coopy::ChangeType SAME_CHANGE_dyn() { return SAME_CHANGE; }
};

} // end namespace coopy

#endif /* INCLUDED_coopy_ChangeType */ 
