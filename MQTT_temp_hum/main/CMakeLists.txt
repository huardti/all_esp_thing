set(srcs
    main.cpp
    dht.cpp
    PubSubClient.cpp
)

set(includedirs
    .
)



idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${includedirs})