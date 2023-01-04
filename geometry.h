
template <typename T> class Vector3{
    public:
    // public data
    T x, y, z;

    // public methods
    T operator[](int i) const;
    T &operator[](int i);
};


