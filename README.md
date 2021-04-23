# Arlo-account-checker
Checks if account credentials are registered on arlo.com. The main purpose of a tool like this is to check if breached users have reused the same credentials on other services; in this case [arlo.com](https://www.arlo.com).

## Usage
1) Install the [CPR](https://github.com/whoshuu/cpr) and [JSON](https://github.com/nlohmann/json) library — [vcpkg](https://github.com/Microsoft/vcpkg) is a great package manager.
2) Create a file called `combos.txt` and paste account credentials in there with the following format:
```
user@mail.com:passw0rd
john@gmail.com:J0hn123
hailey123@hotmail.com:Haily321!
```
4) Compile, and run!

## Contribute
This is my first project in C++ so the code can definitely be improved. If you have any suggestions in mind, feel free to submit a pull request or [contact](https://t.me/tvenne) me. Feedback is always welcome.

## Legal
This was developed for educational purposes only, and should only be tested with own accounts.
