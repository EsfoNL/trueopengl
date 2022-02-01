#include "types.h"

namespace types {
        template <class T> 
        ArrayHolder<T>::ArrayHolder(T* inputarray, int inputsize) {
            size = inputsize;
            bytesize = inputsize*sizeof(T);
            storedarray = std::make_unique<T[]>(size);
            std::copy(inputarray, inputarray + size - 1, storedarray.get());
        }

        template <class T>
        ArrayHolder<T>::ArrayHolder() {
            storedarray = nullptr;
            size = 0;
            bytesize = 0;
        }
        
        template <class T>
        ArrayHolder<T>::ArrayHolder(ArrayHolder<T>& inputarrayholder) {
            size = inputarrayholder.size;
            bytesize = inputarrayholder.bytesize;
            storedarray = std::make_unique<T[]>(size);
            std::copy(inputarrayholder.storedarray.get(), inputarrayholder.storedarray.get() + size - 1, inputarrayholder.storedarray.get());
        }

        template <class T>
        ArrayHolder<T>& ArrayHolder<T>::operator= (ArrayHolder<T>& inputarrayholder) {
            size = inputarrayholder.size;
            bytesize = inputarrayholder.bytesize;
            storedarray = std::make_unique<T[]>(size);
            std::copy(inputarrayholder.storedarray.get(), inputarrayholder.storedarray.get() + size - 1, storedarray.get());
            return *this;
        }

        template <class T>
        ArrayHolder<T>& ArrayHolder<T>::operator= (const ArrayHolder<T>& inputarrayholder) {
            size = inputarrayholder.size;
            bytesize = inputarrayholder.bytesize;
            storedarray = std::make_unique<T[]>(size);
            std::copy(inputarrayholder.storedarray.get(), inputarrayholder.storedarray.get() + size - 1, storedarray.get());
            return *this;
        }

        template <class T>
        void ArrayHolder<T>::append(T& input) {
            size++;
            bytesize += sizeof(T);
            std::unique_ptr<T[]> oldarray = std::make_unique<T[]>(size);;
            storedarray.swap(oldarray);
            std::copy(oldarray.get(), oldarray.get() + size - 2, storedarray.get());
            storedarray[size - 1] = input;
        }

        //constructors
        

        template class ArrayHolder<GLfloat>;
        template class ArrayHolder<ArrayHolder<GLfloat>>;
        class Renderdata;
}