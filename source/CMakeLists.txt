
set(forth_common_srcs
    common/Buffer3.h
    common/Buffer4.cpp
    common/Buffer4.h
    common/BufferGL.h
    common/Color.h
    common/Enums.h
    common/VertexProfile.h
    )

set(forth_math_srcs
    math/Bounds4.h
    math/Euler4.h
    math/Math.h
    math/Matrix4.cpp
    math/Matrix4.h
    math/Plane4.h
    math/SphereBounds4.h
    math/Transform4.h
    math/Vector3.h
    math/Vector4.h
    )

set(forth_rendering_srcs
    rendering/CrossSection.cpp
    rendering/CrossSection.h
    rendering/Projector4.h
    rendering/Visualizer4.h
)

set(forth_visualizer_srcs
    visualizer/CustomVisualizer.h
    visualizer/ParticleVisualizer.h
    visualizer/SolidVisualizer.cpp
    visualizer/SolidVisualizer.h
    visualizer/WireVisualizer.h
)

add_library(forth_static
    ../include/forth.h
    forth.h
    ${forth_common_srcs}
    ${forth_math_srcs}
    ${forth_rendering_srcs}
    ${forth_visualizer_srcs}
    )

set_target_properties(forth_static PROPERTIES LINKER_LANGUAGE CXX)

# install(TARGETS source
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib)