class B {
};
class BC : public B {
};

class A {
    B* p;
    virtual void func(B) = 0;
};
class AC : public A {
    BC* p;
    void func(BC);
};
