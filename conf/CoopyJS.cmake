include(FindPkgConfig)

pkg_search_module(MOZJS REQUIRED mozilla-js xulrunner-js firefox-js seamonkey-js)
  
include_directories(${MOZJS_INCLUDE_DIRS})
link_directories(${MOZJS_LIBRARY_DIRS})

macro(js_to_cpp src dest name)
  add_custom_command(
    OUTPUT ${dest}
    COMMAND codify ${src} ${dest} ${name}
    DEPENDS codify ${src}
    )
endmacro()

