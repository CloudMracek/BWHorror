# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.20)

project(
	BWHorror
	LANGUAGES    C ASM
	VERSION      1.0.0
	DESCRIPTION  "Bandwidth horror game"
	HOMEPAGE_URL "https://www.youtube.com/channel/UCzp1RaZ3HmejKl723qlKOAw"
)

file(GLOB TEXTURES textures/*.tim)


psn00bsdk_add_executable(horror STATIC 
	src/main.c 
	src/display.c
	src/mesh.c
	src/texture.c
	src/input.c
	src/collision.c
	src/sound.c
)

psn00bsdk_target_incbin(horror PRIVATE bandwidth_face textures/bandwidth_face.tim)
psn00bsdk_target_incbin(horror PRIVATE duch textures/duch.tim)

psn00bsdk_target_incbin(horror PRIVATE steps sounds/steps.vag)
psn00bsdk_target_incbin(horror PRIVATE yay sounds/yay.vag)
psn00bsdk_target_incbin(horror PRIVATE victory sounds/victory.vag)

foreach(TEXTURE ${TEXTURES})
	get_filename_component(BARENAME ${TEXTURE} NAME)
	string(REGEX REPLACE "\\.[^.]*$" "" TEXTURE_NAME ${BARENAME})
	psn00bsdk_target_incbin(horror PRIVATE ${TEXTURE_NAME} ${TEXTURE})
endforeach()


psn00bsdk_add_cd_image(
	iso      # Target name
	horror # Output file name (= template.bin + template.cue)
	iso.xml  # Path to config file
	DEPENDS horror
)
