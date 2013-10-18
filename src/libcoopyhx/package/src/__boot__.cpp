#include <hxcpp.h>

#include <haxe/ds/StringMap.h>
#include <haxe/ds/IntMap.h>
#include <haxe/Log.h>
#include <coopy/Workspace.h>
#include <coopy/Viterbi.h>
#include <coopy/ViewedDatum.h>
#include <coopy/Unit.h>
#include <coopy/TableView.h>
#include <coopy/TableText.h>
#include <coopy/TableModifier.h>
#include <coopy/TableIO.h>
#include <coopy/TableDiff.h>
#include <coopy/TableComparisonState.h>
#include <coopy/SparseSheet.h>
#include <coopy/SimpleView.h>
#include <coopy/View.h>
#include <coopy/SimpleTable.h>
#include <coopy/Table.h>
#include <coopy/SimpleCell.h>
#include <coopy/Report.h>
#include <coopy/Ordering.h>
#include <coopy/Mover.h>
#include <coopy/IndexPair.h>
#include <coopy/IndexItem.h>
#include <coopy/Index.h>
#include <coopy/HighlightPatchUnit.h>
#include <coopy/HighlightPatch.h>
#include <coopy/Row.h>
#include <coopy/DiffRender.h>
#include <coopy/Csv.h>
#include <coopy/CrossMatch.h>
#include <coopy/Coopy.h>
#include <coopy/CompareTable.h>
#include <coopy/CompareFlags.h>
#include <coopy/Compare.h>
#include <coopy/ChangeType.h>
#include <coopy/Change.h>
#include <coopy/CellInfo.h>
#include <coopy/Bag.h>
#include <coopy/Alignment.h>
#include <StringBuf.h>
#include <Std.h>
#include <IMap.h>
#include <List.h>
#include <Lambda.h>

void __boot_all()
{
hx::RegisterResources( hx::GetResources() );
::haxe::ds::StringMap_obj::__register();
::haxe::ds::IntMap_obj::__register();
::haxe::Log_obj::__register();
::coopy::Workspace_obj::__register();
::coopy::Viterbi_obj::__register();
::coopy::ViewedDatum_obj::__register();
::coopy::Unit_obj::__register();
::coopy::TableView_obj::__register();
::coopy::TableText_obj::__register();
::coopy::TableModifier_obj::__register();
::coopy::TableIO_obj::__register();
::coopy::TableDiff_obj::__register();
::coopy::TableComparisonState_obj::__register();
::coopy::SparseSheet_obj::__register();
::coopy::SimpleView_obj::__register();
::coopy::View_obj::__register();
::coopy::SimpleTable_obj::__register();
::coopy::Table_obj::__register();
::coopy::SimpleCell_obj::__register();
::coopy::Report_obj::__register();
::coopy::Ordering_obj::__register();
::coopy::Mover_obj::__register();
::coopy::IndexPair_obj::__register();
::coopy::IndexItem_obj::__register();
::coopy::Index_obj::__register();
::coopy::HighlightPatchUnit_obj::__register();
::coopy::HighlightPatch_obj::__register();
::coopy::Row_obj::__register();
::coopy::DiffRender_obj::__register();
::coopy::Csv_obj::__register();
::coopy::CrossMatch_obj::__register();
::coopy::Coopy_obj::__register();
::coopy::CompareTable_obj::__register();
::coopy::CompareFlags_obj::__register();
::coopy::Compare_obj::__register();
::coopy::ChangeType_obj::__register();
::coopy::Change_obj::__register();
::coopy::CellInfo_obj::__register();
::coopy::Bag_obj::__register();
::coopy::Alignment_obj::__register();
::StringBuf_obj::__register();
::Std_obj::__register();
::IMap_obj::__register();
::List_obj::__register();
::Lambda_obj::__register();
::haxe::Log_obj::__boot();
::Lambda_obj::__boot();
::List_obj::__boot();
::IMap_obj::__boot();
::Std_obj::__boot();
::StringBuf_obj::__boot();
::coopy::Alignment_obj::__boot();
::coopy::Bag_obj::__boot();
::coopy::CellInfo_obj::__boot();
::coopy::Change_obj::__boot();
::coopy::ChangeType_obj::__boot();
::coopy::Compare_obj::__boot();
::coopy::CompareFlags_obj::__boot();
::coopy::CompareTable_obj::__boot();
::coopy::Coopy_obj::__boot();
::coopy::CrossMatch_obj::__boot();
::coopy::Csv_obj::__boot();
::coopy::DiffRender_obj::__boot();
::coopy::Row_obj::__boot();
::coopy::HighlightPatch_obj::__boot();
::coopy::HighlightPatchUnit_obj::__boot();
::coopy::Index_obj::__boot();
::coopy::IndexItem_obj::__boot();
::coopy::IndexPair_obj::__boot();
::coopy::Mover_obj::__boot();
::coopy::Ordering_obj::__boot();
::coopy::Report_obj::__boot();
::coopy::SimpleCell_obj::__boot();
::coopy::Table_obj::__boot();
::coopy::SimpleTable_obj::__boot();
::coopy::View_obj::__boot();
::coopy::SimpleView_obj::__boot();
::coopy::SparseSheet_obj::__boot();
::coopy::TableComparisonState_obj::__boot();
::coopy::TableDiff_obj::__boot();
::coopy::TableIO_obj::__boot();
::coopy::TableModifier_obj::__boot();
::coopy::TableText_obj::__boot();
::coopy::TableView_obj::__boot();
::coopy::Unit_obj::__boot();
::coopy::ViewedDatum_obj::__boot();
::coopy::Viterbi_obj::__boot();
::coopy::Workspace_obj::__boot();
::haxe::ds::IntMap_obj::__boot();
::haxe::ds::StringMap_obj::__boot();
}

