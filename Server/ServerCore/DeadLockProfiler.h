#pragma once

#include<stack>
#include<map>
#include<vector>

class DeadLockProfiler
{
public:
    void PushLock(const char *name);
    void PopLock(const char *name);

private:
    void CheckCycle();
    void DFS(int32 here);

private:
    std::unordered_map<const char *, int32> _nameToId;
    std::unordered_map<int32, const char *> _idToName;
    std::map<int32, std::set<int32>> _lockHistory;
    Mutex _lock;

private:
    std::vector<int32> _findOrder;
    int32 _lockOrder;
    std::vector<bool> _finished;
    std::vector<int32> _parent;
};