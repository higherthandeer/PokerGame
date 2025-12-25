#include "managers/UndoManager.h"
#include "cocos2d.h"

USING_NS_CC;

UndoManager::UndoManager() : _maxSteps(100) {
}

UndoManager::~UndoManager() {
}

void UndoManager::init(int maxSteps) {
    _maxSteps = maxSteps;
    _undoStack.clear();
    CCLOG("UndoManager initialized with max steps: %d", _maxSteps);
}

void UndoManager::clear() {
    _undoStack.clear();
    CCLOG("UndoManager cleared");
}

void UndoManager::pushRecord(const UndoRecord& record) {
    // 限制栈大小
    if (_undoStack.size() >= static_cast<size_t>(_maxSteps)) {
        _undoStack.erase(_undoStack.begin());
        CCLOG("UndoManager: Stack full, removed oldest record");
    }

    _undoStack.push_back(record);
    CCLOG("UndoManager:  Pushed record, cardId=%d, type=%d, stack size=%zu",
        record.cardId, static_cast<int>(record.actionType), _undoStack.size());
}

UndoRecord UndoManager::popRecord() {
    if (_undoStack.empty()) {
        CCLOG("UndoManager: Cannot pop, stack is empty");
        return UndoRecord();
    }

    UndoRecord record = _undoStack.back();
    _undoStack.pop_back();

    CCLOG("UndoManager: Popped record, cardId=%d, remaining=%zu",
        record.cardId, _undoStack.size());

    return record;
}

bool UndoManager::canUndo() const {
    return !_undoStack.empty();
}

int UndoManager::getUndoCount() const {
    return static_cast<int>(_undoStack.size());
}