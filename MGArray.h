//
//  MGArray.h
//  MGAILibrary
//
//  Created by zlm on 2019/7/19.
//  Copyright © 2019 zlm. All rights reserved.
//
/*
 数组2700以内
 vector 添加测试耗时 0.020020
 自定义Array 添加测试耗时 0.002930
 vector insert耗时 0.014893
 MGArray insert耗时 0.004883
 20,20
 
 vector 添加测试耗时 0.154053
 自定义Array 添加测试耗时 0.005127
 vector 删除第二个元素5次 耗时 0.025879
 MGArray 删除第二个元素5次 耗时 0.002930
 5,5
 
 使用注意事项:
 1.当元素并非指针时, sort方法较占用内存
 2.数组需要手动释放数据, 即当不再使用数组时,需要手动调用clear()方法清除内部元素指针
 3.如果要用到排序功能,又对速度有一定要求的话,那么元素最好为对象指针
 */



#ifndef MGArray_h
#define MGArray_h

#include <stdio.h>


typedef enum : int {
    MGArrayQuickSortTypeNeedReplace,
    MGArrayQuickSortTypeNoReplace,
    MGArrayQuickSortTypeSame
} MGArrayQuickSortType;

template <typename T>
class MGArray {
    
public:
    class MGArrayObject;
    typedef MGArrayObject Iterator;
    
    class MGArrayObject {
    public:
        
        T data;
        
        /**
         下一个数据地址
         @return 下一个数据
         */
        Iterator* next() {
            return this->_lastData;
        };
        
        
        //内部使用
        Iterator* _preData;
        Iterator* _lastData;
    };
private:
    MGArrayObject* _beginData;
    MGArrayObject* _endData;
    int _count;

public:
    
    MGArray() {
        _count = 0;
        _beginData = NULL;
        _endData = NULL;
    };
    ~MGArray() {
//        clear();
    };
    
    int count() {
        return _count;
    };
    int size() {
        return count();
    };
    
    //MARK: 添加
    void appendIter(Iterator* iter) {
        if (_beginData == NULL) {
            _beginData = iter;
        }
        if (_endData != NULL) {
            _endData->_lastData = iter;
            iter->_preData = _endData;
        }
        _endData = iter;
        _count++;
    };
    //添加到第一个
    void appendFirstIter(Iterator* iter) {
        if (_endData == NULL) {
            _endData = iter;
        }
        if (_beginData != NULL) {
            iter->_lastData = _beginData;
            _beginData->_preData = iter;
        }
        _beginData = iter;//第一个元素
        _count++;
    };
    
    /**
     添加元素
     @param object 需要加入的元素
     */
    void append(T object) {
        MGArrayObject* model = new MGArrayObject();
        model->data = object;
        if (_beginData == NULL) {
            _beginData = model;
        }
        if (_endData != NULL) {
            _endData->_lastData = model;
            model->_preData = _endData;
        }
        _endData = model;
        _count ++;
    };
    void push_back(T object) {
        append(object);
    };
    
    //    在指定位置添加元素
    bool insert(T object, int index) {
        if (index > count()) {
            //要插入的下标越界
            return false;
        }
        Iterator* _iterator = _beginData;
        for (int i = 0; i < index; i++) {
            _iterator = _iterator->_lastData;
        }
        
        MGArrayObject* model = new MGArrayObject();
        model->data = object;
        // 空数组
        if (_iterator == NULL) {
            _beginData = model;
            _endData = model;
        } else {
            Iterator* preIterator = _iterator->_preData;
            //插入首个
            if (preIterator == NULL) {
                _beginData = model;
            } else {
                preIterator->_lastData = model;
                model->_preData = preIterator;
            }
            _iterator->_preData = model;
            model->_lastData = _iterator;
            
        }
        _count ++;
        return true;
    };
    
    //MARK: 查找
    //    查找指定位置的元素
    T& at(int index) {
        Iterator* _iterator = _beginData;
        for (int i = 0; i < index; i++) {
            _iterator = _iterator->_lastData;
        }
        
        return _iterator->data;
    };
    //    查找指定位置的Iterator
    Iterator* atToIter(int index) {
        Iterator* _iterator = _beginData;
        for (int i = 0; i < index; i++) {
            if (_iterator == NULL) {
                return _iterator;
            }
            _iterator = _iterator->_lastData;
        }
        return _iterator;
    };
    
    
    //MARK: 遍历 example:
    /*
     for (MGArray<TestModel2*>::Iterator* iter = newArr->begin(); iter != NULL; iter = iter->next()) {
     printf("%d ", iter->data->index);
     }
     for (MGArray<int>::Iterator* iter = newIntArr->begin(); iter != NULL; iter = iter->next()) {
     printf("%d ", iter->data);
     }
     
     */
    /**
     获取第一个元素
     
     @return 第一个元素
     */
    Iterator* begin() {
        MGArrayObject* obj = _beginData;
        if (obj == NULL) {
            return NULL;
        }
        return obj;
    };
    
    /**
     获取最后一个元素
     
     @return 最后一个元素
     */
    Iterator* end() {
        MGArrayObject* obj = _endData;
        if (obj == NULL) {
            return NULL;
        }
        return obj;
    };
    
    //MARK: 删除
    /**
     删除最后一个元素,未释放
     
     @return 返回删除的指针 主要用于指针释放
     */
private:
    Iterator* _removeLastIter() {
        MGArrayObject* cur = _endData;
        if (cur == nullptr) {
            return nullptr;
        }
        _endData = cur->_preData;
        if (cur->_preData == nullptr) { //已删除所有元素
            _beginData = nullptr;
        } else {
            _endData->_lastData = nullptr;
        }
        _count --;
        return cur;
    };
    
    Iterator* _removeFirstIter() {
        MGArrayObject* cur = _beginData;
        if (cur == nullptr) {
            return nullptr;
        }
        _beginData = cur->_lastData;
        if (cur->_lastData == nullptr) {
            _endData = nullptr;
        } else {
            _beginData->_preData = nullptr;
        }
        _count --;
        return cur;
    }
    
    
public:
    
    //元素为非指针时使用该方法
    void removeLast() {
        Iterator* iter = _removeLastIter();
        if (iter == nullptr) {
            return;
        }
        delete iter;
    };
    void removeFirst() {
        Iterator* iter = _removeFirstIter();
        if (iter == nullptr) {
            return;
        }
        delete iter;
    }
    
    
    
    //删除 不释放指针 返回前一个指针
    Iterator* eraseToIter(Iterator* iter) {
        Iterator* preIter = iter->_preData;
        Iterator* lastIter = iter->_lastData;
        
        if (preIter != NULL) {
            preIter->_lastData = lastIter;
        }else {
            //当前是起始数据
            _beginData = lastIter;
            if (_beginData) {
                _beginData->_preData = NULL;
            }
            
        }
        if (lastIter != NULL) {
            lastIter->_preData = preIter;
        } else {
            //            当前是末尾数据
            _endData = preIter;
            if (_endData) {
                _endData->_lastData = NULL;
            }
        }
        iter->_preData = NULL;
        iter->_lastData = NULL;
        _count --;
        return preIter;
    };
    
    T& erase(Iterator* iter) {
        eraseToIter(iter);
        T& data = iter->data;
        delete iter;
        return data;
    };
    
    /**
     删除指定位置的元素
     
     @param index 下标
     @return 返回删除的元素 主要用于指针释放
     */
    T& removeAt(int index) {
        MGArrayObject* cur = _beginData;
        for (int i = 0; i < index; i ++) {
            cur = cur->_lastData;
        }
        MGArrayObject* preObj = cur->_preData;
        if (preObj != NULL) {
            preObj->_lastData = cur->_lastData;
        }else {
            _beginData = cur->_lastData;
        }
        
        MGArrayObject* lastObj = cur->_lastData;
        if (lastObj != NULL) {
            lastObj->_preData = cur->_preData;
        } else {
            _endData = cur->_preData;
        }
        T& data = cur->data;
        //        释放 cur
        delete cur;
        _count --;
        return data;
    };
    
    //清空
    void clear() {
        clear(NULL);
    };
   
    void clear(void(*handle)(T item)) {
        Iterator* curData = begin();
       
        int i = 0;
        while (curData != NULL) {
            Iterator* nextData = curData->next();
            if (handle != NULL) {
                handle(curData->data);
            }
            delete curData;
            curData = nextData;
            //            printf("---zlm---:%d", i);
            i++;
        }

        _beginData = NULL;
        _endData = NULL;
        _count = 0;
    };
    
    T& data() {
        
        T dataList[this->count()];
        int count = 0;
        for (Iterator* iter = begin(); iter != NULL; iter = iter->next()) {
            dataList[count] = iter->data;
            count++;
        }
        return *dataList;
    }
    
    void resize(int maxCount) {
        if (count() > 0) {
            while (count() > maxCount) {
                removeLast();
            }
        } else {
            for (int i = 0; i < maxCount; i++) {
                T item;
                append(item);
            }
        }
    }
    

private:
    //相邻数据互换位置
    void _replaceNearData(Iterator* first, Iterator* second) {
        Iterator* firstIter = first->_preData;
        Iterator* endIter = second->_lastData;
        
        second->_lastData = first;
        second->_preData = firstIter;
        if (firstIter != NULL) {
            firstIter->_lastData = second;
        } else {
            _beginData = second;
        }
        
        first->_preData = second;
        first->_lastData = endIter;
        if (endIter != NULL) {
            endIter->_preData = first;
        }else {
            _endData = first;
        }
    };
    
    //某两个数据互换位置 要保证array中, first在second之前
    void _replaceData(Iterator* first, Iterator* second) {
//        如果first和second相邻,则直接调用replaceNearData
        if (first->_lastData == second) {
            _replaceNearData(first, second);
            return;
        }
        
//        记录第一个指针的前后指针
        Iterator* firstPri = first->_preData;
        Iterator* firstLast = first->_lastData;
//        记录第二个指针的前后指针
        Iterator* secondPri = second->_preData;
        Iterator* secondLast = second->_lastData;
        if (firstPri == NULL) {
            _beginData = second;
        } else {
            firstPri->_lastData = second;
        }
        
        firstLast->_preData = second;
        second->_preData = firstPri;
        second->_lastData = firstLast;
        
        secondPri->_lastData = first;
        if (secondLast == NULL) {
            _endData = first;
        } else {
            secondLast->_preData = first;
        }
        
        first->_preData = secondPri;
        first->_lastData = secondLast;
        
    };
    
    //根据指定两个元素,获取这两个元素之间一共有多少个元素
    int _getCountByPart(Iterator* begin, Iterator* end) {
        int count = 1;
        Iterator* countIter = begin;
        while (countIter != end) {
            countIter = countIter->next();
            count ++;
        }
        
        return count;
    };
    
    Iterator* _atToIter(Iterator* beginIter, int index) {
        Iterator* _iterator = beginIter;
        for (int i = 0; i < index; i++) {
            _iterator = _iterator->_lastData;
        }
        return _iterator;
    };
    
    void _plitData(Iterator* list, int count, MGArrayQuickSortType(*handle)(T item1, T item2) ) {
        if (count >= 2) {
            int index = count / 2;
            Iterator middle = list[index];
            Iterator leftList[count];
            int leftCount = 0;
            Iterator rightList[count];
            int rightCount = 0;
            Iterator middleList[count];
            int middleCount = 0;
            
            int i = 0;
            Iterator value;
            while (i < count) {
                value = list[i];
                MGArrayQuickSortType resultType = handle(value.data, middle.data);
                if (resultType == MGArrayQuickSortTypeNoReplace) {
                    leftList[leftCount] = value;
                    leftCount ++;
                
                } else if (resultType == MGArrayQuickSortTypeNeedReplace) {
                    rightList[rightCount] = value;
                    rightCount ++;
                } else {
                    middleList[middleCount] = value;
                    middleCount ++;
                }
                i += 1;
            }
            
            _plitData(leftList, leftCount, handle);
            
            _plitData(rightList, rightCount, handle);
            
            int j = 0;
            for (int i = 0; i < leftCount; i ++) {
                list[j] = leftList[i];
                j++;
            }
            
            for (int i = 0; i < middleCount; i++) {
                list[j] = middleList[i];
                j++;
            }
            
            for (int i = 0; i < rightCount; i++) {
                list[j] = rightList[i];
                j++;
            }
        }
    };
    
public:
    //快速排序法
    void sortQuick(MGArrayQuickSortType(*handle)(T item1, T item2)) {
        //转数组
        Iterator iterList[this->count()];
        int count = 0;
        for (Iterator* iter = begin(); iter != NULL; iter = iter->next()) {
            iterList[count] = *iter;
            count++;
        }
        _plitData(iterList, count, handle);
        //重新修改链表关系
        int i = 0;
        for (Iterator* iter = begin(); iter != NULL; iter = iter->next()) {
             Iterator* newIter = &iterList[i];
            iter->data = newIter->data;
            i ++;
        }
    };
    
    //冒泡排序法 (较慢)
    void sort(Iterator* begin, Iterator* end, bool(*handle)(T item1, T item2)) {
        if (begin == NULL || end == NULL) {
            return;
        }
        int curCount = 1;
        Iterator* countIter = begin;
        while (countIter != end) {
            countIter = countIter->next();
            curCount ++;
        }
        
//        标记起始位置
        Iterator* markPreIter = begin->_preData;
        Iterator* markLastIter = end->_lastData;
        
        for (int i = 0; i < curCount; i++) {
            Iterator* iterJ = begin;
            for (int j = 0; j < curCount - i - 1; j++) {
                if (iterJ == NULL || iterJ->_lastData == NULL) {
                    break;
                }
                if (!handle(iterJ->data, iterJ->_lastData->data)) {
                    _replaceData(iterJ, iterJ->_lastData);
                } else {
                    iterJ = iterJ->next();
                }
            }
            
            Iterator* getFirst = iterJ;
            while (getFirst->_preData != markPreIter) {
                getFirst = getFirst->_preData;
            }
            begin = getFirst;
            
            Iterator* getEnd = iterJ;
            while (getEnd->_lastData != markLastIter) {
                getEnd = getEnd->_lastData;
            }
            end = getEnd;
        }
    };
    
};




#endif /* MGArray_h */
