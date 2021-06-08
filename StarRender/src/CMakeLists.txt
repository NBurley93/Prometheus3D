file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.h")
target_sources(StarRender PRIVATE
    ${sources}
)