template<typename T>
Array<T>::Array() {
    this->A = new T[1];
    this->cap = 1;
    this->size = 1;
}

template<typename T>
Array<T>::Array(size_t size) {
    this->cap = size * 2;
    this->size = size;
    this->A = new T[this->cap];
    for(int i = 0; i<this->size; i++)
    {
        this->A[i] = T();
    }
}

template<typename T>
Array<T>::Array(size_t size, bool behaviour_stack)
{
    this->cap = size * 2;
    if(behaviour_stack) this->size = 0;
    else this->size = size;
    this->A = new T[this->cap];
    for(int i = 0; i<this->size; i++)
    {
        this->A[i] = T();
    }
}

template<typename T>
Array<T>::Array(size_t size, size_t cap)
{
    this->cap = cap;
    this->size = size;
    this->A = new T[this->cap];
    for(int i = 0; i<this->size; i++)
    {
        this->A[i] = T();
    }
}

template<typename T>
Array<T>::Array(T fill, size_t size) {
    this->cap = 2 * size;
    this->size = size;
    this->A = new T[this->cap];
    for(int i = 0; i<this->size; i++)
    {
        this->A[i] = fill;
    }
}

template<typename T>
Array<T>::Array(T fill, size_t size, size_t cap)
{
    this->cap = cap;
    this->size = size;
    this->A = new T[this->cap];
    for(int i = 0; i<this->size; i++)
    {
        this->A[i] = fill;
    }
}

template<typename T>
Array<T>::Array(const Array<T>& copy)
{
    this->cap = copy.cap;
    this->size = copy.size;
    this->A = new T[this->cap];
    for (int i = 0; i < this->size; i++)
    {
        this->A[i] = copy[i];
    }
}

template<typename T>
Array<T>::Array(const std::initializer_list<T>& raw) {
    if(raw.size() != 0) {
        this->cap = 2 * raw.size();
        this->size = raw.size();
        this->A = new T[this->cap];
        for(int i = 0; i<this->size; i++)
        {
            this->A[i] = raw.begin()[i];
        }
    } else {
        this->A = new T[1];
        this->cap = 1;
        this->size = 1;
    }
    
}

template<typename T>
Array<T>::~Array() {
    delete[] this->A;
}

template<typename T>
Array<T>& Array<T>::operator =(const Array<T>& copy) {
    this->cap = copy.cap;
    this->size = copy.size;
    this->A = new T[copy.cap];
    for(int i = 0; i<this->size; i++)
    {
        this->A[i] = (T)copy[i];
    }
    return *this;
}

template<typename T>
Array<T>& Array<T>::operator*=(T && m)
{
    for(int i = 0; i<this->size; i++) { this->A[i] *= m; }
    return *this;
}
template<typename T>
Array<T>& Array<T>::operator+=(T && m)
{
    for(int i = 0; i<this->size; i++) { this->A[i] += m; }
    return *this;
}

template<typename T>
T& Array<T>::operator [](size_t idx) {
    if(idx >= this->size) { throw std::invalid_argument("idx must be in range [0, " + std::to_string(this->size) + "]. index given: " + std::to_string(idx)); }
    return this->A[idx];
}

template<typename T>
T* Array<T>::get_c()
{
    return &this->A[0];
}


template<typename T>
const T& Array<T>::operator [](size_t idx) const
{
    if(idx >= this->size) { throw std::invalid_argument("idx must be in range [0, " + std::to_string(this->size) + "]. index given: " + std::to_string(idx)); }
    return this->A[idx];
}

template<typename T>
const T* Array<T>::get_c() const
{
    return &this->A[0];
}

template<typename T>
void Array<T>::push_back(const T v) {
    this->size++;
    if (this->size < this->cap)
        this->A[this->size - 1] = std::move(v);
    else {
        this->cap = this->size * 2;
        T* alloc_A = new T[this->cap];
        for (int i = 0; i < this->size - 1; i++) { alloc_A[i] = this->A[i]; }
        alloc_A[this->size - 1] = v;
        delete[] this->A;
        this->A = alloc_A;
    }
}

template<typename T>
void Array<T>::pop()
{
    this->size--;
}

template<typename T>
T Array<T>::top()
{
    return this->A[this->size-1];
}

template<typename T>
T Array<T>::sum()
{
    T sum = 0;
    for(int i = 0; i<this->size; i++)
    {
        sum += this->A[i];
    }
    return sum;
}

template<typename T>
size_t Array<T>::size_bytes() {  
    return this->size * sizeof(T);
}

template<typename T>
std::string Array<T>::to_str() {
    std::string l = "[";
    for(int i = 0; i<this->size-1; i++)
    {
        l += std::to_string(this->A[i]) + " ";
    }
    l += std::to_string(this->A[this->size-1]) + "]";
    return l;
}