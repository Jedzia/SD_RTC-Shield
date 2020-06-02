# This itself is Version 1.0.0

macro(set_target_version_properties _target _version)
    set_target_properties(${_target}
            PROPERTIES
            VERSION ${_version}
            SOVERSION ${_version}
            )
endmacro(set_target_version_properties)