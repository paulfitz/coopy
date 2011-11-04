
option(DOXYGEN_TRICKS "Process *.paradox files for documentation" FALSE)

if (DOXYGEN_TRICKS)
  file(GLOB paradox ${CMAKE_SOURCE_DIR}/doc/*.paradox)
  set(PARADOXES)
  foreach(f ${paradox})
    get_filename_component(pbase ${f} NAME_WE)
    message(STATUS "${f} ${pbase}")
    set(ODIR ${CMAKE_SOURCE_DIR}/doc)
    # set(ODIR ${CMAKE_BINARY_DIR}/dox)
    set(ONAME ${pbase}.dox)
    ADD_CUSTOM_COMMAND(OUTPUT ${ODIR}/${ONAME}
      COMMAND mkdir -p ${ODIR}
      COMMAND ${CMAKE_SOURCE_DIR}/scripts/process_dox.sh ${f} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} > ${ODIR}/${ONAME}
      MAIN_DEPENDENCY ${f}
      DEPENDS ${CMAKE_SOURCE_DIR}/scripts/process_dox.sh csv2html ssdiff
      ssresolve ssrediff ssformat sspatch ssmerge coopy
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      )
    set(PARADOXES ${PARADOXES} ${ODIR}/${ONAME})
  endforeach()
endif ()

find_program(DOXYGEN_EXE NAMES doxygen)
mark_as_advanced(DOXYGEN_EXE)
if (DOXYGEN_EXE)
  foreach (mode html latex man)
    string(TOUPPER ${mode} mode_upper)
    set(ENABLED_SECTIONS)
    set(INPUT "${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/doc")
    set(RECURSIVE YES)
    set(EXCLUDE)
    set(GENERATE_LATEX NO)
    set(GENERATE_HTML NO)
    set(GENERATE_MAN NO)
    set(FILE_PATTERNS "*.cpp *.h *.c *.dox")
    set(GENERATE_${mode_upper} YES)
    if (GENERATE_HTML)
      set(ENABLED_SECTIONS "link_code link_examples link_internal")
    endif ()
    if (GENERATE_LATEX)
      include(CoopyDocLatex)
      set(INPUT ${CMAKE_SOURCE_DIR}/doc/images)
      foreach (i ${LATEX_MANUAL_DOCS})
	set(INPUT "${INPUT} ${CMAKE_SOURCE_DIR}/doc/${i}")
      endforeach()
      set(RECURSIVE YES)
      set(FILE_PATTERNS "*.dox")
      set(ENABLED_SECTIONS "link_internal")
      set(EXCLUDE "${CMAKE_SOURCE_DIR}/doc/generated_examples/merge_example_big_merge_with_lots_of_changes.dox ${CMAKE_SOURCE_DIR}/doc/example.dox ${CMAKE_SOURCE_DIR}/doc/diff_example.dox ${CMAKE_SOURCE_DIR}/doc/merge_example.dox ${CMAKE_SOURCE_DIR}/doc/patch_format_csv_v_0_2.dox ${CMAKE_SOURCE_DIR}/doc/patch_format_csv_v_0_4.dox ${CMAKE_SOURCE_DIR}/doc/patch_format_csv.dox ${CMAKE_SOURCE_DIR}/doc/patch_format_human.dox")
    endif ()
    if (GENERATE_MAN)
      set(INPUT "${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/doc")
      set(RECURSIVE YES)
      set(FILE_PATTERNS "cmd_*.dox")
      set(ENABLED_SECTIONS "link_internal")
    endif ()
    set(DOXYGEN_TEMPLATE_DIR "" CACHE STRING "Directory holding doxygen styling, if desired")
    if (DOXYGEN_TEMPLATE_DIR)
      set(HTML_HEADER ${DOXYGEN_TEMPLATE_DIR}/header.html)
      set(HTML_FOOTER ${DOXYGEN_TEMPLATE_DIR}/footer.html)
      set(HTML_STYLESHEET ${DOXYGEN_TEMPLATE_DIR}/style.css)
    endif ()
    configure_file(${CMAKE_SOURCE_DIR}/conf/coopy_doxygen.conf.in
      ${CMAKE_BINARY_DIR}/coopy_doxygen_${mode}.conf IMMEDIATE)

    make_directory(${CMAKE_BINARY_DIR}/layout/${mode})

    add_custom_target(${mode} 
      COMMAND ${DOXYGEN_EXE} ${CMAKE_BINARY_DIR}/coopy_doxygen_${mode}.conf
      DEPENDS ${PARADOXES})

  endforeach ()
endif ()

if (DOXYGEN_TRICKS)
  add_custom_target(xpdf 
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/make_pdf.sh)
endif ()

configure_file(${CMAKE_SOURCE_DIR}/doc/tdiff/tdiff_spec_draft.html
  ${CMAKE_BINARY_DIR}/gendoc/html/tdiff_spec_draft.html IMMEDIATE)
