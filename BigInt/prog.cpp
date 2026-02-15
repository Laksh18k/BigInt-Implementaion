#include <bits/stdc++.h>
using namespace std;

const long long BASE = 100000000LL; // 10^8

// Remove leading zeros
void trim(vector<long long>& a) {
    while (a.size() > 1 && a.back() == 0)
        a.pop_back();
}

// Convert string to base-10^8 blocks
void convert(const string& s, vector<long long>& num) {

    int len = s.length();
    int t = len % 8;
    int pos = 0;

    if (t != 0) {
        num.push_back(stoll(s.substr(0, t)));
        pos = t;
    }

    while (pos < len) {
        num.push_back(stoll(s.substr(pos, 8)));
        pos += 8;
    }
}

// Addition
vector<long long> add(const vector<long long>& a,
                      const vector<long long>& b) {

    vector<long long> res;

    int i = a.size() - 1;
    int j = b.size() - 1;

    long long carry = 0;

    while (i >= 0 || j >= 0 || carry) {

        long long x = (i >= 0 ? a[i] : 0);
        long long y = (j >= 0 ? b[j] : 0);

        long long sum = x + y + carry;

        carry = sum / BASE;
        sum %= BASE;

        res.push_back(sum);

        i--;
        j--;
    }

    reverse(res.begin(), res.end());
    return res;
}

// Subtraction (a >= b)
vector<long long> sub(const vector<long long>& a,
                      const vector<long long>& b) {

    vector<long long> res;

    int i = a.size() - 1;
    int j = b.size() - 1;

    long long borrow = 0;

    while (i >= 0) {

        long long x = a[i] - borrow;
        long long y = (j >= 0 ? b[j] : 0);

        if (x < y) {
            x += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        res.push_back(x - y);

        i--;
        j--;
    }

    reverse(res.begin(), res.end());
    trim(res);

    return res;
}

// Shift left (multiply by BASE^k)
vector<long long> shift(const vector<long long>& a, int k) {

    vector<long long> res = a;

    while (k--)
        res.push_back(0);

    return res;
}

// Naive multiplication (base case)
vector<long long> multiplyNaive(const vector<long long>& a,
                                const vector<long long>& b) {

    int n = a.size(), m = b.size();

    vector<long long> res(n + m, 0);

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {

            __int128 cur = (__int128)a[i] * b[j];
            cur += res[i + j + 1];

            res[i + j + 1] = (long long)(cur % BASE);
            res[i + j] += (long long)(cur / BASE);
        }
    }

    trim(res);
    return res;
}

// Karatsuba multiplication
vector<long long> karatsuba(const vector<long long>& x,
                            const vector<long long>& y) {

    int n = max(x.size(), y.size());

    // Base case
    if (n < 32)
        return multiplyNaive(x, y);

    int k = n / 2;

    vector<long long> a = x;
    vector<long long> b = y;

    // Pad
    while (a.size() < n) a.insert(a.begin(), 0);
    while (b.size() < n) b.insert(b.begin(), 0);

    // Split
    vector<long long> a1(a.begin(), a.begin() + k);
    vector<long long> a0(a.begin() + k, a.end());

    vector<long long> b1(b.begin(), b.begin() + k);
    vector<long long> b0(b.begin() + k, b.end());

    // Recursive calls
    auto z2 = karatsuba(a1, b1);
    auto z0 = karatsuba(a0, b0);

    auto aSum = add(a1, a0);
    auto bSum = add(b1, b0);

    auto z1 = karatsuba(aSum, bSum);

    z1 = sub(z1, z2);
    z1 = sub(z1, z0);

    // Combine
    auto res = add(
        add(shift(z2, 2 * k),
            shift(z1, k)),
        z0
    );

    trim(res);
    return res;
}

// Print big number
void printNum(const vector<long long>& a) {

    cout << a[0];

    for (int i = 1; i < a.size(); i++) {

        string s = to_string(a[i]);

        while (s.length() < 8)
            s = "0" + s;

        cout << s;
    }

    cout << endl;
}

// Main
int main() {

    string s1, s2;
    int sign1, sign2;
    cout<< "Enter sign and first number:\n";
    cin >>sign1 >> s1;
    cout<< "Enter sign and second number:\n";
    cin >> sign2 >>s2;

    vector<long long> num1, num2;

    convert(s1, num1);
    convert(s2, num2);

    cout << "Choose Operation: 1.Addition 2.Subtraction 3.Multiplication\n";

    int choice;
    cin >> choice;

    if (choice == 1) {
        auto res = add(num1, num2);
        printNum(res);

    }
    else if (choice == 2) {

        auto res = sub(num1, num2);
        printNum(res);

    }
    else if (choice == 3) {

        auto res = karatsuba(num1, num2);
        printNum(res);

    }

    return 0;
}
