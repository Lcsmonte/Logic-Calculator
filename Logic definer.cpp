// Welcome to my logic Calculator for C++
// This calculator accepts ( ! ~ & ^ | ) prioritizing as written.
// the Unary operator only works as Suffix
// Normal usage !1 == 0, Program usage 1! == 0. 

#include <iostream>

using namespace std;


class Trial {
public:
    char kind;
    int TF;
    Trial(char ch)
        :kind(ch) {}
    Trial(char ch, int ys)
        :kind('2'), TF(ys) {}
};

class Trial_stream {
public:
    Trial_stream();
    Trial get();
    void putback(Trial t);

private:
    bool full;
    Trial buffer;
};

Trial_stream::Trial_stream()
    :full(false), buffer(2) {}

void Trial_stream::putback(Trial t)
{
 if (full) cerr <<"putback() into a full buffer";
 buffer = t;
 full = true;
}

Trial Trial_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch (ch) {
    case ';':    // for "print"
    case 'q':    // for "quit"
    case '(': case ')': case '{':case '}':case '!':case'|':case '~': case '&': case '^':
        return Trial(ch);       

    case '0': case '1':
    {
        cin.putback(ch);     
        int ys;
        cin >> ys;        
        return Trial('2', ys);   //'2' represent "a number"
    }
    default:
        cerr <<"Bad token";
    }
}


Trial_stream tr;        // provides get() and putback() 

int Or();    // declaration so that primary() can call Or()


int Primary() // deal with True, False and parentheses
{
    Trial t = tr.get();
    switch (t.kind)
    {
    case '(':
    {
        int d = Or();
        t = tr.get();
        if (t.kind != ')') cerr << "')' expected";
        return d;
    }
    case '{':
    {
        int j = Or();
        t = tr.get();
        if (t.kind != '}') cerr << "'}' expected";
        return j;
    }
    case '2':
        return t.TF;

    default:
        cerr << "primary expected";
    }
}

int Negate() // deal with '!' - '~'
{
    int last = Primary();
    Trial t = tr.get();

    while (true) {
        switch (t.kind) {
        case '!':
            last = !last;
            t = tr.get();
            break;
        case '~':
            last = !last;
            t = tr.get();
            break;
        default:
            tr.putback(t);
            return last;
        }
    }
}

int And() { // deal with &

    int last = Negate();
    Trial t = tr.get();

    while (true) {
        switch (t.kind) {
        case '&':

            last &= Negate();
            t = tr.get();
            break;
        
        default:
            tr.putback(t);
            return last;
        }
    }
}

int Or_exclusive() { // deal with ^

    int last = And();
    Trial t = tr.get();

    while (true) {
        switch (t.kind) {
        case '^':

            last ^= And();
            t = tr.get();
            break;

        default:
            tr.putback(t);
            return last;
        }
    }
}


int Or() // deal with |
{
    int last = Or_exclusive();
    Trial t = tr.get();

    while (true) {
        switch (t.kind) {
        case '|':
            last |= Or_exclusive();
            t = tr.get();
            break;

        default:
            tr.putback(t);
            return last;
        }
    }

}

int main()
try
{
    int TF = 2;
    while (cin) {

        Trial t = tr.get();

        if (t.kind == 'q') break; // 'q' for quit
        if (t.kind == ';')        // ';' for "print now"}
            cout << "=" << TF << '\n';
        else
            tr.putback(t);
        TF = Or();
    }
    system("PAUSE");
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    system("PAUSE");
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    system("PAUSE");
    return 2;
}
