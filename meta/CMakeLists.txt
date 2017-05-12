SET({% Solution %}{% Project %}SRC
        config/{% Solution %}{% Project %}Config.h
        model/{% Solution %}{% Project %}Model.h
        )

add_executable({% Project %}train train.cpp process/Train{% Solution %}{% Project %}.cpp ${{% Solution %}{% Project %}SRC})
target_link_libraries({% Project %}train dynet ${LIBS})

add_executable({% Project %}pred pred.cpp process/Pred{% Solution %}{% Project %}.cpp ${{% Solution %}{% Project %}SRC})
target_link_libraries({% Project %}pred dynet ${LIBS})
