# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.20)

project(
	BWHorror
	LANGUAGES    C
	VERSION      1.0.0
	DESCRIPTION  "Bandwidth horror game"
	HOMEPAGE_URL "https://www.youtube.com/channel/UCzp1RaZ3HmejKl723qlKOAw"
)

psn00bsdk_add_executable(horror STATIC main.c)

psn00bsdk_add_cd_image(
	iso      # Target name
	horror # Output file name (= template.bin + template.cue)
	iso.xml  # Path to config file
	DEPENDS horror
)
