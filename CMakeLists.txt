# $Id: 6854c7ea46f1fb8281e85b0a014b719bcf64edd0 $

lfl_set_os_toolkit(SHINE)
lfl_project(IdleShine com.lucidfusionlabs.IdleShine)
lfl_add_screensaver(IdleShine SOURCES shine.cpp
                    LINK_LIBRARIES ${LFL_APP_LIB} ${SHINE_SCREENSAVER} ${SHINE_GRAPHICS}
                    app_null_audio app_null_camera app_null_matrix app_null_fft
                    app_simple_resampler app_simple_loader ${LFL_APP_CONVERT} app_libpng_png
                    app_null_jpeg app_null_gif app_null_ogg app_null_css ${LFL_APP_FONT}
                    ${LFL_APP_SSL} app_null_js app_null_toolkit ${LFL_APP_OS} app_null_crashreporting
                    LIB_FILES ${LFL_APP_LIB_FILES}
                    ASSET_DIRS ${LFL_APP_ASSET_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/drawable-xhdpi
                    ASSET_FILES ${LFL_APP_ASSET_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/assets/*
                    ${LFL_SOURCE_DIR}/core/app/shaders/shadertoy/*.frag)
lfl_post_build_start(IdleShine)
