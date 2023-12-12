#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <iostream>

typedef long long int64;

int64 gcd(int64 a, int64 b) {
    return a == 0 ? b : gcd(b % a, a);
}

class fraction {
    public:

    int64 nume() const { return m_nume; }
    int64 deno() const { return m_deno; }

    friend fraction operator+ (const fraction &a, const fraction &b);
    friend fraction operator- (const fraction &x);
    friend fraction operator- (const fraction &a, const fraction &b);
    friend fraction operator* (const fraction &a, const fraction &b);
    friend fraction operator/ (const fraction &a, const fraction &b);
    friend std::istream &operator>> (std::istream &in, fraction &x);
    friend std::ostream &operator<< (std::ostream &out, const fraction &x);
    friend bool operator== (const fraction &a, const fraction &b);
    friend bool operator!= (const fraction &a, const fraction &b);
    friend bool operator< (const fraction &a, const fraction &b);
    friend bool operator> (const fraction &a, const fraction &b);
    friend bool operator<= (const fraction &a, const fraction &b);
    friend bool operator>= (const fraction &a, const fraction &b);

    public:

    fraction() {};
    fraction(const fraction &x) : m_nume(x.m_nume), m_deno(x.m_deno) {}
    fraction operator= (const fraction &x) {
        m_nume = x.m_nume;
        m_deno = x.m_deno;
        return *this;
    }
    fraction(int num) : m_nume(num), m_deno(1) {}
    fraction(int64 num) : m_nume(num), m_deno(1) {}
    fraction(int64 nume, int64 deno) : m_nume(nume), m_deno(deno) {}

    private:

    int64 m_nume = 0, m_deno = 1;

    private:

    fraction reduction();   /* we make sure that after reduction, m_deno > 0 */
};

fraction operator+ (const fraction &a, const fraction &b) {
    return fraction(a.m_nume * b.m_deno + b.m_nume * a.m_deno, a.m_deno * b.m_deno).reduction();
}

fraction operator- (const fraction &x) {
    return fraction(-x.m_nume, x.m_deno).reduction();
}

fraction operator- (const fraction &a, const fraction &b) {
    return a + (-b);
}

fraction operator* (const fraction &a, const fraction &b) {
    return fraction(a.m_nume * b.m_nume, a.m_deno * b.m_deno).reduction();
}

fraction operator/ (const fraction &a, const fraction &b) {
    return fraction(a.m_nume * b.m_deno, a.m_deno * b.m_nume).reduction();
}

std::istream &operator>> (std::istream &in, fraction &x) {
    int64 num = 0;
    in >> num;
    x = fraction(num, 1);
    return in;
}

std::ostream &operator<< (std::ostream &out, const fraction &x) {
    if (x.deno() == 1) out << x.nume();
    else out << x.nume() << '/' << x.deno();
    return out;
}

bool operator== (const fraction &a, const fraction &b) {
    return (a - b).reduction().nume() == 0;
}

bool operator!= (const fraction &a, const fraction &b) {
    return !(a == b);
}

bool operator< (const fraction &a, const fraction &b) {
    return (a - b).reduction().nume() < 0;
}

bool operator> (const fraction &a, const fraction &b) {
    return (a - b).reduction().nume() > 0;
}

bool operator<= (const fraction &a, const fraction &b) {
    return (a - b).reduction().nume() <= 0;
}

bool operator>= (const fraction &a, const fraction &b) {
    return (a - b).reduction().nume() >= 0;
}

fraction fraction::reduction() {
    if (m_nume == 0) {
        m_deno = 1;
    }
    else if (m_deno == 0) {
        throw "divided by 0";
    }
    else {
        int64 tmp = gcd(m_nume, m_deno);
        m_nume /= tmp;
        m_deno /= tmp;
    }
    return *this;
}

#endif