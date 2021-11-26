set(hdrs Robot_operation_support.h
    PARENT_SCOPE
    )

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

