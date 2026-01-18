# implement features

### sensor classes with filtering
`Potentiometer<MedianFilter>()`

gpt pseudocode:
```
class BaseFilter {
public:
    virtual float filter(float newValue) = 0; // Pure virtual function for filtering
    virtual ~BaseFilter() {} // Virtual destructor
};

class AverageFilter : public BaseFilter {
    float k;
public:
    AverageFilter(float k) : k(k) {}
    float filter(float newValue) override {
        // Implement averaging logic
        // Example: this->value = newValue * k + this->value * (1 - k);
        return newValue; // Placeholder
    }
};

class MedianFilter : public BaseFilter {
    // Implement median filtering
public:
    float filter(float newValue) override {
        // Implement median filtering logic
        return newValue; // Placeholder
    }
};

template <class T>
class Potentiometer {
    T filter;
public:
    Potentiometer(T filter) : filter(filter) {}

    float readValue() {
        float rawValue = analogRead(A0); // Assuming A0 is the potentiometer pin
        return filter.filter(rawValue);
    }
};

// Usage Example

// Create a potentiometer object with AverageFilter
AverageFilter avgFilter(0.5); // Assuming k=0.5 for averaging
Potentiometer<AverageFilter> pot(avgFilter);

// Create a potentiometer object with MedianFilter
MedianFilter medFilter;
Potentiometer<MedianFilter> pot2(medFilter);

void setup() {
    // Setup code...
}

void loop() {
    float filteredValue = pot.readValue();
    // Do something with filteredValue

    float filteredValue2 = pot2.readValue();
    // Do something with filteredValue2
}

```