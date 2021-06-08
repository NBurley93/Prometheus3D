file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp" "*.h")
target_sources(Prometheus3D PRIVATE
    ${sources}
)