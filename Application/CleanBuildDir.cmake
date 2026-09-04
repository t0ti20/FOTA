# Removes every "Test_*" build artifact in DIR except the one just built (KEEP).
# Invoked via `cmake -DDIR=... -DKEEP=... -P CleanBuildDir.cmake` as a POST_BUILD step.
file(GLOB Old_Artifacts "${DIR}/Test_*")
foreach(Artifact ${Old_Artifacts})
    get_filename_component(Artifact_Name "${Artifact}" NAME)
    if(NOT Artifact_Name MATCHES "^${KEEP}")
        file(REMOVE "${Artifact}")
    endif()
endforeach()
