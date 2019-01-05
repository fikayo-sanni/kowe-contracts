cmake_minimum_required(VERSION 3.5)
project(test_example VERSION 1.0.0)

find_package(eosio.cdt)

add_contract( cxtapp cxtapp cxtapp.cpp )
