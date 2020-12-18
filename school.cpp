// #define _GLIBCXX_DEBUG

#include<bits/stdc++.h>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <map>

typedef long long ll;
typedef long double ld;

int const MAX = 1401;
int const MOD = 179179179;
int const pppower = 179;
int const INFint = 2e9 + 1000;

// #pragma GCC optimize("-ffast-math")

void solve();

signed main() {
    //freopen("a.in", "r", stdin);
    solve();
    return 0;
}

bool Error;

int toNum(std::string& str) {  // consists only of digits
    int ret = 0;
    int minus = str[0] == '-' ? -1 : 1;
    for (char i : str) {
        if (i == '-') continue;
        ret *= 10;
        ret += i - '0';
    }
    return minus * ret;
}

bool isDig(char ch) {
    return ch >= '0' && ch <= '9';
}

bool isNum(const std::string& str) {
    bool ok = true;
    bool minus = true;
    if (str.size() == 1) {
        return isDig(str[0]);
    }
    for (auto i : str) {
        if (i == '-' && minus) {
            minus = false;
            continue;
        }
        ok = ok && isDig(i);
    }
    return ok;
}

double post(std::deque<std::string>& str) {
    std::deque<double> last;
    for (std::string i : str) {
        if (isNum(i)) {
            int num = toNum(i);
            last.push_back(num);
        } else {
            if (last.empty()) {
                Error = true;
                std::cout << "Not enough numbers\n";
                return 0;
            }
            double x_second = last.back();  // er
            last.pop_back();

            if (i == "*") {
                if (last.empty()) {
                    Error = true;
                    std::cout << "Not enough numbers\n";
                    return 0;
                }
                double x_first = last.back();  // error can be here
                last.pop_back();
                last.push_back(x_first * x_second);

            } else if (i == "+") {
                if (last.empty()) {
                    Error = true;
                    std::cout << "Not enough numbers\n";
                    return 0;
                }
                double x_first = last.back();  // error can be here
                last.pop_back();
                last.push_back(x_second + x_first);

            } else if (i == "-") {
                if (last.empty()) {
                    Error = true;
                    std::cout << "Not enough numbers\n";
                    return 0;
                }
                double x_first = last.back();  // error can be here
                last.pop_back();
                last.push_back(x_first - x_second);
            } else if (i == "/") {
                if (last.empty()) {
                    Error = true;
                    std::cout << "Not enough numbers\n";
                    return 0;
                }
                double x_first = last.back();
                if (x_second >= -1e-7 && x_second <= 1e-7) {
                    Error = true;
                    std::cout << "Division by zero\n";
                    return 0;
                }
                last.pop_back();
                last.push_back(x_first / x_second);

            } else if (i == "tan") {
                last.push_back(tan(x_second * M_PI / 180));

            } else if (i == "log2") {
                last.push_back(log2(x_second));

            } else if (i == "sin") {
                last.push_back(sin(x_second * M_PI / 180));

            } else if (i == "cos") {
                last.push_back(cos(x_second * M_PI / 180));

            } else if (i == "^") {
                if (last.empty()) {
                    Error = true;
                    std::cout << "Not enough numbers\n";
                    return 0;
                }
                double x_first = last.back();  // error can be here
                last.pop_back();
                last.push_back(pow(x_first, x_second));

            } else if (i == "_") {
                last.push_back(-x_second);

            } else {
                std::cout << "Unkown operator\n";
            }
        }
    }

    if (last.empty()) {
        Error = true;
        std::cout << "Nothing to show, no expression\n";
        return 0;
    }
    return last.back();  // error here
}

bool isChar(char ch) {
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}

bool isOper(char ch) {
    return ch == '*' || ch == '-' || ch == '+' || ch == ')' || ch == '(' || ch == '/' || ch == '^';
}


bool isSmth(char ch) {
    return ch != ' ' && ch != '\n';
    // return isOper(ch) || isDig(ch) || isChar(ch);
}

void split(std::string& str, std::vector<std::string>& vec) {
    str += ' ';
    for (int i = 0, j = 0; i < str.size(); ++i) {
        if (!(isSmth(str[i])) && i != j) {
            vec.push_back(str.substr(j, i - j));
            j = i + 1;

        } else if (isOper(str[i]) || (i > 0 && isSmth(str[i - 1]) &&
        (isChar(str[i]) && !isChar(str[i - 1]) || isDig(str[i]) && !isDig(str[i - 1])))) {  // oper without '_'
            vec.push_back(str.substr(j, i - j));
            j = i;

        } else if (!(isSmth(str[i]))) {
            j = i + 1;
        }
        if (!vec.empty() && vec.back().empty()) vec.pop_back();
    }
}


std::deque<std::string> toPoland(std::vector<std::string>& str) {
    std::deque<std::string> operations;
    std::deque<std::string> ret;
    str.insert(str.begin(),  "(");
    str.emplace_back(")");

    bool lastOperIsUnaryMinus = false;
    bool lastOper = true;
    bool lastBrackets = false;

    for (const std::string& i : str) {
        if (isNum(i)) {
            if (lastBrackets) {
                std::cout << "Where are brackets???";
                Error = true;
                return {};
            }
            if (lastOperIsUnaryMinus) {
                ret.emplace_back(i);
                ret.emplace_back("_");
            }
            else ret.push_back(i);
            lastOperIsUnaryMinus = false;
            lastOper = false;

        } else if (i == "+") {
            if (lastBrackets) {
                std::cout << "Where are brackets???";
                Error = true;
                return {};
            }
            lastOper = true;
            if (lastOperIsUnaryMinus) {
                Error = true;
                std::cout << "Too many operations";
                return {};
            }
            while (!operations.empty() && operations.back() != "(") {
                ret.push_back(operations.back());
                operations.pop_back();
            }
            operations.push_back(i);

        } else if (i == "-") {
            if (lastBrackets) {
                std::cout << "Where are brackets???";
                Error = true;
                return {};
            }

            if (lastOper) {
                if (lastOperIsUnaryMinus) {
                    Error = true;
                    std::cout << "Too many operations";
                    return {};
                } else {
                    lastOperIsUnaryMinus = true;
                }
            } else {
                while (!operations.empty() && operations.back() != "(") {
                    ret.push_back(operations.back());
                    operations.pop_back();
                }
                operations.push_back(i);
            }
            lastOper = true;

        } else if (i == "(") {
            operations.emplace_back("(");
            lastOper = true;
            lastBrackets = false;

        } else if (i == ")") {
            if (lastBrackets) {
                std::cout << "Where are OPEN brackets???";
                Error = true;
                return {};
            }
            while (!operations.empty() && operations.back() != "(") {
                ret.push_back(operations.back());
                operations.pop_back();
            }
            if (operations.empty()) {
                Error = true;
                std::cout << "not enough brackets\n";
                return {};
            }
            operations.pop_back();

        } else if (i == "*" || i == "/") {
            if (lastBrackets) {
                std::cout << "Where are brackets???";
                Error = true;
                return {};
            }
            while (!operations.empty() && operations.back() != "(" &&
                   operations.back() != "+" && operations.back() != "-" && operations.back() != "^") {
                ret.push_back(operations.back());
                operations.pop_back();
            }
            operations.push_back(i);
            lastOper = true;

        } else if (i == "^") {
            operations.emplace_back("^");
            lastOper = true;

        } else {
            operations.push_back(i);
            lastOper = true;
            lastBrackets = true;
        }
    }

    if (!operations.empty()) {
        std::cout << "А хде скобки еще?";
        Error = true;
        return {};
    }
    return ret;
}

void solve() {
    for (int i = 0; i < 100; ++i) {
        Error = false;
        std::vector<std::string> str;
        std::string input;
        getline(std::cin, input);
        split(input, str);
        std::deque<std::string> poland = toPoland(str);
        if (Error) continue;

        if (!Error) {
            double x = post(poland);
            std::cout << "Poland cow: ";
            for (auto f : poland) {
                std::cout << f << ' ';
            }
            std::cout << std::endl;
            if (Error) continue;
            std::cout << x << '\n';
        }
    }
}