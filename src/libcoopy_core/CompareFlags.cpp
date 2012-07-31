#include <coopy/CompareFlags.h>

using namespace coopy::cmp;

void CompareFlags::copy(const CompareFlags& alt) {
  head_trimmed = alt.head_trimmed;
  tail_trimmed = alt.tail_trimmed;
  out = alt.out;
  tables = alt.tables;
  acts = alt.acts;
  ordered_tables = alt.ordered_tables;
  ids = alt.ids;
  coined = alt.coined;
  ids_local = alt.ids_local;
  ids_remote = alt.ids_remote;
  trust_ids = alt.trust_ids;
  bias_ids = alt.bias_ids;
  trust_column_names = alt.trust_column_names;
  mapping = alt.mapping;
  mapping_book = alt.mapping_book;
  meta_book = alt.meta_book;
  pivot_sides_with_local = alt.pivot_sides_with_local;
  has_pivot = alt.has_pivot;
  use_order = alt.use_order;
  omit_format_name = alt.omit_format_name;
  omit_sheet_name = alt.omit_sheet_name;
  fixed_columns = alt.fixed_columns;
  assume_header = alt.assume_header;
  ignore_case = alt.ignore_case;
  create_unknown_sheets = alt.create_unknown_sheets;
  clean_sheets = alt.clean_sheets;
  pivot_uri = alt.pivot_uri;
  local_uri = alt.local_uri;
  remote_uri = alt.remote_uri;
  resolving = alt.resolving;
  resolve = alt.resolve;
  variant = alt.variant;
  pool = alt.pool;
  foreign_pool = alt.foreign_pool;
  foreign_pool_set = alt.foreign_pool_set;
  offload_to_sql_when_possible = alt.offload_to_sql_when_possible;
}
