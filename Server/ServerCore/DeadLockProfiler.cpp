#include "pch.h"
#include "DeadLockProfiler.h"

void DeadLockProfiler::PushLock(const char *name)
{
    LockGuard lock(_lock);

    int32 lockId;

    auto findId = _nameToId.find(name);
    if (findId == _nameToId.end())
    {
        lockId = static_cast<int32>(_nameToId.size());
        _nameToId[name] = lockId;
        _idToName[lockId] = name;
    }
    else
        lockId = _nameToId[name];

    if (LLStack.empty() == false)
    {
        int32 prevId = LLStack.top();
        if (prevId != lockId)
        {
            std::set<int32> &history = _lockHistory[prevId];
            if (history.find(lockId) == history.end())
            {
                history.insert(lockId);
                CheckCycle();
            }
        }
    }

    LLStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char *name)
{
    LockGuard lock(_lock);
    if (_nameToId.find(name) == _nameToId.end())
        CRASH("INVALID LOCK NAME");

    int curLockId = _nameToId[name];
    if (curLockId != LLStack.top())
        CRASH("INVALID LOCK NAME");

    LLStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
    int32 lockCount = static_cast<int>(_nameToId.size());
    _findOrder = std::vector<int32>(lockCount, -1);
    _lockOrder = 0;
    _finished = std::vector<bool>(lockCount, false);
    _parent = std::vector<int32>(lockCount, -1);

    for (int32 lockNum = 0; lockNum < lockCount; ++lockNum)
        DFS(lockNum);
}

void DeadLockProfiler::DFS(int32 here)
{
    // 이미 확인했다면 return
    if (_findOrder[here] != -1)
        return;

    _findOrder[here] = _lockOrder++;

    // 해당 노드와 연결된 노드가 있는 지 확인.
    auto findHistory = _lockHistory.find(here);
    if (findHistory == _lockHistory.end())
    {
        _finished[here] = true;
        return;
    }

    std::set<int32>& nextNodes = findHistory->second;
    for (int32 there : nextNodes)
    {
        if (_findOrder[there] == -1)
        {
            _parent[there] = here;
            DFS(there);
            continue;
        }

        // 순방향 간선
        // 0 -> 1 -> 2, 0-> 2
        // 위의 Lock 순서 일 경우 순방향 간선으로 확인되어 DeadLock으로 판단하지 않는다.
        if (_findOrder[here] < _findOrder[there])
            continue;

        if (_finished[there] == false)
        {
            printf("%s -> %s\n", _idToName[here], _idToName[there]);

            int32 now = here;
            while (true)
            {
                printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
                now = _parent[now];
                if (now == there)
                    break;
            }

            CRASH("DEADLOCK DETECTED");
        }
    }

    _finished[here] = true;
}
