
set(forth_common_srcs
    common/Buffer3.h
    common/Buffer4.cpp
    common/Buffer4.h
    common/BufferGL.h
    common/Color.h
    common/Enums.h
    common/VertexProfile.h
    )

set(forth_extras_srcs
    extras/MeshGen.h
    extras/MeshGen.cpp
    extras/Utils.h
    )

set(forth_math_srcs
    math/Bounds4.h
    math/Euler4.h
    math/Math.h
    math/Matrix4.cpp
    math/Matrix4.h
    math/Plane4.h
    math/Ray4.h
    math/SphereBounds4.h
    math/Tensor4.cpp
    math/Tensor4.h
    math/Transform4.h
    math/Vector3.h
    math/Vector4.cpp
    math/Vector4.h
    )

set(forth_rendering_srcs
    rendering/CrossSection.cpp
    rendering/CrossSection.h
    rendering/Model4.h
    rendering/Projector4.cpp
    rendering/Projector4.h
    rendering/Scene4.h
    rendering/Visualizer4.h
)

set(forth_visualizer_srcs
    visualizer/CustomVisualizer.h
    visualizer/ParticleVisualizer.h
    visualizer/SolidVisualizer.cpp
    visualizer/SolidVisualizer.h
    visualizer/WireVisualizer.h
)

set(forth_physics_broadphase_srcs
    physics/broadphase/BroadPhase.cpp
    physics/broadphase/BroadPhase.h
    physics/broadphase/DynamicTree.cpp
    physics/broadphase/DynamicTree.h
)

set(forth_physics_collision_srcs
    physics/collision/Algorithm.cpp
    physics/collision/Algorithm.h
    physics/collision/Box.cpp
    physics/collision/Box.h
    physics/collision/Capsule.cpp
    physics/collision/Capsule.h
    physics/collision/Collide.cpp
    physics/collision/Collide.h
    physics/collision/Convex.h
    physics/collision/Shape.cpp
    physics/collision/Shape.h
    physics/collision/Sphere.cpp
    physics/collision/Sphere.h
)

set(forth_physics_dynamics_srcs
    physics/dynamics/Body.cpp
    physics/dynamics/Body.h
    physics/dynamics/Contact.cpp
    physics/dynamics/Contact.h
    physics/dynamics/ContactManager.cpp
    physics/dynamics/ContactManager.h
    physics/dynamics/ContactSolver.cpp
    physics/dynamics/ContactSolver.h
    physics/dynamics/Island.cpp
    physics/dynamics/Island.h
    physics/dynamics/RaycastHit4.h
    physics/dynamics/Scene.cpp
    physics/dynamics/Scene.h
)

set(forth_physics_srcs
    physics/Common.cpp
    physics/Common.h
    ${forth_physics_broadphase_srcs}
    ${forth_physics_collision_srcs}
    ${forth_physics_dynamics_srcs}
)

add_library(forth_static
    ../include/forth.h
    forth.h
    ${forth_common_srcs}
    ${forth_extras_srcs}
    ${forth_math_srcs}
    ${forth_rendering_srcs}
    ${forth_visualizer_srcs}
    ${forth_physics_srcs}
    )

set_target_properties(forth_static PROPERTIES LINKER_LANGUAGE CXX)

# install(TARGETS source
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib)