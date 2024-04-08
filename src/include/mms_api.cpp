#include "mms_api.hpp"

#include <iostream>

using std::cout, std::cin;
int API::mazeWidth() {
    cout << "mazeWidth" << '\n';
    string response;
    cin >> response;
    return std::stoi(response);
}

int API::mazeHeight() {
    cout << "mazeHeight" << '\n';
    string response;
    cin >> response;
    return std::stoi(response);
}

bool API::wallFront() {
    cout << "wallFront" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

bool API::wallRight() {
    cout << "wallRight" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

bool API::wallLeft() {
    cout << "wallLeft" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

void API::moveForward(int distance) {
    cout << "moveForward ";
    // Don't print distance argument unless explicitly specified, for
    // backwards compatibility with older versions of the simulator
    if (distance != 1) {
        cout << distance;
    }
    cout << '\n';
    string response;
    cin >> response;
    if (response != "ack") {
        std::cerr << response << '\n';
        throw;
    }
}

void API::turnRight() {
    cout << "turnRight" << '\n';
    string ack;
    cin >> ack;
}

void API::turnLeft() {
    cout << "turnLeft" << '\n';
    string ack;
    cin >> ack;
}

void API::setWall(int x, int y, char direction) { cout << "setWall " << x << " " << y << " " << direction << '\n'; }

void API::clearWall(int x, int y, char direction) { cout << "clearWall " << x << " " << y << " " << direction << '\n'; }

void API::setColor(int x, int y, char color) { cout << "setColor " << x << " " << y << " " << color << '\n'; }

void API::clearColor(int x, int y) { cout << "clearColor " << x << " " << y << '\n'; }

void API::clearAllColor() { cout << "clearAllColor" << '\n'; }

void API::setText(int x, int y, const string& text) { cout << "setText " << x << " " << y << " " << text << '\n'; }

void API::clearText(int x, int y) { cout << "clearText " << x << " " << y << '\n'; }

void API::clearAllText() { cout << "clearAllText" << '\n'; }

bool API::wasReset() {
    cout << "wasReset" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

void API::ackReset() {
    cout << "ackReset" << '\n';
    string ack;
    cin >> ack;
}
