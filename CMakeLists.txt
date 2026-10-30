cmake_minimum_required(VERSION 3.10)
project(QuizMaster1 CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(QuizMaster1)

set(SOURCES
    QuizMaster1/App.cpp
    QuizMaster1/Player.cpp
    QuizMaster1/Question.cpp
    QuizMaster1/QuizMaster1.cpp
    QuizMaster1/Quiz.cpp
    QuizMaster1/SingleChoiceQuestion.cpp
    QuizMaster1/TrueOrFalseQuestion.cpp
    QuizMaster1/User.cpp
)

add_executable(QuizMaster1 ${SOURCES})

# Copy data files to build directory so the executable can access them
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/QuizMaster1/quizzes.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/QuizMaster1/users.dat)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/QuizMaster1/users.dat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
