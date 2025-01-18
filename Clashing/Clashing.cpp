#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <windows.h>
#include <tlhelp32.h>

bool isRobloxRunning() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }
    do {
        if (wcsstr(pe32.szExeFile, L"Roblox")) {
            CloseHandle(hProcessSnap);
            return true;
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return false;
}

void simulateKeyPress(WORD key, int delay) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void simulateMouseClick(int delay) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void pressKeys(int clicksPerSecond) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 20);

    int delayBetweenClicks = 1000 / clicksPerSecond;

    while (GetAsyncKeyState('1') & 0x8000) {
        int randomDelay = dis(gen);
        simulateKeyPress('F', randomDelay);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayBetweenClicks - randomDelay));

        randomDelay = dis(gen);
        simulateMouseClick(randomDelay);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayBetweenClicks - randomDelay));
    }
}

int main() {
    std::cout << "Waiting for Roblox to start..." << std::endl;

    while (!isRobloxRunning()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Roblox is running." << std::endl;

    int clicksPerSecond;
    std::cout << "Enter the number of clicks per second: ";
    std::cin >> clicksPerSecond;

    std::cout << "Hold '1' to start pressing 'F' and 'M1' with randomized delays." << std::endl;

    while (true) {
        if (GetAsyncKeyState('1') & 0x8000) {
            pressKeys(clicksPerSecond);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}