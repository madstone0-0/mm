#include "mms_api.hpp"

using std::cout, std::cin;
// int MmsAPI::mazeWidth() {
//     cout << "mazeWidth" << '\n';
//     string response;
//     cin >> response;
//     return std::stoi(response);
// }
//
// int MmsAPI::mazeHeight() {
//     cout << "mazeHeight" << '\n';
//     string response;
//     cin >> response;
//     return std::stoi(response);
// }
//

MmsAPI::MmsAPI() = default;

bool MmsAPI::wallFront() {
    cout << "wallFront" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

bool MmsAPI::wallRight() {
    cout << "wallRight" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

bool MmsAPI::wallLeft() {
    cout << "wallLeft" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

void MmsAPI::moveForward(int distance) {
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
        std::cout << response << '\n';
    }
}

void MmsAPI::turnRight() {
    cout << "turnRight" << '\n';
    string ack;
    cin >> ack;
}

void MmsAPI::turnLeft() {
    cout << "turnLeft" << '\n';
    string ack;
    cin >> ack;
}

void MmsAPI::setWall(int x, int y, char direction) { cout << "setWall " << x << " " << y << " " << direction << '\n'; }

void MmsAPI::clearWall(int x, int y, char direction) {
    cout << "clearWall " << x << " " << y << " " << direction << '\n';
}

void MmsAPI::setColor(int x, int y, char color) { cout << "setColor " << x << " " << y << " " << color << '\n'; }

void MmsAPI::clearColor(int x, int y) { cout << "clearColor " << x << " " << y << '\n'; }

void MmsAPI::clearAllColor() { cout << "clearAllColor" << '\n'; }

void MmsAPI::setText(int x, int y, const string& text) { cout << "setText " << x << " " << y << " " << text << '\n'; }

void MmsAPI::clearText(int x, int y) { cout << "clearText " << x << " " << y << '\n'; }

void MmsAPI::clearAllText() { cout << "clearAllText" << '\n'; }

bool MmsAPI::wasReset() {
    cout << "wasReset" << '\n';
    string response;
    cin >> response;
    return response == "true";
}

void MmsAPI::ackReset() {
    cout << "ackReset" << '\n';
    string ack;
    cin >> ack;
}
