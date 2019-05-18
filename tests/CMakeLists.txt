
set(demo_srcs
	demo.cpp
)

include_directories (
	${OPENGL_INCLUDE_DIR}
	${forth_SOURCE_DIR}/source
	.
)

add_executable(
	demo
	${demo_srcs}
)

target_link_libraries(
	demo
	freeglut_static
	${ADDITIONAL_LIBRARIES}
	${OPENGL_LIBRARIES}
)
