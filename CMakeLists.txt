cmake_minimum_required(VERSION 3.10)

project(KingsOfEngland)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)

#cmake_minimum_required(VERSION 3.26)
#project(untitled4)
#
#set(CMAKE_CXX_STANDARD 17)

#add_executable(untitled4


#        main.cpp
#        src/royal_family_tree/heir/Heir.h
#        src/royal_family_tree/heir/Heir.cpp
#        src/royal_family_tree/tree/Hierarchy.h
#        src/royal_family_tree/tree/Hierarchy.cpp
#        src/runner/Runner.h
#        src/runner/Runner.cpp
#        src/royal_family_tree/exceptions/AlreadyExistingMemberException.cpp
#        src/royal_family_tree/exceptions/AlreadyExistingMemberException.h
#        src/utils/Religion.h
#        src/royal_family_tree/exceptions/EmptyHierarchyException.cpp
#        src/royal_family_tree/exceptions/EmptyHierarchyException.h
#        src/royal_family_tree/exceptions/MemberNotFoundException.cpp
#        src/royal_family_tree/exceptions/MemberNotFoundException.h
#)


