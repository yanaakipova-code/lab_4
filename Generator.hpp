template<typename T>
class Cenerator{
public:
    virtual ~Generator();
    virtual T GetNext() = 0;
    virtual bool HasNext() const = 0;
    virtual Optional<T> TryGetNext(){
        if (HasNext()){
            return Optional<T>(GetNext());
        }
        return Optional<T>();
    }
    virtual Generator <T>* Append(T item) const = 0;
    virtual Generator <T>* Append (Sequnce<T>* items) const = 0;
    virtual Generator <T>* Insert (T item) const = 0;
    virtual Generator <T>* Insert (Sequnce<T>* items) const = 0;
    virtual Generator <T>* Remove (T item) const = 0;
    virtual Generator <T>* Remove (Sequnce<T>* items) const = 0;
};