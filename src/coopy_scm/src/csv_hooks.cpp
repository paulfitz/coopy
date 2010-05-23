#include <stdio.h>

extern "C" {
#include "config.h"
#include "blob.h"
}

extern "C" { 
#include "csv.h"
}

#include <coopy/CsvSheet.h>
#include <coopy/CsvCompare.h>
#include <coopy/CsvRender.h>

#include "ssfossil.h"

extern "C" int csv_merge(Blob *pPivot, Blob *pV1, Blob *pV2, Blob *pOut);
extern "C" int csv_render(Blob *in, Blob *out);

extern "C" void cvs_merge_cb1 (void *s, size_t i, void *p) {
  ((CsvSheet*)p)->addField((char *)s, i);
}

extern "C" void cvs_merge_cb2 (int c, void *p) {
  ((CsvSheet*)p)->addRecord();
}

int blob_to_csv(Blob *pIn, CsvSheet& csv) {
  SheetStyle style;
  style.setFromInspection(blob_buffer(pIn),blob_size(pIn));
  csv.clear();
  csv.setStyle(style);
  if (pIn==NULL) return -1;
  struct csv_parser p;
  csv_init(&p,0); // CSV_APPEND_NULL does not seem reliable
  csv_set_delim(&p,style.getDelimiter()[0]);
  int result = csv_parse(&p,
			 blob_buffer(pIn),
			 blob_size(pIn),
			 cvs_merge_cb1,
			 cvs_merge_cb2,
			 (void*)(&csv));
  csv_fini(&p,cvs_merge_cb1,
	   cvs_merge_cb2,
	   (void*)(&csv));
  csv_free(&p);
  if (result!=blob_size(pIn)) {
    return -1;
  }
  if (!csv.isValid()) {
    return -1;
  }
  return 0;
}


void blob_show_csv(const CsvSheet& csv, const SheetStyle& style, Blob *pOut) {
  //blob_appendf(pOut,"CSV: %dx%d\n",csv.width(),csv.height());
  //for (int y=0;y<csv.height();y++) {
  //for (int x=0;x<csv.width();x++) {
  //blob_appendf(pOut,"%s ",csv.cell(x,y).c_str());
  //}
  //blob_appendf(pOut,"\n");
  //}
  blob_appendf(pOut,"%s",csv.encode(style.getDelimiter()).c_str());
}


/*
  return 1 if a result has been set, otherwise 0; -1 aborts.
 */
int csv_merge(Blob *pPivot, Blob *pV1, Blob *pV2, Blob *pOut) {
  if (ssfossil_debug()) {
    printf("csv_merge\n");
    printf("BASE [%s]\n", blob_buffer(pPivot));
    printf("VERSION 1 [%s]\n", blob_buffer(pV1));
    printf("VERSION 2 [%s]\n", blob_buffer(pV2));
  }
  CsvSheet csv0, csv1, csv2;
  if (blob_to_csv(pPivot,csv0)==0 && 
      blob_to_csv(pV1,csv1)==0 && 
      blob_to_csv(pV2,csv2)==0) {
    CsvCompare merger;
    if (merger.compare(csv0,csv1,csv2)==0) {
      blob_zero(pOut);
      //blob_appendf(pOut,"Hello from %s:%d\n", __FILE__, __LINE__);
      //blob_appendf(pOut,"Conflict resolution is being modified.\n");
      blob_show_csv(merger.get(),csv1.getStyle(),pOut);
      return 1;
    }
  }
  return 0;
}



/*
  return 1 if a result has been set, otherwise 0; -1 aborts.
 */
int csv_render(Blob *in, Blob *out) {
  CsvSheet sheet;
  if (blob_to_csv(in,sheet)==0) {
    CsvRender render;
    render.setFull(false);
    render.setDecorate(true);
    std::string result = render.renderHtml(sheet);
    blob_appendf(out,"%s",result.c_str());
    return 1;
  }
  return 0;
}
