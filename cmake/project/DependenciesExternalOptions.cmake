# Copyright 2019-present, Joseph Garnier
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.
# =============================================================================
# What Is This?
# -------------
# See README file in the root directory of this source tree.


#------------------------------------------------------------------------------
# Import and link external libraries from here.
#------------------------------------------------------------------------------

############
#  OpenCV  #
############
message("\n** Include OpenCV **")
if(DEFINED ENV{OpenCV_DIR}) 
	set(OpenCV_DIR "$ENV{OpenCV_DIR}")
else()
	set(OpenCV_DIR "D:/library/opencv-4.5.4/install")
endif()
find_package(OpenCV REQUIRED)
target_include_directories("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
 	PUBLIC
 		"$<BUILD_INTERFACE:${OpenCV_INCLUDE_DIRS}>"
 		"$<INSTALL_INTERFACE:${OpenCV_INCLUDE_DIRS}>"
)
target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
 	PUBLIC
 		"$<BUILD_INTERFACE:${OpenCV_LIBS}>"
 		"$<INSTALL_INTERFACE:${OpenCV_LIBS}>"
)

