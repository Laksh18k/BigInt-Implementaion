// Laksh Kachure BT24CSE051 Cpl Assignment 2

#include <bits/stdc++.h>
using namespace std;
const long long BASE = 100000000LL; // 10^8

class BigInt {
    public:
    bool sign; // 0 for positive, 1 for negative
    vector<long long> num; // base-10^8 blocks
    BigInt() {                 
        sign = false;
        num.push_back(0);
    }
    BigInt(string s) {                          // Constructor to convert input to BigInt format
        sign = false;
        if (s[0] == '-') {
            sign = true;
            s = s.substr(1);
        }

        int len = s.size();
        int rem = len % 8;
        int pos = 0;

        if (rem != 0) {
            num.push_back(stoll(s.substr(0, rem)));
            pos = rem;
        }

        while (pos < len) {
            num.push_back(stoll(s.substr(pos, 8)));
            pos += 8;
        }

        removezeroes();
    }
    
    void display(){                                     // Function to display the BigInt in the correct format
        if(sign) cout<<"-";
        cout << num[0];
        for (int i = 1; i < num.size(); i++) {

            string s = to_string(num[i]);

            while (s.length() < 8)
                s = "0" + s;

            cout << s;
        }
        cout << endl;   
    }
    void removezeroes() {                                      // Remove leading zeros and handle the case of zero
        while (num.size() > 1 && num[0] == 0) {
            num.erase(num.begin());
        }
        if (num.size() == 1 && num[0] == 0) {              // If the number is zero, ensuring the sign is positive
            sign= false;
        }
    }

};



//////////////// Class Ends //////////////////////

int cmpMag(const vector<long long>& a, const vector<long long>& b) {       // Compare magnitudes of two numbers represented as vectors
    if (a.size() < b.size()) return -1;
    if (a.size() > b.size()) return 1;

    for (int i = 0; i < a.size(); i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}


vector<long long> addMag(const vector<long long>& a,                     // Add magnitudes of two numbers represented as vectors
                         const vector<long long>& b) {

    vector<long long> res;
    int i = a.size() - 1;
    int j = b.size() - 1;
    long long carry = 0;

    while (i >= 0 || j >= 0 || carry != 0) {                           // Loop until all digits are processed and no carry remains

        long long x = 0;
        long long y = 0;

        if (i >= 0) x = a[i];
        if (j >= 0) y = b[j];

        long long sum = x + y + carry;
        carry = sum / BASE;
        res.push_back(sum % BASE);

        i--;
        j--;
    }

    reverse(res.begin(), res.end());
    return res;
}


//a >= b
vector<long long> subMag(const vector<long long>& a,                    // Subtract magnitudes of two numbers represented as vectors, assuming a >= b
                         const vector<long long>& b) {                         

    vector<long long> res;
    int i = a.size() - 1;
    int j = b.size() - 1;
    long long borrow = 0;

    while (i >= 0) {

        long long x = a[i] - borrow;
        long long y = 0;
        if (j >= 0) y = b[j];

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

    while (res.size() > 1 && res[0] == 0) {
        res.erase(res.begin());
    }

    return res;
}


vector<long long> multiplybasecase(const vector<long long>& a,            // Base case foe karatsuba algo to multiply magnitudes of two numbers represented as vectors using the standard O(n^2) method
                                const vector<long long>& b) {

    vector<long long> res(a.size() + b.size(), 0);

    for (int i = a.size() - 1; i >= 0; i--) {
        for (int j = b.size() - 1; j >= 0; j--) {

            __int128 cur = (__int128)a[i] * b[j];
            cur += res[i + j + 1];

            res[i + j + 1] = (long long)(cur % BASE);
            res[i + j] += (long long)(cur / BASE);
        }
    }

    while (res.size() > 1 && res[0] == 0) {
        res.erase(res.begin());
    }

    return res;
}


vector<long long> shift(const vector<long long>& a, int k) {
    vector<long long> res = a;
    for (int i = 0; i < k; i++) {
        res.push_back(0);
    }
    return res;
}

vector<long long> karatsuba(const vector<long long>& x,                    // Multiply magnitudes of two numbers represented as vectors using the Karatsuba algorithm
                            const vector<long long>& y) {

    int n = max(x.size(), y.size());
    if (n < 32) {
        return multiplybasecase(x, y);
    }

    int low = n / 2;
    int high = n - low;

    vector<long long> a = x;
    vector<long long> b = y;

    while (a.size() < n) a.insert(a.begin(), 0);
    while (b.size() < n) b.insert(b.begin(), 0);

    vector<long long> a1(a.begin(), a.begin() + high);
    vector<long long> a0(a.begin() + high, a.end());
    vector<long long> b1(b.begin(), b.begin() + high);
    vector<long long> b0(b.begin() + high, b.end());

    vector<long long> z2 = karatsuba(a1, b1);
    vector<long long> z0 = karatsuba(a0, b0);

    vector<long long> aSum = addMag(a1, a0);
    vector<long long> bSum = addMag(b1, b0);

    vector<long long> z1 = karatsuba(aSum, bSum);
    z1 = subMag(z1, z2);
    z1 = subMag(z1, z0);

    vector<long long> res = addMag(
        addMag(shift(z2, 2 * low),
               shift(z1, low)),
        z0
    );

    while (res.size() > 1 && res[0] == 0) {
        res.erase(res.begin());
    }

    return res;
}

BigInt add(const BigInt& A, const BigInt& B) {            // wrapper function to add two BigInts, handling signs and calling the appropriate magnitude functions
    BigInt res;

    if (A.sign == B.sign) {
        res.num = addMag(A.num, B.num);
        res.sign = A.sign;
    } else {
        int cmp = cmpMag(A.num, B.num);
        if (cmp >= 0) {
            res.num = subMag(A.num, B.num);
            res.sign = A.sign;
        } else {
            res.num = subMag(B.num, A.num);
            res.sign = B.sign;
        }
    }

    res.removezeroes();
    return res;
}


BigInt subtract(const BigInt& A, const BigInt& B) {                 // wrapper function to subtract two BigInts, handling signs and calling the appropriate magnitude functions
    BigInt temp = B;
    temp.sign = !temp.sign;
    return add(A, temp);
}

BigInt multiply(const BigInt& A, const BigInt& B) {            // wrapper function to multiply two BigInts, handling signs and calling the karatsuba function for magnitude multiplication
    BigInt res;
    res.num = karatsuba(A.num, B.num);

    if (A.sign != B.sign) res.sign = true;
    else res.sign = false;

    res.removezeroes();
    return res;
}

//////////////////////////////////////  Main //////////////////////////////////////

int main() {

    string s1, s2;
    cout << "Enter First number:\n";
    cin >> s1 ;
    cout << "Enter Second number:\n";
    cin>>s2;

    BigInt A(s1), B(s2);

    cout << "1.Addition 2.Subtraction 3.Multiplication\n";
    int op;
    cin >> op;
    cout << "Result:\n";
    if (op == 1) add(A, B).display();
    if (op == 2) subtract(A, B).display();
    if (op == 3) multiply(A, B).display();

    return 0;
}

