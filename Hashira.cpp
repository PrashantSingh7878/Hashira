#include <bits/stdc++.h>
#include <nlohmann/json.hpp> // Add this line
using namespace std;
using json = nlohmann::json; // Add this line

#define int128 __int128_t

int128 stringToBase128(const string& s, int base) {
    int128 res = 0;
    for (char c : s) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'z') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') digit = c - 'A' + 10;
        else throw runtime_error("Invalid character.");
        if (digit >= base) throw runtime_error("Digit out of range.");
        res = res * base + digit;
    }
    return res;
}

int128 modInverse(int128 a, int128 mod) {
    int128 m0 = mod, t, q;
    int128 x0 = 0, x1 = 1;
    if (mod == 1) return 0;
    while (a > 1) {
        q = a / mod;
        t = mod;
        mod = a % mod, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

int128 lagrangeInterpolation(const vector<pair<int128, int128>>& points) {
    int128 res = 0;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        int128 xi = points[i].first;
        int128 yi = points[i].second;
        int128 num = 1, den = 1;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int128 xj = points[j].first;
                num *= -xj;
                den *= (xi - xj);
            }
        }
        int128 inv = modInverse(den, LLONG_MAX);
        res += yi * num * inv;
    }
    return res;
}

string toString(int128 num) {
    if (num == 0) return "0";
    string res;
    while (num > 0) {
        res += '0' + (num % 10);
        num /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

void solve(const string& filename) {
    ifstream file(filename);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    json j = json::parse(content);

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<int128, int128>> points;
    for (int i = 1; i <= n && points.size() < k; i++) {
        string idx = to_string(i);
        if (j.contains(idx)) {
            int x = stoi(idx);
            int base = stoi(j[idx]["base"].get<string>());
            string val = j[idx]["value"];
            int128 y = stringToBase128(val, base);
            points.emplace_back(x, y);
        }
    }

    int128 secret = lagrangeInterpolation(points);
    cout << toString(secret) << endl;
}

int main() {
    solve("testcase1.json");
    solve("testcase2.json");
    return 0;
}
