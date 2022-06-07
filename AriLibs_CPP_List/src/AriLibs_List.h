#if !defined (__ARILIBS_LIST__)
#define __ARILIBS_LIST__

#if !defined (nullptr)
    #define nullptr NULL
#endif

#if !defined (Arduino_h)
    #include <vector>
#endif

#include <AriLibs_Container.h>
#include <AriLibs_Func1P.h>
#include <AriLibs_Func2P.h>
#include <AriLibs_Func3P.h>
//#include <FunctionPointers/AriLibs_Func1P.h>
//#include <FunctionPointers/AriLibs_Func2P.h>
//#include <FunctionPointers/AriLibs_Func3P.h>

namespace AriLibs
{
    /**
     * A generic list
     */
    template <class T>    
    class List : public Container<T>
    {
        protected:
            T** array;
            int capacity = 10;
            int currentSlot = 0;            

        public:
            List ();
            List (const List<T>& anotherList);

            #if !defined (Arduino_h)
            List (std::vector<T> v);
            List (std::vector<T>& v);
            List (std::vector<T>* v);
            #endif

            ~List ();

            using Container<T>::AddItem;
            void AddItem (T* item) override;            
            void AddList (List<T>* anotherList);
            void AddList (List<T> anotherList);
            using Container<T>::AddRange;
            void Clear () override;
            using Container<T>::Contains;
            bool Contains (T* item, bool clearItem = false) const override;            

        private:
            void expand (int slots);

        public:
            void ForEachItem (Func1P_Ptr<T> func) override;

            template <class P1>
            void ForEachItem (Func2P_Ptr<T,P1> func, P1 p1);

            template <class P1>
            void ForEachItem (Func2PValRef_Ptr<T,P1> func, P1* p1);

            template <class P1, class P2>
            void ForEachItem (Func3P_Ptr<T,P1,P2> func, P1 p1, P2 p2);

            template <class P1, class P2>
            void ForEachItem (Func3PValRef_Ptr<T,P1,P2> func, P1* p1, P2* p2);            

            using Container<T>::IndexOf;
            int IndexOf (T* item, bool clearItem = false) const override;
            
            void InsertItemAt (T* item, int index);
            void InsertItemAt (T item, int index);

        private:
            bool isFull ();

        public:
            T* operator[] (const int index) const;
            void RemoveItem (T* item, bool clearItem = false);
            void RemoveItem (T item);
            void RemoveItemByIndex (int itemIndex);
            void RemoveLastItem ();
            void ReplaceItem (T* item, int index);
            void ReplaceItem (T item, int index);
            
            #if !defined (Arduino_h)
            std::vector<T>& ToVector ();
            #endif
    };


    template <class T>
    List<T>::List ()
    {
        this->array = new T*[this->capacity];
    }


    template <class T>
    List<T>::List (const List<T>& anotherList)
    {
        this->capacity = (anotherList.Size() + 10);
        this->array = new T*[this->capacity];

        for (int slot = 0; slot < anotherList.Size(); slot ++)
        {
            T* item = anotherList[slot];
            this->AddItem (item);
        }
    }


    #if !defined (Arduino_h)

    template <class T>
    List<T>::List (std::vector<T> v)
    {
        this->capacity = (v.size() + 10);
        this->size = v.size();

        this->array = new T*[this->capacity];

        for (int i = 0; i < v.size(); i ++)
        {
            this->AddItem (v[i]);
        }
    }


    template <class T>
    List<T>::List (std::vector<T>& v)
    {
        this->capacity = (v.size() + 10);
        this->size = v.size();

        this->array = new T*[this->capacity];

        for (int i = 0; i < v.size(); i ++)
        {
            this->AddItem (v[i]);
        }
    }


    template <class T>
    List<T>::List (std::vector<T>* v)
    {
        if (v != nullptr)
        {
            this->capacity = (v->size() + 10);
            this->size = v->size();

            this->array = new T*[this->capacity];

            for (int i = 0; i < v.size(); i ++)
            {
                this->AddItem (v[i]);
            }
        }
    }

    #endif


    template <class T>
    List<T>::~List ()
    {
        delete this->array;
    }


    template <class T>
    void List<T>::AddItem (T* item)
    {
        if (isFull())
        {
            expand (10);
        }

        this->array[this->currentSlot] = item;

        this->currentSlot ++;
        this->size ++;
    }


    template <class T>
    void List<T>::AddList (List<T>* anotherList)
    {
        this->AddList (*anotherList);
    }


    template <class T>
    void List<T>::AddList (List<T> anotherList)
    {
        for (int slot = 0; slot < anotherList.Size(); slot ++)
        {
            this->AddItem (anotherList[slot]);
        }        
    }


    template <class T>
    void List<T>::Clear ()
    {
        delete this->array;
        this->array = new T*[this->capacity];

        this->size = 0;
        this->capacity = 10;
    }


    template <class T>
    bool List<T>::Contains (T* item, bool clearItem) const
    {
        int indexOfItem = this->IndexOf (item);

        if (clearItem)
        {
            delete item;
        }

        return (indexOfItem > -1);
    }


    template <class T>
    void List<T>::expand (int slots)
    {
        int newArrayCapacity = this->capacity + slots;
        T** newArray = new T*[newArrayCapacity];

        for (int s = 0; s < this->size; s ++)
        {
            newArray[s] = this->array[s];
        }

        delete this->array;

        this->array = newArray;
        this->capacity = newArrayCapacity;
    }


    template <class T>
    void List<T>::ForEachItem (Func1P_Ptr<T> func)
    {
        if (this->size > 0)
        {
            for (int item = 0; item < this->size; item ++)
            {
                func (*this->array[item]);
            }
        }
    }


    template <class T>
    template <class P1>
    void List<T>::ForEachItem (Func2P_Ptr<T,P1> func, P1 p1)
    {
        if (this->size > 0)
        {
            for (int item = 0; item < this->size; item ++)
            {
                func (*this->array[item], p1);
            }
        }
    }


    template <class T>
    template <class P1>
    void List<T>::ForEachItem (Func2PValRef_Ptr<T,P1> func, P1* p1)
    {
        if (this->size > 0)
        {
            for (int item = 0; item < this->size; item ++)
            {
                func (*this->array[item], p1);
            }
        }
    }


    template <class T>
    template <class P1, class P2>
    void List<T>::ForEachItem (Func3P_Ptr<T,P1,P2> func, P1 p1, P2 p2)
    {
        if (this->size > 0)
        {
            for (int item = 0; item < this->size; item ++)
            {
                func (*this->array[item], p1, p2);
            }
        }
    }


    template <class T>
    template <class P1, class P2>
    void List<T>::ForEachItem (Func3PValRef_Ptr<T,P1,P2> func, P1* p1, P2* p2)
    {
        if (this->size > 0)
        {
            for (int item = 0; item < this->size; item ++)
            {
                func (*this->array[item], p1, p2);
            }
        }
    }


    template <class T>
    bool List<T>::isFull ()
    {
        return (this->size == this->capacity);
    }


    template <class T>
    int List<T>::IndexOf (T* item, bool clearItem) const
    {
        for (int slot = 0; slot < this->size; slot ++)
        {
            T currentItem = *this->array[slot];

            if (currentItem == *item)
            {
                if (clearItem)
                {
                    delete item;
                }

                return slot;
            }
        }

        if (clearItem)
        {
            delete item;
        }

        return -1;
    }


    template <class T>
    void List<T>::InsertItemAt (T* item, int index)
    {
        int newArraySize = (this->size + 1);
        int newArrayCapacity = this->capacity;

        if (newArraySize > this->capacity)
        {
            newArrayCapacity++;
        }

        T** newArray = new T*[newArrayCapacity];

        for (int slot = 0; slot <= newArraySize; slot ++)
        {
            if (slot < index)
            {
                newArray[slot] = this->array[slot];
            }
            else if (slot > index)
            {
                newArray[slot] = this->array[slot - 1];
            }
            else
            {
                newArray[slot] = item;
            }
        }

        delete this->array;
        this->array = newArray;
        this->size = newArraySize;
    }


    template <class T>
    void List<T>::InsertItemAt (T item, int index)
    {
        T* itemPtr = new T (item);
        Insert (itemPtr, index);
    }


    template <class T>
    T* List<T>::operator[] (const int index) const
    {
        if ((index >= 0) && (index < this->Size()))
        {
            return this->array[index];
        }

        return nullptr;
    }


    template <class T>
    void List<T>::RemoveItem (T* item, bool clearItem)
    {
        int itemIndex = IndexOf (item);

        if (itemIndex > -1)
        {
            RemoveItem (itemIndex);
        }

        if (clearItem)
        {
            delete item;
        }
    }


    template <class T>
    void List<T>::RemoveItem (T item)
    {
        T* itemPtr = new T (item);
        RemoveItem (itemPtr);
    }


    template <class T>
    void List<T>::RemoveItemByIndex (int itemIndex)
    {
        if ((itemIndex >= 0) && (itemIndex < this->Size()))
        {
            for (int slot = itemIndex; slot < this->size; slot ++)
            {
                if (slot == (this->size - 1))
                {
                    delete this->array[slot];
                }
                else
                {
                    *this->array[slot] = *this->array[slot + 1];
                }
            }

            this->size --;
        }
    }


    template <class T>
    void List<T>::RemoveLastItem ()
    {
        delete this->array[this->size - 1];
        this->array[this->size - 1] = nullptr;
        this->size --;
    }


    template <class T>
    void List<T>::ReplaceItem (T* item, int index)
    {
        if (index >= this->size)
        {
            if (item != nullptr)
            {
                this->array[index] = item;
            }
        }
    }


    template <class T>
    void List<T>::ReplaceItem (T item, int index)
    {
        if (index >= this->size)
        {
            T* itemPtr = new T (item);
            ReplaceItem (itemPtr, index);
        }
    }


    #if !defined (Arduino_h)
    template <class T>
    std::vector<T>& List<T>::ToVector ()
    {
        std::vector<T> v = std::vector<T> ();

        if (this->size > 0)
        {
            this->ForEachItem ([v] (T item)
            {
                v.push_back (item);
            });
        }

        return v;
    }
    #endif
}

#endif
