/*
定义一个 Slice（切片），它只存两个东西：
1.指针 (const char* data_)：指向数据在哪里。
2.长度 (size_t size_)：数据有多长。
非常轻量，像一个“窗口”，只看数据，不拥有数据。
*/

#pragma once

#include<cassert>
#include<cstddef>
#include<cstring>
#include<string>

namespace LSM{
    class Slice{
    public:
        Slice():data(""),size(0){}
        //从c风格的字符串构造char*->Slice
        Slice(const char* d,size_t n):data(d),size(n){}
        Slice(const std::string& s):data(s.data()),size(s.size()){}
        
        //Getters
        const char* data() const { return data; }
        size_t size() const { return size; }
        bool empty() const { return size==0; }

        //比较功能
        int compare(const Slice& b) const {
            const size_t min_len=size<b.size()?size:b.size();
            int r=memcmp(data,b.data,min_len);
            if(r==0){
                if(size<b.size) r=-1;
                else if(size>b.size) r+=1;
            }
            return r;
        }
        //把切片转回string，方便打印调试
        std::string ToString() const{ return std::string(data,size); }
    private:
        const char* data; //数据指针
        size_t size; //数据长度
    };
    inline bool operator==(const Slice& x,const Slice& y){
        return ((x.size()==y.size())&&(memcmp(x.data(),y.data(),x.size())==0));
    }
}
