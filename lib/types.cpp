#include "types.h"

namespace types {
        template <class T> 
        ArrayHolder<T>::ArrayHolder(T* inputarray, int inputsize) {
            size = inputsize;
            bytesize = sizeof(T)*inputsize;
            storedarray = (T*)malloc(bytesize);
            std::copy(inputarray, inputarray + size, storedarray);
        }

        template <class T>
        ArrayHolder<T>::ArrayHolder() {
            size = 0;
            bytesize = 0;
            storedarray = nullptr;
        }
        
        template <class T>
        ArrayHolder<T>::ArrayHolder(ArrayHolder<T>& inputarrayholder) {
            size = inputarrayholder.size;
            bytesize = sizeof(T)*size;
            storedarray = (T*)malloc(bytesize);
            std::copy(inputarrayholder.storedarray, inputarrayholder.storedarray + size, storedarray);
        }

        template <class T>
        ArrayHolder<T>& ArrayHolder<T>::operator= (ArrayHolder<T>& inputarrayholder) {
            size = inputarrayholder.size;
            bytesize = sizeof(T)*size;
            storedarray = (T*)malloc(bytesize);
            std::copy(inputarrayholder.storedarray, inputarrayholder.storedarray + size, storedarray);
            return *this;
        }

        template <class T>
        ArrayHolder<T>& ArrayHolder<T>::operator= (const ArrayHolder<T>& inputarrayholder) {
            size = inputarrayholder.size;
            bytesize = sizeof(T)*size;
            storedarray = (T*)malloc(bytesize);
            std::copy(inputarrayholder.storedarray, inputarrayholder.storedarray + size, storedarray);
            return *this;
        }

        template <class T>
        void ArrayHolder<T>::append(T& input) {
            size += 1;
            bytesize += sizeof(T);
            storedarray = (T*)realloc(storedarray,bytesize);
            storedarray[size - 1] = input;
        }

        template <class T>
        ArrayHolder<T>::~ArrayHolder() {
            for (int i = 0; i < size; i++) {
                storedarray[i].~T();
            }
            free(storedarray);
        }
        //constructors
        

        template class ArrayHolder<GLfloat>;
        template class ArrayHolder<ArrayHolder<GLfloat>>;
        class Renderdata;
}