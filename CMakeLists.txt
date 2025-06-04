cmake_minimum_required(VERSION 3.31)
project(Blackjack)

set(CMAKE_CXX_STANDARD 20)

add_executable(Blackjack main.cpp Card.cpp Deck.cpp Hand.cpp Game.cpp)
