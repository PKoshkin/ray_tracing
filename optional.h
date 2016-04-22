#ifndef OPTIONAL_KOSHKIN_493 
#define OPTIONAL_KOSHKIN_493

template<class Type>
class Optional {
private:
    Type value;
    bool containsValue;
    
public:
    Optional();
    Optional(Type inValue);

    Type getValue();
    bool hasValue();
};

template<class Type>
Optional<Type>::Optional() : containsValue(false) {}

template<class Type>
Optional<Type>::Optional(Type inValue) : value(inValue), containsValue(true)  {}

template<class Type>
Type Optional<Type>::getValue() {
    return value;
}

template<class Type>
bool Optional<Type>::hasValue() {
    return containsValue;
}

#endif
