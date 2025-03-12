#pragma once

template<typename Type, typename... Args> 
Type* XNew(Args&&... args)
{
    Type* ptr = static_cast<Type*>(XAlloc(sizeof(Type)));
    new (ptr) Type(std::forward<Args>(args)...);
    return ptr;
}

template<typename Type> 
void XDelete(Type* obj)
{
    obj->~Type();
    XFree(obj);
}