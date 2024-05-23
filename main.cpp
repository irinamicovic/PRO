#pragma once
#include <memory>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <cstring>

namespace timiimit
{
    namespace GenericStructures
    {
        template <class T>
        class ArrayList
        {
        private:
            size_t resizeCapacityCount;
            size_t size;
            size_t capacity;
            T* data;

            void resize(size_t newSize);

        public:
            ArrayList(size_t capacity, size_t resizeCapacityAdditionCount);
            ArrayList(size_t capacity);
            ArrayList();
            ArrayList(const ArrayList<T>& o);
            ~ArrayList();

            ArrayList<T>& operator=(const ArrayList<T>& that);

            size_t GetCurrentCapacity() const;
            size_t GetSize() const;
            T& operator[](size_t index) const;
            T* GetData() const;
            size_t FindNearestTo(const T& value) const;
            size_t Find(const T& value) const;

            bool Contains(const T& val) const;

            void Add(const T& val);
            void Insert(const T& val, size_t index);
            void Insert(const T* vals, size_t count, size_t index);

            void Move(size_t destinationIndex, size_t sourceIndex);

            void Swap(size_t index1, size_t index2);

            void Remove(size_t index);
            void Remove(size_t index, size_t count);
            void Remove(T* pointer);
            void Clear();

            void ShiftRight(size_t start, size_t count);
            void ShiftLeft(size_t start, size_t count);

            void ShrinkToFit();
        };

        ///////////////////////////////////////////
        //              IMPLEMENTATION           //
        ///////////////////////////////////////////

        template <class T>
        ArrayList<T>::ArrayList() : ArrayList<T>::ArrayList(16)
        {
        }

        template<class T>
        inline ArrayList<T>::ArrayList(const ArrayList<T>& o) :
            resizeCapacityCount(o.resizeCapacityCount),
            size(0),
            capacity(o.capacity),
            data(new T[o.capacity])
        {
            for (size_t i = 0; i < o.size; ++i)
            {
                this->Insert(o[i], i);
            }
        }

        template <class T>
        ArrayList<T>::ArrayList(size_t capacity) : ArrayList<T>::ArrayList(capacity, 16)
        {
        }

        template <class T>
        ArrayList<T>::ArrayList(size_t capacity, size_t resizeCapacityAdditionCount) :
            resizeCapacityCount(resizeCapacityAdditionCount),
            size(0),
            capacity(capacity),
            data(new T[capacity])
        {
        }


        template <class T>
        ArrayList<T>::~ArrayList()
        {
            delete[] data;
            data = nullptr;
            size = 0;
            capacity = 0;
        }

        template<class T>
        inline ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& that)
        {
            /*
            resizeCapacityCount = that.resizeCapacityCount;
            if (that.size > capacity)
            {
                capacity = that.capacity;
                resize(capacity);
            }
            else
            {
                // capacity stays since we dont need to make capacity larger
            }
            size = that.size;
            */

            for (size_t i = 0; i < that.size; ++i)
            {
                this->Insert(that[i], i);
            }
            //memcpy(data, that.data, that.size);

            return *this;
        }

        template<class T>
        inline size_t ArrayList<T>::GetCurrentCapacity() const
        {
            return capacity;
        }

        template <class T>
        inline size_t ArrayList<T>::GetSize() const
        {
            return size;
        }

        template<class T>
        inline T& ArrayList<T>::operator[](size_t index) const
        {
            if (index >= size)
            {
                throw std::exception("index out of range");;
            }
            return *(data + index);
        }

        template<class T>
        inline T* ArrayList<T>::GetData() const
        {
            return data;
        }

        template<class T>
        inline size_t ArrayList<T>::FindNearestTo(const T& value) const
        {
            size_t min = 0;
            size_t max = size - 1;

            while (min < max)
            {
                size_t mid = (max + min) / 2;
                if (data[mid] < value)
                {
                    min = mid + 1;
                }
                else if (data[mid] > value)
                {
                    max = mid;
                }
                else
                {
                    return mid;
                }
            }
            return min;
        }

        template<class T>
        inline size_t ArrayList<T>::Find(const T& value) const
        {
            size_t index = FindNearestTo(value);
            if (*(data + index) == value)
                return index;
            return -1;
        }

        template<class T>
        inline bool ArrayList<T>::Contains(const T& val) const
        {
            T* p = data;
            T* end = data + size;
            while (p < end)
            {
                if (*p == val)
                    return true;
                p++;
            }
            return false;
        }

        template <class T>
        inline void ArrayList<T>::Add(const T& val)
        {
            if (size >= capacity)
            {
                resize(capacity + resizeCapacityCount);
            }
            data[size++] = T(val);
        }

        template<class T>
        inline void ArrayList<T>::Insert(const T& val, size_t index)
        {
            if (index > size)
            {
                throw std::exception("index out of range");;
            }
            if (size >= capacity)
            {
                resize(capacity + resizeCapacityCount);
            }
            if (index > size)
            {
                return;
            }
            if (size + 1 > capacity)
            {
                size_t newCapacity = size + 1 + resizeCapacityCount;
                size_t sameCpyCount = index;
                size_t shiftedCpyCount = size - index;
                T* larger = new T[newCapacity];
                std::memcpy(larger, data, sameCpyCount * sizeof(T));
                std::memcpy(larger + index + 1, data + index, shiftedCpyCount * sizeof(T));
                larger[index] = T(val);
                delete[] data;
                data = larger;
                capacity = newCapacity;
            }
            else
            {
                size_t sameCpyCount = index;
                size_t shiftedCpyCount = size - index;
                std::memmove(data + index + 1, data + index, shiftedCpyCount * sizeof(T));
                data[index] = T(val);
            }
            size++;
        }

        template<class T>
        inline void ArrayList<T>::Insert(const T* vals, size_t count, size_t index)
        {
            if (index > size)
            {
                throw std::exception("index out of range");;
            }
            if (size + count > capacity)
            {
                size_t newCapacity = size + count + resizeCapacityCount;
                size_t sameCpyCount = index;
                size_t shiftedCpyCount = size - index;
                T* larger = new T[newCapacity];
                std::memcpy(larger, data, sameCpyCount * sizeof(T));
                std::memcpy(larger + index + count, data + index, shiftedCpyCount * sizeof(T));
                std::memcpy(larger + index, vals, count * sizeof(T)); // copy data to array
                delete[] data;
                data = larger;
                capacity = newCapacity;
            }
            else
            {
                size_t sameCpyCount = index;
                size_t shiftedCpyCount = size - index;
                std::memmove(data + index + count, data + index, shiftedCpyCount * sizeof(T));
                std::memmove(data + index, vals, count * sizeof(T)); // copy data to array
            }
            size += count;
        }

        template<class T>
        inline void ArrayList<T>::Move(size_t destinationIndex, size_t sourceIndex)
        {
            if (destinationIndex >= size)
            {
                throw std::exception("destinationIndex out of range");
            }
            else if (sourceIndex >= size)
            {
                throw std::exception("sourceIndex out of range");
            }

            T tmp = *(data + sourceIndex);
            //memcpy(&tmp, data + sourceIndex, sizeof(T));
            if (sourceIndex < destinationIndex) // move element to right
            {
                std::memmove(data + sourceIndex, data + sourceIndex + 1, (destinationIndex - sourceIndex) * sizeof(T));
            }
            else if (sourceIndex > destinationIndex) // move element to left
            {
                std::memmove(data + destinationIndex + 1, data + destinationIndex, (sourceIndex - destinationIndex) * sizeof(T));
            }
            *(data + destinationIndex) = tmp;
            //memcpy(data + destinationIndex, &tmp, sizeof(T));
        }

        template<class T>
        inline void ArrayList<T>::Swap(size_t index1, size_t index2)
        {
            if (index1 >= size)
            {
                throw std::exception("index1 out of range");
            }
            else if (index2 >= size)
            {
                throw std::exception("index2 out of range");
            }

            T tmp = data[index1];
            data[index1] = data[index2];
            data[index2] = tmp;
        }

        template<class T>
        inline void ArrayList<T>::Remove(size_t index)
        {
            ShiftLeft(index, 1);
            size--;
        }

        template<class T>
        inline void ArrayList<T>::Remove(T* pointer)
        {
            Remove(size_t(pointer - data) / sizeof(T));
        }

        template<class T>
        inline void ArrayList<T>::Remove(size_t index, size_t count)
        {
            if (index + count > size)
            {
                throw std::exception("selection out of range");
            }
            ShiftLeft(index + count, count);
            size -= count;
        }

        template <class T>
        inline void ArrayList<T>::resize(size_t newCapacity)
        {
            T* larger = new T[newCapacity];
            if (newCapacity >= size)
            {
                std::memcpy(larger, data, size * sizeof(T));
            }
            else
            {
                std::memcpy(larger, data, newCapacity * sizeof(T));
            }
            delete[] data;
            data = larger;
            capacity = newCapacity;
        }

        template<class T>
        inline void ArrayList<T>::ShiftLeft(size_t start, size_t count)
        {
            std::memmove(data + start - count, data + start, (size - start) * sizeof(T));
        }

        template<class T>
        inline void ArrayList<T>::ShiftRight(size_t start, size_t count)
        {
            if (size + count > capacity)
            {
                size_t newCapacity = size + count + resizeCapacityCount;
                size_t shiftedCpyCount = size - start;
                T* larger = new T[newCapacity];
                std::memcpy(larger, data, start * sizeof(T));
                std::memcpy(larger + start, data + start, count * sizeof(T));
                std::memcpy(larger + start + count, data + start, shiftedCpyCount * sizeof(T));
                delete[] data;
                data = larger;
                capacity = newCapacity;
            }
            else
            {
                size_t sameCpyCount = start;
                size_t shiftedCpyCount = size - start;
                std::memmove(data + start + count, data + start, shiftedCpyCount * sizeof(T));
            }
            size += count;
        }

        template<class T>
        inline void ArrayList<T>::Clear()
        {
            size = 0;
        }

        template<class T>
        inline void ArrayList<T>::ShrinkToFit()
        {
            resize(size);
        }
    }
}

using namespace std;
using namespace timiimit::GenericStructures;

typedef unsigned char byte;
typedef unsigned int uint;

template<class T = byte>
void RadixSortSlow(T* arr, size_t count)
{
    size_t bits = sizeof(T) * 8;
    ArrayList<T> zero = ArrayList<T>(count);
    ArrayList<T> one = ArrayList<T>(count);

    for (size_t bit = 0; bit < bits; bit++)
    {
        // put in buckets
        for (size_t i = 0; i < count; i++)
        {
            if (((arr[i] >> bit) & 1))
            {
                one.Add(arr[i]);
            }
            else
            {
                zero.Add(arr[i]);
            }
        }

        // take out of buckets;
        size_t zerosize = zero.GetSize();
        size_t onesize = one.GetSize();
        for (size_t i = 0; i < zerosize; i++)
        {
            arr[i] = zero.GetData()[i];
        }
        for (size_t i = 0; i < onesize; i++)
        {
            arr[i + zerosize] = one.GetData()[i];
        }

        zero.Clear();
        one.Clear();
    }
}

template<class T = byte>
inline void RadixSort(T* arr, size_t count)
{
    T* arrend = arr + count;
    register byte bits = sizeof(T) * 8;
    T* zeroarr = new T[count];// zero.GetData();
    T* onearr = new T[count];// one.GetData()

    register T* iter;
    register T* zeroiter = zeroarr;
    register T* zeroiter2 = zeroiter;
    register T* oneiter = onearr;
    register T* oneiter2 = oneiter;


    for (byte bit = 0; bit < bits; bit++)
    {
        zeroiter2 = zeroiter = zeroarr;
        oneiter2 = oneiter = onearr;

        // put in buckets
        iter = arr;
        while (iter < arrend)
        {
            if (((*iter >> bit) & 1))
            {
                //one.Add(buckets[i])
                *oneiter = *iter;
                oneiter++;
            }
            else
            {
                //zero.Add(buckets[i]);
                *zeroiter = *iter;
                zeroiter++;
            }
            iter++;
        }

        iter = arr;

        // take out of buckets;
        while (zeroiter2 < zeroiter)
        {
            *iter = *zeroiter2;
            iter++;
            zeroiter2++;
        }
        while (oneiter2 < oneiter)
        {
            *iter = *oneiter2;
            iter++;
            oneiter2++;
        }
    }

    delete[] zeroarr;
    delete[] onearr;
}


template<class T = byte>
inline void RadixSort2(T* arr, size_t count)
{
    T* arrend = arr + count;
    register byte bits = sizeof(T) * 8;
    T* zeroarr = new T[count << 1];
    T* onearr = zeroarr + count;

    register T* iter;
    register T* bucketiter;
    register T* zeroiter;
    register T* oneiter;


    for (byte bit = 0; bit < bits; bit++)
    {
        oneiter = ((bucketiter = zeroiter = zeroarr) + count);
        // put in buckets
        iter = arr;
        while (iter < arrend)
        {
            if (((*iter >> bit) & 1))
            {
                *oneiter = *iter;
                oneiter++;
            }
            else
            {
                *zeroiter = *iter;
                zeroiter++;
            }
            iter++;
        }

        iter = arr;
        //bucketiter = arr0;

        // take out of buckets;
        while (bucketiter < zeroiter)
        {
            *iter = *bucketiter;
            iter++;
            bucketiter++;
        }
        bucketiter = onearr;
        while (bucketiter < oneiter)
        {
            *iter = *bucketiter;
            iter++;
            bucketiter++;
        }
    }

    delete[] zeroarr;
}

template<class T = byte>
inline void RadixSort2Bit(T* arr, size_t count)
{
    T* arrend = arr + count;
    register byte bits = sizeof(T) * 8;
    T* arr0 = new T[count];
    T* arr1 = new T[count];
    T* arr2 = new T[count];
    T* arr3 = new T[count];

    register T* iter;
    register T* iter0 = arr0;
    register T* _iter0 = iter0;
    register T* iter1 = arr1;
    register T* _iter1 = iter1;
    register T* iter2 = arr2;
    register T* _iter2 = arr2;
    register T* iter3 = arr3;
    register T* _iter3 = arr3;


    for (byte bit = 0; bit < bits; bit += 2)
    {
        _iter0 = iter0 = arr0;
        _iter1 = iter1 = arr1;
        _iter2 = iter2 = arr2;
        _iter3 = iter3 = arr3;

        // put in buckets
        iter = arr;
        while (iter < arrend)
        {
            switch ((*iter >> bit) & 0b11)
            {
                case 0b00:
                    *iter0 = *iter;
                    iter0++;
                    break;

                case 0b01:
                    *iter1 = *iter;
                    iter1++;
                    break;

                case 0b10:
                    *iter2 = *iter;
                    iter2++;
                    break;

                case 0b11:
                    *iter3 = *iter;
                    iter3++;
                    break;
            }
            iter++;
        }


        iter = arr;

        // take out of buckets;
        while (_iter0 < iter0)
        {
            *iter = *_iter0;
            iter++;
            _iter0++;
        }
        while (_iter1 < iter1)
        {
            *iter = *_iter1;
            iter++;
            _iter1++;
        }
        while (_iter2 < iter2)
        {
            *iter = *_iter2;
            iter++;
            _iter2++;
        }
        while (_iter3 < iter3)
        {
            *iter = *_iter3;
            iter++;
            _iter3++;
        }
    }

    delete[] arr0;
    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
}


template<class T = byte>
inline void RadixSortAnyBitCount(T* arr, size_t count, byte bucketBitCount)
{
    T* arrend = arr + count;
    register byte bits = sizeof(T) * 8;
    size_t bucketCount = (size_t)pow(2, bucketBitCount);
    size_t mask = 0;
    for (size_t i = 0; i < bucketBitCount; i++)
    {
        mask <<= 1;
        mask |= 1;
    }

    T** buckets = new T*[bucketCount];
    for (size_t i = 0; i < bucketCount; i++)
    {
        buckets[i] = new T[count];
    }

    register T* iter;

    T** iters = new T*[bucketCount];
    T** _iters = new T*[bucketCount];


    for (byte bit = 0; bit < bits; bit += bucketBitCount)
    {
        for (size_t i = 0; i < bucketCount; i++)
        {
            iters[i] = _iters[i] = buckets[i];
        }

        // put in buckets
        iter = arr;
        while (iter < arrend)
        {
            byte b = (*iter >> bit) & mask;

            *iters[b] = *iter;
            iters[b]++;

            iter++;
        }


        iter = arr;

        // take out of buckets;
        for (size_t i = 0; i < bucketCount; i++)
        {
            while (_iters[i] < iters[i])
            {
                *iter = *_iters[i];
                iter++;
                _iters[i]++;
            }

        }
    }

    for (size_t i = 0; i < bucketCount; i++)
    {
        delete buckets[i];
    }
    delete[] buckets;
    delete[] iters;
    delete[] _iters;
}

template<class T>
void Print(T* arr, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

#define MEASURETIME(x, time) { auto start = std::chrono::system_clock::now(); x; auto end=std::chrono::system_clock::now(); time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0; }

byte* ReadInts(const char* const filename, size_t* count)
{
    ifstream in(filename);
    if (in.is_open())
    {
        ArrayList<byte> arr(100, 100);
        int num;
        while (in.good())
        {
            in >> num;
            arr.Add((byte)num);
        }
        byte* ret = new byte[arr.GetSize()];
        memcpy(ret, arr.GetData(), sizeof(byte) * arr.GetSize());
        *count = arr.GetSize();
        return ret;
    }
    return nullptr;
}

template<class T>
void WriteInts(const char* const filename, const T* const ints, const size_t count)
{
    ofstream out(filename);
    if (out.is_open())
    {
        for (size_t i = 0; i < count; i++)
        {
            out << (int)ints[i] << " ";
        }
    }
    delete[] ints;
}

int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;

    size_t count;// = 10000;
    byte* ints = ReadInts(argv[1], &count);
    //int* ints = new int[count];

    if (!ints)
        return 2;

    /*double avg = 0;
    size_t repeat = 100;
    for (size_t i = 0; i < repeat; i++)
    {
        for (size_t j = 0; j < count; j++)
        {
            ints[j] = rand();
        }
        double time;
        MEASURETIME(RadixSortAnyBitCount(ints, count, 6), time);
        avg += time;
    }
    cout << "sort time: " << (avg / repeat) << "ms" << endl;*/
    RadixSort(ints, count);

    WriteInts("out.txt", ints, count);
    // delete ints;
    system("pause");
    return 0;
}
