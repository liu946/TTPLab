set(CommonSRC
        model/AffineTransformModelBuilder.cpp
        model/CNN1dLayerBuilder.cpp
        process/LabelModelSGDSeqTrainer.h)


add_library(common_static_lib STATIC ${CommonSRC})
target_link_libraries(common_static_lib ${LIBS})
add_library(common_shared_lib SHARED ${CommonSRC})
target_link_libraries(common_shared_lib ${LIBS})