#pragma once
#define PDC_WIDE
#define NCURSES_MOUSE_VERSION

#include <curses.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <windows.h>
#include <memory>

#include <mosquitto.h>
#include <nlohmann/json.hpp>

std::string toUtf8(const std::wstring& utf16);