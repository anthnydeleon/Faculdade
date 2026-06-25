#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

struct tInteiro {
    string val;
};

struct tInteiro inteiro(string s) {
    int start = 0;
    while (start < (int)s.size() - 1 && s[start] == '0')
        start++;
    tInteiro t;
    t.val = s.substr(start);
    return t;
}

int cmpMag(const string& a, const string& b) {
    if (a.size() != b.size())
        return a.size() > b.size() ? 1 : -1;
    return a.compare(b) < 0 ? -1 : (a == b ? 0 : 1);
}

struct tInteiro soma(struct tInteiro n1, struct tInteiro n2) {
    string a = n1.val, b = n2.val;
    string result = "";
    int carry = 0;
    int i = a.size() - 1;
    int j = b.size() - 1;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        carry = sum / 10;
        result += (char)('0' + sum % 10);
    }

    reverse(result.begin(), result.end());
    return inteiro(result);
}

string subStrings(const string& a, const string& b) {
    string result = "";
    int borrow = 0;
    int i = a.size() - 1;
    int j = b.size() - 1;

    while (i >= 0) {
        int diff = (a[i--] - '0') - borrow;
        if (j >= 0) diff -= (b[j--] - '0');
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result += (char)('0' + diff);
    }

    reverse(result.begin(), result.end());
    return result;
}

struct tInteiro sub(struct tInteiro n1, struct tInteiro n2) {
    string a = n1.val, b = n2.val;
    int cmp = cmpMag(a, b);

    if (cmp == 0) return inteiro("0");

    string result;
    if (cmp > 0) {
        result = subStrings(a, b);
    } else {
        result = "-" + subStrings(b, a);
    }
    return inteiro(result);
}

struct tInteiro mult(struct tInteiro n1, struct tInteiro n2) {
    string a = n1.val, b = n2.val;

    if (a == "0" || b == "0") return inteiro("0");

    int na = a.size(), nb = b.size();
    vector<int> digits(na + nb, 0);

    for (int i = na - 1; i >= 0; i--) {
        for (int j = nb - 1; j >= 0; j--) {
            int mul = (a[i] - '0') * (b[j] - '0');
            int p1 = i + j;
            int p2 = i + j + 1;
            int sum = mul + digits[p2];
            digits[p2] = sum % 10;
            digits[p1] += sum / 10;
        }
    }

    string result = "";
    for (int d : digits)
        if (!(result.empty() && d == 0))
            result += (char)('0' + d);

    return inteiro(result.empty() ? "0" : result);
}

int main() {
    string s1, s2, op;
    while (cin >> s1 >> op >> s2) {
        struct tInteiro n1 = inteiro(s1);
        struct tInteiro n2 = inteiro(s2);
        struct tInteiro result;

        if      (op == "+") result = soma(n1, n2);
        else if (op == "-") result = sub(n1, n2);
        else                result = mult(n1, n2);

        cout << result.val << "\n";
    }
    return 0;
}