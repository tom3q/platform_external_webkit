/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef Heap_h
#define Heap_h

#include "MarkStack.h"
#include "MarkedSpace.h"
#include <wtf/Forward.h>
#include <wtf/HashSet.h>

namespace JSC {

    class GCActivityCallback;
    class GlobalCodeBlock;
    class JSCell;
    class JSGlobalData;
    class JSValue;
    class JSValue;
    class LiveObjectIterator;
    class MarkStack;
    class MarkedArgumentBuffer;
    class RegisterFile;
    class UString;
    class WeakGCHandlePool;

    typedef std::pair<JSValue, UString> ValueStringPair;
    typedef HashCountedSet<JSCell*> ProtectCountSet;
    typedef HashCountedSet<const char*> TypeCountSet;

    enum OperationInProgress { NoOperation, Allocation, Collection };

    class Heap {
        WTF_MAKE_NONCOPYABLE(Heap);
    public:
        static Heap* heap(JSValue); // 0 for immediate values
        static Heap* heap(JSCell*);

        static bool isMarked(const JSCell*);
        static bool testAndSetMarked(const JSCell*);
        static void setMarked(JSCell*);
        
        Heap(JSGlobalData*);
        ~Heap();
        void destroy(); // JSGlobalData must call destroy() before ~Heap().

        JSGlobalData* globalData() const { return m_globalData; }
        MarkedSpace& markedSpace() { return m_markedSpace; }
        MachineStackMarker& machineStackMarker() { return m_machineStackMarker; }

        GCActivityCallback* activityCallback();
        void setActivityCallback(PassOwnPtr<GCActivityCallback>);

        bool isBusy(); // true if an allocation or collection is in progress
        void* allocate(size_t);
        void collectAllGarbage();

        void reportExtraMemoryCost(size_t cost);

        void protect(JSValue);
        bool unprotect(JSValue); // True when the protect count drops to 0.

        bool contains(void*);

        size_t size() const;
        size_t capacity() const;
        size_t objectCount() const;
        size_t globalObjectCount();
        size_t protectedObjectCount();
        size_t protectedGlobalObjectCount();
        PassOwnPtr<TypeCountSet> protectedObjectTypeCounts();
        PassOwnPtr<TypeCountSet> objectTypeCounts();

        WeakGCHandle* addWeakGCHandle(JSCell*);

        void pushTempSortVector(Vector<ValueStringPair>*);
        void popTempSortVector(Vector<ValueStringPair>*);
        
        HashSet<GlobalCodeBlock*>& codeBlocks() { return m_codeBlocks; }

        HashSet<MarkedArgumentBuffer*>& markListSet() { if (!m_markListSet) m_markListSet = new HashSet<MarkedArgumentBuffer*>; return *m_markListSet; }
        
        template <typename Functor> void forEach(Functor&);
        
    private:
        friend class JSGlobalData;

        static const size_t minExtraCost = 256;
        static const size_t maxExtraCost = 1024 * 1024;

        void reportExtraMemoryCostSlowCase(size_t);

        void markRoots();
        void markProtectedObjects(MarkStack&);
        void markTempSortVectors(MarkStack&);

        void updateWeakGCHandles();
        WeakGCHandlePool* weakGCHandlePool(size_t index);
        
        enum SweepToggle { DoNotSweep, DoSweep };
        void reset(SweepToggle);

        RegisterFile& registerFile();

        OperationInProgress m_operationInProgress;
        MarkedSpace m_markedSpace;

        ProtectCountSet m_protectedValues;
        Vector<PageAllocationAligned> m_weakGCHandlePools;
        Vector<Vector<ValueStringPair>* > m_tempSortingVectors;
        HashSet<GlobalCodeBlock*> m_codeBlocks;

        HashSet<MarkedArgumentBuffer*>* m_markListSet;

        OwnPtr<GCActivityCallback> m_activityCallback;

        JSGlobalData* m_globalData;
        
        MachineStackMarker m_machineStackMarker;
        MarkStack m_markStack;
        
        size_t m_extraCost;
    };

    inline bool Heap::isMarked(const JSCell* cell)
    {
        return MarkedSpace::isMarked(cell);
    }

    inline bool Heap::testAndSetMarked(const JSCell* cell)
    {
        return MarkedSpace::testAndSetMarked(cell);
    }

    inline void Heap::setMarked(JSCell* cell)
    {
        MarkedSpace::setMarked(cell);
    }

    inline bool Heap::contains(void* p)
    {
        return m_markedSpace.contains(p);
    }

    inline void Heap::reportExtraMemoryCost(size_t cost)
    {
        if (cost > minExtraCost) 
            reportExtraMemoryCostSlowCase(cost);
    }
    
    inline WeakGCHandlePool* Heap::weakGCHandlePool(size_t index)
    {
        return static_cast<WeakGCHandlePool*>(m_weakGCHandlePools[index].base());
    }

    template <typename Functor> inline void Heap::forEach(Functor& functor)
    {
        m_markedSpace.forEach(functor);
    }

} // namespace JSC

#endif // Heap_h