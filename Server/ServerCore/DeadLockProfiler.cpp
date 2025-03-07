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
    // �̹� Ȯ���ߴٸ� return
    if (_findOrder[here] != -1)
        return;

    _findOrder[here] = _lockOrder++;

    // �ش� ���� ����� ��尡 �ִ� �� Ȯ��.
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

        // ������ ����
        // 0 -> 1 -> 2, 0-> 2
        // ���� Lock ���� �� ��� ������ �������� Ȯ�εǾ� DeadLock���� �Ǵ����� �ʴ´�.
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
